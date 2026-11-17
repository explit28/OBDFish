#ifndef BLUETOOTHDATA_H
#define BLUETOOTHDATA_H

#include <QObject>
#include <QSocketNotifier>

class BluetoothData : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothData(QObject *parent = nullptr);
    ~BluetoothData() override;
    Q_INVOKABLE void connect(QString address, int port);
    Q_INVOKABLE void sendHex(QString sString);
    Q_INVOKABLE void disconnect();

signals:
    void sigReadDataReady(QString sData);
    void sigConnected();
    void sigDisconnected();
    void sigError(QString sError);

private:
    void readData();
    void finishConnect();
    void fail(const QString &message);
    qint64 write(const QByteArray &data);

    int m_fd = -1;
    bool m_connected = false;
    bool m_connecting = false;
    QSocketNotifier *m_readNotifier = nullptr;
    QSocketNotifier *m_writeNotifier = nullptr;
};

#endif
