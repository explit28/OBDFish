#include "bluetoothconnection.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMetaType>
#include <QDBusObjectPath>
#include <QDBusReply>
#include <QDebug>

using BluezInterfaces = QMap<QString, QVariantMap>;
using BluezObjects = QMap<QDBusObjectPath, BluezInterfaces>;
Q_DECLARE_METATYPE(BluezInterfaces)
Q_DECLARE_METATYPE(BluezObjects)

BluetoothConnection::BluetoothConnection(QObject *parent) : QObject(parent)
{
    qDBusRegisterMetaType<BluezInterfaces>();
    qDBusRegisterMetaType<BluezObjects>();
    m_refreshTimer.setInterval(1000);
    connect(&m_refreshTimer, &QTimer::timeout, this, &BluetoothConnection::refreshDevices);
}

void BluetoothConnection::vStartDeviceDiscovery()
{
    vStopDeviceDiscovery();
    m_seenAddresses.clear();
    m_adapterPath.clear();

    QDBusInterface manager("org.bluez", "/", "org.freedesktop.DBus.ObjectManager",
                          QDBusConnection::systemBus());
    QDBusReply<BluezObjects> result = manager.call("GetManagedObjects");
    if (!result.isValid()) {
        qWarning() << "Cannot query Bluetooth devices:" << result.error().message();
        emit scanFinished();
        return;
    }

    for (auto it = result.value().cbegin(); it != result.value().cend(); ++it) {
        if (it.value().contains(QStringLiteral("org.bluez.Adapter1"))) {
            m_adapterPath = it.key().path();
            break;
        }
    }
    if (m_adapterPath.isEmpty()) {
        qWarning() << "No BlueZ adapter found";
        emit scanFinished();
        return;
    }

    m_scanning = true;
    refreshDevices();
    QDBusInterface adapter("org.bluez", m_adapterPath, "org.bluez.Adapter1",
                           QDBusConnection::systemBus());
    QDBusMessage response = adapter.call("StartDiscovery");
    if (response.type() == QDBusMessage::ErrorMessage)
        qWarning() << "Bluetooth StartDiscovery:" << response.errorMessage();

    // Include devices found during discovery, without requiring an
    // unsupported QtBluetooth library on Sailfish OS 5.x.
    m_refreshTimer.start();
    QTimer::singleShot(12000, this, [this]() { vStopDeviceDiscovery(); });
}

void BluetoothConnection::refreshDevices()
{
    if (!m_scanning) return;
    QDBusInterface manager("org.bluez", "/", "org.freedesktop.DBus.ObjectManager",
                          QDBusConnection::systemBus());
    QDBusReply<BluezObjects> result = manager.call("GetManagedObjects");
    if (!result.isValid()) {
        qWarning() << "BlueZ device refresh failed:" << result.error().message();
        return;
    }

    for (auto it = result.value().cbegin(); it != result.value().cend(); ++it) {
        auto device = it.value().constFind(QStringLiteral("org.bluez.Device1"));
        if (device == it.value().cend()) continue;
        const QString address = device->value(QStringLiteral("Address")).toString();
        if (address.isEmpty() || m_seenAddresses.contains(address)) continue;
        m_seenAddresses.insert(address);
        const QString name = device->value(QStringLiteral("Alias")).toString();
        emit deviceFound(name.isEmpty() ? address : name, address);
    }
}

void BluetoothConnection::vStopDeviceDiscovery()
{
    if (!m_scanning) return;
    m_scanning = false;
    m_refreshTimer.stop();
    QDBusInterface adapter("org.bluez", m_adapterPath, "org.bluez.Adapter1",
                           QDBusConnection::systemBus());
    adapter.call("StopDiscovery");
    emit scanFinished();
}
