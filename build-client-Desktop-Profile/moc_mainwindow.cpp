/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../client/mainwindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[27];
    char stringdata0[560];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 14), // "on_sockConnect"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 14), // "on_receiveData"
QT_MOC_LITERAL(4, 42, 17), // "on_sockDisconnect"
QT_MOC_LITERAL(5, 60, 12), // "on_sockError"
QT_MOC_LITERAL(6, 73, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(7, 102, 5), // "error"
QT_MOC_LITERAL(8, 108, 24), // "on_userToChose_triggered"
QT_MOC_LITERAL(9, 133, 32), // "on_connectToServerButton_clicked"
QT_MOC_LITERAL(10, 166, 35), // "on_loginLabel_cursorPositionC..."
QT_MOC_LITERAL(11, 202, 4), // "arg1"
QT_MOC_LITERAL(12, 207, 4), // "arg2"
QT_MOC_LITERAL(13, 212, 23), // "on_updateButton_clicked"
QT_MOC_LITERAL(14, 236, 28), // "on_sendMessageButton_clicked"
QT_MOC_LITERAL(15, 265, 50), // "on_messageRecieversOptionList..."
QT_MOC_LITERAL(16, 316, 17), // "createTextBrowser"
QT_MOC_LITERAL(17, 334, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(18, 351, 4), // "user"
QT_MOC_LITERAL(19, 356, 23), // "on_switchButton_clicked"
QT_MOC_LITERAL(20, 380, 25), // "on_gameExitButton_clicked"
QT_MOC_LITERAL(21, 406, 22), // "on_startButton_clicked"
QT_MOC_LITERAL(22, 429, 22), // "on_checkButton_clicked"
QT_MOC_LITERAL(23, 452, 30), // "on_generateFieldButton_clicked"
QT_MOC_LITERAL(24, 483, 27), // "on_applyFieldButton_clicked"
QT_MOC_LITERAL(25, 511, 25), // "on_generateButton_clicked"
QT_MOC_LITERAL(26, 537, 22) // "on_clearButton_clicked"

    },
    "MainWindow\0on_sockConnect\0\0on_receiveData\0"
    "on_sockDisconnect\0on_sockError\0"
    "QAbstractSocket::SocketError\0error\0"
    "on_userToChose_triggered\0"
    "on_connectToServerButton_clicked\0"
    "on_loginLabel_cursorPositionChanged\0"
    "arg1\0arg2\0on_updateButton_clicked\0"
    "on_sendMessageButton_clicked\0"
    "on_messageRecieversOptionList_itemSelectionChanged\0"
    "createTextBrowser\0QListWidgetItem*\0"
    "user\0on_switchButton_clicked\0"
    "on_gameExitButton_clicked\0"
    "on_startButton_clicked\0on_checkButton_clicked\0"
    "on_generateFieldButton_clicked\0"
    "on_applyFieldButton_clicked\0"
    "on_generateButton_clicked\0"
    "on_clearButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x0a /* Public */,
       3,    0,  110,    2, 0x0a /* Public */,
       4,    0,  111,    2, 0x0a /* Public */,
       5,    1,  112,    2, 0x0a /* Public */,
       8,    0,  115,    2, 0x0a /* Public */,
       9,    0,  116,    2, 0x08 /* Private */,
      10,    2,  117,    2, 0x08 /* Private */,
      13,    0,  122,    2, 0x08 /* Private */,
      14,    0,  123,    2, 0x08 /* Private */,
      15,    0,  124,    2, 0x08 /* Private */,
      16,    1,  125,    2, 0x08 /* Private */,
      19,    0,  128,    2, 0x08 /* Private */,
      20,    0,  129,    2, 0x08 /* Private */,
      21,    0,  130,    2, 0x08 /* Private */,
      22,    0,  131,    2, 0x08 /* Private */,
      23,    0,  132,    2, 0x08 /* Private */,
      24,    0,  133,    2, 0x08 /* Private */,
      25,    0,  134,    2, 0x08 /* Private */,
      26,    0,  135,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_sockConnect(); break;
        case 1: _t->on_receiveData(); break;
        case 2: _t->on_sockDisconnect(); break;
        case 3: _t->on_sockError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: _t->on_userToChose_triggered(); break;
        case 5: _t->on_connectToServerButton_clicked(); break;
        case 6: _t->on_loginLabel_cursorPositionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->on_updateButton_clicked(); break;
        case 8: _t->on_sendMessageButton_clicked(); break;
        case 9: _t->on_messageRecieversOptionList_itemSelectionChanged(); break;
        case 10: _t->createTextBrowser((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->on_switchButton_clicked(); break;
        case 12: _t->on_gameExitButton_clicked(); break;
        case 13: _t->on_startButton_clicked(); break;
        case 14: _t->on_checkButton_clicked(); break;
        case 15: _t->on_generateFieldButton_clicked(); break;
        case 16: _t->on_applyFieldButton_clicked(); break;
        case 17: _t->on_generateButton_clicked(); break;
        case 18: _t->on_clearButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
