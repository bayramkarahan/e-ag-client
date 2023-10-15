/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[27];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 18), // "webBlockAktifPasif"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "listToFile"
QT_MOC_LITERAL(4, 42, 4), // "path"
QT_MOC_LITERAL(5, 47, 4), // "list"
QT_MOC_LITERAL(6, 52, 8), // "filename"
QT_MOC_LITERAL(7, 61, 10), // "fileToList"
QT_MOC_LITERAL(8, 72, 11), // "listGetLine"
QT_MOC_LITERAL(9, 84, 4), // "data"
QT_MOC_LITERAL(10, 89, 10), // "listRemove"
QT_MOC_LITERAL(11, 100, 12), // "myMessageBox"
QT_MOC_LITERAL(12, 113, 6), // "baslik"
QT_MOC_LITERAL(13, 120, 5), // "mesaj"
QT_MOC_LITERAL(14, 126, 4), // "evet"
QT_MOC_LITERAL(15, 131, 5), // "hayir"
QT_MOC_LITERAL(16, 137, 5), // "tamam"
QT_MOC_LITERAL(17, 143, 17), // "QMessageBox::Icon"
QT_MOC_LITERAL(18, 161, 4), // "icon"
QT_MOC_LITERAL(19, 166, 17), // "cellDoubleClicked"
QT_MOC_LITERAL(20, 184, 4), // "iRow"
QT_MOC_LITERAL(21, 189, 7), // "iColumn"
QT_MOC_LITERAL(22, 197, 24), // "hostAddressMacButtonSlot"
QT_MOC_LITERAL(23, 222, 15), // "komutSudoExpect"
QT_MOC_LITERAL(24, 238, 5), // "komut"
QT_MOC_LITERAL(25, 244, 8), // "username"
QT_MOC_LITERAL(26, 253, 8) // "password"

    },
    "MainWindow\0webBlockAktifPasif\0\0"
    "listToFile\0path\0list\0filename\0fileToList\0"
    "listGetLine\0data\0listRemove\0myMessageBox\0"
    "baslik\0mesaj\0evet\0hayir\0tamam\0"
    "QMessageBox::Icon\0icon\0cellDoubleClicked\0"
    "iRow\0iColumn\0hostAddressMacButtonSlot\0"
    "komutSudoExpect\0komut\0username\0password"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    3,   60,    2, 0x0a /* Public */,
       7,    2,   67,    2, 0x0a /* Public */,
       8,    2,   72,    2, 0x0a /* Public */,
      10,    2,   77,    2, 0x0a /* Public */,
      11,    6,   82,    2, 0x0a /* Public */,
      19,    2,   95,    2, 0x0a /* Public */,
      22,    0,  100,    2, 0x0a /* Public */,
      23,    3,  101,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList, QMetaType::QString,    4,    5,    6,
    QMetaType::QStringList, QMetaType::QString, QMetaType::QString,    4,    6,
    QMetaType::QString, QMetaType::QStringList, QMetaType::QString,    5,    9,
    QMetaType::QStringList, QMetaType::QStringList, QMetaType::QString,    5,    9,
    QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 17,   12,   13,   14,   15,   16,   18,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   20,   21,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   24,   25,   26,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->webBlockAktifPasif(); break;
        case 1: _t->listToFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: { QStringList _r = _t->fileToList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->listGetLine((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QStringList _r = _t->listRemove((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->myMessageBox((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QMessageBox::Icon(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->cellDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->hostAddressMacButtonSlot(); break;
        case 8: _t->komutSudoExpect((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
