/****************************************************************************
** Meta object code from reading C++ file 'pantallaAnadirPueblo.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pantallaAnadirPueblo.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pantallaAnadirPueblo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN20PantallaAnadirPuebloE_t {};
} // unnamed namespace

template <> constexpr inline auto PantallaAnadirPueblo::qt_create_metaobjectdata<qt_meta_tag_ZN20PantallaAnadirPuebloE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PantallaAnadirPueblo",
        "puebloGuardado",
        "",
        "const char*",
        "nombre",
        "introduccion",
        "lugarMisterio",
        "misterio",
        "Zona**",
        "zonas",
        "cantZonas",
        "volverAlMenu"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'puebloGuardado'
        QtMocHelpers::SignalData<void(const char *, const char *, const char *, const char *, Zona * *, const int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 3, 5 }, { 0x80000000 | 3, 6 }, { 0x80000000 | 3, 7 },
            { 0x80000000 | 8, 9 }, { QMetaType::Int, 10 },
        }}),
        // Signal 'volverAlMenu'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PantallaAnadirPueblo, qt_meta_tag_ZN20PantallaAnadirPuebloE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PantallaAnadirPueblo::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20PantallaAnadirPuebloE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20PantallaAnadirPuebloE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20PantallaAnadirPuebloE_t>.metaTypes,
    nullptr
} };

void PantallaAnadirPueblo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PantallaAnadirPueblo *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->puebloGuardado((*reinterpret_cast< std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<const char*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<const char*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<const char*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<Zona**>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[6]))); break;
        case 1: _t->volverAlMenu(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PantallaAnadirPueblo::*)(const char * , const char * , const char * , const char * , Zona * * , const int )>(_a, &PantallaAnadirPueblo::puebloGuardado, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PantallaAnadirPueblo::*)()>(_a, &PantallaAnadirPueblo::volverAlMenu, 1))
            return;
    }
}

const QMetaObject *PantallaAnadirPueblo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PantallaAnadirPueblo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20PantallaAnadirPuebloE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PantallaAnadirPueblo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PantallaAnadirPueblo::puebloGuardado(const char * _t1, const char * _t2, const char * _t3, const char * _t4, Zona * * _t5, const int _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}

// SIGNAL 1
void PantallaAnadirPueblo::volverAlMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
