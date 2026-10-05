/****************************************************************************
** Meta object code from reading C++ file 'PantallaAnadirPersonaje.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PantallaAnadirPersonaje.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PantallaAnadirPersonaje.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN23PantallaAnadirPersonajeE_t {};
} // unnamed namespace

template <> constexpr inline auto PantallaAnadirPersonaje::qt_create_metaobjectdata<qt_meta_tag_ZN23PantallaAnadirPersonajeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PantallaAnadirPersonaje",
        "personajeGuardado",
        "",
        "const char*",
        "nombre",
        "dialogo",
        "pista",
        "pregunta",
        "respuesta",
        "volverAlMenu"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'personajeGuardado'
        QtMocHelpers::SignalData<void(const char *, const char *, const char *, const char *, const char *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 3, 5 }, { 0x80000000 | 3, 6 }, { 0x80000000 | 3, 7 },
            { 0x80000000 | 3, 8 },
        }}),
        // Signal 'volverAlMenu'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PantallaAnadirPersonaje, qt_meta_tag_ZN23PantallaAnadirPersonajeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PantallaAnadirPersonaje::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23PantallaAnadirPersonajeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23PantallaAnadirPersonajeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN23PantallaAnadirPersonajeE_t>.metaTypes,
    nullptr
} };

void PantallaAnadirPersonaje::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PantallaAnadirPersonaje *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->personajeGuardado((*reinterpret_cast< std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<const char*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<const char*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<const char*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<const char*>>(_a[5]))); break;
        case 1: _t->volverAlMenu(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PantallaAnadirPersonaje::*)(const char * , const char * , const char * , const char * , const char * )>(_a, &PantallaAnadirPersonaje::personajeGuardado, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PantallaAnadirPersonaje::*)()>(_a, &PantallaAnadirPersonaje::volverAlMenu, 1))
            return;
    }
}

const QMetaObject *PantallaAnadirPersonaje::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PantallaAnadirPersonaje::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23PantallaAnadirPersonajeE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PantallaAnadirPersonaje::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void PantallaAnadirPersonaje::personajeGuardado(const char * _t1, const char * _t2, const char * _t3, const char * _t4, const char * _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 1
void PantallaAnadirPersonaje::volverAlMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
