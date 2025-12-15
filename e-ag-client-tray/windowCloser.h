#pragma once
#include <QObject>
#include <QProcess>
#include <QDebug>

class WindowCloser : public QObject
{
    Q_OBJECT
public:
    explicit WindowCloser(QObject *parent = nullptr) : QObject(parent) {}

    void closeDesktopWindows()
    {
        const QString MY_APP_CLASS = "e-ag-client-gui";

        //
        // 1) NORMAL X11 PENCERELERİ KAPAT
        //
        {
            QProcess listProc;
            listProc.start("bash", QStringList() << "-c" << "wmctrl -lx");
            if (!listProc.waitForFinished(3000)) {
                qWarning() << "wmctrl command failed or timed out";
                return;
            }

            QString out = listProc.readAllStandardOutput();
            QStringList lines = out.split('\n', Qt::SkipEmptyParts);

            for (const QString &line : lines)
            {
                QStringList parts = line.simplified().split(' ');
                if (parts.size() < 3) continue;

                QString winId = parts[0];
                QString wmClass = parts[2];
                QString lower = wmClass.toLower();

                // Kendi uygulamanı kapatma
                if (wmClass == MY_APP_CLASS) continue;

                // xprop ile _NET_WM_STATE kontrolü
                QProcess xpropProc;
                xpropProc.start("bash", QStringList() << "-c" <<
                                            QString("xprop -id %1 _NET_WM_STATE").arg(winId));
                if (!xpropProc.waitForFinished(1000)) continue;

                QString state = xpropProc.readAllStandardOutput();
                if (state.contains("_NET_WM_STATE_SKIP_TASKBAR", Qt::CaseInsensitive)) continue;

                // PANEL / DESKTOP / DOCK filtreleme (güncel)
                if (lower == "xfdesktop" ||
                    lower == "xfdesktop4" ||
                    lower.contains("xfce4-panel") ||
                    lower.contains("gnome-shell") ||
                    lower.contains("cinnamon-panel") ||
                    lower.contains("mate-panel") ||
                    lower.contains("dock"))
                {
                    continue;
                }

                // Kalan normal pencereleri kapat
                QProcess::execute("bash", QStringList() << "-c" <<
                                              QString("wmctrl -ic %1").arg(winId));
            }
        }

        //
        // 2) X11BypassWindowManagerHint pencerelerini PROSES üzerinden kapat
        //
        {
            QProcess psProc;
            psProc.start("bash", QStringList() << "-c" <<
                                     "ps -eo pid,cmd | grep -i qt | grep -v grep");
            if (!psProc.waitForFinished(2000)) return;

            QString out = psProc.readAllStandardOutput();
            QStringList lines = out.split('\n', Qt::SkipEmptyParts);

            for (const QString &line : lines)
            {
                // kendi uygulamanı kapatma
                if (line.contains(MY_APP_CLASS)) continue;

                // bypass flag araması
                if (line.contains("X11BypassWindowManagerHint", Qt::CaseInsensitive) ||
                    line.contains("FramelessWindowHint", Qt::CaseInsensitive) ||
                    line.contains("WindowStaysOnTopHint", Qt::CaseInsensitive))
                {
                    QString pid = line.split(' ', Qt::SkipEmptyParts).first();

                    // Process kill
                    QProcess::execute("kill", QStringList() << "-9" << pid);
                }
            }
        }

        qDebug() << "Desktop windows closed (normal + bypass), except panel/tray apps.";
    }
};
