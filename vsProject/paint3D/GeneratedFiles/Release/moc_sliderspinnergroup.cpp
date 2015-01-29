/****************************************************************************
** Meta object code from reading C++ file 'sliderspinnergroup.h'
**
** Created: Mon Sep 22 21:18:34 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../sliderspinnergroup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sliderspinnergroup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SliderSpinnerGroup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       4,   69, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   20,   19,   19, 0x08,
      68,   20,   19,   19, 0x08,
      96,   19,   89,   19, 0x0a,
     104,   19,   89,   19, 0x0a,
     114,   19,   89,   19, 0x0a,
     124,   19,   89,   19, 0x0a,
     137,   20,   19,   19, 0x0a,
     154,   20,   19,   19, 0x0a,
     173,   20,   19,   19, 0x0a,
     192,   20,   19,   19, 0x0a,

 // properties: name, type, flags
     214,   89, 0x06095103,
     220,   89, 0x06095103,
     228,   89, 0x06095103,
     236,   89, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_SliderSpinnerGroup[] = {
    "SliderSpinnerGroup\0\0val\0valueChanged(double)\0"
    "setSliderValue(double)\0setSpinBoxValue(int)\0"
    "double\0value()\0minimum()\0maximum()\0"
    "singleStep()\0setValue(double)\0"
    "setMinimum(double)\0setMaximum(double)\0"
    "setSingleStep(double)\0value\0maximum\0"
    "minimum\0singleStep\0"
};

void SliderSpinnerGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SliderSpinnerGroup *_t = static_cast<SliderSpinnerGroup *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setSliderValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setSpinBoxValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { double _r = _t->value();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 4: { double _r = _t->minimum();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 5: { double _r = _t->maximum();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 6: { double _r = _t->singleStep();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 7: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setMinimum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setMaximum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setSingleStep((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SliderSpinnerGroup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SliderSpinnerGroup::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SliderSpinnerGroup,
      qt_meta_data_SliderSpinnerGroup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SliderSpinnerGroup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SliderSpinnerGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SliderSpinnerGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SliderSpinnerGroup))
        return static_cast<void*>(const_cast< SliderSpinnerGroup*>(this));
    return QWidget::qt_metacast(_clname);
}

int SliderSpinnerGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = value(); break;
        case 1: *reinterpret_cast< double*>(_v) = maximum(); break;
        case 2: *reinterpret_cast< double*>(_v) = minimum(); break;
        case 3: *reinterpret_cast< double*>(_v) = singleStep(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setValue(*reinterpret_cast< double*>(_v)); break;
        case 1: setMaximum(*reinterpret_cast< double*>(_v)); break;
        case 2: setMinimum(*reinterpret_cast< double*>(_v)); break;
        case 3: setSingleStep(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SliderSpinnerGroup::valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
