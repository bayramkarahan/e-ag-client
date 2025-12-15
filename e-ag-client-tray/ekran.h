#ifndef EKRAN_H
#define EKRAN_H
#include <QCloseEvent>
#include<QWidget>
#include<QLabel>
#include<QDesktopWidget>
#include<QRect>
#include<QDebug>
#include <QApplication>
#include <QScreen>
#include<QToolButton>
#include<MyCommand.h>
#include<QMessageBox>
class Ekran : public QWidget
{
    Q_OBJECT
public:
    QString command="";
    QString commandDetail="";
    QString commandState="0";
    QString commandMessage="";
    QString commandVisible="";
private:
     QLabel *basliktext;
     QLabel *mesajtext;
     QLabel *logo;
     QToolButton *kapatButton;
     QToolButton *commandDetailButton;

     void closeEvent(QCloseEvent *bar)
    {
        // Do something
       // bar->accept();
          bar->ignore();
          hide();
    }

public:
     Ekran()
     {
         setWindowFlags(Qt::FramelessWindowHint);
         setWindowFlags(Qt::WindowStaysOnTopHint);
         setWindowFlags(Qt::X11BypassWindowManagerHint);

         setAttribute(Qt::WA_StaticContents);
         setAttribute(Qt::WA_TranslucentBackground, true);
         basliktext=new QLabel(this);
         mesajtext=new QLabel(this);
         logo=new QLabel(this);
         kapatButton= new QToolButton(this);
         kapatButton->hide();
         commandDetailButton= new QToolButton(this);
         commandDetailButton->hide();

        logo->hide();

        }
     void ekranKomutMesaj()
     {
         QDesktopWidget widget;
         QRect desktopScreenSize = widget.availableGeometry(widget.primaryScreen());
         this->setFixedSize(desktopScreenSize.width()*0.5,40);
         this->move(desktopScreenSize.width()-this->width(),0);
         this->setWindowTitle("Mesaj");
         this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                              | Qt::X11BypassWindowManagerHint);

         this->setAttribute(Qt::WA_TranslucentBackground, true);
         this->setAttribute(Qt::WA_NoSystemBackground, false);
         QPalette palet;
         if(commandState=="0")
            palet.setBrush(QPalette::Background, QColor(0,150,0,100));
         else
            palet.setBrush(QPalette::Background, QColor(150,0,0,100));
         this->setPalette(palet);

         mesajtext->resize(this->width(),40);
         mesajtext->move(0,0);
         mesajtext->setText("<center><font size=4>"+commandMessage+"</font></center>");

         commandDetailButton->setFixedSize(QSize(80,40));
         commandDetailButton->setStyleSheet("Text-align:center");
         commandDetailButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
         commandDetailButton->setText(">_");
         commandDetailButton->move(desktopScreenSize.width()*0.5-120,0);
         commandDetailButton->show();
         connect(commandDetailButton, &QToolButton::clicked, [=]() {
            MyCommand *dlg= new MyCommand(tr("Komut Süreci"),QString(commandMessage+"\n"+command),commandDetail,commandState,"", "", "", desktopScreenSize.width()*0.6, desktopScreenSize.height()*0.5,this);
            dlg->exec();
            //delete dlg;
            dlg = nullptr;  // Güvenlik için
            this->close();
          });

         kapatButton->setFixedSize(QSize(40,40));
         kapatButton->setStyleSheet("Text-align:center");
         kapatButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
         kapatButton->setText("X");
         kapatButton->move(desktopScreenSize.width()*0.5-40,0);
         kapatButton->show();
         connect(kapatButton, &QToolButton::clicked, [=]() {
             this->close();
         });


        //this->show();
     }

     QString myMessageBox(QString baslik, QString mesaj, QString evet, QString hayir, QString tamam,int _w,int _h,QMessageBox::Icon icon)
     {
         Qt::WindowFlags flags = 0;
         flags |= Qt::Dialog;
         flags |= Qt::X11BypassWindowManagerHint;

         QMessageBox messageBox(this);
         messageBox.setFixedSize(_w, _h);
         messageBox.setWindowFlags(flags);
         messageBox.setText(baslik+"\t\t\t");
         messageBox.setDetailedText(mesaj);
         QAbstractButton *evetButton;
         QAbstractButton *hayirButton;
         QAbstractButton *tamamButton;

         if(evet=="evet") evetButton =messageBox.addButton(tr("Evet"), QMessageBox::ActionRole);
         if(hayir=="hayir") hayirButton =messageBox.addButton(tr("Hayır"), QMessageBox::ActionRole);
         if(tamam=="tamam") tamamButton =messageBox.addButton(tr("Tamam"), QMessageBox::ActionRole);

         messageBox.setIcon(icon);
         messageBox.exec();
         if(messageBox.clickedButton()==evetButton) return "evet";
         if(messageBox.clickedButton()==hayirButton) return "hayır";
         if(messageBox.clickedButton()==tamamButton) return "tamam";
         return "";
     }

     void ekranMesaj(QString baslik,QString mesaj)
    {
        QDesktopWidget widget;
        QRect desktopScreenSize = widget.availableGeometry(widget.primaryScreen());
        this->setFixedSize(desktopScreenSize.width()*0.8,200);
       // qDebug()<<"ekran boyut:"<<desktopScreenSize.width();
        this->move(desktopScreenSize.width()/2-desktopScreenSize.width()*0.8/2,200);
       //this->setStyleSheet("background-color: #a3acac");
        this->setWindowTitle("Mesaj");
        this->setAttribute(Qt::WA_TranslucentBackground, true);
        this->setAttribute(Qt::WA_NoSystemBackground, false);
        //this->setAttribute(Qt::WA_NoBackground,true);
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                             | Qt::X11BypassWindowManagerHint);

        this->setAttribute(Qt::WA_TranslucentBackground, true);
        this->setAttribute(Qt::WA_NoSystemBackground, false);
        this->repaint();
        QPalette palet;
        palet.setBrush(QPalette::Background, QColor(163,172,172,255));
        this->setPalette(palet);  //setZeminColor(myZeminColor);

        basliktext->resize(desktopScreenSize.width()*0.8,100);
        basliktext->move(0,0);
        basliktext->setText("<center><font size=8>"+baslik+"</font></center>");

        mesajtext->resize(desktopScreenSize.width()*0.8,70);
        mesajtext->move(0,100);
        mesajtext->setText("<center><font size=8>"+mesaj+"</font></center>");


        kapatButton->setFixedSize(QSize(150,30));
       // kapatButton->setIconSize(QSize(boy,boy));
        kapatButton->setStyleSheet("Text-align:center");
      //  kapatButton->setIcon(QIcon(":/icons/save.svg"));
       /// kapatButton->setAutoRaise(true);
        kapatButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
       // kapatButton->setFont(f2);
        kapatButton->setText("Mesajı Kapat");
        kapatButton->move(desktopScreenSize.width()*0.8/2-75,170);
        kapatButton->show();
        connect(kapatButton, &QToolButton::clicked, [=]() {
            this->close();
        });


    }

     void ekranKilit(QString baslik,QString mesaj)
    {
         kapatButton->hide();
          QSize screenSize = qApp->screens()[0]->size();
         this->setFixedSize(screenSize.width(),screenSize.height());

        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                             | Qt::X11BypassWindowManagerHint);



       // qDebug()<<"ekran boyut:"<<desktopScreenSize.width();
        this->move(0,0);
        //this->setStyleSheet("background-color: #a3acac");

        this->setAttribute(Qt::WA_TranslucentBackground, true);
        this->setAttribute(Qt::WA_NoSystemBackground, false);
        //this->setAttribute(Qt::WA_NoBackground,true);
        this->repaint();
        QPalette palet;
        palet.setBrush(QPalette::Background,QColor(163,172,172,255));
        this->setPalette(palet);  //setZeminColor(myZeminColor);

        this->setWindowTitle("Mesaj");

        basliktext->resize(screenSize.width(),100);
        basliktext->move(0,0);
        basliktext->setText("<center><font size=8>"+baslik+"</font></center>");

        mesajtext->resize(screenSize.width(),100);
        mesajtext->move(0,100);
        mesajtext->setText("<center><font size=8>"+mesaj+"</font></center>");

        logo->setFixedSize(screenSize.height()-250,screenSize.height()-250);
        logo->move(screenSize.width()/2-(screenSize.height()-250)/2,200);
        logo->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        logo->setScaledContents( true );
        logo->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        logo->setPixmap(QPixmap(":/icons/lock.png" ) );
        logo->show();
    }

     void ekranTransparanKilit()
    {
         kapatButton->hide();
        QDesktopWidget widget;

        QRect desktopScreenSize = widget.availableGeometry(widget.primaryScreen());
        this->setFixedSize(desktopScreenSize.width(),desktopScreenSize.height()+35);
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                             | Qt::X11BypassWindowManagerHint);

        this->setAttribute(Qt::WA_TranslucentBackground, true);
        this->setAttribute(Qt::WA_NoSystemBackground, false);
        //this->setAttribute(Qt::WA_NoBackground,true);
        this->repaint();
        QPalette palet;
        palet.setBrush(QPalette::Background, QColor(0,0,0,1));
        this->setPalette(palet);  //setZeminColor(myZeminColor);

         basliktext->resize(desktopScreenSize.width(),100);
         basliktext->move(0,0);
         basliktext->setText("<center><font size=8>Ekran Kilitlendi</font></center>");

         mesajtext->resize(desktopScreenSize.width(),100);
         mesajtext->move(0,100);
         mesajtext->setText("");


        this->move(0,0);
         logo->setFixedSize(desktopScreenSize.height()-250,desktopScreenSize.height()-250);
         logo->move(desktopScreenSize.width()/2-(desktopScreenSize.height()-250)/2,200);
         logo->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
         logo->setScaledContents( true );
         logo->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
         logo->setPixmap(QPixmap(":/icons/transparanlock.png" ) );
         logo->hide();




    }

    void manageCtrlAltDelete(bool lockState)
    {
        QString desktop = qgetenv("XDG_CURRENT_DESKTOP").toLower();
        bool isCinnamon = desktop.contains("cinnamon");
        bool isGnome    = desktop.contains("gnome");

        if (lockState) {
            // Kilit aktif: Ctrl+Alt+Delete devre dışı
            if (isCinnamon)
                system("gsettings set org.cinnamon.desktop.keybindings.media-keys logout \"[]\"");
            else if (isGnome)
                system("gsettings set org.gnome.settings-daemon.plugins.media-keys logout \"[]\"");
        } else {
            // Kilit kaldırıldı: eski haline geri al
            if (isCinnamon)
                system("gsettings set org.cinnamon.desktop.keybindings.media-keys logout \"['<Control><Alt>Delete']\"");
            else if (isGnome)
                system("gsettings reset org.gnome.settings-daemon.plugins.media-keys logout");
        }
    }



};



#endif // EKRAN_H
