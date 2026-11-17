#include "bluetoothdata.h"
//#include <../qtconnectivity-5.2_sailfish_backport/src/bluetooth/qbluetoothsocket.h>

BluetoothData::BluetoothData (QObject *parent):QObject(parent)
{

}

BluetoothData::~BluetoothData ()
{
    delete this->_socket;
}

void BluetoothData::connect(QString address, int port)
{
    this->_port = port;
    qDebug("Trying to connect to: %s_%d", address.toUtf8().constData(), _port);

    delete this->_socket;
    this->_socket = nullptr;

    this->_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    QObject::connect(this->_socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(this->_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(this->_socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(error(QBluetoothSocket::SocketError)));
    QObject::connect(this->_socket, SIGNAL(readyRead()), this, SLOT(readData()));

    qDebug("Connecting...");
    this->_socket->connectToService(QBluetoothAddress(address), this->_port);
}

void BluetoothData::connected()
{
    qDebug() << "Connected";

    emit this->sigConnected();
}
void BluetoothData::disconnected()
{
    qDebug() << "Disconnected!";

    emit this->sigDisconnected();
}
void BluetoothData::error(QBluetoothSocket::SocketError errorCode)
{
    qDebug() << "Error: " << this->_socket->errorString();
    qDebug() << "Errorcode: " << errorCode;

    emit this->sigError(this->_socket->errorString());
}


void BluetoothData::disconnect()
{
    qDebug("Disconnecting...");

    if(!this->_socket)
        return;

    if(this->_socket->isOpen())
        this->_socket->close();

    delete this->_socket;
    this->_socket = nullptr;

    qDebug("Disconnected.");
}

void BluetoothData::readData()
{
    qDebug("Entering readData...");

    QByteArray data = _socket->readAll();   

    qDebug() << "Data size:" << data.size();
    qDebug() << "Data[" + QString::number(_port) + "]:" << data.toHex();

    qDebug() << "Text: " << data;

    emit this->sigReadDataReady(data);
}

void BluetoothData::sendHex(QString sString)
{
    //qDebug() << "sString: " << sString;

    QByteArray data = sString.toUtf8();

    //qDebug() << "data1: " << data.toHex();

    data.append("\r");

    //qDebug() << "data2: " << data.toHex();

    this->write(data);
}

qint64 BluetoothData::write(QByteArray data)
{
    qDebug() << "Writing:" << data.toHex();


    if (!this->_socket || this->_socket->state() != QBluetoothSocket::ConnectedState) {
        emit sigError(QStringLiteral("Bluetooth adapter is not connected"));
        return -1;
    }

    qint64 ret = this->_socket->write(data);


    //qint64 ret = this->_socket->write("ATZ\r");
    //ret = this->_socket->write("AT RV\r");
    //qint64 ret = this->_socket->write("AT L0\r");

    qDebug() << "Write returned:" << ret;



    return ret;
}
