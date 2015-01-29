/****************************************************************************
** Meta object code from reading C++ file 'BrushSettingWidget.h'
**
** Created: Mon Sep 22 21:18:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../BrushSettingWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BrushSettingWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BrushSettingWidget[] = {

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
      25,   20,   19,   19, 0x08,
      47,   41,   19,   19, 0x08,
      80,   74,   19,   19, 0x08,
      97,   41,   19,   19, 0x08,
     125,  114,   19,   19, 0x08,
     159,  147,   19,   19, 0x08,
     194,  182,   19,   19, 0x08,
     230,  217,   19,   19, 0x08,
     263,  254,   19,   19, 0x08,
     288,  283,   19,   19, 0x08,
     314,  308,   19,   19, 0x08,
     336,   19,   19,   19, 0x08,
     358,   19,   19,   19, 0x08,
     388,  378,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BrushSettingWidget[] = {
    "BrushSettingWidget\0\0size\0setSize(double)\0"
    "ratio\0setResolutionRatio(double)\0angle\0"
    "setAngle(double)\0setRatio(double)\0"
    "sizeJitter\0setSizeJitter(double)\0"
    "angleJitter\0setAngleJitter(double)\0"
    "ratioJitter\0setRatioJitter(double)\0"
    "offsetJitter\0setOffsetJitter(double)\0"
    "interval\0setInterval(double)\0hard\0"
    "setHardness(double)\0dummy\0"
    "setDepthRange(double)\0loadStrokeFromImage()\0"
    "loadStrokeFromAbr()\0ithStroke\0"
    "setStroke(int)\0"
};

void BrushSettingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrushSettingWidget *_t = static_cast<BrushSettingWidget *>(_o);
        switch (_id) {
        case 0: _t->setSize((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setResolutionRatio((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setRatio((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setSizeJitter((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setAngleJitter((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setRatioJitter((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->setOffsetJitter((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setInterval((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setHardness((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setDepthRange((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->loadStrokeFromImage(); break;
        case 12: _t->loadStrokeFromAbr(); break;
        case 13: _t->setStroke((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BrushSettingWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BrushSettingWidget::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_BrushSettingWidget,
      qt_meta_data_BrushSettingWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BrushSettingWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BrushSettingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BrushSettingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BrushSettingWidget))
        return static_cast<void*>(const_cast< BrushSettingWidget*>(this));
    if (!strcmp(_clname, "Ui::BrushSetting"))
        return static_cast< Ui::BrushSetting*>(const_cast< BrushSettingWidget*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int BrushSettingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
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
