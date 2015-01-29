/****************************************************************************
** Meta object code from reading C++ file 'PaintSettingWidget.h'
**
** Created: Mon Sep 22 21:18:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../PaintSettingWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PaintSettingWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PaintSettingWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x0a,
      42,   36,   19,   19, 0x0a,
      65,   59,   19,   19, 0x0a,
      82,   59,   19,   19, 0x0a,
     107,  101,   19,   19, 0x0a,
     136,  128,   19,   19, 0x0a,
     170,  161,   19,   19, 0x0a,
     191,   59,   19,   19, 0x0a,
     217,   19,   19,   19, 0x0a,
     231,   19,   19,   19, 0x0a,
     244,   19,   19,   19, 0x0a,
     257,   19,   19,   19, 0x0a,
     270,   19,   19,   19, 0x0a,
     282,   19,   19,   19, 0x0a,
     300,   19,   19,   19, 0x0a,
     316,   19,   19,   19, 0x0a,
     331,   19,   19,   19, 0x0a,
     346,   19,   19,   19, 0x0a,
     361,   19,   19,   19, 0x0a,
     375,   19,   19,   19, 0x0a,
     404,  395,   19,   19, 0x0a,
     424,  395,   19,   19, 0x0a,
     448,  395,   19,   19, 0x0a,
     472,  395,   19,   19, 0x0a,
     496,  395,   19,   19, 0x0a,
     514,  395,   19,   19, 0x0a,
     538,   59,   19,   19, 0x0a,
     570,   59,   19,   19, 0x0a,
     606,  601,   19,   19, 0x0a,
     624,   59,   19,   19, 0x0a,
     648,   19,   19,   19, 0x0a,
     675,   19,   19,   19, 0x0a,
     707,  695,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PaintSettingWidget[] = {
    "PaintSettingWidget\0\0updateWidgets()\0"
    "color\0setColor(QColor)\0dummy\0"
    "setColor(double)\0setSurface(double)\0"
    "thick\0setThickness(double)\0opacity\0"
    "setPickerOpacity(double)\0isAttach\0"
    "attachToCamera(bool)\0setAttachingParam(double)\0"
    "setColorTex()\0setReflTex()\0setGlosTex()\0"
    "setRefrTex()\0setIORTex()\0setThicknessTex()\0"
    "clearColorTex()\0clearReflTex()\0"
    "clearGlosTex()\0clearRefrTex()\0"
    "clearIORTex()\0clearThicknessTex()\0"
    "isAffect\0setAffectColor(int)\0"
    "setAffectReflLevel(int)\0setAffectRefrLevel(int)\0"
    "setAffectGlossness(int)\0setAffectIOR(int)\0"
    "setAffectThickness(int)\0"
    "computeNormalBlendCurve(double)\0"
    "computeDepthBlendCurve(double)\0mode\0"
    "setDepthMode(int)\0changeColorPicker(bool)\0"
    "pickColorPickerFromScene()\0"
    "refreshPickerList()\0isBrushMode\0"
    "setBrushMode(bool)\0"
};

void PaintSettingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PaintSettingWidget *_t = static_cast<PaintSettingWidget *>(_o);
        switch (_id) {
        case 0: _t->updateWidgets(); break;
        case 1: _t->setColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->setColor((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setSurface((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setThickness((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setPickerOpacity((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->attachToCamera((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setAttachingParam((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setColorTex(); break;
        case 9: _t->setReflTex(); break;
        case 10: _t->setGlosTex(); break;
        case 11: _t->setRefrTex(); break;
        case 12: _t->setIORTex(); break;
        case 13: _t->setThicknessTex(); break;
        case 14: _t->clearColorTex(); break;
        case 15: _t->clearReflTex(); break;
        case 16: _t->clearGlosTex(); break;
        case 17: _t->clearRefrTex(); break;
        case 18: _t->clearIORTex(); break;
        case 19: _t->clearThicknessTex(); break;
        case 20: _t->setAffectColor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->setAffectReflLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->setAffectRefrLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->setAffectGlossness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->setAffectIOR((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->setAffectThickness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->computeNormalBlendCurve((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 27: _t->computeDepthBlendCurve((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 28: _t->setDepthMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->changeColorPicker((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->pickColorPickerFromScene(); break;
        case 31: _t->refreshPickerList(); break;
        case 32: _t->setBrushMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PaintSettingWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PaintSettingWidget::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_PaintSettingWidget,
      qt_meta_data_PaintSettingWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PaintSettingWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PaintSettingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PaintSettingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PaintSettingWidget))
        return static_cast<void*>(const_cast< PaintSettingWidget*>(this));
    if (!strcmp(_clname, "Ui::PaintSetting"))
        return static_cast< Ui::PaintSetting*>(const_cast< PaintSettingWidget*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int PaintSettingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
