/****************************************************************************
** Meta object code from reading C++ file 'client.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "client.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Client_t {
    QByteArrayData data[26];
    char stringdata0[323];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Client_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Client_t qt_meta_stringdata_Client = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Client"
QT_MOC_LITERAL(1, 7, 10), // "listRemove"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 4), // "list"
QT_MOC_LITERAL(4, 24, 4), // "data"
QT_MOC_LITERAL(5, 29, 10), // "fileToList"
QT_MOC_LITERAL(6, 40, 4), // "path"
QT_MOC_LITERAL(7, 45, 8), // "filename"
QT_MOC_LITERAL(8, 54, 10), // "listToFile"
QT_MOC_LITERAL(9, 65, 11), // "listGetLine"
QT_MOC_LITERAL(10, 77, 15), // "getIpPortStatus"
QT_MOC_LITERAL(11, 93, 7), // "service"
QT_MOC_LITERAL(12, 101, 21), // "tcpMesajSendTimerSlot"
QT_MOC_LITERAL(13, 123, 15), // "komutSudoExpect"
QT_MOC_LITERAL(14, 139, 5), // "komut"
QT_MOC_LITERAL(15, 145, 8), // "username"
QT_MOC_LITERAL(16, 154, 8), // "password"
QT_MOC_LITERAL(17, 163, 18), // "commandExecuteSlot"
QT_MOC_LITERAL(18, 182, 7), // "command"
QT_MOC_LITERAL(19, 190, 16), // "udpSocketGetRead"
QT_MOC_LITERAL(20, 207, 22), // "udpSocketGetMyDispRead"
QT_MOC_LITERAL(21, 230, 21), // "udpSocketGetMyEnvRead"
QT_MOC_LITERAL(22, 252, 20), // "udpSocketSendTServer"
QT_MOC_LITERAL(23, 273, 5), // "_data"
QT_MOC_LITERAL(24, 279, 24), // "hostAddressMacButtonSlot"
QT_MOC_LITERAL(25, 304, 18) // "webBlockAktifPasif"

    },
    "Client\0listRemove\0\0list\0data\0fileToList\0"
    "path\0filename\0listToFile\0listGetLine\0"
    "getIpPortStatus\0service\0tcpMesajSendTimerSlot\0"
    "komutSudoExpect\0komut\0username\0password\0"
    "commandExecuteSlot\0command\0udpSocketGetRead\0"
    "udpSocketGetMyDispRead\0udpSocketGetMyEnvRead\0"
    "udpSocketSendTServer\0_data\0"
    "hostAddressMacButtonSlot\0webBlockAktifPasif"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Client[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   84,    2, 0x0a /* Public */,
       5,    2,   89,    2, 0x0a /* Public */,
       8,    3,   94,    2, 0x0a /* Public */,
       9,    2,  101,    2, 0x0a /* Public */,
      10,    1,  106,    2, 0x0a /* Public */,
      12,    0,  109,    2, 0x0a /* Public */,
      13,    3,  110,    2, 0x0a /* Public */,
      17,    3,  117,    2, 0x08 /* Private */,
      19,    0,  124,    2, 0x08 /* Private */,
      20,    0,  125,    2, 0x08 /* Private */,
      21,    0,  126,    2, 0x08 /* Private */,
      22,    1,  127,    2, 0x08 /* Private */,
      24,    0,  130,    2, 0x08 /* Private */,
      25,    0,  131,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::QStringList, QMetaType::QStringList, QMetaType::QString,    3,    4,
    QMetaType::QStringList, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList, QMetaType::QString,    6,    3,    7,
    QMetaType::QString, QMetaType::QStringList, QMetaType::QString,    3,    4,
    QMetaType::QString, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   18,   15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Client *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QStringList _r = _t->listRemove((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 1: { QStringList _r = _t->fileToList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->listToFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: { QString _r = _t->listGetLine((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->getIpPortStatus((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->tcpMesajSendTimerSlot(); break;
        case 6: _t->komutSudoExpect((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->commandExecuteSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 8: _t->udpSocketGetRead(); break;
        case 9: _t->udpSocketGetMyDispRead(); break;
        case 10: _t->udpSocketGetMyEnvRead(); break;
        case 11: _t->udpSocketSendTServer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->hostAddressMacButtonSlot(); break;
        case 13: _t->webBlockAktifPasif(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Client::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Client.data,
    qt_meta_data_Client,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Client::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Client.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
