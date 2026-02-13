
#ifndef NETWORKPROFIL_H
#define NETWORKPROFIL_H
#include <QTcpSocket>
#include <QDataStream>
#include<QTimer>
#include<QUdpSocket>
#include<QNetworkInterface>
#include<QProcess>
#include<QObject>
#include<QSysInfo>
#include<QDir>
#include<QFileSystemWatcher>

class IpMac
{
public:
    QString ip;
    QString mac;
    QString broadcast;
    QString subnet;

};
class NetProfil
{
public:
    QString networkIndex;
    bool selectedNetworkProfil;
    QString networkName;
    QString serverAddress;
    QString ipAddress;
    QString macAddress;
    QString networkBroadCastAddress;
    QString networkTcpPort;
    QString ftpPort;
    QString rootPath;
    QString language;
    bool lockScreenState;
    bool webblockState;

    bool operator==(const NetProfil& other) const
    {
        //networkIndex == other.networkIndex &&
        return selectedNetworkProfil == other.selectedNetworkProfil &&
               networkName == other.networkName &&
               serverAddress == other.serverAddress &&
               ipAddress == other.ipAddress &&
               macAddress == other.macAddress &&
               networkBroadCastAddress == other.networkBroadCastAddress &&
               networkTcpPort == other.networkTcpPort &&
               ftpPort == other.ftpPort &&
               rootPath == other.rootPath &&
               language == other.language &&
               lockScreenState == other.lockScreenState &&
               webblockState == other.webblockState;
    }

    bool operator!=(const NetProfil& other) const
    {
        return !(*this == other);
    }
};


class NewtworkProfil: public QObject
{
  Q_OBJECT
public:
    NewtworkProfil();
    ~NewtworkProfil();

signals:
  public slots:
    void networkProfilLoad();
    bool stringToBool(const QString& str) {
        return str.toLower() == "true"; // Büyük/küçük harf duyarsız karşılaştırma
    }
private slots:
    void udpServerGetSlot();
    void networkProfilSave(NetProfil np);
    void hostAddressMacButtonSlot();
 private:
    QProcess process;
    QList<IpMac> interfaceList;
    QList<NetProfil> NetProfilList;
    QString localDir;
    QString localDir1;
    QString ip;
    QString tcpPort;
    QUdpSocket *udpServerGet = nullptr;
    QFileSystemWatcher networkProfilWather;
};

#endif // NETWORKPROFIL_H
