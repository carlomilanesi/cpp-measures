// measures.hpp
// Released under Mozilla Public Licence 2.0
// by Carlo Milanesi (carlo.milanesi@tiscali.it) in december 2014
#ifndef MEASURES_HPP
#define MEASURES_HPP
#include <cmath>
#include <type_traits>

#ifdef MEASURES_USE_ALL
#define MEASURES_USE_2D
#define MEASURES_USE_3D
#define MEASURES_USE_ANGLES
#define MEASURES_USE_IOSTREAM
#endif

//////////////////// STATIC ASSERTS FOR MAGNITUDE ////////////////////

namespace measures
{
    template <typename T> inline void assert_same_type(T, T) { }
}
#define ASSERT_HAVE_SAME_MAGNITUDE(U1,U2)\
    assert_same_type(typename U1::magnitude(0),\
        typename U2::magnitude(0));
#ifdef MEASURES_USE_ANGLES
#define ASSERT_IS_ANGLE(U)\
    assert_same_type(typename U::magnitude(0), Angle(0));
#endif

//////////////////// UNIT FEATURES ////////////////////

namespace measures
{
    template <typename Num> Num sqr(Num x) { return x * x; }

    struct unit_features
    { double ratio, offset; char const* suffix; };
#ifdef MEASURES_USE_ANGLES
    struct angle_unit_features
    { double ratio, offset, turn_fraction; char const* suffix; };
#endif
}


//////////////////// MAGNITUDE AND UNIT DEFINITIONS ////////////////////

#define MEASURES_UNIT(UnitName,MagnitudeName,Suffix,Ratio,Offset)\
    namespace measures\
    {\
        static unit_features UnitName##_features_\
            = { Ratio, Offset, Suffix };\
        class UnitName\
        {\
        private:\
            /* prevent instatiation */\
            UnitName();\
        public:\
            typedef MagnitudeName magnitude;\
            static MagnitudeName id()\
                { return MagnitudeName(&UnitName##_features_); }\
            static char const* suffix() { return Suffix; }\
            static double ratio() { return Ratio; }\
            static double offset() { return Offset; }\
        };\
    }

#ifdef MEASURES_USE_ANGLES
#define MEASURES_ANGLE_UNIT(UnitName,Suffix,TurnFraction,Offset)\
    namespace measures\
    {\
        static angle_unit_features UnitName##_features_\
            = { 2 * pi / (TurnFraction), Offset, TurnFraction, Suffix };\
        class UnitName\
        {\
        private:\
            /* prevent instatiation */\
            UnitName();\
        public:\
            typedef Angle magnitude;\
            static Angle id() { return Angle(&UnitName##_features_); }\
            static char const* suffix() { return Suffix; }\
            static double ratio() { return 2 * pi / (TurnFraction); }\
            static double offset() { return Offset; }\
            template <typename Num>\
            static Num turn_fraction()\
            { return static_cast<Num>(TurnFraction); }\
        };\
    }
#endif

// Every magnitude class is used as a template parameter
// or it may be instantiated to represent a dynamic unit.
#define MEASURES_MAGNITUDE(MagnitudeName,BaseUnitName,BaseUnitSuffix)\
    namespace measures\
    {\
        class BaseUnitName;\
        class MagnitudeName\
        {\
        public:\
            typedef BaseUnitName base_unit;\
            explicit MagnitudeName(unit_features const* features):\
                features_(features) { }\
            char const* suffix() const { return features_->suffix; }\
            double ratio() const { return features_->ratio; }\
            double offset() const { return features_->offset; }\
        private:\
            unit_features const* features_;\
        };\
    }\
    MEASURES_UNIT(BaseUnitName, MagnitudeName, BaseUnitSuffix, 1, 0)


//////////////////// PREDEFINED MAGNITUDES AND UNITS ////////////////////

#ifdef MEASURES_USE_ANGLES
// The "Angle" magnitude and its "radians" unit are required.
namespace measures
{
    static double const pi = atan(1) * 4;
    class radians;
    class Angle
    {
    public:
        typedef radians base_unit;
        explicit Angle(angle_unit_features const* features):
            features_(features) { }
        char const* suffix() const { return features_->suffix; }
        double ratio() const { return features_->ratio; }
        double offset() const { return features_->offset; }
        template <typename Num>\
        Num turn_fraction()\
        { return static_cast<Num>(features_->turn_fraction); }\
    private:
        angle_unit_features const* features_;
    };
}
MEASURES_ANGLE_UNIT(radians, " rad", 2 * pi, 0)
#endif

//////////////////// DERIVED UNITS ////////////////////

/*
The following different-unit operations are supported:
Scalar * Scalar -> Scalar // simple product
Scalar / Scalar -> Scalar // simple division
sqrt(Scalar) -> Scalar // simple square root
Scalar * Vector -> Vector // vector external product
Vector * Scalar -> Vector // vector external product, commutated
Vector / Scalar -> Vector // vector external product by the reciprocal
Vector * Vector -> Scalar // vector dot product
cross_product(Vector, Vector) -> Vector // vector cross product
*/

#ifdef MEASURES_USE_2D
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_1_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_1_1(U1,U2,U3)\
    MEASURES_DERIVED_1_2(U1,U2,U3)
#endif

#ifdef MEASURES_USE_3D
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_1_3_ALL(U1,U2,U3)\
    MEASURES_DERIVED_1_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_1_3(U1,U2,U3)
#endif

#ifdef MEASURES_USE_2D
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U4 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_2_2_ALL(U1,U2,U3,U4)\
    MEASURES_DERIVED_1_1(U1,U2,U3)\
    MEASURES_DERIVED_2_2(U1,U2,U3,U4)
#endif

#ifdef MEASURES_USE_3D
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U4 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_3_3_ALL(U1,U2,U3,U4)\
    MEASURES_DERIVED_2_2_ALL(U1,U2,U3,U4)\
    MEASURES_DERIVED_3_3(U1,U2,U3,U4)
#endif

#ifdef MEASURES_USE_2D
// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U3 (Vector)
#define MEASURES_DERIVED_SQ_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_SQ_1(U1,U2)\
    MEASURES_DERIVED_SQ_2(U1,U2,U3)
#endif

#ifdef MEASURES_USE_3D
// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U3 (Vector)
#define MEASURES_DERIVED_SQ_3_ALL(U1,U2,U3)\
    MEASURES_DERIVED_SQ_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_SQ_3(U1,U2,U3)
#endif

// U1 (Scalar) * U2 (Scalar) == U3 (Scalar)
// with U1 != U2
#define MEASURES_DERIVED_1_1(U1,U2,U3)\
    namespace measures\
    {\
        /* vect1 * vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect1<U2,Num2> m2)\
        {\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.value() * m2.value());\
        }\
        \
        /* vect1 * vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U2,Num1> m2, vect1<U1,Num2> m1)\
        {\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m2.value() * m1.value());\
        }\
        \
        /* vect1 / vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()/Num2())> operator /(\
            vect1<U3,Num1> m3, vect1<U1,Num2> m1)\
        {\
            return vect1<U2,decltype(Num1()/Num2())>(\
                m3.value() / m1.value());\
        }\
        \
        /* vect1 / vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U1,decltype(Num1()/Num2())> operator /(\
            vect1<U3,Num1> m3, vect1<U2,Num2> m2)\
        {\
            return vect1<U1,decltype(Num1()/Num2())>(\
                m3.value() / m2.value());\
        }\
    }

// U1 (Scalar) * U1 (Scalar) == U2 (Scalar)
#define MEASURES_DERIVED_SQ_1(U1,U2)\
    namespace measures\
    {\
        /* vect1 * vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect1<U1,Num2> m2)\
        {\
            return vect1<U2,decltype(Num1()*Num2())>(\
                m1.value() * m2.value());\
        }\
        \
        /* sqr(vect1) -> vect1 */\
        template <typename Num>\
        vect1<U2,Num> sqr(vect1<U1,Num> m) { return m * m; }\
        \
        /* vect1 / vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U1,decltype(Num1()/Num2())> operator /(\
            vect1<U2,Num1> m2, vect1<U1,Num2> m1)\
        {\
            return vect1<U1,decltype(Num1()/Num2())>(\
                m2.value() / m1.value());\
        }\
        \
        /* sqrt(vect1) -> vect1 */\
        template <typename Num>\
        vect1<U1,Num> sqrt(vect1<U2,Num> m)\
        {\
            return vect1<U1,Num>(\
                static_cast<Num>(std::sqrt(m.value())));\
        }\
    }

#ifdef MEASURES_USE_2D
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_1_2(U1,U2,U3)\
    namespace measures\
    {\
        /* vect1 * vect2 -> vect2 */\
        template <typename Num1, typename Num2>\
        vect2<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect2<U2,Num2> m2)\
        {\
            return vect2<U3,decltype(Num1()*Num2())>(\
                m1.value() * m2.x().value(),\
                m1.value() * m2.y().value());\
        }\
        \
        /* vect2 * vect1 -> vect2 */\
        template <typename Num2, typename Num1>\
        vect2<U3,decltype(Num2()*Num1())> operator *(\
            vect2<U2,Num2> m2, vect1<U1,Num1> m1)\
        {\
            return vect2<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.value(),\
                m2.y().value() * m1.value());\
        }\
        \
        /* vect2 / vect1 -> vect2 */\
        template <typename Num3, typename Num1>\
        vect2<U2,decltype(Num3()/Num1())> operator /(\
            vect2<U3,Num3> m3, vect1<U1,Num1> m1)\
        {\
            return vect2<U2,decltype(Num3()/Num1())>(\
                m3.x().value() / m1.value(),\
                m3.y().value() / m1.value());\
        }\
    }
#endif

#ifdef MEASURES_USE_3D
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_1_3(U1,U2,U3)\
    namespace measures\
    {\
        /* vect1 * vect3 -> vect3 */\
        template <typename Num1, typename Num2>\
        vect3<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect3<U2,Num2> m2)\
        {\
            return vect3<U3,decltype(Num1()*Num2())>(\
                m1.value() * m2.x().value(),\
                m1.value() * m2.y().value(),\
                m1.value() * m2.z().value());\
        }\
        \
        /* vect3 * vect1 -> vect3 */\
        template <typename Num2, typename Num1>\
        vect3<U3,decltype(Num2()*Num1())> operator *(\
            vect3<U2,Num2> m2, vect1<U1,Num1> m1)\
        {\
            return vect3<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.value(),\
                m2.y().value() * m1.value(),\
                m2.z().value() * m1.value());\
        }\
        \
        /* vect3 / vect1 -> vect3 */\
        template <typename Num3, typename Num1>\
        vect3<U2,decltype(Num3()*Num1())> operator /(\
            vect3<U3,Num3> m3, vect1<U1,Num1> m1)\
        {\
            return vect3<U2,decltype(Num3()*Num1())>(\
                m3.x().value() / m1.value(),\
                m3.y().value() / m1.value(),\
                m3.z().value() / m1.value());\
        }\
    }
#endif

#ifdef MEASURES_USE_2D
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U4 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_2_2(U1,U2,U3,U4)\
    namespace measures\
    {\
        /* vect2 * vect2 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect2<U1,Num1> m1, vect2<U2,Num2> m2)\
        {\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() +\
                m1.y().value() * m2.y().value());\
        }\
        \
        /* vect2 * vect2 -> vect1 */\
        template <typename Num2, typename Num1>\
        vect1<U3,decltype(Num2()*Num1())> operator *(\
            vect2<U2,Num1> m2, vect2<U1,Num2> m1)\
        {\
            return vect1<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.x().value() +\
                m2.y().value() * m1.y().value());\
        }\
        \
        /* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U4,decltype(Num1()*Num2())> cross_product(\
            vect2<U1,Num1> m1, vect2<U2,Num2> m2)\
        {\
            return vect1<U4,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
        \
        /* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num2, typename Num1>\
        vect1<U4,decltype(Num2()*Num1())> cross_product(\
            vect2<U2,Num1> m2, vect2<U1,Num2> m1)\
        {\
            return vect1<U4,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.y().value() -\
                m2.y().value() * m1.x().value());\
        }\
    }
#endif

#ifdef MEASURES_USE_3D
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U4 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_3_3(U1,U2,U3,U4)\
    namespace measures\
    {\
        /* vect3 * vect3 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect3<U1,Num1> m1, vect3<U2,Num2> m2)\
        {\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() +\
                m1.y().value() * m2.y().value() +\
                m1.z().value() * m2.z().value());\
        }\
        \
        /* vect3 * vect3 -> vect1 */\
        template <typename Num2, typename Num1>\
        vect1<U3,decltype(Num2()*Num1())> operator *(\
            vect3<U2,Num2> m2, vect3<U1,Num1> m1)\
        {\
            return vect1<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.x().value() +\
                m2.y().value() * m1.y().value() +\
                m2.z().value() * m1.z().value());\
        }\
        \
        /* cross_product(vect3, vect3) -> vect3 */\
        template <typename Num1, typename Num2>\
        vect3<U4,decltype(Num1()*Num2())> cross_product(\
            vect3<U1,Num1> m1, vect3<U2,Num2> m2)\
        {\
            return vect3<U4,decltype(Num1()*Num2())>(\
                m1.y().value() * m2.z().value() -\
                m1.z().value() * m2.y().value(),\
                m1.z().value() * m2.x().value() -\
                m1.x().value() * m2.z().value(),\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
        \
        /* cross_product(vect3, vect3) -> vect3 */\
        template <typename Num2, typename Num1>\
        vect3<U4,decltype(Num2()*Num1())> cross_product(\
            vect3<U2,Num2> m2, vect3<U1,Num1> m1)\
        {\
            return vect3<U4,decltype(Num2()*Num1())>(\
                m2.y().value() * m1.z().value() -\
                m2.z().value() * m1.y().value(),\
                m2.z().value() * m1.x().value() -\
                m2.x().value() * m1.z().value(),\
                m2.x().value() * m1.y().value() -\
                m2.y().value() * m1.x().value());\
        }\
    }
#endif

#ifdef MEASURES_USE_2D
// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U3 (Vector)
#define MEASURES_DERIVED_SQ_2(U1,U2,U3)\
    namespace measures\
    {\
        /* vect2 * vect2 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> operator *(\
            vect2<U1,Num1> m1, vect2<U1,Num2> m2)\
        {\
            return vect1<U2,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() + \
                m1.y().value() * m2.y().value());\
        }\
        \
        /* sqr(vect2) -> vect1 */\
        template <typename Num>\
        vect1<U2,Num> sqr(vect2<U1,Num> m)\
        { return m * m; }\
        \
        /* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> cross_product(\
            vect2<U1,Num1> m1, vect2<U1,Num2> m2)\
        {\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
    }
#endif

#ifdef MEASURES_USE_3D
// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U3 (Vector)
#define MEASURES_DERIVED_SQ_3(U1,U2,U3)\
    namespace measures\
    {\
        /* vect3 * vect3 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> operator *(\
            vect3<U1,Num1> m1, vect3<U1,Num2> m2)\
        {\
            return vect1<U2,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() + \
                m1.y().value() * m2.y().value() + \
                m1.z().value() * m2.z().value());\
        }\
        \
        /* sqr(vect3) -> vect1 */\
        template <typename Num>\
        vect1<U2,Num> sqr(vect3<U1,Num> m)\
        { return m * m; }\
        \
        /* cross_product(vect3, vect3) -> vect3 */\
        template <typename Num1, typename Num2>\
        vect3<U3,decltype(Num1()*Num2())> cross_product(\
            vect3<U1,Num1> m1, vect3<U1,Num2> m2)\
        {\
            return vect3<U3,decltype(Num1()*Num2())>(\
                m1.y().value() * m2.z().value() -\
                m1.z().value() * m2.y().value(),\
                m1.z().value() * m2.x().value() -\
                m1.x().value() * m2.z().value(),\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
    }
#endif

namespace measures
{
//////////////////// PROTOTYPES ////////////////////

    template <class Unit, typename Num> class vect1;
    template <class Unit, typename Num> class point1;
#ifdef MEASURES_USE_2D
    template <class Unit, typename Num> class vect2;
    template <class Unit, typename Num> class affine_map2;
    template <class Unit, typename Num> class point2;
#endif
#ifdef MEASURES_USE_3D
    template <class Unit, typename Num> class vect3;
    template <class Unit, typename Num> class affine_map3;
    template <class Unit, typename Num> class point3;
#endif
#ifdef MEASURES_USE_ANGLES
    template <class Unit, typename Num> class signed_azimuth;
    template <class Unit, typename Num> class unsigned_azimuth;
#endif

//////////////////// UNIT CONVERSIONS ////////////////////

    // 1d measures
    template <class ToUnit1, class FromUnit, typename Num>
    vect1<ToUnit1,Num> convert(vect1<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit1, FromUnit)
        return vect1<ToUnit1,Num>(m.value()
            * static_cast<Num>(FromUnit::ratio() / ToUnit1::ratio()));
    }

    template <class ToUnit2, class FromUnit, typename Num>
    point1<ToUnit2,Num> convert(point1<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit2, FromUnit)
        return point1<ToUnit2,Num>(m.value()
            * static_cast<Num>(FromUnit::ratio() / ToUnit2::ratio())
            + static_cast<Num>((FromUnit::offset() - ToUnit2::offset())
            / ToUnit2::ratio()));
    }

#ifdef MEASURES_USE_2D
    // 2d measures
    template <class ToUnit3, class FromUnit, typename Num>
    vect2<ToUnit3,Num> convert(vect2<FromUnit,Num> m)
    {
        return vect2<ToUnit3,Num>(
            convert<ToUnit3,FromUnit,Num>(m.x()),
            convert<ToUnit3,FromUnit,Num>(m.y()));
    }

    template <class ToUnit4, class FromUnit, typename Num>
    affine_map2<ToUnit4,Num> convert(affine_map2<FromUnit,Num> map)
    {
        affine_map2<ToUnit4,Num> result = map;
        result.coeff(0, 2) = convert<ToUnit4,FromUnit,Num>(map.coeff(0, 2));
        result.coeff(1, 2) = convert<ToUnit4,FromUnit,Num>(map.coeff(1, 2));
        return result;
    }

    template <class ToUnit5, class FromUnit, typename Num>
    point2<ToUnit5,Num> convert(point2<FromUnit,Num> m)
    {
        return point2<ToUnit5,Num>(
            convert<ToUnit5,FromUnit,Num>(m.x()),
            convert<ToUnit5,FromUnit,Num>(m.y()));
    }
#endif

#ifdef MEASURES_USE_3D
    // 3d measures
    template <class ToUnit6, class FromUnit, typename Num>
    vect3<ToUnit6,Num> convert(vect3<FromUnit,Num> m)
    {
        return vect3<ToUnit6,Num>(
            convert<ToUnit6,FromUnit,Num>(m.x()),
            convert<ToUnit6,FromUnit,Num>(m.y()),
            convert<ToUnit6,FromUnit,Num>(m.z()));
    }

    template <class ToUnit7, class FromUnit, typename Num>
    affine_map3<ToUnit7,Num> convert(affine_map3<FromUnit,Num> map)
    {
        affine_map3<ToUnit7,Num> result = map;
        result.coeff(0, 3) = convert<ToUnit7,FromUnit,Num>(map.coeff(0, 3));
        result.coeff(1, 3) = convert<ToUnit7,FromUnit,Num>(map.coeff(1, 3));
        result.coeff(2, 3) = convert<ToUnit7,FromUnit,Num>(map.coeff(2, 3));
        return result;
    }

    template <class ToUnit8, class FromUnit, typename Num>
    point3<ToUnit8,Num> convert(point3<FromUnit,Num> m)
    {
        return point3<ToUnit8,Num>(
            convert<ToUnit8,FromUnit,Num>(m.x()),
            convert<ToUnit8,FromUnit,Num>(m.y()),
            convert<ToUnit8,FromUnit,Num>(m.z()));
    }
#endif

#ifdef MEASURES_USE_ANGLES
    // Azimuths
    template <class ToUnit9, class FromUnit, typename Num>
    signed_azimuth<ToUnit9,Num> convert(signed_azimuth<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit9, FromUnit)
        return signed_azimuth<ToUnit9,Num>(
            convert<ToUnit9>(point1<FromUnit,Num>(m.value())));
    }

    template <class ToUnit10, class FromUnit, typename Num>
    unsigned_azimuth<ToUnit10,Num> convert(unsigned_azimuth<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit10, FromUnit)
        return unsigned_azimuth<ToUnit10,Num>(
            convert<ToUnit10>(point1<FromUnit,Num>(m.value())));
    }
#endif


    //////////////////// 1-DIMENSIONAL VECTORS AND POINTS ////////////////////

    template <class Unit, typename Num = double>
    class vect1
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit vect1() { }

        // Constructs using one number.
        template <typename Num1>
        explicit vect1(Num1 x): x_(x) { }

        // Constructs using another vect1 of the same unit.
        template <typename Num1>
        vect1(vect1<Unit,Num1> const& o): x_(o.value()) { }
        
        // Constructs using a unit and a value.
        template <typename Num1>
        vect1(typename Unit::magnitude unit, Num1 x):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio()))) { }

        // Get unmutable value for the given unit.
        Num value(typename Unit::magnitude unit) const
        {
            return static_cast<Num>(x_ * (Unit::ratio() / unit.ratio()));
        }

        // Get unmutable value.
        Num value() const { return x_; }

        // Get mutable value.
        Num& value() { return x_; }

        // +vect1 -> vect1
        vect1<Unit,Num> operator +() const { return *this; }

        // -vect1 -> vect1
        vect1<Unit,Num> operator -() const { return vect1<Unit,Num>(-x_); }

        // vect1 += vect1 -> vect1
        template <typename Num2>
        vect1<Unit,Num> operator +=(vect1<Unit,Num2> m2)
        { x_ += m2.value(); return *this; }

        // vect1 -= vect1 -> vect1
        template <typename Num2>
        vect1<Unit,Num> operator -=(vect1<Unit,Num2> m2)
        { x_ -= m2.value(); return *this; }

        // vect1 *= N -> vect1
        template <typename Num2>
        vect1<Unit,Num> operator *=(Num2 n)
        { x_ *= n; return *this; }

        // vect1 /= N -> vect1
        template <typename Num2>
        vect1<Unit,Num> operator /=(Num2 n)
        { x_ /= n; return *this; }

    private:
        Num x_;
    };

    // vect1 == vect1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    { return m1.value() == m2.value(); }

    // vect1 != vect1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    { return m1.value() != m2.value(); }

    // vect1 < vect1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator <(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    { return m1.value() < m2.value(); }

    // vect1 <= vect1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator <=(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    { return m1.value() <= m2.value(); }

    // vect1 > vect1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator >(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    { return m1.value() > m2.value(); }

    // vect1 >= vect1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator >=(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    { return m1.value() >= m2.value(); }

    // is_equal(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        return std::abs((m1 - m2).value()) <= tolerance.value();
    }

    // is_less(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2,\
        vect1<Unit,Num3> tolerance)
    {
//        return static_cast<Num3>(m1.value())
//            < static_cast<Num3>(m2.value()) - tolerance.value();
        return tolerance.value() < (m2 - m1).value();
    }

    // is_less_or_equal(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
//        return static_cast<Num3>(m1.value())
//            <= static_cast<Num3>(m2.value()) + tolerance.value();
        return (m1 - m2).value() <= tolerance.value();
    }

    template <class Unit, typename Num = double>
    class point1
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit point1() { }

        // Constructs using one number of the same number type.
        template <typename Num1>
        explicit point1(Num1 x): x_(x) { }

        // Constructs using another point1 of the same unit.
        template <typename Num1>
        point1(point1<Unit,Num1> const& o): x_(o.value()) { }

        // Constructs using a unit and a value.
        template <typename Num1>
        point1(typename Unit::magnitude unit, Num1 x):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio())
                + (unit.offset() - Unit::offset()) / Unit::ratio())) { }

#ifdef MEASURES_USE_ANGLES
        // Constructs using a signed azimuth.
        template <typename Num1>
        explicit point1(signed_azimuth<Unit,Num1> a): x_(a.value()) { }

        // Constructs using an unsigned azimuth.
        template <typename Num1>
        explicit point1(unsigned_azimuth<Unit,Num1> a): x_(a.value()) { }
#endif

        // Get unmutable value for the given unit.
        Num value(typename Unit::magnitude unit) const
        {
            return static_cast<Num>(x_ * (Unit::ratio() / unit.ratio())
                + (Unit::offset() - unit.offset()) / unit.ratio());
        }

        // Get unmutable value.
        Num value() const { return x_; }

        // Get mutable value.
        Num& value() { return x_; }

        // point1 += vect1 -> point1
        point1<Unit,Num> operator +=(vect1<Unit,Num> m2)
        { x_ += m2.value(); return *this; }

        // point1 -= vect1 -> point1
        point1<Unit,Num> operator -=(vect1<Unit,Num> m2)
        { x_ -= m2.value(); return *this; }

    private:
        Num x_;
    };

    // midpoint(point1, point1, weight) -> point1
    template <class Unit, typename Num1, typename Num2, typename Num3>
    point1<Unit,decltype(Num1()+Num2())> midpoint(
        point1<Unit,Num1> p1, point1<Unit,Num2> p2, Num3 weight)
    {
        typedef decltype(Num1()+Num2()) ResultNum;
        return point1<Unit,ResultNum>(
            p1.value() * static_cast<ResultNum>(1 - weight)
            + p2.value() * static_cast<ResultNum>(weight));
    }

    // midpoint(point1, point1) -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype(Num1()+Num2())> midpoint(
        point1<Unit,Num1> p1, point1<Unit,Num2> p2)
    {
        typedef decltype(Num1()+Num2()) ResultNum;
        return point1<Unit,ResultNum>(
            (p1.value() + p2.value()) / static_cast<ResultNum>(2));
    }

    // barycentric_combination(int, point1[], Num[]) -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype(Num1()+Num2())> barycentric_combination(
        int n, point1<Unit,Num1> p[], Num2 weights[])
    {
        typedef decltype(Num1()+Num2()) ResultNum;
        ResultNum result = 0;
        for (int i = 0; i < n; ++i)
        { result += p[i].value() * weights[i]; }
        return point1<Unit,ResultNum>(result);
    }

    // point1 - point1 -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()-Num2())> operator -(
        point1<Unit,Num1> m1, point1<Unit,Num2> m2)
    {
        return vect1<Unit,decltype(Num1()-Num2())>(
            m1.value() - m2.value());
    }

    // point1 == point1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(point1<Unit,Num1> m1, point1<Unit,Num2> m2)
    { return m1.value() == m2.value(); }

    // point1 != point1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(point1<Unit,Num1> m1, point1<Unit,Num2> m2)
    { return m1.value() != m2.value(); }

    // point1 < point1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator <(point1<Unit,Num1> m1, point1<Unit,Num2> m2)
    { return m1.value() < m2.value(); }

    // point1 <= point1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator <=(point1<Unit,Num1> m1, point1<Unit,Num2> m2)
    { return m1.value() <= m2.value(); }

    // point1 > point1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator >(point1<Unit,Num1> m1, point1<Unit,Num2> m2)
    { return m1.value() > m2.value(); }

    // point1 >= point1 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator >=(point1<Unit,Num1> m1, point1<Unit,Num2> m2)
    { return m1.value() >= m2.value(); }

    // is_equal(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(point1<Unit,Num1> m1, point1<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        return std::abs((m1 - m2).value()) <= tolerance.value();
    }

    // is_less(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less(point1<Unit,Num1> m1, point1<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
//        return static_cast<Num3>(m1.value())
//            < static_cast<Num3>(m2.value()) - tolerance.value();
        return tolerance.value() < (m2 - m1).value();
    }

    // is_less_or_equal(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(point1<Unit,Num1> m1, point1<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
//        return static_cast<Num3>(m1.value())
//            <= static_cast<Num3>(m2.value()) + tolerance.value();
        return (m1 - m2).value() <= tolerance.value();
    }

    // point1 + vect1 -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype(Num1()+Num2())> operator +(point1<Unit,Num1> m1,
        vect1<Unit,Num2> m2)
    {
        return point1<Unit,decltype(Num1()+Num2())>(
            m1.value() + m2.value());
    }

    // point1 - vect1 -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype(Num1()-Num2())> operator -(
        point1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
        return point1<Unit,decltype(Num1()-Num2())>(
            m1.value() - m2.value());
    }

    // vect1 + vect1 -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()+Num2())> operator +(
        vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
        return vect1<Unit,decltype(Num1()+Num2())>(m1.value() + m2.value());
    }

    // vect1 - vect1 -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()-Num2())> operator -(
        vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
        return vect1<Unit,decltype(Num1()-Num2())>(m1.value() - m2.value());
    }

    // N * vect1 -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()*Num2())> operator *(
        Num1 n, vect1<Unit,Num2> m)
    { return vect1<Unit,decltype(Num1()*Num2())>(n * m.value()); }

    // vect1 * N -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()*Num2())> operator *(vect1<Unit,Num1> m, Num2 n)
    { return vect1<Unit,decltype(Num1()*Num2())>(m.value() * n); }

    // vect1 / N -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()/Num2())> operator /(vect1<Unit,Num1> m, Num2 n)
    { return vect1<Unit,decltype(Num1()/Num2())>(m.value() / n); }

    // vect1 / vect1 -> N
    template <class Unit, typename Num1, typename Num2>
    decltype(Num1()/Num2()) operator /(
        vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    { return m1.value() / m2.value(); }

    // squared_norm_value(vect1) -> N
    template <class Unit, typename Num>
    Num squared_norm_value(vect1<Unit,Num> v)
    { return sqr(v.value()); }

    // norm(vect1) -> vect1
    template <class Unit, typename Num>
    vect1<Unit,Num> norm(vect1<Unit,Num> v)
    { return vect1<Unit,Num>(static_cast<Num>(std::abs(v.value()))); }

    // normalized(vect1) -> vect1
    template <class Unit, typename Num>
    vect1<Unit,Num> normalized(vect1<Unit,Num> v)
    { return vect1<Unit,Num>(v.value() < 0 ? -1 : +1); }


#ifdef MEASURES_USE_2D
    //////////////////// 2-DIMENSIONAL VECTORS AND POINTS ////////////////////

    template <typename Num = double>
    class linear_map2
    {
        template <class, typename> friend class vect2;    
    public:

        //// No translations
    
#ifdef MEASURES_USE_ANGLES
        //// Rotation by a relative angle.
        template <class Unit, typename Num2>
        static linear_map2 rotation(vect1<Unit,Num2> angle)
        {
            linear_map2 result;
            result.set_rotation_(convert<radians>(angle).value());
            return result;
        }

        static linear_map2 rotation_at_right()
        {
            linear_map2 result;
            result.c_[0][0] = 0; result.c_[0][1] = 1;
            result.c_[1][0] = -1; result.c_[1][1] = 0;
            return result;
        }

        static linear_map2 rotation_at_left()
        {
            linear_map2 result;
            result.c_[0][0] = 0; result.c_[0][1] = -1;
            result.c_[1][0] = 1; result.c_[1][1] = 0;
            return result;
        }
#endif

        //// Projections
        
#ifdef MEASURES_USE_ANGLES
        // Projection onto a line identified by a point angle.
        template <class Unit, typename Num2>
        static linear_map2 projection(point1<Unit,Num2> a)
        {
            linear_map2 result;
            result.set_projection_(convert<radians>(a).value());
            return result;
        }
        
        // Projection onto a line identified by a signed azimuth.
        template <class Unit, typename Num2>
        static linear_map2 projection(signed_azimuth<Unit,Num2> a)
        {
            linear_map2 result;
            result.set_projection_(convert<radians>(a).value());
            return result;
        }
        
        // Projection onto a line identified by an unsigned azimuth.
        template <class Unit, typename Num2>
        static linear_map2 projection(unsigned_azimuth<Unit,Num2> a)
        {
            linear_map2 result;
            result.set_projection_(convert<radians>(a).value());
            return result;
        }
#endif

        // Projection onto a line identified by a unit plane vector.
        // Precondition: norm(v).value() == 1
        template <class Unit, typename Num2>
        static linear_map2 projection(vect2<Unit,Num2> v)
        {
            linear_map2 result;
            result.set_projection_(v.x().value(), v.y().value());
            return result;
        }
        
        //// Reflections
        
#ifdef MEASURES_USE_ANGLES
        // Reflection over a line identified by a point angle.
        template <class Unit, typename Num2>
        static linear_map2 reflection(point1<Unit,Num2> a)
        {
            linear_map2 result;
            result.set_reflection_(convert<radians>(a).value());
            return result;
        }
        
        // Reflection over a line identified by a signed azimuth.
        template <class Unit, typename Num2>
        static linear_map2 reflection(signed_azimuth<Unit,Num2> a)
        {
            linear_map2 result;
            result.set_reflection_(convert<radians>(a).value());
            return result;
        }
        
        // Reflection over a line identified by an unsigned azimuth.
        template <class Unit, typename Num2>
        static linear_map2 reflection(unsigned_azimuth<Unit,Num2> a)
        {
            linear_map2 result;
            result.set_reflection_(convert<radians>(a).value());
            return result;
        }
#endif

        // Reflection over a line identified by a unit plane vector.
        // Precondition: norm(v) == 1
        template <class Unit, typename Num2>
        static linear_map2 reflection(vect2<Unit,Num2> v)
        {
            linear_map2 result;
            result.set_reflection_(v.x().value(), v.y().value());
            return result;
        }

        //// Scaling by two factors.
        template <typename Num2, typename Num3>
        static linear_map2 scaling(Num2 kx, Num3 ky)
        {
            linear_map2 result;
            result.c_[0][0] = kx; result.c_[0][1] = 0;
            result.c_[1][0] = 0; result.c_[1][1] = ky;
            return result;
        }

        //// Inversion 
        linear_map2 inverted()
        {
            auto determinant = c_[0][0] * c_[1][1] - c_[0][1] * c_[1][0];
            linear_map2 result;
            if (determinant == 0)
            {
                result.c_[0][0] = 0;
                result.c_[0][1] = 0;
                result.c_[1][0] = 0;
                result.c_[1][1] = 0;
            }
            else
            {
                auto inverse_determinant = 1 / determinant;
                result.c_[0][0] = c_[1][1] * inverse_determinant;
                result.c_[0][1] = c_[0][1] * -inverse_determinant;
                result.c_[1][0] = c_[1][0] * -inverse_determinant;
                result.c_[1][1] = c_[0][0] * inverse_determinant;
            }
            return result;
        }
        
        //// Access
        
        Num coeff(int row, int col) const { return c_[row][col]; }
        
        Num& coeff(int row, int col) { return c_[row][col]; }
        
    private:

        void set_rotation_(Num a)
        {
            auto cos_a = std::cos(a);
            auto sin_a = std::sin(a);
            c_[0][0] = cos_a; c_[0][1] = -sin_a;
            c_[1][0] = sin_a; c_[1][1] = cos_a;
        }

        void set_projection_(Num cos_a, Num sin_a)
        {
            c_[0][0] = cos_a * cos_a; c_[0][1] = cos_a * sin_a;
            c_[1][0] = sin_a * cos_a; c_[1][1] = sin_a * sin_a;
        }

        void set_projection_(Num a)
        {
            set_projection_(std::cos(a), std::sin(a));
        }

        void set_reflection_(Num cos_a, Num sin_a)
        {
            c_[0][0] = 2 * cos_a * cos_a - 1; c_[0][1] = 2 * cos_a * sin_a;
            c_[1][0] = 2 * cos_a * sin_a; c_[1][1] = 2 * sin_a * sin_a - 1;
        }
        
        void set_reflection_(Num a)
        {
            set_reflection_(std::cos(a), std::sin(a));
        }
        
        Num c_[2][2];
    };

    // Composition of two plane linear transformations.
    template <typename Num1, typename Num2>
    linear_map2<decltype(Num1()*Num2())> combine(
        linear_map2<Num1> const& lm1, linear_map2<Num2> const& lm2)
    {
        linear_map2<decltype(Num1()*Num2())> result;
        result.coeff(0, 0)
            = lm1.coeff(0, 0) * lm2.coeff(0, 0)
            + lm1.coeff(0, 1) * lm2.coeff(1, 0);
        result.coeff(0, 1)
            = lm1.coeff(0, 0) * lm2.coeff(0, 1)
            + lm1.coeff(0, 1) * lm2.coeff(1, 1);
        result.coeff(1, 0)
            = lm1.coeff(1, 0) * lm2.coeff(0, 0)
            + lm1.coeff(1, 1) * lm2.coeff(1, 0);
        result.coeff(1, 1)
            = lm1.coeff(1, 0) * lm2.coeff(0, 1)
            + lm1.coeff(1, 1) * lm2.coeff(1, 1);
        return result;
    }

#ifdef MEASURES_USE_ANGLES
    //// Rotations

    template <class AngleUnit, typename AngleNum>
    linear_map2<AngleNum> create_rotation(
        vect1<AngleUnit,AngleNum> angle)
    {
        return linear_map2<AngleNum>::rotation(angle);
    }

    template <typename Num>
    linear_map2<Num> create_rotation_at_right()
    {
        return linear_map2<Num>::rotation_at_right();
    }

    template <typename Num>
    linear_map2<Num> create_rotation_at_left()
    {
        return linear_map2<Num>::rotation_at_left();
    }
#endif

    //// Projections

#ifdef MEASURES_USE_ANGLES
    template <class AngleUnit, typename AngleNum>
    linear_map2<AngleNum> create_projection(
        point1<AngleUnit,AngleNum> angle)
    {
        return linear_map2<AngleNum>::projection(angle);
    }

    template <class AngleUnit, typename AngleNum>
    linear_map2<AngleNum> create_projection(
        signed_azimuth<AngleUnit,AngleNum> angle)
    {
        return linear_map2<AngleNum>::projection(angle);
    }
    
    template <class AngleUnit, typename AngleNum>
    linear_map2<AngleNum> create_projection(
        unsigned_azimuth<AngleUnit,AngleNum> angle)
    {
        return linear_map2<AngleNum>::projection(angle);
    }
#endif
    
    template <class VectUnit, typename VectNum>
    linear_map2<VectNum> create_projection(
        vect2<VectUnit,VectNum> unit_v)
    {
        return linear_map2<VectNum>::projection(unit_v);
    }

    //// Reflections

#ifdef MEASURES_USE_ANGLES
    template <class AngleUnit, typename AngleNum>
    linear_map2<AngleNum> create_reflection(
        point1<AngleUnit,AngleNum> angle)
    {
        return linear_map2<AngleNum>::reflection(angle);
    }

    template <class AngleUnit, typename AngleNum>
    linear_map2<AngleNum> create_reflection(
        signed_azimuth<AngleUnit,AngleNum> angle)
    {
        return linear_map2<AngleNum>::reflection(angle);
    }
    
    template <class AngleUnit, typename AngleNum>
    linear_map2<AngleNum> create_reflection(
        unsigned_azimuth<AngleUnit,AngleNum> angle)
    {
        return linear_map2<AngleNum>::reflection(angle);
    }
#endif
    
    template <class VectUnit, typename VectNum>
    linear_map2<VectNum> create_reflection(
        vect2<VectUnit,VectNum> unit_v)
    {
        return linear_map2<VectNum>::reflection(unit_v);
    }

    //// Scaling
    
    template <typename NumX, typename NumY>
    linear_map2<decltype(NumX()+NumY())> create_scaling(
        NumX kx, NumY ky)
    {
        return linear_map2<decltype(NumX()+NumY())>
            ::scaling(kx, ky);
    }

    /*
        //// Access
        
        Num coeff(int row, int col) const { return c_[row][col]; }
        
        Num& coeff(int row, int col) { return c_[row][col]; }
    inversa matrice 3x3
    detA = a11a22a33+a21a32a13+a31a12a23 -a11a32a23-a31a22a13-a21a12a33
    a22a33-a23a32, a13a32-a12a33, a12a23-a13a22
    a23a31-a21a33, a11a33-a13a31, a13a21-a11a23 / detA
    a21a32-a22a31, a12a31-a11a32, a11a22-a12a21
    
    se a31=0, a32=0 e a33 = 1, si ottiene
    detA=a11a22 -a21a12
    a22, -a12, a12a23-a13a22 |
    -a21, a11, a13a21-a11a23 | / detA
    0, 0, a11a22-a12a21      |
    ossia l'ultima riga è [0, 0, 1]
??
    inverted matrice 4x4
    detA = a11a22a33a44+a11a23a34a42+a11a24a32a43
          +a12a21a34a43+a12a23a31a44+a12a24a33a41
          +a13a21a32a44+a13a22a34a41+a13a24a31a42
          +a14a21a33a42+a14a22a31a43+a14a23a32a41
          -a11a22a34a43-a11a23a32a44-a11a24a33a42
          -a12a21a33a44-a12a23a34a41-a12a24a31a43
          -a13a21a34a42-a13a22a31a44-a13a24a32a41
          -a14a21a32a43-a14a22a33a41-a14a23a31a42          
    a22a33a44+a23a34a42+a24a32a43-a22a34a43-a23a32a44-a24a33a42, a12a34a43+a13a32a44+a14a33a42-a12a33a44-a13a34a42-a14a32a43, a12a23a44+a13a24a42+a14a22a43-a12a24a43-a13a22a44-a14a23a42, a12a24a33+a13a22a34+a14a23a32-a12a23a34-a13a24a32-a14a22a33
    a21a34a43+a23a31a44+a24a33a41-a21a33a44-a23a34a41-a24a31a43, a11a33a44+a13a34a41+a14a31a43-a11a34a43-a13a31a44-a14a33a41, a11a24a43+a13a21a44+a14a23a41-a11a23a44-a13a24a41-a14a21a43, a11a23a34+a13a24a31+a14a21a33-a11a24a33-a13a21a34-a14a23a31 / detA
    a21a32a44+a22a34a41+a24a31a42-a21a34a42-a22a31a44-a24a32a41, a11a34a42+a12a31a44+a14a32a41-a11a32a44-a12a34a41-a14a31a42, a11a22a44+a12a24a41+a14a21a42-a11a24a42-a12a21a44-a14a22a41, a11a24a32+a12a21a34+a14a22a31-a11a22a34-a12a24a31-a14a21a32
    a21a33a42+a22a31a43+a23a32a41-a21a32a43-a22a33a41-a23a31a42, a11a32a43+a12a33a41+a13a31a42-a11a33a42-a12a31a43-a13a32a41, a11a23a42+a12a21a43+a13a22a41-a11a22a43-a12a23a41-a13a21a42, a11a22a33+a12a23a31+a13a21a32-a11a23a32-a12a21a33-a13a22a31

    se a41=0, a42=0, a43=0 e a44=1, si ottiene
    detA = a11a22a33+a12a23a31+a13a21a32-a11a23a32-a12a21a33-a13a22a31
    a22a33-a23a32, a13a32-a12a33, a12a23-a13a22, a12a24a33+a13a22a34+a14a23a32-a12a23a34-a13a24a32-a14a22a33 |
    a23a31-a21a33, a11a33-a13a31, a13a21-a11a23, a11a23a34+a13a24a31+a14a21a33-a11a24a33-a13a21a34-a14a23a31 | / detA
    a21a32-a22a31, a12a31-a11a32, a11a22-a12a21, a11a24a32+a12a21a34+a14a22a31-a11a22a34-a12a24a31-a14a21a32 |
    0, 0, 0, a11a22a33+a12a23a31+a13a21a32-a11a23a32-a12a21a33-a13a22a31                                     |
    ossia l'ultima riga è [0, 0, 0, 1]
    */
    
    //v.mapped_by(lt1).mapped_by(lt2) == v.mapped_by(combine(lt1, lt2))
    /*TODO
    linear_map2 combine(
        const linear_map2& lt1,
        const linear_map2& lt2)
    {
        linear_map2 result;
        result.c_[0][0] = lt1[0][0] * lt1[0][0] + lt1[0][1] * lt1[1][0];
        result.c_[0][1] = lt1[0][0] * lt1[0][1] + lt1[0][1] * lt1[1][1];
        result.c_[1][0] = lt1[1][0] * lt1[0][0] + lt1[1][1] * lt1[1][0];
        result.c_[1][1] = lt1[1][0] * lt1[0][1] + lt1[1][1] * lt1[1][1];
        *this = result;
    }
    
    */

    
    template <class Unit, typename Num = double>
    class vect2
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit vect2() { }

        // Constructs using two numbers.
        template <typename Num2, typename Num3>
        explicit vect2(Num2 x, Num3 y): x_(x), y_(y) { }

        // Constructs using an array of two numbers.
        template <typename Num2>
        explicit vect2(Num2 const values[]): x_(values[0]), y_(values[1]) { }

        // Constructs using two vect1s.
        template <typename Num2, typename Num3>
        explicit vect2(vect1<Unit,Num2> x, vect1<Unit,Num3> y):
            x_(x.value()), y_(y.value()) { }

        // Constructs using another vect2 of the same unit.
        template <typename Num1>
        vect2(vect2<Unit,Num1> const& o):
            x_(o.x().value()), y_(o.y().value()) { }
        
        // Constructs using a unit and two values.
        template <typename Num2, typename Num3>
        vect2(typename Unit::magnitude unit, Num2 x, Num3 y):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio()))),
            y_(static_cast<Num>(y * (unit.ratio() / Unit::ratio()))) { }

#ifdef MEASURES_USE_ANGLES
        // Returns a vector of norm 1 having the direction represented
        // by a point angle.
        template <class Unit1, typename Num1>
        static vect2<Unit,Num> make_unit_vector(point1<Unit1,Num1> a)
        {
            ASSERT_IS_ANGLE(Unit1)
            Num1 a_val = convert<radians>(a).value();
            return vect2<Unit,Num>(
                static_cast<Num>(std::cos(a_val)),
                static_cast<Num>(std::sin(a_val)));
        }

        // Returns a vector of norm 1 having the direction represented
        // by a signed azimuth.
        template <class Unit1, typename Num1>
        static vect2<Unit,Num> make_unit_vector(signed_azimuth<Unit1,Num1> a)
        {
            ASSERT_IS_ANGLE(Unit1)
            Num1 a_val = convert<radians>(a).value();
            return vect2<Unit,Num>(
                static_cast<Num>(std::cos(a_val)),
                static_cast<Num>(std::sin(a_val)));
        }

        // Returns a vector of norm 1 having the direction represented
        // by an unsigned azimuth.
        template <class Unit1, typename Num1>
        static vect2<Unit,Num> make_unit_vector(unsigned_azimuth<Unit1,Num1> a)
        {
            ASSERT_IS_ANGLE(Unit1)
            Num1 a_val = convert<radians>(a).value();
            return vect2<Unit,Num>(
                static_cast<Num>(std::cos(a_val)),
                static_cast<Num>(std::sin(a_val)));
        }
#endif

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }

        // Get unmutable x component.
        vect1<Unit,Num> const x() const { return vect1<Unit,Num>(x_); }

        // Get mutable x component.
        vect1<Unit,Num>& x()
            { return reinterpret_cast<vect1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        vect1<Unit,Num> const y() const { return vect1<Unit,Num>(y_); }

        // Get mutable y component.
        vect1<Unit,Num>& y()
            { return reinterpret_cast<vect1<Unit,Num>&>(y_); }

        // +vect2 -> vect2
        vect2<Unit,Num> operator +() const { return *this; }

        // -vect2 -> vect2
        vect2<Unit,Num> operator -() const
        { return vect2<Unit,Num>(-x_, -y_); }

        // vect2 += vect2 -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator +=(vect2<Unit,Num2> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            return *this;
        }

        // vect2 -= vect2 -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator -=(vect2<Unit,Num2> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            return *this;
        }

        // vect2 *= N -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator *=(Num2 n)
        {
            x_ *= n;
            y_ *= n;
            return *this;
        }

        // vect2 /= N -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator /=(Num2 n)
        {
            x_ /= n;
            y_ /= n;
            return *this;
        }

        template <typename Num2>
        vect2<Unit,Num> mapped_by(linear_map2<Num2> const& lt) const
        {
            return vect2<Unit,Num>(
                lt.c_[0][0] * x_ + lt.c_[0][1] * y_,
                lt.c_[1][0] * x_ + lt.c_[1][1] * y_);
        }
    private:

        // Components.
        Num x_, y_;
    };

    // vect2 == vect2 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(vect2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // vect2 != vect2 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(vect2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }

    // is_equal(vect2, vect2, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(vect2<Unit,Num1> m1, vect2<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        // "abs" is needed for complex numbers.
        return std::abs(squared_norm_value(m1 - m2))
            <= squared_norm_value(tolerance);
    }

    template <class Unit, typename Num = double>
    class affine_map2
    {
        template <class, typename> friend class point2;    
    public:

        // Translation.
        template <typename VectNum>
        static affine_map2 translation(vect2<Unit,VectNum> v)
        {
            affine_map2 result;
            result.c_[0][0] = 1; result.c_[0][1] = 0;
            result.c_[0][2] = v.x().value();
            result.c_[1][0] = 0; result.c_[1][1] = 1;
            result.c_[1][2] = v.y().value();
            return result;
        }
        
#ifdef MEASURES_USE_ANGLES
        // Rotation about a point by a relative angle.
        template <typename PointNum, class AngleUnit, typename AngleNum>
        static affine_map2 rotation(point2<Unit,PointNum> fixed_p,
            vect1<AngleUnit,AngleNum> angle)
        {
            affine_map2 result;
            result.set_rotation_(
                fixed_p.x().value(), fixed_p.y().value(),
                convert<radians>(angle).value());
            return result;
        }

        template <typename PointNum>
        static affine_map2 rotation_at_right(
            point2<Unit,PointNum> fixed_p)
        {
            affine_map2 result;
            result.set_right_rotation_(
                fixed_p.x().value(), fixed_p.y().value(), -1);
            return result;
        }

        template <typename PointNum>
        static affine_map2 rotation_at_left(
            point2<Unit,PointNum> fixed_p)
        {
            affine_map2 result;
            result.set_right_rotation_(
                fixed_p.x().value(), fixed_p.y().value(), 1);
            return result;
        }
#endif
        
        // Projections
        
#ifdef MEASURES_USE_ANGLES
        // Projection onto a line identified by a fixed point
        // and a point angle.
        template <typename PointNum, class AngleUnit, typename AngleNum>
        static affine_map2 projection(point2<Unit,PointNum> fixed_p,
            point1<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_map2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Projection onto a line identified by a fixed point
        // and a signed azimuth.
        template <typename PointNum, class AngleUnit, typename AngleNum>
        static affine_map2 projection(point2<Unit,PointNum> fixed_p,
            signed_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_map2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Projection onto a line identified by a fixed point
        // and an unsigned azimuth.
        template <typename PointNum, class AngleUnit, typename AngleNum>
        static affine_map2 projection(point2<Unit,PointNum> fixed_p,
            unsigned_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_map2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
#endif

        // Projection onto a line identified by a fixed point
        // and a unit plane vector.
        // Precondition: norm(v).value() == 1
        template <typename PointNum, class VectUnit, typename VectNum>
        static affine_map2 projection(
            point2<Unit,PointNum> fixed_p,
            vect2<VectUnit,VectNum> uv)
        {
            affine_map2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                uv.x().value(), uv.y().value());
            return result;
        }
        
        // Reflections
        
#ifdef MEASURES_USE_ANGLES
        // Reflection over a line identified by a fixed point
        // and a point angle.
        template <typename PointNum, class AngleUnit, typename AngleNum>
        static affine_map2 reflection(point2<Unit,PointNum> fixed_p,
            point1<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_map2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Reflection over a line identified by a fixed point
        // and a signed azimuth.
        template <typename PointNum, class AngleUnit, typename AngleNum>
        static affine_map2 reflection(point2<Unit,PointNum> fixed_p,
            signed_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_map2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Reflection over a line identified by a fixed point
        // and an unsigned azimuth.
        template <typename PointNum, class AngleUnit, typename AngleNum>
        static affine_map2 reflection(point2<Unit,PointNum> fixed_p,
            unsigned_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_map2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
#endif

        // Reflection over a line identified by a fixed point
        // and a unit plane vector.
        // Precondition: norm(v).value() == 1
        template <typename PointNum, class VectUnit, typename VectNum>
        static affine_map2 reflection(
            point2<Unit,PointNum> fixed_p,
            vect2<VectUnit,VectNum> uv)
        {
            affine_map2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                uv.x().value(), uv.y().value());
            return result;
        }

        // Scaling by two factors from a fixed point.
        template <typename PointNum, typename Num2, typename Num3>
        static affine_map2 scaling(point2<Unit,PointNum> fixed_p,
            Num2 kx, Num3 ky)
        {
            affine_map2 result;
            result.c_[0][0] = kx;
            result.c_[0][1] = 0;
            result.c_[0][2] = fixed_p.x().value() * (1 - kx);
            result.c_[1][0] = 0;
            result.c_[1][1] = ky;
            result.c_[1][2] = fixed_p.y().value() * (1 - ky);
            return result;
        }

        
        affine_map2 inverted()
        {
            auto determinant = c_[0][0] * c_[1][1] - c_[0][1] * c_[1][0];
            affine_map2 result;
            if (determinant == 0)
            {
                result.c_[0][0] = 0;
                result.c_[0][1] = 0;
                result.c_[0][2] = 0;
                result.c_[1][0] = 0;
                result.c_[1][1] = 0;
                result.c_[1][2] = 0;
            }
            else
            {
                auto inverse_determinant = 1 / determinant;
                result.c_[0][0] = c_[1][1] * inverse_determinant;
                result.c_[0][1] = c_[0][1] * -inverse_determinant;
                result.c_[0][2] = (c_[0][1] * c_[1][2]
                    - c_[0][2] * c_[1][1]) * inverse_determinant;
                result.c_[1][0] = c_[1][0] * -inverse_determinant;
                result.c_[1][1] = c_[0][0] * inverse_determinant;
                result.c_[1][2] = (c_[0][2] * c_[1][0]
                    - c_[0][0] * c_[1][2]) * inverse_determinant;
            }
            return result;
        }

        //// Access
        
        Num coeff(int row, int col) const { return c_[row][col]; }
        
        Num& coeff(int row, int col) { return c_[row][col]; }

    private:
    
#ifdef MEASURES_USE_ANGLES
        void set_rotation_(Num fp_x, Num fp_y, Num angle)
        {
            auto cos_a = std::cos(angle);
            auto sin_a = std::sin(angle);
            c_[0][0] = cos_a; c_[0][1] = -sin_a;
            c_[0][2] = fp_x - cos_a * fp_x + sin_a * fp_y;
            c_[1][0] = sin_a; c_[1][1] = cos_a;
            c_[1][2] = fp_y - sin_a * fp_x - cos_a * fp_y;
        }

        void set_right_rotation_(Num fp_x, Num fp_y, Num sine)
        {
            c_[0][0] = 0; c_[0][1] = -sine;
            c_[0][2] = fp_x + sine * fp_y;
            c_[1][0] = sine; c_[1][1] = 0;
            c_[1][2] = fp_y - sine * fp_x;
        }
#endif

        void set_projection_(Num fp_x, Num fp_y, Num cos_a, Num sin_a)
        {
            auto cc = cos_a * cos_a;
            auto cs = cos_a * sin_a;
            auto ss = sin_a * sin_a;
            auto sxmcy = sin_a * fp_x - cos_a * fp_y;
            c_[0][0] = cc; c_[0][1] = cs;
            c_[0][2] = sin_a * sxmcy;
            c_[1][0] = cs; c_[1][1] = ss;
            c_[1][2] = - cos_a * sxmcy;
        }
        
        void set_reflection_(Num fp_x, Num fp_y, Num cos_a, Num sin_a)
        {
            auto c2ms2 = cos_a * cos_a - sin_a * sin_a;
            auto cs_bis = cos_a * sin_a;
            auto sxmcy = sin_a * fp_x - cos_a * fp_y;
            c_[0][0] = c2ms2; c_[0][1] = cs_bis;
            c_[0][2] = 2 * sin_a * sxmcy;
            c_[1][0] = cs_bis; c_[1][1] = - c2ms2;
            c_[1][2] = -2 * cos_a * sxmcy;
        }
        
        Num c_[2][3];
    };

    // Composition of two plane affine transformations.
    template <class Unit, typename Num1, typename Num2>
    affine_map2<Unit,decltype(Num1()*Num2())> combine(
        affine_map2<Unit,Num1> const& am1, affine_map2<Unit,Num2> const& am2)
    {
        affine_map2<Unit,decltype(Num1()*Num2())> result;
        result.coeff(0, 0)
            = am1.coeff(0, 0) * am2.coeff(0, 0)
            + am1.coeff(0, 1) * am2.coeff(1, 0);
        result.coeff(0, 1)
            = am1.coeff(0, 0) * am2.coeff(0, 1)
            + am1.coeff(0, 1) * am2.coeff(1, 1);
        result.coeff(0, 2)
            = am1.coeff(0, 0) * am2.coeff(0, 2)
            + am1.coeff(0, 1) * am2.coeff(1, 2)
            + am1.coeff(0, 2);
        result.coeff(1, 0)
            = am1.coeff(1, 0) * am2.coeff(0, 0)
            + am1.coeff(1, 1) * am2.coeff(1, 0);
        result.coeff(1, 1)
            = am1.coeff(1, 0) * am2.coeff(0, 1)
            + am1.coeff(1, 1) * am2.coeff(1, 1);
        result.coeff(1, 2)
            = am1.coeff(1, 0) * am2.coeff(0, 2)
            + am1.coeff(1, 1) * am2.coeff(1, 2)
            + am1.coeff(1, 2);
        return result;
    }

    //// Translation
    
    template <class Unit, typename Num>
    affine_map2<Unit,Num> create_translation(vect2<Unit,Num> v)
    {
        return affine_map2<Unit,Num>::translation(v);
    }

#ifdef MEASURES_USE_ANGLES
    //// Rotations
    
    template <class PointUnit, typename PointNum,
        class AngleUnit, typename AngleNum>
    affine_map2<PointUnit,decltype(PointNum()*AngleNum())> create_rotation(
        point2<PointUnit,PointNum> fixed_p, vect1<AngleUnit,AngleNum> angle)
    {
        return affine_map2<PointUnit,decltype(PointNum()*AngleNum())>
            ::rotation(fixed_p, angle);
    }

    template <class PointUnit, typename PointNum>
    affine_map2<PointUnit,PointNum> create_rotation_at_right(
        point2<PointUnit,PointNum> fixed_p)
    {
        return affine_map2<PointUnit,PointNum>::rotation_at_right(fixed_p);
    }

    template <class PointUnit, typename PointNum>
    affine_map2<PointUnit,PointNum> create_rotation_at_left(
        point2<PointUnit,PointNum> fixed_p)
    {
        return affine_map2<PointUnit,PointNum>::rotation_at_left(fixed_p);
    }
#endif

    //// Projections

#ifdef MEASURES_USE_ANGLES
    template <class PointUnit, typename PointNum,
        class AngleUnit, typename AngleNum>
    affine_map2<PointUnit,decltype(PointNum()*AngleNum())> create_projection(
        point2<PointUnit,PointNum> fixed_p,
        point1<AngleUnit,AngleNum> angle)
    {
        return affine_map2<PointUnit,decltype(PointNum()*AngleNum())>
            ::projection(fixed_p, angle);
    }

    template <class PointUnit, typename PointNum,
        class AngleUnit, typename AngleNum>
    affine_map2<PointUnit,decltype(PointNum()*AngleNum())> create_projection(
        point2<PointUnit,PointNum> fixed_p,
        signed_azimuth<AngleUnit,AngleNum> angle)
    {
        return affine_map2<PointUnit,decltype(PointNum()*AngleNum())>
            ::projection(fixed_p, angle);
    }
    
    template <class PointUnit, typename PointNum,
        class AngleUnit, typename AngleNum>
    affine_map2<PointUnit,decltype(PointNum()*AngleNum())> create_projection(
        point2<PointUnit,PointNum> fixed_p,
        unsigned_azimuth<AngleUnit,AngleNum> angle)
    {
        return affine_map2<PointUnit,decltype(PointNum()*AngleNum())>
            ::projection(fixed_p, angle);
    }
#endif
    
    template <class PointUnit, typename PointNum,
        class VectUnit, typename VectNum>
    affine_map2<PointUnit,decltype(PointNum()*VectNum())> create_projection(
        point2<PointUnit,PointNum> fixed_p,
        vect2<VectUnit,VectNum> unit_v)
    {
        return affine_map2<PointUnit,decltype(PointNum()*VectNum())>
            ::projection(fixed_p, unit_v);
    }

    //// Reflections

#ifdef MEASURES_USE_ANGLES
    template <class PointUnit, typename PointNum,
        class AngleUnit, typename AngleNum>
    affine_map2<PointUnit,decltype(PointNum()*AngleNum())> create_reflection(
        point2<PointUnit,PointNum> fixed_p,
        point1<AngleUnit,AngleNum> angle)
    {
        return affine_map2<PointUnit,decltype(PointNum()*AngleNum())>
            ::reflection(fixed_p, angle);
    }

    template <class PointUnit, typename PointNum,
        class AngleUnit, typename AngleNum>
    affine_map2<PointUnit,decltype(PointNum()*AngleNum())> create_reflection(
        point2<PointUnit,PointNum> fixed_p,
        signed_azimuth<AngleUnit,AngleNum> angle)
    {
        return affine_map2<PointUnit,decltype(PointNum()*AngleNum())>
            ::reflection(fixed_p, angle);
    }
    
    template <class PointUnit, typename PointNum,
        class AngleUnit, typename AngleNum>
    affine_map2<PointUnit,decltype(PointNum()*AngleNum())> create_reflection(
        point2<PointUnit,PointNum> fixed_p,
        unsigned_azimuth<AngleUnit,AngleNum> angle)
    {
        return affine_map2<PointUnit,decltype(PointNum()*AngleNum())>
            ::reflection(fixed_p, angle);
    }
#endif
    
    template <class PointUnit, typename PointNum,
        class VectUnit, typename VectNum>
    affine_map2<PointUnit,decltype(PointNum()*VectNum())> create_reflection(
        point2<PointUnit,PointNum> fixed_p,
        vect2<VectUnit,VectNum> unit_v)
    {
        return affine_map2<PointUnit,decltype(PointNum()*VectNum())>
            ::reflection(fixed_p, unit_v);
    }

    //// Scaling
    
    template <class PointUnit, typename PointNum, typename NumX, typename NumY>
    affine_map2<PointUnit,decltype(PointNum()*(NumX()+NumY()))> create_scaling(
        point2<PointUnit,PointNum> fixed_p,
        NumX kx, NumY ky)
    {
        return affine_map2<PointUnit,decltype(PointNum()*(NumX()+NumY()))>
            ::scaling(fixed_p, kx, ky);
    }

    
    //// Point2
    
    template <class Unit, typename Num = double>
    class point2
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit point2() { }

        // Constructs using two numbers.
        template <typename Num2, typename Num3>
        explicit point2(Num2 x, Num3 y): x_(x), y_(y) { }

        // Constructs using an array of two numbers.
        template <typename Num2>
        explicit point2(Num2 const values[]): x_(values[0]), y_(values[1]) { }

        // Constructs using two point1s of the same unit.
        template <typename Num2, typename Num3>
        explicit point2(point1<Unit,Num2> x, point1<Unit,Num3> y):
            x_(x.value()), y_(y.value()) { }

        // Constructs using another point2 of the same unit.
        template <typename Num1>
        point2(point2<Unit,Num1> const& o):
            x_(o.x().value()), y_(o.y().value()) { }

        // Constructs using a unit and two values.
        template <typename Num2, typename Num3>
        point2(typename Unit::magnitude unit, Num2 x, Num3 y):
            x_(static_cast<Num>((unit.offset() - Unit::offset()
                + x * unit.ratio()) / Unit::ratio())),
            y_(static_cast<Num>((unit.offset() - Unit::offset()
                + y * unit.ratio()) / Unit::ratio())) { }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }

        // Get unmutable x component.
        point1<Unit,Num> const x() const { return point1<Unit,Num>(x_); }

        // Get mutable x component.
        point1<Unit,Num>& x()
        { return reinterpret_cast<point1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        point1<Unit,Num> const y() const { return point1<Unit,Num>(y_); }

        // Get mutable y component.
        point1<Unit,Num>& y()
        { return reinterpret_cast<point1<Unit,Num>&>(y_); }
        
        // point2 += vect2 -> point2
        template <typename Num2>
        point2<Unit,Num> operator +=(vect2<Unit,Num2> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            return *this;
        }

        // point2 -= vect2 -> point2
        template <typename Num2>
        point2<Unit,Num> operator -=(vect2<Unit,Num2> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            return *this;
        }

        template <typename Num2>
        point2<Unit,Num> mapped_by(affine_map2<Unit,Num2> const& am) const
        {
            return point2<Unit,Num>(
                am.c_[0][0] * x_ + am.c_[0][1] * y_ + am.c_[0][2],
                am.c_[1][0] * x_ + am.c_[1][1] * y_ + am.c_[1][2]);
        }
        
    private:

        // Components.
        Num x_, y_;
    };

    // midpoint(point2, point2, weight) -> point2
    template <class Unit, typename Num1, typename Num2, typename Num3>
    point2<Unit,decltype(Num1()+Num2())> midpoint(
        point2<Unit,Num1> p1, point2<Unit,Num2> p2, Num3 weight)
    {
        typedef decltype(Num1()+Num2()) ResultNum;
        return point2<Unit,ResultNum>(
            p1.x().value() * static_cast<ResultNum>(1 - weight)
                + p2.x().value() * static_cast<ResultNum>(weight),
            p1.y().value() * static_cast<ResultNum>(1 - weight)
                + p2.y().value() * static_cast<ResultNum>(weight));
    }

    // midpoint(point2, point2) -> point2
    template <class Unit, typename Num1, typename Num2>
    point2<Unit,decltype(Num1()+Num2())> midpoint(
        point2<Unit,Num1> p1, point2<Unit,Num2> p2)
    {
        typedef decltype(Num1()+Num2()) ResultNum;
        return point2<Unit,ResultNum>(
            (p1.x().value() + p2.x().value()) / static_cast<ResultNum>(2),
            (p1.y().value() + p2.y().value()) / static_cast<ResultNum>(2));
    }

    // barycentric_combination(int, point2[], Num[]) -> point2
    template <class Unit, typename Num1, typename Num2>
    point2<Unit,decltype(Num1()*Num2())> barycentric_combination(
        int n, point2<Unit,Num1> p[], Num2 weights[])
    {
        typedef decltype(Num1()*Num2()) ResultNum;
        ResultNum result_x = 0, result_y = 0;
        for (int i = 0; i < n; ++i)
        {
            result_x += p[i].x().value() * weights[i];
            result_y += p[i].y().value() * weights[i];
        }
        return point2<Unit,ResultNum>(result_x, result_y);
    }

    // point2 - point2 -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()-Num2())> operator -(
        point2<Unit,Num1> m1, point2<Unit,Num2> m2)
    {
        return vect2<Unit,decltype(Num1()-Num2())>(
            m1.x().value() - m2.x().value(),
            m1.y().value() - m2.y().value());
    }

    // point2 == point2 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(point2<Unit,Num1> m1, point2<Unit,Num2> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // point2 != point2 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(point2<Unit,Num1> m1, point2<Unit,Num2> m2)
    {
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }

    // is_equal(point2, point2, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(point2<Unit,Num1> m1, point2<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        // "abs" is needed for complex numbers.
        return std::abs(squared_norm_value(m1 - m2))
            <= squared_norm_value(tolerance);
    }

    // point2 + vect2 -> point2
    template <class Unit, typename Num1, typename Num2>
    point2<Unit,decltype(Num1()+Num2())> operator +(
        point2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
        return point2<Unit,decltype(Num1()+Num2())>(
            m1.x().value() + m2.x().value(),
            m1.y().value() + m2.y().value());
    }

    // point2 - vect2 -> point2
    template <class Unit, typename Num1, typename Num2>
    point2<Unit,decltype(Num1()-Num2())> operator -(
        point2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
        return point2<Unit,decltype(Num1()-Num2())>(
            m1.x().value() - m2.x().value(),
            m1.y().value() - m2.y().value());
    }

    // vect2 + vect2 -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()+Num2())> operator +(
        vect2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
        return vect2<Unit,decltype(Num1()+Num2())>(
            m1.x().value() + m2.x().value(),
            m1.y().value() + m2.y().value());
    }

    // vect2 - vect2 -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()-Num2())> operator -(
        vect2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
        return vect2<Unit,decltype(Num1()-Num2())>(
            m1.x().value() - m2.x().value(),
            m1.y().value() - m2.y().value());
    }

    // N * vect2 -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()*Num2())> operator *(
        Num1 n, vect2<Unit,Num2> m)
    {
        return vect2<Unit,decltype(Num1()*Num2())>(
            n * m.x().value(),
            n * m.y().value());
    }

    // vect2 * N -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()*Num2())> operator *(
        vect2<Unit,Num1> m, Num2 n)
    {
        return vect2<Unit,decltype(Num1()*Num2())>(
            m.x().value() * n,
            m.y().value() * n);
    }

    // vect2 / N -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()/Num2())> operator /(
        vect2<Unit,Num1> m, Num2 n)
    {
        return vect2<Unit,decltype(Num1()*Num2())>(
            m.x().value() / n,
            m.y().value() / n);
    }

    template <class Unit, typename Num>
    Num squared_norm_value(vect2<Unit,Num> v)
    { return sqr(v.x().value()) + sqr(v.y().value()); }

    template <class Unit, typename Num>
    vect1<Unit,Num> norm(vect2<Unit,Num> v)
    {
        return vect1<Unit,Num>(
            static_cast<Num>(std::sqrt(squared_norm_value(v))));
    }

    // normalized(vect2) -> vect2
    template <class Unit, typename Num>
    vect2<Unit,Num> normalized(vect2<Unit,Num> v)
    {
        return v / norm(v).value();
    }
#endif


#ifdef MEASURES_USE_3D
    //////////////////// 3-DIMENSIONAL POINTS AND VECTORS ////////////////////

/*??

        linear_map3 inverted(linear_map3 const& at)
        {
            auto determinant
                = c_[0][0] * c_[1][1] * c_[2][2]
                + c_[1][0] * c_[2][1] * c_[0][2]
                + c_[2][0] * c_[0][1] * c_[1][2]
                - c_[0][0] * c_[2][1] * c_[1][2]
                - c_[2][0] * c_[1][1] * c_[0][2]
                - c_[1][0] * c_[0][1] * c_[2][2];
            linear_map3 result;
            if (determinant == 0)
            {
                result.c_[0][0] = 0;
                result.c_[0][1] = 0;
                result.c_[0][2] = 0;
                result.c_[1][0] = 0;
                result.c_[1][1] = 0;
                result.c_[1][2] = 0;
                result.c_[2][0] = 0;
                result.c_[2][1] = 0;
                result.c_[2][2] = 0;
            }
            else
            {
                result.c_[0][0] = (lt.c_[1][1] * lt.c_[2][2]
                    - lt.c_[1][2] * lt.c_[2][1]) * (1 / determinant);
                result.c_[0][1] = (lt.c_[0][2] * lt.c_[2][1]
                    - lt.c_[0][1] * lt.c_[2][2]) * (1 / determinant);
                result.c_[0][2] = (lt.c_[0][1] * lt.c_[1][2]
                    - lt.c_[0][2] * lt.c_[1][1]) * (1 / determinant);
                result.c_[1][0] = (lt.c_[1][2] * lt.c_[2][0]
                    - lt.c_[1][0] * lt.c_[2][2]) * (1 / determinant);
                result.c_[1][1] = (lt.c_[0][0] * lt.c_[2][2]
                    - lt.c_[0][2] * lt.c_[2][0]) * (1 / determinant);
                result.c_[1][2] = (lt.c_[0][2] * lt.c_[1][0]
                    - lt.c_[0][0] * lt.c_[1][2]) * (1 / determinant);
                result.c_[2][0] = (lt.c_[1][0] * lt.c_[2][1]
                    - lt.c_[1][1] * lt.c_[2][0]) * (1 / determinant);
                result.c_[2][1] = (lt.c_[0][1] * lt.c_[2][0]
                    - lt.c_[0][0] * lt.c_[2][1]) * (1 / determinant);
                result.c_[2][2] = (lt.c_[0][0] * lt.c_[1][1]
                    - lt.c_[0][1] * lt.c_[1][0]) * (1 / determinant);
            }
            return result;
        }

        Num coeff(int row, int col) const { return c_[row][col]; }
        
        Num& coeff(int row, int col) { return c_[row][col]; }
*/

    template <typename Num = double>
    class linear_map3
    {
        template <class, typename> friend class vect3;    
    public:

#ifdef MEASURES_USE_ANGLES
        // Rotation by a relative angle
        // about a line identified by a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <class DirUnit, typename DirNum,
            class AngleUnit, typename AngleNum>
        static linear_map3 rotation(
            vect3<DirUnit,DirNum> unit_v,
            vect1<AngleUnit,AngleNum> angle)
        {
            linear_map3 result;
            auto cos_a = cos(angle);
            auto sin_a = sin(angle);
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            result.c_[0][0] = u * u * (1 - cos_a) + cos_a;
            result.c_[0][1] = u * v * (1 - cos_a) - w * sin_a;
            result.c_[0][2] = u * w * (1 - cos_a) + v * sin_a;
            result.c_[1][0] = u * v * (1 - cos_a) + w * sin_a;
            result.c_[1][1] = v * v * (1 - cos_a) + cos_a;
            result.c_[1][2] = v * w * (1 - cos_a) - u * sin_a;
            result.c_[2][0] = u * w * (1 - cos_a) - v * sin_a;
            result.c_[2][1] = v * w * (1 - cos_a) + u * sin_a;
            result.c_[2][2] = w * w * (1 - cos_a) + cos_a;
            return result;
        }

        // Rotation at right by a right angle about a line
        // identified by a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <class DirUnit, typename DirNum>
        static linear_map3 rotation_at_right(
            vect3<DirUnit,DirNum> unit_v)
        {
            return rotation_at_left_about_unit_vector_(unit_v, -1);
        }

        // Rotation at left by a right angle about a line
        // identified by a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <class DirUnit, typename DirNum>
        static linear_map3 rotation_at_left(
            vect3<DirUnit,DirNum> unit_v)
        {
            return rotation_at_left_about_unit_vector_(unit_v, 1);
        }
#endif
        
        //// Projections
        
        // Projection onto a line identified by a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <class DirUnit, typename DirNum>
        static linear_map3 projection_onto_line(vect3<DirUnit,DirNum> unit_v)
        {
            linear_map3 result;
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            result.c_[0][0] = u * u;
            result.c_[0][1] = u * v;
            result.c_[0][2] = u * w;
            result.c_[1][0] = u * v;
            result.c_[1][1] = v * v;
            result.c_[1][2] = v * w;
            result.c_[2][0] = u * w;
            result.c_[2][1] = v * w;
            result.c_[2][2] = w * w;
            return result;
        }
        
        // Projection onto a plane identified
        // by a unit normal vector.
        // Precondition: norm(unit_v).value() == 1
        template <class NormalUnit, typename NormalNum>
        static linear_map3 projection_onto_plane(
            vect3<NormalUnit,NormalNum> unit_v)
        {
            return projection_onto_plane(
                unit_v.x().value(), unit_v.y().value(), unit_v.z().value());
        }

        // Projection onto a plane identified by the coefficients
        // of the normalized equation ax + by + cz = 0.
        // Precondition: a * a + b * b + c * c == 1.
        template <typename CoeffNum>
        static linear_map3 projection_onto_plane(
            CoeffNum a, CoeffNum b, CoeffNum c)
        {
            linear_map3 result;
            result.c_[0][0] = b * b + c * c;
            result.c_[0][1] = -a * b;
            result.c_[0][2] = -a * c;
            result.c_[1][0] = -a * b;
            result.c_[1][1] = a * a + c * c;
            result.c_[1][2] = -b * c;
            result.c_[2][0] = -a * c;
            result.c_[2][1] = -b * c;
            result.c_[2][2] = a * a + b * b;
            return result;
        }
        
        //// Reflections
        
        // Reflection over a line identified by a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <class DirUnit, typename DirNum>
        static linear_map3 reflection_over_line(vect3<DirUnit,DirNum> unit_v)
        {
            linear_map3 result;
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            result.c_[0][0] = 2 * u * u - 1;
            result.c_[0][1] = 2 * u * v;
            result.c_[0][2] = 2 * u * w;
            result.c_[1][0] = 2 * u * v;
            result.c_[1][1] = 2 * v * v - 1;
            result.c_[1][2] = 2 * v * w;
            result.c_[2][0] = 2 * u * w;
            result.c_[2][1] = 2 * v * w;
            result.c_[2][2] = 2 * w * w - 1;
            return result;
        }

        // Reflection over a plane identified
        // by a unit normal vector.
        // Precondition: norm(unit_v).value() == 1
        template <class NormalUnit, typename NormalNum>
        static linear_map3 reflection_over_plane(
            vect3<NormalUnit,NormalNum> unit_v)
        {
            return reflection_over_plane(
                unit_v.x().value(), unit_v.y().value(), unit_v.z().value());
        }
        
        // Reflection over a plane identified by the coefficients
        // of the normalized equation ax + by + cz = 0.
        // Precondition: a * a + b * b + c * c == 1.
        template <typename CoeffNum>
        static linear_map3 reflection_over_plane(
            CoeffNum a, CoeffNum b, CoeffNum c)
        {
            linear_map3 result;
            result.c_[0][0] = 2 * (b * b + c * c) - 1;
            result.c_[0][1] = -2 * a * b;
            result.c_[0][2] = -2 * a * c;
            result.c_[1][0] = -2 * a * b;
            result.c_[1][1] = 2 * (a * a + c * c) - 1;
            result.c_[1][2] = -2 * b * c;
            result.c_[2][0] = -2 * a * c;
            result.c_[2][1] = -2 * b * c;
            result.c_[2][2] = 2 * (a * a + b * b) - 1;
            return result;
        }

        //// Scaling by three factors.
        template <typename NumX, typename NumY, typename NumZ>
        static linear_map3 scaling(NumX kx, NumY ky, NumZ kz)
        {
            linear_map3<Num> result;
            result.c_[0][0] = kx; result.c_[0][1] = 0; result.c_[0][2] = 0;
            result.c_[1][0] = 0; result.c_[1][1] = ky; result.c_[1][2] = 0;
            result.c_[2][0] = 0; result.c_[2][1] = 0; result.c_[2][2] = kz;
            return result;
        }

        //// Inversion 
        linear_map3 inverted()
        {
            auto determinant
                = c_[0][0] * c_[1][1] * c_[2][2]
                + c_[1][0] * c_[2][1] * c_[0][2]
                + c_[2][0] * c_[0][1] * c_[1][2]
                - c_[0][0] * c_[2][1] * c_[1][2]
                - c_[2][0] * c_[1][1] * c_[0][2]
                - c_[1][0] * c_[0][1] * c_[2][2];
            linear_map3 result;
            if (determinant == 0)
            {
                result.c_[0][0] = 0;
                result.c_[0][1] = 0;
                result.c_[0][2] = 0;
                result.c_[1][0] = 0;
                result.c_[1][1] = 0;
                result.c_[1][2] = 0;
                result.c_[2][0] = 0;
                result.c_[2][1] = 0;
                result.c_[2][2] = 0;
            }
            else
            {
                result.c_[0][0] = (c_[1][1] * c_[2][2]
                    - c_[1][2] * c_[2][1]) * (1 / determinant);
                result.c_[0][1] = (c_[0][2] * c_[2][1]
                    - c_[0][1] * c_[2][2]) * (1 / determinant);
                result.c_[0][2] = (c_[0][1] * c_[1][2]
                    - c_[0][2] * c_[1][1]) * (1 / determinant);
                result.c_[1][0] = (c_[1][2] * c_[2][0]
                    - c_[1][0] * c_[2][2]) * (1 / determinant);
                result.c_[1][1] = (c_[0][0] * c_[2][2]
                    - c_[0][2] * c_[2][0]) * (1 / determinant);
                result.c_[1][2] = (c_[0][2] * c_[1][0]
                    - c_[0][0] * c_[1][2]) * (1 / determinant);
                result.c_[2][0] = (c_[1][0] * c_[2][1]
                    - c_[1][1] * c_[2][0]) * (1 / determinant);
                result.c_[2][1] = (c_[0][1] * c_[2][0]
                    - c_[0][0] * c_[2][1]) * (1 / determinant);
                result.c_[2][2] = (c_[0][0] * c_[1][1]
                    - c_[0][1] * c_[1][0]) * (1 / determinant);
            }
            return result;
        }
        
        //// Access
        
        Num coeff(int row, int col) const { return c_[row][col]; }
        
        Num& coeff(int row, int col) { return c_[row][col]; }
        
    private:

#ifdef MEASURES_USE_ANGLES
        template <typename PointNum, class DirUnit, typename DirNum,
            typename AngleNum>
        static linear_map3 rotation_at_left_about_unit_vector_(
            vect3<DirUnit,DirNum> unit_v, AngleNum sine)
        {
            linear_map3 result;
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            result.c_[0][0] = u * u;
            result.c_[0][1] = u * v - w * sine;
            result.c_[0][2] = u * w + v * sine;
            result.c_[1][0] = u * v + w * sine;
            result.c_[1][1] = v * v;
            result.c_[1][2] = v * w - u * sine;
            result.c_[2][0] = u * w - v * sine;
            result.c_[2][1] = v * w + u * sine;
            result.c_[2][2] = w * w;
            return result;
        }
#endif
       
        Num c_[3][3];
    };

    // Composition of two space linear transformations.
    template <typename Num1, typename Num2>
    linear_map3<decltype(Num1()*Num2())> combine(
        linear_map3<Num1> const& lm1, linear_map3<Num2> const& lm2)
    {
        linear_map3<decltype(Num1()*Num2())> result;
        result.coeff(0, 0)
            = lm1.coeff(0, 0) * lm2.coeff(0, 0)
            + lm1.coeff(0, 1) * lm2.coeff(1, 0)
            + lm1.coeff(0, 2) * lm2.coeff(2, 0);
        result.coeff(0, 1)
            = lm1.coeff(0, 0) * lm2.coeff(0, 1)
            + lm1.coeff(0, 1) * lm2.coeff(1, 1)
            + lm1.coeff(0, 2) * lm2.coeff(2, 1);
        result.coeff(0, 2)
            = lm1.coeff(0, 0) * lm2.coeff(0, 2)
            + lm1.coeff(0, 1) * lm2.coeff(1, 2)
            + lm1.coeff(0, 2) * lm2.coeff(2, 2);
        result.coeff(1, 0)
            = lm1.coeff(1, 0) * lm2.coeff(0, 0)
            + lm1.coeff(1, 1) * lm2.coeff(1, 0)
            + lm1.coeff(1, 2) * lm2.coeff(2, 0);
        result.coeff(1, 1)
            = lm1.coeff(1, 0) * lm2.coeff(0, 1)
            + lm1.coeff(1, 1) * lm2.coeff(1, 1)
            + lm1.coeff(1, 2) * lm2.coeff(2, 1);
        result.coeff(1, 2)
            = lm1.coeff(1, 0) * lm2.coeff(0, 2)
            + lm1.coeff(1, 1) * lm2.coeff(1, 2)
            + lm1.coeff(1, 2) * lm2.coeff(2, 2);
        result.coeff(2, 0)
            = lm1.coeff(2, 0) * lm2.coeff(0, 0)
            + lm1.coeff(2, 1) * lm2.coeff(1, 0)
            + lm1.coeff(2, 2) * lm2.coeff(2, 0);
        result.coeff(2, 1)
            = lm1.coeff(2, 0) * lm2.coeff(0, 1)
            + lm1.coeff(2, 1) * lm2.coeff(1, 1)
            + lm1.coeff(2, 2) * lm2.coeff(2, 1);
        result.coeff(2, 2)
            = lm1.coeff(2, 0) * lm2.coeff(0, 2)
            + lm1.coeff(2, 1) * lm2.coeff(1, 2)
            + lm1.coeff(2, 2) * lm2.coeff(2, 2);
        return result;
    }

    
    template <class Unit, typename Num = double>
    class vect3
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit vect3() { }

        // Constructs using three numbers.
        template <typename Num1, typename Num2, typename Num3>
        explicit vect3(Num1 x, Num2 y, Num3 z): x_(x), y_(y), z_(z) { }

        // Constructs using an array of three numbers.
        template <typename Num1>
        explicit vect3(Num1 const values[]):
            x_(values[0]), y_(values[1]), z_(values[2]) { }

        // Constructs using three vect1s of the same unit.
        template <typename Num1, typename Num2, typename Num3>
        explicit vect3(vect1<Unit,Num1> x, vect1<Unit,Num2> y,
            vect1<Unit,Num3> z):
            x_(x.value()), y_(y.value()), z_(z.value()) { }

        // Constructs using another vect3 of the same unit.
        template <typename Num1>
        vect3(vect3<Unit,Num1> const& o):
            x_(o.x().value()), y_(o.y().value()), z_(o.z().value()) { }

        // +vect3 -> vect3
        vect3<Unit,Num> operator +() const { return *this; }

        // -vect3 -> vect3
        vect3<Unit,Num> operator -() const
        { return vect3<Unit,Num>(-x_, -y_, -z_); }

        // vect3 += vect3 -> vect3
        template <typename Num1>
        vect3<Unit,Num> operator +=(vect3<Unit,Num1> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            z_ += m2.z().value();
            return *this;
        }

        // vect3 -= vect3 -> vect3
        template <typename Num1>
        vect3<Unit,Num> operator -=(vect3<Unit,Num1> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            z_ -= m2.z().value();
            return *this;
        }

        // vect3 *= N -> vect3
        template <typename Num1>
        vect3<Unit,Num> operator *=(Num1 n)
        {
            x_ *= n;
            y_ *= n;
            z_ *= n;
            return *this;
        }

        // vect3 /= N -> vect3
        template <typename Num1>
        vect3<Unit,Num> operator /=(Num1 n)
        {
            x_ /= n;
            y_ /= n;
            z_ /= n;
            return *this;
        }

        // Constructs using a unit and a value.
        template <typename Num1, typename Num2, typename Num3>
        vect3(typename Unit::magnitude unit, Num1 x, Num2 y, Num3 z):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio()))),
            y_(static_cast<Num>(y * (unit.ratio() / Unit::ratio()))),
            z_(static_cast<Num>(z * (unit.ratio() / Unit::ratio()))) { }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }

        // Get unmutable x component.
        vect1<Unit,Num> const x() const { return vect1<Unit,Num>(x_); }

        // Get mutable x component.
        vect1<Unit,Num>& x()
        { return reinterpret_cast<vect1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        vect1<Unit,Num> const y() const
        { return vect1<Unit,Num>(y_); }

        // Get mutable y component.
        vect1<Unit,Num>& y()
        { return reinterpret_cast<vect1<Unit,Num>&>(y_); }

        // Get unmutable z component.
        vect1<Unit,Num> const z() const { return vect1<Unit,Num>(z_); }

        // Get mutable z component.
        vect1<Unit,Num>& z()
		{ return reinterpret_cast<vect1<Unit,Num>&>(z_); }

        template <typename Num2>
        vect3<Unit,Num> mapped_by(linear_map3<Num2> const& lt) const
        {
            return vect3<Unit,Num>(
                lt.c_[0][0] * x_ + lt.c_[0][1] * y_ + lt.c_[0][2] * z_,
                lt.c_[1][0] * x_ + lt.c_[1][1] * y_ + lt.c_[1][2] * z_,
                lt.c_[2][0] * x_ + lt.c_[2][1] * y_ + lt.c_[2][2] * z_);
        }

    private:

        // Components.
        Num x_, y_, z_;
    };

    // vect3 == vect3 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(vect3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // vect3 != vect3 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(vect3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value()
            || m1.z().value() != m2.z().value();
    }

    // is_equal(vect3, vect3, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(vect3<Unit,Num1> m1, vect3<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        // "abs" is needed for complex numbers.
        return std::abs(squared_norm_value(m1 - m2))
            <= squared_norm_value(tolerance);
    }

    
    template <class Unit, typename Num = double>
    class affine_map3
    {
        template <class, typename> friend class point3;    
    public:

        // Translation.
        template <typename VectNum>
        static affine_map3 translation(vect3<Unit,VectNum> v)
        {
            affine_map3 result;
            result.c_[0][0] = 1;
            result.c_[0][1] = 0;
            result.c_[0][2] = 0;
            result.c_[0][3] = v.x().value();
            result.c_[1][0] = 0;
            result.c_[1][1] = 1;
            result.c_[1][2] = 0;
            result.c_[1][3] = v.y().value();
            result.c_[2][0] = 0;
            result.c_[2][1] = 0;
            result.c_[2][2] = 1;
            result.c_[2][3] = v.y().value();
            return result;
        }
        
#ifdef MEASURES_USE_ANGLES
        //// Rotations.
        
        // Rotation by a relative angle
        // about a line identified by a point and a unit vector.
        // Precondition: norm(v).value() == 1
        template <typename PointNum, class DirUnit,
            typename DirNum, class AngleUnit, typename AngleNum>
        static affine_map3 rotation(
            point3<Unit,PointNum> fixed_p,
            vect3<DirUnit,DirNum> unit_v,
            vect1<AngleUnit,AngleNum> angle)
        {
            affine_map3 result;
            auto cos_a = cos(angle);
            auto sin_a = sin(angle);
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            auto u2 = u * u;
            auto v2 = v * v;
            auto w2 = w * w;
            auto a = fixed_p.x().value();
            auto b = fixed_p.y().value();
            auto c = fixed_p.z().value();
            result.c_[0][0] = u2 * (1 - cos_a) + cos_a;
            result.c_[0][1] = u * v * (1 - cos_a) - w * sin_a;
            result.c_[0][2] = u * w * (1 - cos_a) + v * sin_a;
            result.c_[0][3] = (a * (1 - u2) - u * (b * v + c * w))
                * (1 - cos_a) + (b * w - c * v) * sin_a;
            result.c_[1][0] = u * v * (1 - cos_a) + w * sin_a;
            result.c_[1][1] = v2 * (1 - cos_a) + cos_a;
            result.c_[1][2] = v * w * (1 - cos_a) - u * sin_a;
            result.c_[1][3] = (b * (1 - v2) - v * (a * u + c * w))
                * (1 - cos_a) + (c * u - a * w) * sin_a;
            result.c_[2][0] = u * w * (1 - cos_a) - v * sin_a;
            result.c_[2][1] = v * w * (1 - cos_a) + u * sin_a;
            result.c_[2][2] = w2 * (1 - cos_a) + cos_a;
            result.c_[2][3] = (c * (1 - w2) - w * (a * u + b * v))
                * (1 - cos_a) + (a * v - b * u) * sin_a;
            return result;
        }

        // Rotation at right by a right angle about a line
        // identified by a point and a unit vector.
        // Precondition: norm(v).value() == 1
        template <typename PointNum, class DirUnit, typename DirNum>
        static affine_map3 rotation_at_right(
            point3<Unit,PointNum> fixed_p, vect3<DirUnit,DirNum> unit_v)
        {
            return rotation_at_left_about_unit_vector_(
                fixed_p, unit_v, -1);
        }

        // Rotation at left by a right angle about a line
        // identified by a point and a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <typename PointNum, class DirUnit, typename DirNum>
        static affine_map3 rotation_at_left(
            point3<Unit,PointNum> fixed_p, vect3<DirUnit,DirNum> unit_v)
        {
            return rotation_at_left_about_unit_vector_(
                fixed_p, unit_v, 1);
        }
#endif
        
        //// Projections.
        
        // Projection onto a line identified by a point and a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <typename PointNum, class DirUnit, typename DirNum>
        static affine_map3 projection_onto_line(
            point3<Unit,PointNum> fixed_p, vect3<DirUnit,DirNum> unit_v)
        {
            affine_map3 result;
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            auto a = fixed_p.x().value();
            auto b = fixed_p.y().value();
            auto c = fixed_p.z().value();
            result.c_[0][0] = u * u;
            result.c_[0][1] = u * v;
            result.c_[0][2] = u * w;
            result.c_[0][3] = a - u * u * a - u * v * b - u * w * c;
            result.c_[1][0] = u * v;
            result.c_[1][1] = v * v;
            result.c_[1][2] = v * w;
            result.c_[1][3] = b - u * v * a - v * v * b - v * w * c;
            result.c_[2][0] = u * w;
            result.c_[2][1] = v * w;
            result.c_[2][2] = w * w;
            result.c_[2][3] = c - u * w * a - v * w * b - w * w * c;
            return result;
        }
        
        // Projection onto a plane identified by a point
        // and a unit normal vector.
        // Precondition: norm(unit_v).value() == 1
        template <typename PointNum, class NormalUnit, typename NormalNum>
        static affine_map3 projection_onto_plane(
            point3<Unit,PointNum> fixed_p, vect3<NormalUnit,NormalNum> unit_v)
        {
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            return projection_onto_plane(u, v, w,
                - u * fixed_p.x().value()
                - v * fixed_p.y().value()
                - w * fixed_p.z().value());
        }

        // Projection onto a plane identified by the coefficients
        // of the normalized equation ax + by + cz + d = 0.
        // Precondition: a * a + b * b + c * c == 1.
        template <typename CoeffNum>
        static affine_map3 projection_onto_plane(
            CoeffNum a, CoeffNum b, CoeffNum c, CoeffNum d)
        {
            affine_map3 result;
            result.c_[0][0] = b * b + c * c;
            result.c_[0][1] = -a * b;
            result.c_[0][2] = -a * c;
            result.c_[0][3] = -a * d;
            result.c_[1][0] = -a * b;
            result.c_[1][1] = a * a + c * c;
            result.c_[1][2] = -b * c;
            result.c_[1][3] = -b * d;
            result.c_[2][0] = -a * c;
            result.c_[2][1] = -b * c;
            result.c_[2][2] = a * a + b * b;
            result.c_[2][3] = -c * d;
            return result;
        }

        //// Reflections.
        
        // Reflection over a line identified by a point and a unit vector.
        // Precondition: norm(unit_v).value() == 1
        template <typename PointNum, class DirUnit, typename DirNum>
        static affine_map3 reflection_over_line(
            point3<Unit,PointNum> fixed_p, vect3<DirUnit,DirNum> unit_v)
        {
            affine_map3 result;
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            auto a = fixed_p.x().value();
            auto b = fixed_p.y().value();
            auto c = fixed_p.z().value();
            result.c_[0][0] = 2 * u * u - 1;
            result.c_[0][1] = 2 * u * v;
            result.c_[0][2] = 2 * u * w;
            result.c_[0][3] = 2 * (a - u * u * a - u * v * b - u * w * c);
            result.c_[1][0] = 2 * u * v;
            result.c_[1][1] = 2 * v * v - 1;
            result.c_[1][2] = 2 * v * w;
            result.c_[1][3] = 2 * (b - u * v * a - v * v * b - v * w * c);
            result.c_[2][0] = 2 * u * w;
            result.c_[2][1] = 2 * v * w;
            result.c_[2][2] = 2 * w * w - 1;
            result.c_[2][3] = 2 * (c - u * w * a - v * w * b - w * w * c);
            return result;
        }
        
        // Reflection over a plane identified by a point
        // and a unit normal vector.
        // Precondition: norm(unit_v).value() == 1
        template <typename PointNum, class NormalUnit, typename NormalNum>
        static affine_map3 reflection_over_plane(
            point3<Unit,PointNum> fixed_p, vect3<NormalUnit,NormalNum> unit_v)
        {
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            return reflection_over_plane(u, v, w,
                - u * fixed_p.x().value()
                - v * fixed_p.y().value()
                - w * fixed_p.z().value());
        }

        // Reflection over a plane identified by the coefficients
        // of the normalized equation ax + by + cz + d = 0.
        // Precondition: a * a + b * b + c * c == 1.
        template <typename CoeffNum>
        static affine_map3 reflection_over_plane(
            CoeffNum a, CoeffNum b, CoeffNum c, CoeffNum d)
        {
            affine_map3 result;
            result.c_[0][0] = 2 * (b * b + c * c) - 1;
            result.c_[0][1] = -2 * a * b;
            result.c_[0][2] = -2 * a * c;
            result.c_[0][3] = -2 * a * d;
            result.c_[1][0] = -2 * a * b;
            result.c_[1][1] = 2 * (a * a + c * c) - 1;
            result.c_[1][2] = -2 * b * c;
            result.c_[1][3] = -2 * b * d;
            result.c_[2][0] = -2 * a * c;
            result.c_[2][1] = -2 * b * c;
            result.c_[2][2] = 2 * (a * a + b * b) - 1;
            result.c_[2][3] = -2 * c * d;
            return result;
        }
        
        // Scaling by three factors from a fixed point.
        template <typename PointNum,
            typename NumX, typename NumY, typename NumZ>
        static affine_map3 scaling(point3<Unit,PointNum> fixed_p,
            NumX kx, NumY ky, NumZ kz)
        {
            affine_map3 result;
            result.c_[0][0] = kx;
            result.c_[0][1] = 0;
            result.c_[0][2] = 0;
            result.c_[0][3] = fixed_p.x().value() * (1 - kx);
            result.c_[1][0] = 0;
            result.c_[1][1] = ky;
            result.c_[1][2] = 0;
            result.c_[1][3] = fixed_p.y().value() * (1 - ky);
            result.c_[2][0] = 0;
            result.c_[2][1] = 0;
            result.c_[2][2] = kz;
            result.c_[2][3] = fixed_p.z().value() * (1 - ky);
            return result;
        }
        
        // Inversion.
        affine_map3 inverted()
        {
            auto determinant
                = c_[0][0] * c_[1][1] * c_[2][2]
                + c_[1][0] * c_[2][1] * c_[0][2]
                + c_[2][0] * c_[0][1] * c_[1][2]
                - c_[0][0] * c_[2][1] * c_[1][2]
                - c_[2][0] * c_[1][1] * c_[0][2]
                - c_[1][0] * c_[0][1] * c_[2][2];
            affine_map3 result;
            if (determinant == 0)
            {
                result.c_[0][0] = 0;
                result.c_[0][1] = 0;
                result.c_[0][2] = 0;
                result.c_[0][3] = 0;
                result.c_[1][0] = 0;
                result.c_[1][1] = 0;
                result.c_[1][2] = 0;
                result.c_[1][3] = 0;
                result.c_[2][0] = 0;
                result.c_[2][1] = 0;
                result.c_[2][2] = 0;
                result.c_[2][3] = 0;
            }
            else
            {
                result.c_[0][0] = (c_[1][1] * c_[2][2]
                    - c_[1][2] * c_[2][1]) * (1 / determinant);
                result.c_[0][1] = (c_[0][2] * c_[2][1]
                    - c_[0][1] * c_[2][2]) * (1 / determinant);
                result.c_[0][2] = (c_[0][1] * c_[1][2]
                    - c_[0][2] * c_[1][1]) * (1 / determinant);
                result.c_[0][3]
                    = (c_[0][1] * c_[1][3] * c_[2][2]
                    + c_[0][2] * c_[1][1] * c_[2][3]
                    + c_[0][3] * c_[1][2] * c_[2][1]
                    - c_[0][1] * c_[1][2] * c_[2][3]
                    - c_[0][2] * c_[1][3] * c_[2][1]
                    - c_[0][3] * c_[1][1] * c_[2][2]
                    ) * (1 / determinant);
                result.c_[1][0] = (c_[1][2] * c_[2][0]
                    - c_[1][0] * c_[2][2]) * (1 / determinant);
                result.c_[1][1] = (c_[0][0] * c_[2][2]
                    - c_[0][2] * c_[2][0]) * (1 / determinant);
                result.c_[1][2] = (c_[0][2] * c_[1][0]
                    - c_[0][0] * c_[1][2]) * (1 / determinant);
                result.c_[1][3]
                    = (c_[0][0] * c_[1][2] * c_[2][3]
                    + c_[0][2] * c_[1][3] * c_[2][0]
                    + c_[0][3] * c_[1][0] * c_[2][2]
                    - c_[0][0] * c_[1][3] * c_[2][2]
                    - c_[0][2] * c_[1][0] * c_[2][3]
                    - c_[0][3] * c_[1][2] * c_[2][0]
                    ) * (1 / determinant);
                result.c_[2][0] = (c_[1][0] * c_[2][1]
                    - c_[1][1] * c_[2][0]) * (1 / determinant);
                result.c_[2][1] = (c_[0][1] * c_[2][0]
                    - c_[0][0] * c_[2][1]) * (1 / determinant);
                result.c_[2][2] = (c_[0][0] * c_[1][1]
                    - c_[0][1] * c_[1][0]) * (1 / determinant);
                result.c_[2][3]
                    = (c_[0][0] * c_[1][3] * c_[2][1]
                    + c_[0][1] * c_[1][0] * c_[2][3]
                    + c_[0][3] * c_[1][1] * c_[2][0]
                    - c_[0][0] * c_[1][1] * c_[2][3]
                    - c_[0][1] * c_[1][3] * c_[2][0]
                    - c_[0][3] * c_[1][0] * c_[2][1]
                    ) * (1 / determinant);
            }
            return result;
        }

        Num coeff(int row, int col) const { return c_[row][col]; }
        
        Num& coeff(int row, int col) { return c_[row][col]; }

    private:
#ifdef MEASURES_USE_ANGLES
        template <typename PointNum, class DirUnit, typename DirNum,
            typename AngleNum>
        static affine_map3 rotation_at_left_about_unit_vector_(
            point3<Unit,PointNum> fixed_p, vect3<DirUnit,DirNum> unit_v,
            AngleNum sine)
        {
            affine_map3 result;
            auto u = unit_v.x().value();
            auto v = unit_v.y().value();
            auto w = unit_v.z().value();
            auto u2 = u * u;
            auto v2 = v * v;
            auto w2 = w * w;
            auto a = fixed_p.x().value();
            auto b = fixed_p.y().value();
            auto c = fixed_p.z().value();
            result.c_[0][0] = u2;
            result.c_[0][1] = u * v - w * sine;
            result.c_[0][2] = u * w + v * sine;
            result.c_[0][3] = (a * (1 - u2) - u * (b * v + c * w))
                + (b * w - c * v) * sine;
            result.c_[1][0] = u * v + w * sine;
            result.c_[1][1] = v2;
            result.c_[1][2] = v * w - u * sine;
            result.c_[1][3] = (b * (1 - v2) - v * (a * u + c * w))
                + (c * u - a * w) * sine;
            result.c_[2][0] = u * w - v * sine;
            result.c_[2][1] = v * w + u * sine;
            result.c_[2][2] = w2;
            result.c_[2][3] = (c * (1 - w2) - w * (a * u + b * v))
                + (a * v - b * u) * sine;
            return result;
        }
#endif
        
        Num c_[3][4];
    };
    
    // Composition of two space affine transformations.
    template <class Unit, typename Num1, typename Num2>
    affine_map3<Unit,decltype(Num1()*Num2())> combine(
        affine_map3<Unit,Num1> const& am1, affine_map3<Unit,Num2> const& am2)
    {
        affine_map3<Unit,decltype(Num1()*Num2())> result;
        result.coeff(0, 0)
            = am1.coeff(0, 0) * am2.coeff(0, 0)
            + am1.coeff(0, 1) * am2.coeff(1, 0)
            + am1.coeff(0, 2) * am2.coeff(2, 0);
        result.coeff(0, 1)
            = am1.coeff(0, 0) * am2.coeff(0, 1)
            + am1.coeff(0, 1) * am2.coeff(1, 1)
            + am1.coeff(0, 2) * am2.coeff(2, 1);
        result.coeff(0, 2)
            = am1.coeff(0, 0) * am2.coeff(0, 2)
            + am1.coeff(0, 1) * am2.coeff(1, 2)
            + am1.coeff(0, 2) * am2.coeff(2, 2);
        result.coeff(0, 3)
            = am1.coeff(0, 0) * am2.coeff(0, 3)
            + am1.coeff(0, 1) * am2.coeff(1, 3)
            + am1.coeff(0, 2) * am2.coeff(2, 3)
            + am1.coeff(0, 3);
        result.coeff(1, 0)
            = am1.coeff(1, 0) * am2.coeff(0, 0)
            + am1.coeff(1, 1) * am2.coeff(1, 0)
            + am1.coeff(1, 2) * am2.coeff(2, 0);
        result.coeff(1, 1)
            = am1.coeff(1, 0) * am2.coeff(0, 1)
            + am1.coeff(1, 1) * am2.coeff(1, 1)
            + am1.coeff(1, 2) * am2.coeff(2, 1);
        result.coeff(1, 2)
            = am1.coeff(1, 0) * am2.coeff(0, 2)
            + am1.coeff(1, 1) * am2.coeff(1, 2)
            + am1.coeff(1, 2) * am2.coeff(2, 2);
        result.coeff(1, 3)
            = am1.coeff(1, 0) * am2.coeff(0, 3)
            + am1.coeff(1, 1) * am2.coeff(1, 3)
            + am1.coeff(1, 2) * am2.coeff(2, 3)
            + am1.coeff(1, 3);
        result.coeff(2, 0)
            = am1.coeff(2, 0) * am2.coeff(0, 0)
            + am1.coeff(2, 1) * am2.coeff(1, 0)
            + am1.coeff(2, 2) * am2.coeff(2, 0);
        result.coeff(2, 1)
            = am1.coeff(2, 0) * am2.coeff(0, 1)
            + am1.coeff(2, 1) * am2.coeff(1, 1)
            + am1.coeff(2, 2) * am2.coeff(2, 1);
        result.coeff(2, 2)
            = am1.coeff(2, 0) * am2.coeff(0, 2)
            + am1.coeff(2, 1) * am2.coeff(1, 2)
            + am1.coeff(2, 2) * am2.coeff(2, 2);
        result.coeff(2, 3)
            = am1.coeff(2, 0) * am2.coeff(0, 3)
            + am1.coeff(2, 1) * am2.coeff(1, 3)
            + am1.coeff(2, 2) * am2.coeff(2, 3)
            + am1.coeff(2, 3);
        return result;
    }

    
    template <class Unit, typename Num = double>
    class point3
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit point3() { }

        // Constructs using two numbers of the same number type.
        template <typename Num1, typename Num2, typename Num3>
        explicit point3(Num1 x, Num2 y, Num3 z): x_(x), y_(y), z_(z) { }

        // Constructs using an array of two numbers of the same number type.
        template <typename Num1>
        explicit point3(Num1 const values[]):
            x_(values[0]), y_(values[1]), z_(values[2]) { }

        // Constructs using three point1s of the same unit and number type.
        template <typename Num1, typename Num2, typename Num3>
        explicit point3(point1<Unit,Num1> x, point1<Unit,Num2> y,
            point1<Unit,Num3> z):
            x_(x.value()), y_(y.value()), z_(z.value()) { }

        // Constructs using another point3 of the same unit and number type.
        template <typename Num1>
        point3(point3<Unit,Num1> const& o):
            x_(o.x().value()), y_(o.y().value()), z_(o.z().value()) { }

        // point3 += vect3 -> point3
        template <typename Num1>
        point3<Unit,Num> operator +=(vect3<Unit,Num1> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            z_ += m2.z().value();
            return *this;
        }

        // point3 -= vect3 -> point3
        template <typename Num1>
        point3<Unit,Num> operator -=(vect3<Unit,Num1> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            z_ -= m2.z().value();
            return *this;
        }

        // Constructs using a unit and a value.
        template <typename Num1, typename Num2, typename Num3>
        point3(typename Unit::magnitude unit, Num1 x, Num2 y, Num3 z):
            x_(static_cast<Num>((unit.offset() - Unit::offset()
                + x * unit.ratio()) / Unit::ratio())),
            y_(static_cast<Num>((unit.offset() - Unit::offset()
                + y * unit.ratio()) / Unit::ratio())),
            z_(static_cast<Num>((unit.offset() - Unit::offset()
                + z * unit.ratio()) / Unit::ratio())) { }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }

        // Get unmutable x component.
        point1<Unit,Num> const x() const { return point1<Unit,Num>(x_); }

        // Get mutable x component.
        point1<Unit,Num>& x()
        { return reinterpret_cast<point1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        point1<Unit,Num> const y() const { return point1<Unit,Num>(y_); }

        // Get mutable y component.
        point1<Unit,Num>& y()
        { return reinterpret_cast<point1<Unit,Num>&>(y_); }

        // Get unmutable z component.
        point1<Unit,Num> const z() const { return point1<Unit,Num>(z_); }

        // Get mutable z component.
        point1<Unit,Num>& z()
        { return reinterpret_cast<point1<Unit,Num>&>(z_); }


        template <typename Num2>
        point3<Unit,Num> mapped_by(affine_map3<Unit,Num2> const& lt) const
        {
            return point3<Unit,Num>(
                lt.c_[0][0] * x_ + lt.c_[0][1] * y_ + lt.c_[0][2] * z_
                    + lt.c_[0][3],
                lt.c_[1][0] * x_ + lt.c_[1][1] * y_ + lt.c_[1][2] * z_
                    + lt.c_[1][3],
                lt.c_[2][0] * x_ + lt.c_[2][1] * y_ + lt.c_[2][2] * z_
                    + lt.c_[2][3]);
        }

    private:

        // Components.
        Num x_, y_, z_;
    };

    // midpoint(point3, point3, weight) -> point3
    template <class Unit, typename Num1, typename Num2, typename Num3>
    point3<Unit,decltype(Num1()+Num2())> midpoint(
        point3<Unit,Num1> p1, point3<Unit,Num2> p2, Num3 weight)
    {
        typedef decltype(Num1()+Num2()) ResultNum;
        return point3<Unit,ResultNum>(
            p1.x().value() * static_cast<ResultNum>(1 - weight)
                + p2.x().value() * static_cast<ResultNum>(weight),
            p1.y().value() * static_cast<ResultNum>(1 - weight)
                + p2.y().value() * static_cast<ResultNum>(weight),
            p1.z().value() * static_cast<ResultNum>(1 - weight)
                + p2.z().value() * static_cast<ResultNum>(weight));
    }

    // midpoint(point3, point3) -> point3
    template <class Unit, typename Num1, typename Num2>
    point3<Unit,decltype(Num1()+Num2())> midpoint(
        point3<Unit,Num1> p1, point3<Unit,Num2> p2)
    {
        typedef decltype(Num1()+Num2()) ResultNum;
        return point3<Unit,ResultNum>(
            (p1.x().value() + p2.x().value()) / static_cast<ResultNum>(2),
            (p1.y().value() + p2.y().value()) / static_cast<ResultNum>(2),
            (p1.z().value() + p2.z().value()) / static_cast<ResultNum>(2));
    }

    // barycentric_combination(int, point3[], Num[]) -> point3
    template <class Unit, typename Num1, typename Num2>
    point3<Unit,decltype(Num1()*Num2())> barycentric_combination(
        int n, point3<Unit,Num1> p[], Num2 weights[])
    {
        typedef decltype(Num1()*Num2()) ResultNum;
        ResultNum result_x = 0, result_y = 0, result_z = 0;
        for (int i = 0; i < n; ++i)
        {
            result_x += p[i].x().value() * weights[i];
            result_y += p[i].y().value() * weights[i];
            result_z += p[i].z().value() * weights[i];
        }
        return point3<Unit,ResultNum>(result_x, result_y, result_z);
    }

    // point3 - point3 -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()-Num2())> operator -(
        point3<Unit,Num1> m1, point3<Unit,Num2> m2)
    {
        return vect3<Unit,decltype(Num1()-Num2())>(
            m1.x().value() - m2.x().value(),
            m1.y().value() - m2.y().value(),
            m1.z().value() - m2.z().value());
    }

    // point3 == point3 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(point3<Unit,Num1> m1, point3<Unit,Num2> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // point3 != point3 -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(point3<Unit,Num1> m1, point3<Unit,Num2> m2)
    {
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value()
            || m1.z().value() != m2.z().value();
    }

    // is_equal(point3, point3, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(point3<Unit,Num1> m1, point3<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        // "abs" is needed for complex numbers.
        return std::abs(squared_norm_value(m1 - m2))
            <= squared_norm_value(tolerance);
    }

    // point3 + vect3 -> point3
    template <class Unit, typename Num1, typename Num2>
    point3<Unit,decltype(Num1()+Num2())> operator +(
        point3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
        return point3<Unit,decltype(Num1()+Num2())>(
            m1.x().value() + m2.x().value(),
            m1.y().value() + m2.y().value(),
            m1.z().value() + m2.z().value());
    }

    // point3 - vect3 -> point3
    template <class Unit, typename Num1, typename Num2>
    point3<Unit,decltype(Num1()-Num2())> operator -(
        point3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
        return point3<Unit,decltype(Num1()-Num2())>(
            m1.x().value() - m2.x().value(),
            m1.y().value() - m2.y().value(),
            m1.z().value() - m2.z().value());
    }


    // vect3 + vect3 -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()+Num2())> operator +(
        vect3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
        return vect3<Unit,decltype(Num1()+Num2())>(
            m1.x().value() + m2.x().value(),
            m1.y().value() + m2.y().value(),
            m1.z().value() + m2.z().value());
    }

    // vect3 - vect3 -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()-Num2())> operator -(
        vect3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
        return vect3<Unit,decltype(Num1()-Num2())>(
            m1.x().value() - m2.x().value(),
            m1.y().value() - m2.y().value(),
            m1.z().value() - m2.z().value());
    }

    // N * vect3 -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()*Num2())> operator *(
        Num1 n, vect3<Unit,Num2> m)
    {
        return vect3<Unit,decltype(Num1()*Num2())>(
            n * m.x().value(),
            n * m.y().value(),
            n * m.z().value());
    }

    // vect3 * N -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()*Num2())> operator *(
        vect3<Unit,Num1> m, Num2 n)
    {
        return vect3<Unit,decltype(Num1()*Num2())>(
            m.x().value() * n,
            m.y().value() * n,
            m.z().value() * n);
    }

    // vect3 / N -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()/Num2())> operator /(
        vect3<Unit,Num1> m, Num2 n)
    {
        return vect3<Unit,decltype(Num1()*Num2())>(
            m.x().value() / n,
            m.y().value() / n,
            m.z().value() / n);
    }

    template <class Unit, typename Num>
    Num squared_norm_value(vect3<Unit,Num> v)
    {
        return sqr(v.x().value()) + sqr(v.y().value()) + sqr(v.z().value());
    }

    template <class Unit, typename Num>
    vect1<Unit,Num> norm(vect3<Unit,Num> v)
    {
        return vect1<Unit,Num>(
            static_cast<Num>(std::sqrt(squared_norm_value(v))));
    }

    // normalized(vect3) -> vect3
    template <class Unit, typename Num>
    vect3<Unit,Num> normalized(vect3<Unit,Num> v)
    {
        return v / norm(v).value();
    }
#endif

    
#ifdef MEASURES_USE_ANGLES
    //////////////////// AZIMUTHS UTILS ////////////////////

    // Private.
    // For integral numbers use %.
    template <typename Num>
    Num normalize_signed_azimuth(Num x, Num one_turn,
        typename std::enable_if<std::is_integral<Num>::value >::type* = 0)
    {
        Num const half_turn = one_turn / 2;
        if (std::abs(x) < half_turn) return x;
        Num x2 = (x + half_turn) % one_turn;
        return x2 >= 0 ? x2 - half_turn : x2 + half_turn;
    }

    // Private.
    // For floating-point numbers use std::fmod.
    template <typename Num>
    Num normalize_signed_azimuth(Num x, Num one_turn,
        typename std::enable_if<std::is_floating_point<Num>
        ::value >::type* = 0)
    {
        Num half_turn = one_turn / 2;
        if (std::abs(x) < half_turn) return x;
        //Num x2 = std::fmod(x + half_turn, one_turn);
        Num x2 = static_cast<Num>(std::fmod(x + half_turn, one_turn));
        return x2 >= 0 ? x2 - half_turn : x2 + half_turn;
    }

    // Private.
    // For integral numbers use %.
    template <typename Num>
    Num normalize_unsigned_azimuth(Num x, Num one_turn,
        typename std::enable_if<std::is_integral<Num>::value >::type* = 0)
    {
        Num x2 = x % one_turn;
        return x2 >= 0 ? x2 : x2 + one_turn;
    }

    // Private.
    // For floating-point numbers use std::fmod.
    template <typename Num>
    Num normalize_unsigned_azimuth(Num x, Num one_turn,
        typename std::enable_if<std::is_floating_point<Num>
        ::value >::type* = 0)
    {
        //Num x2 = std::fmod(x, one_turn);
        Num x2 = static_cast<Num>(std::fmod(x, one_turn));
        return x2 >= 0 ? x2 : x2 + one_turn;
    }


    //////////////////// AZIMUTHS ////////////////////

    // Azimuth are meaningful only if their numeric_type is floating point
    // or their turn fraction is integer.
    // In other words, azimuths with a non-integer turn fraction,
    // like radians, and integer numeric type should be avoided.
    
    template <class Unit = Angle::base_unit, typename Num = double>
    class signed_azimuth
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit signed_azimuth() { }

        // Constructs using one number.
        template <typename Num1>
        explicit signed_azimuth(Num1 x): x_(normalize_(x)) { }

        // Constructs using another signed_azimuth of the same unit.
        template <typename Num1>
        signed_azimuth(signed_azimuth<Unit,Num1> o): x_(o.value()) { }

        // Constructs using a point1 representing an angle.
        template <typename Num1>
        explicit signed_azimuth(point1<Unit,Num1> o):
            x_(normalize_(o.value())) { }

        // Constructs using an unsigned_azimuth.
        template <typename Num1>
        explicit signed_azimuth(unsigned_azimuth<Unit, Num1> o):
            x_(normalize_(o.value())) { }

        // Constructs using a vect2.
        template <class Unit2, typename Num2>
        explicit signed_azimuth(vect2<Unit2,Num2> v):
            x_(convert<Unit>(signed_azimuth<radians,Num>(
                static_cast<Num>(atan2(v.y().value(),
                v.x().value())))).value()) { }

        // Constructs using a unit and a value.
        template <typename Num1>
        explicit signed_azimuth(typename Unit::magnitude unit, Num1 x):
            x_(normalize_(static_cast<Num>(x * (unit.ratio() / Unit::ratio())
                + (unit.offset() - Unit::offset()) / Unit::ratio()))) { }

        // Get unmutable value for the given unit.
        Num value(typename Unit::magnitude unit) const
        {
            return static_cast<Num>(x_ * (Unit::ratio() / unit.ratio())
                + (Unit::offset() - unit.offset()) / unit.ratio());
        }

        // Get unmutable value (mutable getter is not available).
        Num value() const { return x_; }

        // signed_azimuth += vect1 -> signed_azimuth
        template <typename Num1>
        signed_azimuth<Unit,Num> operator +=(vect1<Unit,Num1> m)
        { x_ = normalize_(x_ + m.value()); return *this; }

        // signed_azimuth -= vect1 -> signed_azimuth
        template <typename Num1>
        signed_azimuth<Unit,Num> operator -=(vect1<Unit,Num1> m)
        { x_ = normalize_(x_ - m.value()); return *this; }

    private:

        // Returns the only value that in the current Unit represents `x`, and
        // is between minus half turn included and plus half turn excluded.
        static Num normalize_(Num x)
        {
            return normalize_signed_azimuth(
                x, Unit::template turn_fraction<Num>());
        }

        Num x_;
    };

    // signed_azimuth - signed_azimuth -> vect1
    // As a difference between two point angles it is meant the shortest
    // vect angle that if added to the second point angle gives the first one.
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()-Num2())> operator -(
        signed_azimuth<Unit,Num1> m1, signed_azimuth<Unit,Num2> m2)
    {
        auto difference = m1.value() - m2.value();
        auto one_turn = Unit::template turn_fraction<decltype(Num1()-Num2())>();
        if (difference * 2 < -one_turn) difference += one_turn;
        else if (difference * 2 >= one_turn) difference -= one_turn;
        return vect1<Unit,decltype(Num1()-Num2())>(difference);
    }

    // signed_azimuth == signed_azimuth -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(signed_azimuth<Unit,Num1> m1,
        signed_azimuth<Unit,Num2> m2)
    { return m1.value() == m2.value(); }

    // signed_azimuth != signed_azimuth -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(signed_azimuth<Unit,Num1> m1,
        signed_azimuth<Unit,Num2> m2)
    { return m1.value() != m2.value(); }

    // is_equal(signed_azimuth, signed_azimuth, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(signed_azimuth<Unit,Num1> m1,
        signed_azimuth<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    { return angle_distance(m1, m2) <= tolerance; }

    // angle_distance(signed_azimuth, signed_azimuth) -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()-Num2())> angle_distance(
        signed_azimuth<Unit,Num1> m1, signed_azimuth<Unit,Num2> m2)
    {
        typedef decltype(Num1()-Num2()) NumResult;
        NumResult value_distance(std::abs(m1.value() - m2.value()));
        auto turn_fraction = Unit::template turn_fraction<NumResult>();
        if (value_distance * 2 > turn_fraction)
            value_distance = turn_fraction - value_distance;
        return vect1<Unit,NumResult>(value_distance);
    }

    // signed_azimuth + vect1 -> signed_azimuth
    template <class Unit, typename Num1, typename Num2>
    signed_azimuth<Unit,decltype(Num1()+Num2())> operator +(
        signed_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
        return signed_azimuth<Unit,decltype(Num1()+Num2())>(
            m1.value() + m2.value());
    }

    // signed_azimuth - vect1 -> signed_azimuth
    template <class Unit, typename Num1, typename Num2>
    signed_azimuth<Unit,decltype(Num1()-Num2())> operator -(
        signed_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
        return signed_azimuth<Unit,decltype(Num1()-Num2())>(
            m1.value() - m2.value());
    }


    template <class Unit = Angle::base_unit, typename Num = double>
    class unsigned_azimuth
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Constructs without values.
        explicit unsigned_azimuth() { }

        // Constructs using one number.
        template <typename Num1>
        explicit unsigned_azimuth(Num1 x): x_(normalize_(x)) { }

        // Constructs using another unsigned_azimuth of the same unit.
        template <typename Num1>
        unsigned_azimuth(unsigned_azimuth<Unit,Num1> o): x_(o.value()) { }

        // Constructs using a point1 representing an angle.
        template <typename Num1>
        explicit unsigned_azimuth(point1<Unit,Num1> o):
            x_(normalize_(o.value())) { }

        // Constructs using a signed_azimuth.
        template <typename Num1>
        explicit unsigned_azimuth(signed_azimuth<Unit,Num1> o):
            x_(normalize_(o.value())) { }

        // Constructs using a vect2.
        template <class Unit2, typename Num2>
        explicit unsigned_azimuth(vect2<Unit2,Num2> v):
            x_(convert<Unit>(unsigned_azimuth<radians,Num>(
                static_cast<Num>(atan2(v.y().value(),
                v.x().value())))).value()) { }

        // Constructs using a unit and a value.
        template <typename Num1>
        explicit unsigned_azimuth(typename Unit::magnitude unit, Num1 x):
            x_(normalize_(static_cast<Num>(x * (unit.ratio() / Unit::ratio())
                + (unit.offset() - Unit::offset()) / Unit::ratio()))) { }

        // Get unmutable value for the given unit.
        Num value(typename Unit::magnitude unit) const
        {
            return static_cast<Num>(x_ * (Unit::ratio() / unit.ratio())
                + (Unit::offset() - unit.offset()) / unit.ratio());
        }

        // Get unmutable value (mutable getter is not available).
        Num value() const { return x_; }

        // unsigned_azimuth += vect1 -> unsigned_azimuth
        template <typename Num1>
        unsigned_azimuth<Unit,Num> operator +=(vect1<Unit,Num1> m)
        { x_ = normalize_(x_ + m.value()); return *this; }

        // unsigned_azimuth -= vect1 -> unsigned_azimuth
        template <typename Num1>
        unsigned_azimuth<Unit,Num> operator -=(vect1<Unit,Num1> m)
        { x_ = normalize_(x_ - m.value()); return *this; }

    private:

        // Returns the only value that in the current Unit represents `x`, and
        // is between 0 included and plus one turn excluded.
        static Num normalize_(Num x)
        {
            return normalize_unsigned_azimuth(
                x, Unit::template turn_fraction<Num>());
        }

        Num x_;
    };

    // unsigned_azimuth - unsigned_azimuth -> vect1
    // As a difference between two point angles it is meant the shortest
    // vect angle that if added to the second point angle gives the first one.
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()-Num2())> operator -(
        unsigned_azimuth<Unit,Num1> m1, unsigned_azimuth<Unit,Num2> m2)
    {
        auto difference = m1.value() - m2.value();
        auto one_turn = Unit::template turn_fraction<decltype(Num1()-Num2())>();
        if (difference * 2 < -one_turn) difference += one_turn;
        else if (difference * 2 >= one_turn) difference -= one_turn;
        return vect1<Unit,decltype(Num1()-Num2())>(difference);
    }

    // unsigned_azimuth == unsigned_azimuth -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator ==(unsigned_azimuth<Unit,Num1> m1,
        unsigned_azimuth<Unit,Num2> m2)
    { return m1.value() == m2.value(); }

    // unsigned_azimuth != unsigned_azimuth -> bool
    template <class Unit, typename Num1, typename Num2>
    bool operator !=(unsigned_azimuth<Unit,Num1> m1,
        unsigned_azimuth<Unit,Num2> m2)
    { return m1.value() != m2.value(); }

    // is_equal(unsigned_azimuth, unsigned_azimuth, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(unsigned_azimuth<Unit,Num1> m1,
        unsigned_azimuth<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    { return angle_distance(m1, m2) <= tolerance; }

    // angle_distance(unsigned_azimuth, unsigned_azimuth) -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()-Num2())> angle_distance(
        unsigned_azimuth<Unit,Num1> m1, unsigned_azimuth<Unit,Num2> m2)
    {
        typedef decltype(Num1()-Num2()) NumResult;
        NumResult value_distance(std::abs(m1.value() - m2.value()));
        auto turn_fraction = Unit::template turn_fraction<NumResult>();
        if (value_distance * 2 > turn_fraction)
            value_distance = turn_fraction - value_distance;
        return vect1<Unit,NumResult>(value_distance);
    }

    // unsigned_azimuth + vect1 -> unsigned_azimuth
    template <class Unit, typename Num1, typename Num2>
    unsigned_azimuth<Unit,decltype(Num1()+Num2())> operator +(
        unsigned_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
        return unsigned_azimuth<Unit,decltype(Num1()+Num2())>(
            m1.value() + m2.value());
    }

    // unsigned_azimuth - vect1 -> unsigned_azimuth
    template <class Unit, typename Num1, typename Num2>
    unsigned_azimuth<Unit,decltype(Num1()-Num2())> operator -(
        unsigned_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
        return unsigned_azimuth<Unit,decltype(Num1()-Num2())>(
            m1.value() - m2.value());
    }

    
    //////////////////// TRIGONOMETRIC FUNCTIONS ////////////////////

    // sin(vect1) -> N
    template <class Unit, typename Num>
    Num sin(vect1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return std::sin(convert<radians>(m).value());
    }

    // cos(vect1) -> N
    template <class Unit, typename Num>
    Num cos(vect1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return static_cast<Num>(std::cos(convert<radians>(m).value()));
    }

    // tan(vect1) -> N
    template <class Unit, typename Num>
    Num tan(vect1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return static_cast<Num>(std::tan(convert<radians>(m).value()));
    }

    // sin(point1) -> N
    template <class Unit, typename Num>
    Num sin(point1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return static_cast<Num>(std::sin(convert<radians>(m).value()));
    }

    // cos(point1) -> N
    template <class Unit, typename Num>
    Num cos(point1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return static_cast<Num>(std::cos(convert<radians>(m).value()));
    }

    // tan(point1) -> N
    template <class Unit, typename Num>
    Num tan(point1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return static_cast<Num>(std::tan(convert<radians>(m).value()));
    }

    // sin(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num sin(signed_azimuth<Unit,Num> m)
    { return static_cast<Num>(std::sin(convert<radians>(m).value())); }

    // cos(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num cos(signed_azimuth<Unit,Num> m)
    { return static_cast<Num>(std::cos(convert<radians>(m).value())); }

    // tan(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num tan(signed_azimuth<Unit,Num> m)
    { return static_cast<Num>(std::tan(convert<radians>(m).value())); }
    
    // sin(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num sin(unsigned_azimuth<Unit,Num> m)
    { return static_cast<Num>(std::sin(convert<radians>(m).value())); }

    // cos(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num cos(unsigned_azimuth<Unit,Num> m)
    { return static_cast<Num>(std::cos(convert<radians>(m).value())); }

    // tan(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num tan(unsigned_azimuth<Unit,Num> m)
    { return static_cast<Num>(std::tan(convert<radians>(m).value())); }
#endif
    
    //////////////////// NUMERIC CASTS ////////////////////
    // All these functions return a measure of the same kind, unit,
    // and value, but using the specified ToNum numeric type.

    template <typename ToNum, typename FromNum, class Unit>
    vect1<Unit,ToNum> cast(vect1<Unit,FromNum> m)
    { return vect1<Unit,ToNum>(static_cast<ToNum>(m.value())); }

    template <typename ToNum, typename FromNum, class Unit>
    point1<Unit,ToNum> cast(point1<Unit,FromNum> m)
    { return point1<Unit,ToNum>(static_cast<ToNum>(m.value())); }

#ifdef MEASURES_USE_2D
    template <typename ToNum, typename FromNum, class Unit>
    vect2<Unit,ToNum> cast(vect2<Unit,FromNum> m)
    {
        return vect2<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
            static_cast<ToNum>(m.y().value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    point2<Unit,ToNum> cast(point2<Unit,FromNum> m)
    {
        return point2<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
            static_cast<ToNum>(m.y().value()));
    }
#endif

#ifdef MEASURES_USE_3D
    template <typename ToNum, typename FromNum, class Unit>
    vect3<Unit,ToNum> cast(vect3<Unit,FromNum> m)
    {
        return vect3<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
            static_cast<ToNum>(m.y().value()),
            static_cast<ToNum>(m.z().value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    point3<Unit,ToNum> cast(point3<Unit,FromNum> m)
    {
        return point3<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
            static_cast<ToNum>(m.y().value()),
            static_cast<ToNum>(m.z().value()));
    }
#endif

#ifdef MEASURES_USE_ANGLES
    template <typename ToNum, typename FromNum, class Unit>
    signed_azimuth<Unit,ToNum> cast(signed_azimuth<Unit,FromNum> m)
    {
        return signed_azimuth<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    unsigned_azimuth<Unit,ToNum> cast(unsigned_azimuth<Unit,FromNum> m)
    {
        return unsigned_azimuth<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }
#endif
}


//////////////////// MACROS FOR USER-DEFINED LITERALS ////////////////////
// These macros create user-defined literal operators useful
// to create user-defined literals.
// The first macro creates a `vect1`, the second macro a `point1`.

#define MEASURES_VECT_UNIT_LITERAL(Unit, Num, Operator) \
    namespace measures\
    {\
        /*operator for floating-point literals*/\
        vect1<Unit,Num> operator "" _##Operator(long double n)\
        { return vect1<Unit,Num>(n); }\
        /*operator for integral literals*/\
        vect1<Unit,Num> operator "" _##Operator(unsigned long long n)\
        { return vect1<Unit,Num>(n); }\
    }

#define MEASURES_POINT_UNIT_LITERAL(Unit, Num, Operator) \
    namespace measures\
    {\
        /*operator for floating-point literals*/\
        point1<Unit,Num> operator "" _##Operator(long double n)\
        { return point1<Unit,Num>(n); }\
        /*operator for integral literals*/\
        point1<Unit,Num> operator "" _##Operator(unsigned long long n)\
        { return point1<Unit,Num>(n); }\
    }
#endif

#ifdef MEASURES_USE_IOSTREAM
#include <iostream>
#include <sstream>
#include <limits>

// vect1			1 m
// point1			[1] m
// vect2			1 2 m
// point2			[1 2] m
// vect3			1 2 3 m
// point3			[1 2 3] m
// signed_azimuth	S1^
// unsigned_azimuth	U1^

namespace measures
{
	/////////////////// 1-DIMENSIONAL VECTORS AND POINTS ///////////////////

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		vect1<Unit,Num> const& o)
	{
		os << o.value() << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		vect1<Unit,Num>& o)
	{
		Num v;
		is >> v;
		is.ignore(strlen(Unit::suffix()));
		o = vect1<Unit,Num>(v);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		point1<Unit,Num> const& o)
	{
		os << "[" << o.value() << "]" << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		point1<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(1 + strlen(Unit::suffix()));
		o = point1<Unit,Num>(v);
		return is;
	}


#ifdef MEASURES_USE_2D
	/////////////////// 2-DIMENSIONAL VECTORS AND POINTS ///////////////////

    template <typename Num>
	std::ostream& operator <<(std::ostream& os, linear_map2<Num> const& lm)
	{
        int const n_rows = 2;
        int const n_cols = 2;
        std::ostringstream ostr[n_rows];
        for (int c = 0; c < n_cols; ++c)
        {
            std::stringstream::pos_type max_size = 0;
            for (int r = 0; r < n_rows; ++r)
            {
                if (c == 0) ostr[r] << "| ";
                ostr[r] << lm.coeff(r, c);
                max_size = max(max_size, ostr[r].tellp());
            }
            for (int r = 0; r < n_rows; ++r)
            {
                ostr[r] << std::string(1 + max_size - ostr[r].tellp(), ' ');
            }
        }
        for (int r = 0; r < n_rows; ++r)
        {
            os << ostr[r].str() << "|" << std::endl;
        }
		return os;
	}

	template <typename Num>
	std::istream& operator >>(std::istream& is, linear_map2<Num>& lm)
	{
        int const n_rows = 2;
        int const n_cols = 2;
        for (int r = 0; r < n_rows; ++r)
        {
            is.ignore(2);
            for (int c = 0; c < n_cols; ++c)
            {
                is >> lm.coeff(r, c);
            }
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
		return is;
	}

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
        affine_map2<Unit,Num> const& am)
	{
        int const n_rows = 2;
        int const n_cols = 3;
        std::ostringstream ostr[n_rows];
        for (int c = 0; c < n_cols; ++c)
        {
            std::stringstream::pos_type max_size = 0;
            for (int r = 0; r < n_rows; ++r)
            {
                if (c == 0) ostr[r] << "| ";
                ostr[r] << am.coeff(r, c);
                if (c == n_cols - 1) ostr[r] << Unit::suffix();
                max_size = max(max_size, ostr[r].tellp());
            }
            for (int r = 0; r < n_rows; ++r)
            {
                ostr[r] << std::string(1 + max_size - ostr[r].tellp(), ' ');
            }
        }
        for (int r = 0; r < n_rows; ++r)
        {
            os << ostr[r].str() << "|" << std::endl;
        }
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, affine_map2<Unit,Num>& am)
	{
        int const n_rows = 2;
        int const n_cols = 3;
        for (int r = 0; r < n_rows; ++r)
        {
            is.ignore(2);
            for (int c = 0; c < n_cols; ++c)
            {
                is >> am.coeff(r, c);
            }
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
		return is;
	}

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, vect2<Unit,Num> const& o)
	{
		os << o.x().value() << " " << o.y().value() << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, vect2<Unit,Num>& o)
	{
		Num v1, v2;
		is >> v1 >> v2;
		is.ignore(strlen(Unit::suffix()));
		o = vect2<Unit,Num>(v1, v2);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, point2<Unit,Num> const& o)
	{
		os << "[" << o.x().value() << " " << o.y().value() << "]"
            << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, point2<Unit,Num>& o)
	{
		Num v1, v2;
		is.ignore();
		is >> v1 >> v2;
		is.ignore(1 + strlen(Unit::suffix()));
		o = point2<Unit,Num>(v1, v2);
		return is;
	}
#endif


#ifdef MEASURES_USE_3D
	/////////////////// 3-DIMENSIONAL VECTORS AND POINTS ///////////////////

    template <typename Num>
	std::ostream& operator <<(std::ostream& os, linear_map3<Num> const& lm)
	{
        int const n_rows = 3;
        int const n_cols = 3;
        std::ostringstream ostr[n_rows];
        for (int c = 0; c < n_cols; ++c)
        {
            std::stringstream::pos_type max_size = 0;
            for (int r = 0; r < n_rows; ++r)
            {
                if (c == 0) ostr[r] << "| ";
                ostr[r] << lm.coeff(r, c);
                max_size = max(max_size, ostr[r].tellp());
            }
            for (int r = 0; r < n_rows; ++r)
            {
                ostr[r] << std::string(1 + max_size - ostr[r].tellp(), ' ');
            }
        }
        for (int r = 0; r < n_rows; ++r)
        {
            os << ostr[r].str() << "|" << std::endl;
        }
		return os;
	}

	template <typename Num>
	std::istream& operator >>(std::istream& is, linear_map3<Num>& lm)
	{
        int const n_rows = 3;
        int const n_cols = 3;
        for (int r = 0; r < n_rows; ++r)
        {
            is.ignore(2);
            for (int c = 0; c < n_cols; ++c)
            {
                is >> lm.coeff(r, c);
            }
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
		return is;
	}

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
        affine_map3<Unit,Num> const& am)
	{
        int const n_rows = 3;
        int const n_cols = 4;
        std::ostringstream ostr[n_rows];
        for (int c = 0; c < n_cols; ++c)
        {
            std::stringstream::pos_type max_size = 0;
            for (int r = 0; r < n_rows; ++r)
            {
                if (c == 0) ostr[r] << "| ";
                ostr[r] << am.coeff(r, c);
                if (c == n_cols - 1) ostr[r] << Unit::suffix();
                max_size = max(max_size, ostr[r].tellp());
            }
            for (int r = 0; r < n_rows; ++r)
            {
                ostr[r] << std::string(1 + max_size - ostr[r].tellp(), ' ');
            }
        }
        for (int r = 0; r < n_rows; ++r)
        {
            os << ostr[r].str() << "|" << std::endl;
        }
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, affine_map3<Unit,Num>& am)
	{
        int const n_rows = 3;
        int const n_cols = 4;
        for (int r = 0; r < n_rows; ++r)
        {
            is.ignore(2);
            for (int c = 0; c < n_cols; ++c)
            {
                is >> am.coeff(r, c);
            }
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
		return is;
	}

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, vect3<Unit,Num> const& o)
	{
		os << o.x().value() << " " << o.y().value() << " " << o.z().value()
            << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, vect3<Unit,Num>& o)
	{
		Num v1, v2, v3;
		is >> v1 >> v2 >> v3;
		is.ignore(strlen(Unit::suffix()));
		o = vect3<Unit,Num>(v1, v2, v3);
		return is;
	}

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, point3<Unit,Num> const& o)
	{
		os << "[" << o.x().value() << " " << o.y().value()
            << " " << o.z().value() << "]" << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, point3<Unit,Num>& o)
	{
		Num v1, v2, v3;
		is.ignore();
		is >> v1 >> v2 >> v3;
		is.ignore(1 + strlen(Unit::suffix()));
		o = point3<Unit,Num>(v1, v2, v3);
		return is;
	}
#endif


#ifdef MEASURES_USE_ANGLES
	/////////////////// AZIMUTHS ///////////////////
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		signed_azimuth<Unit,Num> const& o)
	{
		os << "S" << o.value() << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		signed_azimuth<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(strlen(Unit::suffix()));
		o = signed_azimuth<Unit,Num>(v);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		unsigned_azimuth<Unit,Num> const& o)
	{
		os << "U" << o.value() << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		unsigned_azimuth<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(strlen(Unit::suffix()));
		o = unsigned_azimuth<Unit,Num>(v);
		return is;
	}
#endif
}
#endif
