#ifndef WEBBLOCKWIDGET_H
#define WEBBLOCKWIDGET_H
void MainWindow::webBlockWidget()
{
    // qDebug()<<"ayar click";
    QDialog * d = new QDialog();
    d->setWindowTitle("Web Filtresi");
    d->setFixedSize(QSize(boy*95,boy*50));
    d->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");
    auto appIcon = QIcon(":/icons/e-ag.svg");
    d->setWindowIcon(appIcon);
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - d->width())/2;
    int y = (screenGeometry.height() - d->height()) / 2;
    d->move(x, y);
    /***********************************************************************/
    QTableWidget *twlh=new QTableWidget;

    twlh->setFixedSize(QSize(boy*90,boy*35));
    twlh->setColumnCount(5);
    //twlh->setRowCount(0);
    twlh->setHorizontalHeaderItem(0, new QTableWidgetItem("Seç"));
    twlh->setHorizontalHeaderItem(1, new QTableWidgetItem("Index"));
    twlh->setHorizontalHeaderItem(2, new QTableWidgetItem("Engellenen Kelime"));
    twlh->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    twlh->setHorizontalHeaderItem(4, new QTableWidgetItem(""));

    twlh->setSelectionBehavior(QAbstractItemView::SelectRows);
    twlh->setSelectionMode(QAbstractItemView::SingleSelection);
    //connect(tw, &QTableWidget::cellClicked, this, cellClicked());
    twlh->setRowCount(0);
    twlh->setColumnWidth(0, boy*10);
    twlh->setColumnWidth(1, boy*7);
    twlh->setColumnWidth(2,boy*35);
    twlh->setColumnWidth(3,boy*17);
    twlh->setColumnWidth(4,boy*12);

    DatabaseHelper *db=new DatabaseHelper(localDir+"webblockserver.json");
    QJsonArray dizi=db->Oku();
    int sr=0;

    for (const QJsonValue &item : dizi) {
        QJsonObject veri=item.toObject();

        twlh->setRowCount(twlh->rowCount()+1);
        QCheckBox *mCheck = new QCheckBox();
        mCheck->setFixedWidth(boy*15);
        mCheck->setChecked(false);
        QLineEdit * index = new QLineEdit();
        QLineEdit * word = new QLineEdit();
        QToolButton *saveButton= new QToolButton;
        saveButton->setText("Kelimeyi Kaydet");
        saveButton->setFixedWidth(boy*17);
        connect(saveButton, &QPushButton::clicked, [=]() {
            //qDebug()<<"Değişiklikler Kaydedildi.."<<insertButton->toolTip();
            int numRows = twlh->rowCount();
            for ( int i = 0 ; i < numRows ; i++)
            {
                QCheckBox* mBox = static_cast<QCheckBox*> (twlh->cellWidget(i,0));
                QLineEdit * index = static_cast<QLineEdit*> (twlh->cellWidget(i,1));
                QLineEdit * word = static_cast<QLineEdit*> (twlh->cellWidget(i,2));
                if (index->text()==saveButton->toolTip())
                {
                    QJsonArray dizi=db->Ara("index",saveButton->toolTip());
                    if(dizi.count()>0)
                    {
                        qDebug()<<"Kelime Değiştirilecek."<<saveButton->toolTip();
                        QJsonObject veri;
                        if (mBox->isChecked()) veri["selectedWord"] =true;
                        else veri["selectedWord"] =false;
                        veri["index"] = index->text();
                        veri["word"] = word->text();
                        //qDebug()<<"kelime kayıt"<<veri;
                        db->Sil("index",index->text());
                        db->Ekle(veri);
                    }
                }
            }
            d->close();
            webBlockWidget();
        });
        QToolButton *removeButton= new QToolButton;
        removeButton->setText("Profili Sil");
        removeButton->setFixedWidth(boy*12);
        connect(removeButton, &QPushButton::clicked, [=]() {
            //qDebug()<<"Profil Silindi.."<<networkRemoveButton->toolTip();
            QJsonArray dizi=db->Ara("networkIndex",removeButton->toolTip());
            qDebug()<<"Web Kelime Silinecek."<<removeButton->toolTip();
            db->Sil("index",index->text());
            d->close();
            webBlockWidget();
        });



        index->setText(veri.value("index").toString());
        index->setReadOnly(true);
        word->setText(veri.value("word").toString());
        saveButton->setToolTip(index->text());
        twlh->setCellWidget(sr,0,mCheck);
        twlh->setCellWidget(sr,1,index);
        twlh->setCellWidget(sr,2,word);
        twlh->setCellWidget(sr,3,saveButton);
        twlh->setCellWidget(sr,4,removeButton);

        //qDebug()<<"Kayıtlı Host.";
        if(veri.value("selectedWord").toBool())
            mCheck->setChecked(true);
        else
            mCheck->setChecked(false);
        sr++;
    }

    /********************************************************************/
    QToolButton *insertWordButton= new QToolButton;
    insertWordButton->setFixedSize(QSize(boy*40,boy*10));
    insertWordButton->setIconSize(QSize(boy*40,boy*5));
    insertWordButton->setStyleSheet("Text-align:center");
    insertWordButton->setIcon(QIcon(":/icons/add.svg"));
    insertWordButton->setAutoRaise(true);
    insertWordButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // newNetworkButton->setFont(f2);
    insertWordButton->setText("Yeni Profil Ekle");

    connect(insertWordButton, &QPushButton::clicked, [=]() {
        DatabaseHelper *db=new DatabaseHelper(localDir+"webblockserver.json");
        QJsonObject veri;
        veri["index"] =QString::number(db->getIndex("index"));
        if(db->Oku().size()==0) veri["selectedWord"] =true;
        else veri["selectedWord"] =false;
        veri["word"] = "sample";
        db->Ekle(veri);
        d->close();
        webBlockWidget();
    });

    QToolButton *webAyarGuncelleButton= new QToolButton;
    webAyarGuncelleButton->setFixedSize(QSize(en*40,en*10));
    webAyarGuncelleButton->setIconSize(QSize(en*40,en*5));
    webAyarGuncelleButton->setStyleSheet("Text-align:center");
    webAyarGuncelleButton->setIcon(QIcon(":/icons/webblock.svg"));
    webAyarGuncelleButton->setAutoRaise(true);
    webAyarGuncelleButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // webAyarGuncelleButton->setFont(f2);
    webAyarGuncelleButton->setText("Web Filtre Ayarlarını Güncelle");

    connect(webAyarGuncelleButton, &QPushButton::clicked, [=]() {
    udpGuiServerSend = new QUdpSocket();
    QString msg="webblocktrue";
    QByteArray datagram = msg.toUtf8();// +QHostAddress::LocalHost;
    udpGuiServerSend->writeDatagram(datagram,QHostAddress::LocalHost, 51521);
    qDebug()<<"console  gönderildi:"<<msg;

    });
    /*********************************************************************/

    QVBoxLayout * vbox = new QVBoxLayout();
    vbox->addWidget(twlh);
    QHBoxLayout * hbox = new QHBoxLayout();
    hbox->addWidget(insertWordButton);
    hbox->addWidget(webAyarGuncelleButton);

    vbox->addLayout(hbox);

    d->setLayout(vbox);
    d->exec();


}


#endif // WEBBLOCKWIDGET_H
