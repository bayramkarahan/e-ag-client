#include <QApplication>

#include <iostream>
#include<QHostAddress>
#include<QNetworkInterface>
#include<QNetworkAddressEntry>
#include<QProcess>
#include<QScreen>
class IpMac
{
public:
    QString ip;
    QString mac;
    QString broadcast;

};
QList<IpMac> ipmaclist;

void hostAddressMacButtonSlot()
{
    QHostAddress localhost = QHostAddress(QHostAddress::LocalHost);
    ipmaclist.clear();
    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
            QHostAddress *hostadres=new QHostAddress(entry.ip().toString());
            if(hostadres->protocol() == QAbstractSocket::IPv4Protocol &&
                !hostadres->isLoopback() )
            {
                IpMac im;
                im.ip=entry.ip().toString();
                im.mac=networkInterface.hardwareAddress();
                im.broadcast=entry.broadcast().toString();
                ipmaclist.append(im);

                // qDebug()<<"mac:"<<networkInterface.hardwareAddress();
                //qDebug()<<"ip  address:"<<entry.ip().toString();
                // qDebug()<<"broadcast  address:"<<entry.broadcast().toString();
            }
        }
    }
    //qDebug()<<"ip  address:"<<ipmaclist.count();


}
int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    if (argc < 2) {
        qDebug() << "Kullanım: program <target_ip>";
        return -1;
    }

    QString newIp = argv[1]; // Tek IP dışarıdan alınıyor
    qDebug()<<newIp;
    QStringList ipparts=newIp.split(".");
    QString targetIp="239.0."+ipparts[2]+"."+ipparts[3];
    qDebug() << "Hedef IP:" << targetIp;

    QProcess *ffmpegProcess = new QProcess();

    QSize screenSize = QGuiApplication::primaryScreen()->size();
    QString videoSize = QString("%1x%2").arg(screenSize.width()).arg(screenSize.height());



#ifdef Q_OS_WIN
        QString command = "ffmpeg.exe";
        QStringList arguments = {
            "-f", "gdigrab",               // Windows için ekran yakalama
            "-framerate", "30",
            "-video_size", "1280x800",
            "-i", "desktop",
            "-f", "dshow",                // Windows mikrofon
            "-i", "audio=Microphone",
#else
        QString command = "ffmpeg";

        QStringList arguments = {
            "-f", "x11grab",              // Linux için ekran yakalama
            "-framerate", "10",
            "-video_size",videoSize,
            "-i", ":0.0",
#endif
            "-vcodec", "libx264",
            "-preset", "ultrafast",
            "-tune", "zerolatency",
            "-pix_fmt", "yuv420p",
            "-s", "800x600",
            "-b:v", "1500k",
            "-bufsize", "0",
            "-flush_packets", "1",
            "-fflags", "nobuffer",
            "-g", "10",
            "-keyint_min", "10",
            "-f", "mpegts",
            "udp://"+targetIp+":1234?pkt_size=1316&ttl=1"
        };

        ffmpegProcess->start(command, arguments);
        if (!ffmpegProcess->waitForStarted()) {
            qDebug() << "FFmpeg başlatılamadı!";
            return -1;
        } else {
            qDebug() << "Yayın başladı.";
        }

        return a.exec();
}
