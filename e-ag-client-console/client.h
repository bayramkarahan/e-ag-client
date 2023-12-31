
#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include<filecrud.h>
#include <QDataStream>
#include<QTimer>
#include<QUdpSocket>
#include<QNetworkInterface>
#include<QProcess>
#include<QObject>
#include<QSysInfo>
class IpMac
{
public:
    QString ip;
    QString mac;
    QString broadcast;
     QString subnet;

};

class Client: public QObject
{
  Q_OBJECT
public:
    Client();
    ~Client();
signals:
  public slots:
    QStringList listRemove(QStringList list,QString data);
    QStringList fileToList(QString path,QString filename);
    void listToFile(QString path,QStringList list, QString filename);
    QString listGetLine(QStringList list,QString data);
    QString  getIpPortStatus(QString service, int number);
    void tcpMesajSendTimerSlot();
    void komutSudoExpect(QString komut,QString username,QString password);
private slots:
     void commandExecuteSlot(QString command,QString username,QString password);
    void udpSocketGetRead();
     void udpSocketGetMyDispRead();
      void udpSocketGetMyEnvRead();
    void udpSocketSendTServer(QString _data);
   // void timerControlSlot();
    void hostAddressMacButtonSlot();
    void webBlockAktifPasif();
    QString getSeatId();
    QString getSessionInfo(QString id, QString parametre);
    //QString getIpPort///Status(QString ip_,QString prt);
private:
    bool webblockstate;
    QString rootusername;
    QString rootpassword;
    QProcess process;
    QTimer *timer;
    QTimer *timerControl;
    QList<IpMac> ipmaclist;
    QString localDir;
    QString localDir1;
    QString ip;
    QString tcpPort;
    QUdpSocket *udpSocketSend = nullptr;
    QUdpSocket *udpSocketGet = nullptr;
    QUdpSocket *udpSocketGetMyDisp = nullptr;
    QUdpSocket *udpSocketGetMyEnv = nullptr;
    QUdpSocket *udpSocketSendX11Mesaj = nullptr;
    QString x11env="";
    QString myenv="";
    QString tempdata="";
    int dataSayac=0;
    QString seatId;
    QString sessionUser;
    QString sessionUserId;
    QString sessionDisplay;
    QString sessionDisplayType;
    QString sessionDesktopManager;
    int hostportCounter;


};

#endif // CLIENT_H
