#ifndef HOSTNAMEDIALOG_H
#define HOSTNAMEDIALOG_H

#include <QDialog>
#include<QTextStream>
#include<QDebug>
class QLineEdit;
class QPushButton;
class QLabel;

class HostnameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HostnameDialog(int w,int h,QWidget *parent = nullptr);

private slots:
    void saveHostname();
    void updateHostsFile(const QString &newHostname);
private:
    QLineEdit *hostnameEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
};

#endif // HOSTNAMEDIALOG_H
