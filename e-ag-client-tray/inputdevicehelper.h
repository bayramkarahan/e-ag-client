#ifndef INPUTDEVICEHELPER_H
#define INPUTDEVICEHELPER_H

#include <QObject>
#include <QStringList>
#include <QProcess>
#include <QRegularExpression>

struct DeviceNames {
    QStringList keyboards;
    QStringList pointers;
};

class InputDeviceHelper : public QObject
{
    Q_OBJECT
public:
    // Singleton erişimi
    static InputDeviceHelper& instance()
    {
        static InputDeviceHelper _instance;
        return _instance;
    }

    // Ana fonksiyonlar
    void refreshDeviceList();

    void disableAllKeyboards();
    void enableAllKeyboards();

    void disableAllPointers();
    void enableAllPointers();

private:
    explicit InputDeviceHelper(QObject *parent = nullptr);

    // Kopyalama yasak
    InputDeviceHelper(const InputDeviceHelper&) = delete;
    InputDeviceHelper& operator=(const InputDeviceHelper&) = delete;

    // Helper fonksiyonlar
    QStringList getXInputDeviceList();
    DeviceNames detectInputDevices();
    void disableDeviceByName(const QString &name);
    void enableDeviceByName(const QString &name);

private:
    DeviceNames deviceNames;
};

#endif // INPUTDEVICEHELPER_H
