#include "hostnamedialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QFile>

HostnameDialog::HostnameDialog(int w, int h, QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(w,h);
    setWindowTitle("Hostname Değiştir");

    QLabel *label = new QLabel("Yeni hostname:");
    hostnameEdit = new QLineEdit;

    // Mevcut hostname'i oku
    QFile file("/etc/hostname");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString current = QString::fromUtf8(file.readAll()).trimmed();
        hostnameEdit->setText(current);
        file.close();
    }

    saveButton = new QPushButton("Kaydet");
    cancelButton = new QPushButton("İptal");

    connect(saveButton, &QPushButton::clicked, this, &HostnameDialog::saveHostname);
    connect(cancelButton, &QPushButton::clicked, this, &HostnameDialog::reject);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(saveButton);
    btnLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addWidget(hostnameEdit);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
}

void HostnameDialog::saveHostname()
{
    QString newHostname = hostnameEdit->text().trimmed();

    if (newHostname.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Hostname boş olamaz!");
        return;
    }

    // Yeni hostname'i set et
    QString cmd = "hostnamectl set-hostname " + newHostname;
    int ret = QProcess::execute("bash", QStringList() << "-c" << cmd);

    if (ret != 0) {
        QMessageBox::critical(this, "Hata", "Hostname değiştirilemedi! Root yetkisi gerekir.");
        return;
    }

    updateHostsFile(newHostname);

    QMessageBox::information(this, "Başarılı",
                             "Hostname başarıyla değiştirildi \n ve\n /etc/hosts güncellendi.");

    accept();
}

void HostnameDialog::updateHostsFile(const QString &newHostname)
{
    QString path = "/etc/hosts";
    QFile f(path);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "/etc/hosts okunamadı!";
        return;
    }

    QStringList lines;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();

        // 127.0.1.1 ile başlayan TÜM satırları SİL (daha sonra yenisini yazacağız)
        if (line.startsWith("127.0.1.1")) {
            continue;
        }

        // 127.1.1.0 gibi hatalı bir şey varsa onu da tamamen yok say
        if (line.startsWith("127.1.1.0")) {
            continue;
        }

        lines << line;
    }
    f.close();

    // En alta doğru satırı ekle
    lines << QString("127.0.1.1\t%1").arg(newHostname);

    // Dosyaya geri yaz
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qWarning() << "/etc/hosts yazılamadı!";
        return;
    }

    QTextStream out(&f);
    for (const QString &l : lines)
        out << l << "\n";

    f.close();
}
