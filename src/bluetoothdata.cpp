#include "bluetoothdata.h"

#include <QDebug>
#include <QTimer>
#include <cerrno>
#include <cstring>
#include <cstdint>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

// Linux BlueZ RFCOMM socket ABI (same layout as bluetooth/rfcomm.h).
// Avoid QtBluetooth, which is absent from the Sailfish OS 5.1 SDK targets.
// AF_BLUETOOTH and BTPROTO_RFCOMM are Linux protocol constants.
#ifndef AF_BLUETOOTH
#define AF_BLUETOOTH 31
#endif
#ifndef BTPROTO_RFCOMM
#define BTPROTO_RFCOMM 3
#endif

struct RfcommAddress {
    sa_family_t family;
    uint8_t address[6];
    uint8_t channel;
};

BluetoothData::BluetoothData(QObject *parent) : QObject(parent) {}
BluetoothData::~BluetoothData() { disconnect(); }

void BluetoothData::connect(QString address, int port)
{
    disconnect();

    const QStringList pieces = address.split(':');
    if (pieces.size() != 6 || port < 1 || port > 30) {
        emit sigError(QStringLiteral("Invalid Bluetooth address or RFCOMM channel"));
        return;
    }

    RfcommAddress remote = {};
    remote.family = AF_BLUETOOTH;
    remote.channel = static_cast<uint8_t>(port);
    for (int i = 0; i < 6; ++i) {
        bool ok = false;
        const uint byte = pieces.at(5 - i).toUInt(&ok, 16);
        if (!ok || pieces.at(5 - i).length() != 2 || byte > 255) {
            emit sigError(QStringLiteral("Invalid Bluetooth address"));
            return;
        }
        remote.address[i] = static_cast<uint8_t>(byte);
    }

    m_fd = ::socket(AF_BLUETOOTH, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                    BTPROTO_RFCOMM);
    if (m_fd < 0) {
        emit sigError(QString::fromLocal8Bit(std::strerror(errno)));
        return;
    }

    const int result = ::connect(m_fd,
                                 reinterpret_cast<const sockaddr *>(&remote),
                                 sizeof(remote));
    if (result == 0) {
        finishConnect();
    } else if (errno == EINPROGRESS || errno == EAGAIN) {
        m_connecting = true;
        m_writeNotifier = new QSocketNotifier(m_fd, QSocketNotifier::Write, this);
        QObject::connect(m_writeNotifier, &QSocketNotifier::activated,
                         this, [this](int) { finishConnect(); });
        QTimer::singleShot(15000, this, [this]() {
            if (m_connecting) fail(QStringLiteral("Bluetooth connection timed out"));
        });
    } else {
        fail(QString::fromLocal8Bit(std::strerror(errno)));
    }
}

void BluetoothData::finishConnect()
{
    if (m_fd < 0) return;
    int error = 0;
    socklen_t length = sizeof(error);
    if (::getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &error, &length) != 0)
        error = errno;
    if (error != 0) {
        fail(QString::fromLocal8Bit(std::strerror(error)));
        return;
    }
    m_connecting = false;
    m_connected = true;
    delete m_writeNotifier;
    m_writeNotifier = nullptr;
    m_readNotifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    QObject::connect(m_readNotifier, &QSocketNotifier::activated,
                     this, [this](int) { readData(); });
    emit sigConnected();
}

void BluetoothData::readData()
{
    if (m_fd < 0) return;
    char buffer[4096];
    const ssize_t size = ::recv(m_fd, buffer, sizeof(buffer), 0);
    if (size > 0) {
        emit sigReadDataReady(QString::fromLatin1(buffer, static_cast<int>(size)));
    } else if (size == 0) {
        disconnect();
    } else if (errno != EAGAIN && errno != EINTR) {
        fail(QString::fromLocal8Bit(std::strerror(errno)));
    }
}

void BluetoothData::sendHex(QString command)
{
    QByteArray bytes = command.toLatin1();
    bytes.append('\r');
    write(bytes);
}

qint64 BluetoothData::write(const QByteArray &data)
{
    if (!m_connected || m_fd < 0) {
        emit sigError(QStringLiteral("Bluetooth adapter is not connected"));
        return -1;
    }
    const ssize_t count = ::send(m_fd, data.constData(), data.size(), MSG_NOSIGNAL);
    if (count < 0) {
        if (errno != EAGAIN && errno != EINTR)
            fail(QString::fromLocal8Bit(std::strerror(errno)));
        return -1;
    }
    if (count != data.size())
        qWarning() << "Partial RFCOMM write:" << count << "of" << data.size();
    return count;
}

void BluetoothData::fail(const QString &message)
{
    qWarning() << "RFCOMM error:" << message;
    disconnect();
    emit sigError(message);
}

void BluetoothData::disconnect()
{
    const bool wasConnected = m_connected;
    m_connected = false;
    m_connecting = false;
    delete m_readNotifier;
    m_readNotifier = nullptr;
    delete m_writeNotifier;
    m_writeNotifier = nullptr;
    if (m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
    }
    if (wasConnected) emit sigDisconnected();
}
