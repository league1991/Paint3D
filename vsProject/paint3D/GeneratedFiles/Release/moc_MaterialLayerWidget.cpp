/****************************************************************************
** Meta object code from reading C++ file 'MaterialLayerWidget.h'
**
** Created: Mon Sep 22 21:18:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../MaterialLayerWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MaterialLayerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MaterialLayerWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      34,   20,   20,   20, 0x0a,
      45,   20,   20,   20, 0x0a,
      59,   20,   20,   20, 0x0a,
      73,   20,   20,   20, 0x0a,
      98,   89,   20,   20, 0x0a,
     115,   20,   20,   20, 0x0a,
     136,   20,   20,   20, 0x0a,
     155,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MaterialLayerWidget[] = {
    "MaterialLayerWidget\0\0updateList()\0"
    "addLayer()\0deleteLayer()\0moveUpLayer()\0"
    "moveDownLayer()\0ithLayer\0selectLayer(int)\0"
    "loadLayerFromImage()\0saveLayerToImage()\0"
    "saveCanvasToImage()\0"
};

void MaterialLayerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MaterialLayerWidget *_t = static_cast<MaterialLayerWidget *>(_o);
        switch (_id) {
        case 0: _t->updateList(); break;
        case 1: _t->addLayer(); break;
        case 2: _t->deleteLayer(); break;
        case 3: _t->moveUpLayer(); break;
        case 4: _t->moveDownLayer(); break;
        case 5: _t->selectLayer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->loadLayerFromImage(); break;
        case 7: _t->saveLayerToImage(); break;
        case 8: _t->saveCanvasToImage(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MaterialLayerWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MaterialLayerWidget::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_MaterialLayerWidget,
      qt_meta_data_MaterialLayerWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MaterialLayerWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MaterialLayerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MaterialLayerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MaterialLayerWidget))
        return static_cast<void*>(const_cast< MaterialLayerWidget*>(this));
    if (!strcmp(_clname, "Ui::MaterialLayer"))
        return static_cast< Ui::MaterialLayer*>(const_cast< MaterialLayerWidget*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int MaterialLayerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
