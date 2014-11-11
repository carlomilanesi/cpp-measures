// measures.hpp
// Released under Mozilla Public Licence 2.0
// by Carlo Milanesi (carlo.milanesi@tiscali.it) in october 2014
#ifndef MEASURES_HPP
#define MEASURES_HPP
#include <cmath>
#include <type_traits>


//////////////////// STATIC ASSERTS FOR MAGNITUDE ////////////////////

namespace measures
{
    template <typename T> inline void assert_same_type(T, T) { }
}
#define ASSERT_HAVE_SAME_MAGNITUDE(U1,U2)\
    assert_same_type(typename U1::magnitude(0),\
        typename U2::magnitude(0));
#define ASSERT_IS_ANGLE(U)\
    assert_same_type(typename U::magnitude(0), Angle(0));

//////////////////// UNIT FEATURES ////////////////////

namespace measures
{
    template <typename Num> Num sqr(Num x) { return x * x; }

    struct unit_features
    { double ratio, offset; char const* suffix; };
    struct angle_unit_features
    { double ratio, offset, turn_fraction; char const* suffix; };
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

// The "Angle" magnitude and its "radians" unit are required.
namespace measures
{
    static const double pi = atan(1) * 4;
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

// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_1_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_1_1(U1,U2,U3)\
    MEASURES_DERIVED_1_2(U1,U2,U3)

// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_1_3_ALL(U1,U2,U3)\
    MEASURES_DERIVED_1_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_1_3(U1,U2,U3)

// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U4 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_2_2_ALL(U1,U2,U3,U4)\
    MEASURES_DERIVED_1_1(U1,U2,U3)\
    MEASURES_DERIVED_2_2(U1,U2,U3,U4)

// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U4 (Vector)
// with U1 != U2
#define MEASURES_DERIVED_3_3_ALL(U1,U2,U3,U4)\
    MEASURES_DERIVED_2_2_ALL(U1,U2,U3,U4)\
    MEASURES_DERIVED_3_3(U1,U2,U3,U4)

// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U3 (Vector)
#define MEASURES_DERIVED_SQ_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_SQ_1(U1,U2)\
    MEASURES_DERIVED_SQ_2(U1,U2,U3)

// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U3 (Vector)
#define MEASURES_DERIVED_SQ_3_ALL(U1,U2,U3)\
    MEASURES_DERIVED_SQ_2_ALL(U1,U2,U3)\
    MEASURES_DERIVED_SQ_3(U1,U2,U3)

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

namespace measures
{
//////////////////// PROTOTYPES ////////////////////

    template <class Unit, typename Num> class vect1;
    template <class Unit, typename Num> class point1;
    template <class Unit, typename Num> class vect2;
    template <class Unit, typename Num> class point2;
    template <class Unit, typename Num> class vect3;
    template <class Unit, typename Num> class point3;
    template <class Unit, typename Num> class signed_azimuth;
    template <class Unit, typename Num> class unsigned_azimuth;


//////////////////// UNIT CONVERSIONS ////////////////////

    // 1d measures
    template <class ToUnit, class FromUnit, typename Num>
    vect1<ToUnit,Num> convert(vect1<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return vect1<ToUnit,Num>(m.value()
            * static_cast<Num>(FromUnit::ratio() / ToUnit::ratio()));
    }

    template <class ToUnit, class FromUnit, typename Num>
    point1<ToUnit,Num> convert(point1<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return point1<ToUnit,Num>(m.value()
            * static_cast<Num>(FromUnit::ratio() / ToUnit::ratio())
            + static_cast<Num>((FromUnit::offset() - ToUnit::offset())
            / ToUnit::ratio()));
    }

    // 2d measures
    template <class ToUnit, class FromUnit, typename Num>
    vect2<ToUnit,Num> convert(vect2<FromUnit,Num> m)
    {
        return vect2<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()));
    }

    template <class ToUnit, class FromUnit, typename Num>
    point2<ToUnit,Num> convert(point2<FromUnit,Num> m)
    {
        return point2<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()));
    }

    // 3d measures
    template <class ToUnit, class FromUnit, typename Num>
    vect3<ToUnit,Num> convert(vect3<FromUnit,Num> m)
    {
        return vect3<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()),
            convert<ToUnit,FromUnit,Num>(m.z()));
    }

    template <class ToUnit, class FromUnit, typename Num>
    point3<ToUnit,Num> convert(point3<FromUnit,Num> m)
    {
        return point3<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()),
            convert<ToUnit,FromUnit,Num>(m.z()));
    }

    // Azimuths
    template <class ToUnit, class FromUnit, typename Num>
    signed_azimuth<ToUnit,Num> convert(signed_azimuth<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return signed_azimuth<ToUnit,Num>(
            convert<ToUnit>(point1<FromUnit,Num>(m.value())));
    }

    template <class ToUnit, class FromUnit, typename Num>
    unsigned_azimuth<ToUnit,Num> convert(unsigned_azimuth<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return unsigned_azimuth<ToUnit,Num>(
            convert<ToUnit>(point1<FromUnit,Num>(m.value())));
    }

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
        vect1(const vect1<Unit,Num1>& o): x_(o.value()) { }
        
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
        return static_cast<Num3>(std::abs(m1.value() - m2.value()))
            <= tolerance.value();
    }

    // is_less(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2,\
        vect1<Unit,Num3> tolerance)
    {
        return static_cast<Num3>(m1.value())
            < static_cast<Num3>(m2.value()) - tolerance.value();
    }

    // is_less_or_equal(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
        return static_cast<Num3>(m1.value())
            <= static_cast<Num3>(m2.value()) + tolerance.value();
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
        point1(const point1<Unit,Num1>& o): x_(o.value()) { }

        // Constructs using a unit and a value.
        template <typename Num1>
        point1(typename Unit::magnitude unit, Num1 x):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio())
                + (unit.offset() - Unit::offset()) / Unit::ratio())) { }

        // Constructs using a signed azimuth.
        template <typename Num1>
        explicit point1(signed_azimuth<Unit,Num1> a): x_(a.value()) { }

        // Constructs using an unsigned azimuth.
        template <typename Num1>
        explicit point1(unsigned_azimuth<Unit,Num1> a): x_(a.value()) { }

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
    point1<Unit,decltype((Num1()+Num2())*Num3())> midpoint(
        point1<Unit,Num1> p1, point1<Unit,Num2> p2, Num3 weight)
    {
        return point1<Unit,decltype((Num1()+Num2())*Num3())>(
            p1.value() * (1 - weight) + p2.value() * weight);
    }

    // midpoint(point1, point1) -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype((Num1()+Num2())*float())> midpoint(
        point1<Unit,Num1> p1, point1<Unit,Num2> p2)
    {
        return point1<Unit,decltype((Num1()+Num2())*float())>(
            (p1.value() + p2.value()) * 0.5f);
    }

    // barycentric_combination(int, point1[], Num[]) -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype(Num1()*Num2())> barycentric_combination(
        int n, point1<Unit,Num1> p[], Num2 weights[])
    {
        typedef decltype(Num1()*Num2()) ResultNum;
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
        return static_cast<Num3>(std::abs(m1.value() - m2.value()))
            <= tolerance.value();
    }

    // is_less(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less(point1<Unit,Num1> m1, point1<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        return static_cast<Num3>(m1.value())
            < static_cast<Num3>(m2.value()) - tolerance.value();
    }

    // is_less_or_equal(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(point1<Unit,Num1> m1, point1<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        return static_cast<Num3>(m1.value())
            <= static_cast<Num3>(m2.value()) + tolerance.value();
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


    //////////////////// 2-DIMENSIONAL VECTORS AND POINTS ////////////////////

    template <typename Num = double>
    class linear_transformation2
    {
        template <class, typename>
        friend class vect2;    
    public:
    
        // Rotation by a relative angle.
        template <class Unit, typename Num2>
        static linear_transformation2 rotation(vect1<Unit,Num2> a)
        {
            linear_transformation2 result;
            result.set_rotation_(convert<radians>(a).value());
            return result;
        }
        
        // Projections
        
        // Projection onto a line identified by a point angle.
        template <class Unit, typename Num2>
        static linear_transformation2 projection(point1<Unit> a)
        {
            linear_transformation2 result;
            result.set_projection_(convert<radians>(a).value());
            return result;
        }
        
        // Projection onto a line identified by a signed azimuth.
        template <class Unit, typename Num2>
        static linear_transformation2 projection(signed_azimuth<Unit,Num2> a)
        {
            linear_transformation2 result;
            result.set_projection_(convert<radians>(a).value());
            return result;
        }
        
        // Projection onto a line identified by an unsigned azimuth.
        template <class Unit, typename Num2>
        static linear_transformation2 projection(unsigned_azimuth<Unit,Num2> a)
        {
            linear_transformation2 result;
            result.set_projection_(convert<radians>(a).value());
            return result;
        }

        // Projection onto a line identified by any plane vector.
        template <class Unit, typename Num2>
        static linear_transformation2 projection(vect2<Unit,Num2> v)
        {
            // Precondition: norm(v).value() != 0
            return projection_onto_unit_vector(normalized(v));
        }

        // Projection onto a line identified by a unit plane vector.
        // Construction is more efficient.
        template <class Unit, typename Num2>
        static linear_transformation2 projection_onto_unit_vector(vect2<Unit,Num2> v)
        {
            // Precondition: norm(v).value() == 1
            linear_transformation2 result;
            result.set_projection_(v.x().value(), v.y().value());
            return result;
        }
        
        // Reflections
        
        // Reflection over a line identified by a point angle.
        template <class Unit, typename Num2>
        static linear_transformation2 reflection(point1<Unit> a)
        {
            linear_transformation2 result;
            result.set_reflection_(convert<radians>(a).value());
            return result;
        }
        
        // Reflection over a line identified by a signed azimuth.
        template <class Unit, typename Num2>
        static linear_transformation2 reflection(signed_azimuth<Unit,Num2> a)
        {
            linear_transformation2 result;
            result.set_reflection_(convert<radians>(a).value());
            return result;
        }
        
        // Reflection over a line identified by an unsigned azimuth.
        template <class Unit, typename Num2>
        static linear_transformation2 reflection(unsigned_azimuth<Unit,Num2> a)
        {
            linear_transformation2 result;
            result.set_reflection_(convert<radians>(a).value());
            return result;
        }

        // Reflection over a line identified by any plane vector.
        template <class Unit, typename Num2>
        static linear_transformation2 reflection(vect2<Unit,Num2> v)
        {
            // Precondition: norm(v).value() != 0
            return reflection_over_unit_vector(normalized(v));
        }

        // Reflection over a line identified by a unit plane vector.
        // Construction is more efficient.
        template <class Unit, typename Num2>
        static linear_transformation2 reflection_over_unit_vector(vect2<Unit,Num2> v)
        {
            // Precondition: norm(v) == 1
            linear_transformation2 result;
            result.set_reflection_(v.x().value(), v.y().value());
            return result;
        }

        // Access
        
        Num* matrix() { return &c_[0][0]; }
    
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
            set_projection_(cos(a), sin(a));
        }

        void set_reflection_(Num cos_a, Num sin_a)
        {
            c_[0][0] = 2 * cos_a * cos_a - 1; c_[0][1] = 2 * cos_a * sin_a;
            c_[1][0] = 2 * cos_a * sin_a; c_[1][1] = 2 * sin_a * sin_a - 1;
        }
        
        void set_reflection_(Num a)
        {
            set_reflection_(cos(a), sin(a));
        }
        
        Num c_[2][2];
    };

    //v.trasformed_by(lt1).trasformed_by(lt2) == v.trasformed_by(combine(lt1, lt2))
    /*TODO
    linear_transformation2 combine(
        const linear_transformation2& lt1,
        const linear_transformation2& lt2)
    {
        linear_transformation2 result;
        result.c_[0][0] = lt1[0][0] * lt1[0][0] + lt1[0][1] * lt1[1][0];
        result.c_[0][1] = lt1[0][0] * lt1[0][1] + lt1[0][1] * lt1[1][1];
        result.c_[1][0] = lt1[1][0] * lt1[0][0] + lt1[1][1] * lt1[1][0];
        result.c_[1][1] = lt1[1][0] * lt1[0][1] + lt1[1][1] * lt1[1][1];
        *this = result;
    }
    
    linear_transformation2 inverted()
    {
        linear_transformation2 result;
        result.c_[0][0] = lt1[0][0] * lt1[0][0] + lt1[0][1] * lt1[1][0];
        result.c_[0][1] = lt1[0][0] * lt1[0][1] + lt1[0][1] * lt1[1][1];
        result.c_[1][0] = lt1[1][0] * lt1[0][0] + lt1[1][1] * lt1[1][0];
        result.c_[1][1] = lt1[1][0] * lt1[0][1] + lt1[1][1] * lt1[1][1];
        return result;
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
        vect2(const vect2<Unit,Num1>& o):
            x_(o.x().value()), y_(o.y().value()) { }
        
        // Constructs using a unit and two values.
        template <typename Num2, typename Num3>
        vect2(typename Unit::magnitude unit, Num2 x, Num3 y):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio()))),
            y_(static_cast<Num>(y * (unit.ratio() / Unit::ratio()))) { }

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

        vect2<Unit,Num> rotated_left() const
        { return vect2<Unit,Num>(- y_, x_); }

        vect2<Unit,Num> rotated_right() const
        { return vect2<Unit,Num>(y_, - x_); }

        template <typename Num2>
        vect2<Unit,Num> transformed_by(const linear_transformation2<Num2>& lt) const
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
        return squared_norm_value(m1 - m2)
            <= squared_norm_value(tolerance);
    }

    template <typename Num = double>
    class affine_transformation2
    {
        template <class, typename>
        friend class point2;    
    public:

        // Translation.
        template <class VectUnit, typename VectNum>
        static affine_transformation2 translation(vect2<VectUnit,VectNum> v)
        {
            affine_transformation2 result;
            result.c_[0][0] = 1; result.c_[0][1] = 0;
            result.c_[0][2] = v.x().value();
            result.c_[1][0] = 0; result.c_[1][1] = 1;
            result.c_[1][2] = v.y().value();
            return result;
        }
        
        // Rotation around a point by a relative angle.
        template <class PointUnit, typename PointNum,
            class AngleUnit, typename AngleNum>
        static affine_transformation2 rotation(
            point2<PointUnit,PointNum> fixed_p,
            vect1<AngleUnit,AngleNum> angle)
        {
            affine_transformation2 result;
            result.set_rotation_(
                fixed_p.x().value(), fixed_p.y().value(),
                convert<radians>(angle).value());
            return result;
        }
        
        // Projections
        
        // Projection onto a line identified by a fixed point
        // and a point angle.
        template <class PointUnit, typename PointNum,
            class AngleUnit, typename AngleNum>
        static affine_transformation2 projection(
            point2<PointUnit,PointNum> fixed_p,
            point1<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_transformation2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Projection onto a line identified by a fixed point
        // and a signed azimuth.
        template <class PointUnit, typename PointNum,
            class AngleUnit, typename AngleNum>
        static affine_transformation2 projection(
            point2<PointUnit,PointNum> fixed_p,
            signed_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_transformation2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Projection onto a line identified by a fixed point
        // and an unsigned azimuth.
        template <class PointUnit, typename PointNum,
            class AngleUnit, typename AngleNum>
        static affine_transformation2 projection(
            point2<PointUnit,PointNum> fixed_p,
            unsigned_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_transformation2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }

        // Projection onto a line identified by a fixed point
        // and any plane vector.
        template <class PointUnit, typename PointNum,
            class VectUnit, typename VectNum>
        static affine_transformation2 projection(
            point2<PointUnit,PointNum> fixed_p,
            vect2<VectUnit,VectNum> v)
        {
            // Precondition: norm(v).value() != 0
            return projection_onto_unit_vector(fixed_p, normalized(v));
        }

        // Projection onto a line identified by a fixed point
        // and a unit plane vector.
        // Construction is more efficient.
        template <class PointUnit, typename PointNum,
            class VectUnit, typename VectNum>
        static affine_transformation2 projection_onto_unit_vector(
            point2<PointUnit,PointNum> fixed_p,
            vect2<VectUnit,VectNum> uv)
        {
            // Precondition: norm(v).value() == 1
            affine_transformation2 result;
            result.set_projection_(
                fixed_p.x().value(), fixed_p.y().value(),
                uv.x().value(), uv.y().value());
            return result;
        }
        
        // Reflections
        
        // Reflection over a line identified by a fixed point
        // and a point angle.
        template <class PointUnit, typename PointNum,
            class AngleUnit, typename AngleNum>
        static affine_transformation2 reflection(
            point2<PointUnit,PointNum> fixed_p,
            point1<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_transformation2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Reflection over a line identified by a fixed point
        // and a signed azimuth.
        template <class PointUnit, typename PointNum,
            class AngleUnit, typename AngleNum>
        static affine_transformation2 reflection(
            point2<PointUnit,PointNum> fixed_p,
            signed_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_transformation2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }
        
        // Reflection over a line identified by a fixed point
        // and an unsigned azimuth.
        template <class PointUnit, typename PointNum,
            class AngleUnit, typename AngleNum>
        static affine_transformation2 reflection(
            point2<PointUnit,PointNum> fixed_p,
            unsigned_azimuth<AngleUnit,AngleNum> angle)
        {
            auto a = convert<radians>(angle).value();
            affine_transformation2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                std::cos(a), std::sin(a));
            return result;
        }

        // Reflection over a line identified by a fixed point
        // and any plane vector.
        template <class PointUnit, typename PointNum,
            class VectUnit, typename VectNum>
        static affine_transformation2 reflection(
            point2<PointUnit,PointNum> fixed_p,
            vect2<VectUnit,VectNum> v)
        {
            // Precondition: norm(v).value() != 0
            return reflection_over_unit_vector(fixed_p, normalized(v));
        }

        // Reflection over a line identified by a fixed point
        // and a unit plane vector.
        // Construction is more efficient.
        template <class PointUnit, typename PointNum,
            class VectUnit, typename VectNum>
        static affine_transformation2 reflection_over_unit_vector(
            point2<PointUnit,PointNum> fixed_p,
            vect2<VectUnit,VectNum> uv)
        {
            // Precondition: norm(v).value() == 1
            affine_transformation2 result;
            result.set_reflection_(
                fixed_p.x().value(), fixed_p.y().value(),
                uv.x().value(), uv.y().value());
            return result;
        }

        // Access
        
        Num* matrix() { return &c_[0][0]; }
    private:
    
        void set_rotation_(Num fp_x, Num fp_y, Num angle)
        {
            auto cos_a = std::cos(angle);
            auto sin_a = std::sin(angle);
            c_[0][0] = cos_a; c_[0][1] = -sin_a;
            c_[0][2] = fp_x - cos_a * fp_x + sin_a * fp_y;
            c_[1][0] = sin_a; c_[1][1] = cos_a;
            c_[1][2] = fp_y - sin_a * fp_x - cos_a * fp_y;
        }

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
        point2(const point2<Unit,Num1>& o):
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

        template <class Unit2, typename Num2, typename Num3>
        point2<Unit,decltype(Num3()+(Num()-Num3())*Num2())> rotated_by_around(
            vect1<Unit2,Num2> a, point2<Unit,Num3> fixed_p)
        {
            ASSERT_IS_ANGLE(Unit2)
            return fixed_p + (*this - fixed_p).transformed_by(
                linear_transformation2<Num3>::rotation(a));
        }

        template <typename Num2>
        point2<Unit,decltype(Num2()+(Num()-Num2()))>
            rotated_left_around(point2<Unit,Num2> fixed_p)
        {
            return fixed_p + (*this - fixed_p).rotated_left();
        }

        template <typename Num2>
        point2<Unit,decltype(Num2()+(Num()-Num2()))>
            rotated_right_around(point2<Unit,Num2> fixed_p)
        {
            return fixed_p + (*this - fixed_p).rotated_right();
        }

        template <typename Num2>
        point2<Unit,Num> transformed_by(const affine_transformation2<Num2>& at) const
        {
            return point2<Unit,Num>(
                at.c_[0][0] * x_ + at.c_[0][1] * y_ + at.c_[0][2],
                at.c_[1][0] * x_ + at.c_[1][1] * y_ + at.c_[1][2]);
        }
        
    private:

        // Components.
        Num x_, y_;
    };

    // midpoint(point2, point2, weight) -> point2
    template <class Unit, typename Num1, typename Num2, typename Num3>
    point2<Unit,decltype((Num1()+Num2())*Num3())> midpoint(
        point2<Unit,Num1> p1, point2<Unit,Num2> p2, Num3 weight)
    {
        return point2<Unit,decltype((Num1()+Num2())*Num3())>(
            p1.x().value() * (1 - weight) + p2.x().value() * weight,
            p1.y().value() * (1 - weight) + p2.y().value() * weight);
    }

    // midpoint(point2, point2) -> point2
    template <class Unit, typename Num1, typename Num2>
    point2<Unit,decltype((Num1()+Num2())*float())> midpoint(
        point2<Unit,Num1> p1, point2<Unit,Num2> p2)
    {
        return point2<Unit,decltype((Num1()+Num2())*float())>(
            (p1.x().value() + p2.x().value()) * 0.5f,
            (p1.y().value() + p2.y().value()) * 0.5f);
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
        return squared_norm_value(m1 - m2)
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


    //////////////////// 3-DIMENSIONAL POINTS AND VECTORS ////////////////////

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
        vect3(const vect3<Unit,Num1>& o):
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
        return squared_norm_value(m1 - m2)
            <= squared_norm_value(tolerance);
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
        point3(const point3<Unit,Num1>& o):
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

    private:

        // Components.
        Num x_, y_, z_;
    };

    // midpoint(point3, point3, weight) -> point3
    template <class Unit, typename Num1, typename Num2, typename Num3>
    point3<Unit,decltype((Num1()+Num2())*Num3())> midpoint(
        point3<Unit,Num1> p1, point3<Unit,Num2> p2, Num3 weight)
    {
        return point3<Unit,decltype((Num1()+Num2())*Num3())>(
            p1.x().value() * (1 - weight) + p2.x().value() * weight,
            p1.y().value() * (1 - weight) + p2.y().value() * weight,
            p1.z().value() * (1 - weight) + p2.z().value() * weight);
    }

    // midpoint(point3, point3) -> point3
    template <class Unit, typename Num1, typename Num2>
    point3<Unit,decltype((Num1()+Num2())*float())> midpoint(
        point3<Unit,Num1> p1, point3<Unit,Num2> p2)
    {
        return point3<Unit,decltype((Num1()+Num2())*float())>(
            (p1.x().value() + p2.x().value()) * 0.5f,
            (p1.y().value() + p2.y().value()) * 0.5f,
            (p1.z().value() + p2.z().value()) * 0.5f);
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
        return squared_norm_value(m1 - m2)
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

    
    //////////////////// AZIMUTHS UTILS ////////////////////

    // Private.
    // For integral numbers use %.
    template <typename Num>
    Num normalize_signed_azimuth(Num x, Num one_turn,
        typename std::enable_if<std::is_integral<Num>::value >::type* = 0)
    {
        const Num half_turn = one_turn / 2;
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
        Num half_turn = one_turn * 0.5f;
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

    
    //////////////////// NUMERIC CASTS ////////////////////
    // All these functions return a measure of the same kind, unit,
    // and value, but using the specified ToNum numeric type.

    template <typename ToNum, typename FromNum, class Unit>
    vect1<Unit,ToNum> cast(vect1<Unit,FromNum> m)
    { return vect1<Unit,ToNum>(static_cast<ToNum>(m.value())); }

    template <typename ToNum, typename FromNum, class Unit>
    point1<Unit,ToNum> cast(point1<Unit,FromNum> m)
    { return point1<Unit,ToNum>(static_cast<ToNum>(m.value())); }

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
