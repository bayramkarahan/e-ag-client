#include "inputdevicehelper.h"
#include <QDebug>
#include <QThread>

InputDeviceHelper::InputDeviceHelper(QObject *parent)
    : QObject(parent)
{
    refreshDeviceList();
}

QStringList InputDeviceHelper::getXInputDeviceList()
{
    QProcess proc;
    proc.start("bash", {"-c", "xinput list"});
    proc.waitForFinished();
    QString output = proc.readAllStandardOutput();
    return output.split("\n");
}

DeviceNames InputDeviceHelper::detectInputDevices()
{
    DeviceNames names;
    auto lines = getXInputDeviceList();
    QRegularExpression idRegex("id=([0-9]+)");

    for (const QString &line : lines) {
        if (!line.contains("id=")) continue;

        QRegularExpression nameRegex("↳ (.+?)\\s+id=");
        auto match = nameRegex.match(line);
        if (!match.hasMatch()) continue;
        QString name = match.captured(1).trimmed();

        // Klavye (Virtual core keyboard ignore)
        if (line.contains("keyboard", Qt::CaseInsensitive) &&
            !line.contains("Virtual core", Qt::CaseInsensitive))
        {
            names.keyboards.append(name);
        }

        // Fare / Pointer (Virtual core pointer ignore)
        if ((line.contains("pointer", Qt::CaseInsensitive) ||
             line.contains("mouse", Qt::CaseInsensitive)) &&
            !line.contains("Virtual core", Qt::CaseInsensitive))
        {
            names.pointers.append(name);
        }
    }
    return names;
}

void InputDeviceHelper::refreshDeviceList()
{
    deviceNames = detectInputDevices();
}

// -----------------------------------------
// Device name tabanlı disable/enable
void InputDeviceHelper::disableDeviceByName(const QString &name)
{
    QString cmd = QString("xinput list --id-only \"%1\" | xargs -I{} xinput disable {}").arg(name);
    QProcess::execute("bash", {"-c", cmd});
    QThread::msleep(50); // kısa bekleme bazı cihazlar için
}

void InputDeviceHelper::enableDeviceByName(const QString &name)
{
    QString cmd = QString("xinput list --id-only \"%1\" | xargs -I{} xinput enable {}").arg(name);
    QProcess::execute("bash", {"-c", cmd});
    QThread::msleep(50);
}

// -----------------------------------------
// Klavye işlemleri
void InputDeviceHelper::disableAllKeyboards()
{
    refreshDeviceList();
    for (const QString &name : deviceNames.keyboards)
        disableDeviceByName(name);
}

void InputDeviceHelper::enableAllKeyboards()
{
    refreshDeviceList();
    for (const QString &name : deviceNames.keyboards)
        enableDeviceByName(name);
}

// -----------------------------------------
// Fare işlemleri
void InputDeviceHelper::disableAllPointers()
{
    refreshDeviceList();
    for (const QString &name : deviceNames.pointers)
        disableDeviceByName(name);
}

void InputDeviceHelper::enableAllPointers()
{
    refreshDeviceList();
    for (const QString &name : deviceNames.pointers)
        enableDeviceByName(name);
}
