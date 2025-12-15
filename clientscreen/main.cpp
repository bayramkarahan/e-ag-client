#include "mainwindow.h"
#include <QApplication>
#include<iostream>
#include<QDebug>
int main(int argc, char *argv[])
{
    std::vector<std::string> args;
    std::transform(argv, argv + argc, std::back_inserter(args), [](const char* arg) {
        return std::string(arg);
    });
    if(args.size()<2)
    {
        std::cout << " 239.*.*.*ip Yazmalısın."<< std::endl;
        return 0;
    }
    std::string newIp=args[1];
    QApplication a(argc, argv);
    QString ipp=QString::fromStdString(newIp);
    qDebug()<<"ip: "<<ipp;
    MainWindow w(ipp);
    //w.ip=QString::fromStdString(newIp);
    w.show();
    return a.exec();
}
