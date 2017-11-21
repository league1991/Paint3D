/****************************************************************************
** Meta object code from reading C++ file 'Paint3DFrame.h'
**
** Created: Tue Nov 21 21:07:11 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../Paint3DFrame.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Paint3DFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Paint3DFrame[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      28,   13,   13,   13, 0x08,
      41,   13,   13,   13, 0x08,
      58,   13,   13,   13, 0x08,
      69,   13,   13,   13, 0x08,
      82,   13,   13,   13, 0x08,
      94,   13,   13,   13, 0x08,
     106,   13,   13,   13, 0x08,
     118,   13,   13,   13, 0x08,
     129,   13,   13,   13, 0x08,
     140,   13,   13,   13, 0x08,
     150,   13,   13,   13, 0x08,
     164,   13,   13,   13, 0x08,
     185,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Paint3DFrame[] = {
    "Paint3DFrame\0\0importModel()\0selectTool()\0"
    "selectFaceTool()\0moveTool()\0rotateTool()\0"
    "scaleTool()\0focusTool()\0paintTool()\0"
    "openFile()\0saveFile()\0newFile()\0"
    "refreshView()\0createPlaneLocator()\0"
    "showAboutWindow()\0"
};

void Paint3DFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Paint3DFrame *_t = static_cast<Paint3DFrame *>(_o);
        switch (_id) {
        case 0: _t->importModel(); break;
        case 1: _t->selectTool(); break;
        case 2: _t->selectFaceTool(); break;
        case 3: _t->moveTool(); break;
        case 4: _t->rotateTool(); break;
        case 5: _t->scaleTool(); break;
        case 6: _t->focusTool(); break;
        case 7: _t->paintTool(); break;
        case 8: _t->openFile(); break;
        case 9: _t->saveFile(); break;
        case 10: _t->newFile(); break;
        case 11: _t->refreshView(); break;
        case 12: _t->createPlaneLocator(); break;
        case 13: _t->showAboutWindow(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Paint3DFrame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Paint3DFrame::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Paint3DFrame,
      qt_meta_data_Paint3DFrame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Paint3DFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Paint3DFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Paint3DFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Paint3DFrame))
        return static_cast<void*>(const_cast< Paint3DFrame*>(this));
    if (!strcmp(_clname, "Ui::MainWindow"))
        return static_cast< Ui::MainWindow*>(const_cast< Paint3DFrame*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Paint3DFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
