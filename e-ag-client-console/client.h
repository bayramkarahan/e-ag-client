
#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QDataStream>
#include<QTimer>
#include<QUdpSocket>
#include<QNetworkInterface>
#include<QProcess>
#include<QObject>
#include<QSysInfo>
#include<QDir>
#include<QJsonObject>
#include <QFileSystemWatcher>
#include<userprivilegehelper.h>
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
};

class Client: public QObject
{
  Q_OBJECT
public:
    Client();
    ~Client();
    bool webblockStateRun=false;
    QList<NetProfil> NetProfilList;
    void mergeJson(QJsonObject &dest, const QJsonObject &src) {
        for (auto it = src.begin(); it != src.end(); ++it) {
            dest[it.key()] = it.value();  // anahtar varsa günceller, yoksa ekler
        }
    }
signals:
  public slots:
    QString  getIpPortStatus(QString service, int number);
      void tcpMesajSendTimerSlot(bool commandDetailStatus, QString command, QString commandDetail, QString commandStatus);
    void socketBaglama();
    void networkProfilLoad();
    bool stringToBool(const QString& str) {
        return str.toLower() == "true"; // Büyük/küçük harf duyarsız karşılaştırma
    }
private slots:
    void commandExecuteSlot(QString command, QString komutMesaji, QString mesajVisible);
    void udpServerGetSlot();
    bool uygulamaCalisiyorMu(const QString& uygulamaAdi);
    void udpTrayGetSlot();
    void udpGuiGetSlot();
    //void udpServerSendSlot(QString _data, bool sendStatus);
    void udpServerSendSlot(const QJsonObject &mainJson, bool sendStatus);

    void hostAddressMacButtonSlot();
    void webBlockAktifPasif(bool _state);
    QString findX11vncPort(QString _servis);
    QString getSeatId();
    QString getSessionInfo(QString id, QString parametre);
    void clientConfUpdate(QString field,bool state);
    void clientConfLoad();
    void closeSockets();
private:
    QTimer *tcpMesajSendTimer;
    QProcess process;
    QTimer *timer;
    QTimer *timerControl;
    QList<IpMac> interfaceList;

    QString localDir;
    QString localDir1;
    QString ip;

    QUdpSocket *udpServerSend = nullptr;
    QUdpSocket *udpServerGet = nullptr;
    QUdpSocket *udpTraySend = nullptr;
    QUdpSocket *udpTrayGet = nullptr;
    QUdpSocket *udpGuiGet = nullptr;

    QString cclientTrayEnv="";
    QJsonObject trayEnv;
    QJsonObject consoleEnv;
    QString clientGuiEnv="";
     QString cclientConsoleEnv="";
    QString tempdata="";
    int dataSayac=0;
    QString seatId;
    QString sessionUser;
    QString sessionUserId;
    QString sessionDisplay;
    QString sessionDisplayType;
    QString sessionDesktopManager;
    int hostportCounter;
    bool udpServerGetStatus=false;
    bool volumeState=true;
    bool keyboardState=true;
    bool mouseState=true;
    bool internetState=true;
    bool webblockState=false;
    bool onlywebblockState=false;
    bool youtubeState=true;
    QFileSystemWatcher clientConfWather;

    QString hostname;
    QFileSystemWatcher networkProfilWather;
};

#endif // CLIENT_H
