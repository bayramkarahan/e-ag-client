/****************************************************************************
** Meta object code from reading C++ file 'networkprofil.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../e-ag-client-networkprofil/networkprofil.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkprofil.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NewtworkProfil_t {
    QByteArrayData data[10];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NewtworkProfil_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NewtworkProfil_t qt_meta_stringdata_NewtworkProfil = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NewtworkProfil"
QT_MOC_LITERAL(1, 15, 17), // "networkProfilLoad"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 12), // "stringToBool"
QT_MOC_LITERAL(4, 47, 3), // "str"
QT_MOC_LITERAL(5, 51, 16), // "udpServerGetSlot"
QT_MOC_LITERAL(6, 68, 17), // "networkProfilSave"
QT_MOC_LITERAL(7, 86, 9), // "NetProfil"
QT_MOC_LITERAL(8, 96, 2), // "np"
QT_MOC_LITERAL(9, 99, 24) // "hostAddressMacButtonSlot"

    },
    "NewtworkProfil\0networkProfilLoad\0\0"
    "stringToBool\0str\0udpServerGetSlot\0"
    "networkProfilSave\0NetProfil\0np\0"
    "hostAddressMacButtonSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NewtworkProfil[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    1,   40,    2, 0x0a /* Public */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    1,   44,    2, 0x08 /* Private */,
       9,    0,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

       0        // eod
};

void NewtworkProfil::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NewtworkProfil *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->networkProfilLoad(); break;
        case 1: { bool _r = _t->stringToBool((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->udpServerGetSlot(); break;
        case 3: _t->networkProfilSave((*reinterpret_cast< NetProfil(*)>(_a[1]))); break;
        case 4: _t->hostAddressMacButtonSlot(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NewtworkProfil::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NewtworkProfil.data,
    qt_meta_data_NewtworkProfil,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NewtworkProfil::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NewtworkProfil::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NewtworkProfil.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NewtworkProfil::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
