#ifndef BLUETOOTHCONNECTION_H
#define BLUETOOTHCONNECTION_H

#include <QObject>
#include <QSet>
#include <QTimer>

class BluetoothConnection : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothConnection(QObject *parent = nullptr);
    Q_INVOKABLE void vStartDeviceDiscovery();
    Q_INVOKABLE void vStopDeviceDiscovery();

signals:
    void deviceFound(QString sName, QString sAddress);
    void scanFinished();

private slots:
    void refreshDevices();

private:
    QTimer m_refreshTimer;
    QSet<QString> m_seenAddresses;
    QString m_adapterPath;
    bool m_scanning = false;
};

#endif
