/****************************************************************************
** Meta object code from reading C++ file 'ObjectTransformWidget.h'
**
** Created: Mon Sep 22 21:18:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../ObjectTransformWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ObjectTransformWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ObjectTransformWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      39,   22,   22,   22, 0x0a,
      59,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ObjectTransformWidget[] = {
    "ObjectTransformWidget\0\0updateWidgets()\0"
    "updateSceneObject()\0updateObjectName()\0"
};

void ObjectTransformWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ObjectTransformWidget *_t = static_cast<ObjectTransformWidget *>(_o);
        switch (_id) {
        case 0: _t->updateWidgets(); break;
        case 1: _t->updateSceneObject(); break;
        case 2: _t->updateObjectName(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ObjectTransformWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ObjectTransformWidget::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_ObjectTransformWidget,
      qt_meta_data_ObjectTransformWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ObjectTransformWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ObjectTransformWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ObjectTransformWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ObjectTransformWidget))
        return static_cast<void*>(const_cast< ObjectTransformWidget*>(this));
    if (!strcmp(_clname, "Ui::TransformEditor"))
        return static_cast< Ui::TransformEditor*>(const_cast< ObjectTransformWidget*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int ObjectTransformWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
