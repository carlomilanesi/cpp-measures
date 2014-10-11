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

// Every unit class is never instanced.
// It is used only as a template parameter
// or as a collection of static member functions.
#define DEFINE_UNIT(UnitClass,MagnitudeClass,Suffix,Ratio,Offset)\
    namespace measures\
    {\
        static unit_features UnitClass##_features_\
            = { Ratio, Offset, Suffix };\
        class UnitClass\
        {\
        public:\
            typedef MagnitudeClass magnitude;\
            static MagnitudeClass id()\
                { return MagnitudeClass(&UnitClass##_features_); }\
            static char const* suffix() { return Suffix; }\
            static double ratio() { return Ratio; }\
            static double offset() { return Offset; }\
        };\
    }

#define DEFINE_ANGLE_UNIT(UnitClass,Suffix,TurnFraction,Offset)\
    namespace measures\
    {\
        static angle_unit_features UnitClass##_features_\
            = { 2 * pi / (TurnFraction), Offset, TurnFraction, Suffix };\
        class UnitClass\
        {\
        public:\
            typedef Angle magnitude;\
            static Angle id() { return Angle(&UnitClass##_features_); }\
            static char const* suffix() { return Suffix; }\
            static double ratio() { return 2 * pi / (TurnFraction); }\
            static double offset() { return Offset; }\
            static double turn_fraction() { return TurnFraction; }\
        };\
    }

// Every magnitude class is used as a template parameter
// or it may be instanced to represent a dynamic unit.
#define DEFINE_MAGNITUDE(MagnitudeClass,MainUnitClass,MainUnitSuffix)\
    namespace measures\
    {\
        class MainUnitClass;\
        class MagnitudeClass\
        {\
        public:\
            typedef MainUnitClass base_unit;\
            explicit MagnitudeClass(unit_features const* features):\
                features_(features) { }\
            char const* suffix() const { return features_->suffix; }\
            double ratio() const { return features_->ratio; }\
            double offset() const { return features_->offset; }\
        private:\
            unit_features const* features_;\
        };\
    }\
    DEFINE_UNIT(MainUnitClass, MagnitudeClass, MainUnitSuffix, 1, 0)


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
        double turn_fraction() { return features_->turn_fraction; }\
    private:
        angle_unit_features const* features_;
    };
}
DEFINE_ANGLE_UNIT(radians, " rad", 2 * pi, 0)


//////////////////// DERIVED UNITS ////////////////////

/*
The following different-unit operations are supported:
Scalar * Scalar -> Scalar // simple product
Scalar / Scalar -> Scalar // simple division
sqroot(Scalar) -> Scalar // simple square root
Scalar * Vector -> Vector // vector external product
Vector * Scalar -> Vector // vector external product, commutated
Vector / Scalar -> Vector // vector external product by the reciprocal
Vector * Vector -> Scalar // vector dot product
cross_product(Vector, Vector) -> Vector // vector cross product
*/

// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_UNIT_SCALAR_VECTOR(U1,U2,U3)\
    DEFINE_DERIVED_UNIT_SCALAR_SCALAR(U1,U2,U3)\
    DEFINE_DERIVED_OTHER_UNIT_SCALAR_VECTOR(U1,U2,U3)

// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_UNIT_VECTOR_VECTOR(U1,U2,U3)\
    DEFINE_DERIVED_UNIT_SCALAR_SCALAR(U1,U2,U3)\
    DEFINE_DERIVED_OTHER_UNIT_VECTOR_VECTOR(U1,U2,U3)

// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U2 (Vector)
#define DEFINE_DERIVED_UNIT_SQUARED_VECTOR(U1,U2)\
    DEFINE_DERIVED_UNIT_SQUARED_SCALAR(U1,U2)\
    DEFINE_DERIVED_OTHER_UNIT_SQUARED_VECTOR(U1,U2)

// U1 (Scalar) * U2 (Scalar) == U3 (Scalar)
// with U1 != U2
#define DEFINE_DERIVED_UNIT_SCALAR_SCALAR(U1,U2,U3)\
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
#define DEFINE_DERIVED_UNIT_SQUARED_SCALAR(U1,U2)\
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
        /* vect1 / vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U1,decltype(Num1()/Num2())> operator /(\
            vect1<U2,Num1> m2, vect1<U1,Num2> m1)\
        {\
            return vect1<U1,decltype(Num1()/Num2())>(\
                m2.value() / m1.value());\
        }\
        \
        /* sqroot(vect1) -> vect1 */\
        template <typename Num>\
        vect1<U1,Num> sqroot(vect1<U2,Num> m2)\
        {\
            return vect1<U1,Num>(\
                static_cast<Num>(sqrt(m2.value())));\
        }\
    }

// Private
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_OTHER_UNIT_SCALAR_VECTOR(U1,U2,U3)\
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
        \
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

// Private
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_OTHER_UNIT_VECTOR_VECTOR(U1,U2,U3)\
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
        vect1<U3,decltype(Num1()*Num2())> cross_product(\
            vect2<U1,Num1> m1, vect2<U2,Num2> m2)\
        {\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
        \
        /* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num2, typename Num1>\
        vect1<U3,decltype(Num2()*Num1())> cross_product(\
            vect2<U2,Num1> m2, vect2<U1,Num2> m1)\
        {\
            return vect1<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.y().value() -\
                m2.y().value() * m1.x().value());\
        }\
        \
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
        vect3<U3,decltype(Num1()*Num2())> cross_product(\
            vect3<U1,Num1> m1, vect3<U2,Num2> m2)\
        {\
            return vect3<U3,decltype(Num1()*Num2())>(\
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
        vect3<U3,decltype(Num2()*Num1())> cross_product(\
            vect3<U2,Num2> m2, vect3<U1,Num1> m1)\
        {\
            return vect3<U3,decltype(Num2()*Num1())>(\
                m2.y().value() * m1.z().value() -\
                m2.z().value() * m1.y().value(),\
                m2.z().value() * m1.x().value() -\
                m2.x().value() * m1.z().value(),\
                m2.x().value() * m1.y().value() -\
                m2.y().value() * m1.x().value());\
        }\
    }

// Private
// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U2 (Vector)
#define DEFINE_DERIVED_OTHER_UNIT_SQUARED_VECTOR(U1,U2)\
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
        /* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> cross_product(\
            vect2<U1,Num1> m1, vect2<U1,Num2> m2)\
        {\
            return vect1<U2,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
        \
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
        /* cross_product(vect3, vect3) -> vect3 */\
        template <typename Num1, typename Num2>\
        vect3<U2,decltype(Num1()*Num2())> cross_product(\
            vect3<U1,Num1> m1, vect3<U1,Num2> m2)\
        {\
            return vect3<U2,decltype(Num1()*Num2())>(\
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


    //////////////////// 1-DIMENSIONAL VECTORS AND POINTS ////////////////////

    template <class Unit, typename Num = double>
    class vect1
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Construct without values.
        explicit vect1() { }

        // Construct using one number of the same number type.
        explicit vect1(Num x): x_(x) { }

        // Construct using another vect1 of the same unit
        // and the same number type.
        vect1(const vect1<Unit,Num>& o):
            x_(o.value()) { }

        // Construct using a unit and a value.
        vect1(typename Unit::magnitude unit, Num x):
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
        vect1<Unit,Num> operator +=(vect1<Unit,Num> m2)
            { x_ += m2.value(); return *this; }

        // vect1 -= vect1 -> vect1
        vect1<Unit,Num> operator -=(vect1<Unit,Num> m2)
            { x_ -= m2.value(); return *this; }

        // vect1 *= N -> vect1
        template <typename Num2>
        vect1<Unit,Num> operator *=(Num2 n)
            { x_ *= static_cast<Num>(n); return *this; }

        // vect1 /= N -> vect1
        template <typename Num2>
        vect1<Unit,Num> operator /=(Num2 n)
            { x_ /= static_cast<Num>(n); return *this; }

    private:
        Num x_;
    };

    // vect1 == vect1 -> bool
    template <class Unit, typename Num>
    bool operator ==(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
        { return m1.value() == m2.value(); }

    // vect1 != vect1 -> bool
    template <class Unit, typename Num>
    bool operator !=(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
        { return m1.value() != m2.value(); }

    // vect1 < vect1 -> bool
    template <class Unit, typename Num>
    bool operator <(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
        { return m1.value() < m2.value(); }

    // vect1 <= vect1 -> bool
    template <class Unit, typename Num>
    bool operator <=(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
        { return m1.value() <= m2.value(); }

    // vect1 > vect1 -> bool
    template <class Unit, typename Num>
    bool operator >(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
        { return m1.value() > m2.value(); }

    // vect1 >= vect1 -> bool
    template <class Unit, typename Num>
    bool operator >=(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
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

        // Construct without values.
        explicit point1() { }

        // Construct using one number of the same number type.
        explicit point1(Num x): x_(x) { }

        // Construct using another point1 of the same unit
        // and the same number type.
        point1(const point1<Unit,Num>& o):
            x_(o.value()) { }

        // Construct using a unit and a value.
        point1(typename Unit::magnitude unit, Num x):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio())
                + (unit.offset() - Unit::offset()) / Unit::ratio())) { }

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
        point1<Unit,Num1> p1, point1<Unit,Num2> p2, Num3 weight = 0.5f)
    {
        //return p1 + (p2 - p1) * weight;
        return point1<Unit,decltype((Num1()+Num2())*Num3())>(
            p1.value() * (1 - weight) + p2.value() * weight);
    }

    // barycentric_combination(int, point1[], Num[]) -> point1
    template <class Unit, typename Num>
    point1<Unit,Num> barycentric_combination(
        int n, point1<Unit,Num> p[], Num weights[])
    {
        Num result = 0;
        for (int i = 0; i < n; ++i)
            { result += p[i].value() * weights[i]; }
        return point1<Unit,Num>(result);
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
    template <class Unit, typename Num>
    bool operator ==(point1<Unit,Num> m1, point1<Unit,Num> m2)
        { return m1.value() == m2.value(); }

    // point1 != point1 -> bool
    template <class Unit, typename Num>
    bool operator !=(point1<Unit,Num> m1, point1<Unit,Num> m2)
        { return m1.value() != m2.value(); }

    // point1 < point1 -> bool
    template <class Unit, typename Num>
    bool operator <(point1<Unit,Num> m1, point1<Unit,Num> m2)
        { return m1.value() < m2.value(); }

    // point1 <= point1 -> bool
    template <class Unit, typename Num>
    bool operator <=(point1<Unit,Num> m1, point1<Unit,Num> m2)
        { return m1.value() <= m2.value(); }

    // point1 > point1 -> bool
    template <class Unit, typename Num>
    bool operator >(point1<Unit,Num> m1, point1<Unit,Num> m2)
        { return m1.value() > m2.value(); }

    // point1 >= point1 -> bool
    template <class Unit, typename Num>
    bool operator >=(point1<Unit,Num> m1, point1<Unit,Num> m2)
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
		{ return vect1<Unit,Num>(std::abs(v.value())); }


    //////////////////// 2-DIMENSIONAL VECTORS AND POINTS ////////////////////

    template <class Unit, typename Num> class signed_azimuth;

    template <class Unit, typename Num> class unsigned_azimuth;

    template <class Unit, typename Num = double>
    class vect2
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Construct without values.
        explicit vect2() { }

        // Construct using two numbers of the same number type.
        explicit vect2(Num x, Num y): x_(x), y_(y) { }

        // Construct using an array of two numbers of the same number type.
        explicit vect2(Num const values[]): x_(values[0]), y_(values[1]) { }

        // Construct using two vect1s of the same number type.
        explicit vect2(vect1<Unit,Num> x, vect1<Unit,Num> y):
            x_(x.value()), y_(y.value()) { }

        // Construct using another vect2 of the same unit
        // and the same number type.
        vect2(const vect2<Unit,Num>& o):
            x_(o.x_), y_(o.y_) { }

        // Construct a versor using a point1 representing an angle.
        template <class Unit2, typename Num2>
        explicit vect2(const point1<Unit2,Num2>& a):
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a)))
			{ ASSERT_IS_ANGLE(Unit2) }

        // Construct a versor using a signed_azimuth.
        template <class Unit2, typename Num2>
        explicit vect2(const signed_azimuth<Unit2,Num2>& a):
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a))) { }

        // Construct a versor using an unsigned_azimuth.
        template <class Unit2, typename Num2>
        explicit vect2(const unsigned_azimuth<Unit2,Num2>& a):
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a))) { }

        // +vect2 -> vect2
        vect2<Unit,Num> operator +() const { return *this; }

        // -vect2 -> vect2
        vect2<Unit,Num> operator -() const
			{ return vect2<Unit,Num>(-x_, -y_); }

        // vect2 += vect2 -> vect2
        vect2<Unit,Num> operator +=(vect2<Unit,Num> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            return *this;
        }

        // vect2 -= vect2 -> vect2
        vect2<Unit,Num> operator -=(vect2<Unit,Num> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            return *this;
        }

        // vect2 *= N -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator *=(Num2 n)
        {
            x_ *= static_cast<Num>(n);
            y_ *= static_cast<Num>(n);
            return *this;
        }

        // vect2 /= N -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator /=(Num2 n)
        {
            x_ /= static_cast<Num>(n);
            y_ /= static_cast<Num>(n);
            return *this;
        }

        template <class Unit2, typename Num2>
        vect2<Unit,decltype(Num()*Num2())> rotated_by(vect1<Unit2,Num2> a)
        {
            return vect2<Unit,decltype(Num()*Num2())>(
                x_ * cos(a) - y_ * sin(a),
                x_ * sin(a) + y_ * cos(a));
        }

        vect2<Unit,Num> rotated_left() const
			{ return vect2<Unit,Num>(- y_, x_); }

        vect2<Unit,Num> rotated_right() const
			{ return vect2<Unit,Num>(y_, - x_); }

        // Construct using a unit and two values.
        vect2(typename Unit::magnitude unit, Num x, Num y):
            x_(static_cast<Num>(x * (unit.ratio() / Unit::ratio()))),
            y_(static_cast<Num>(y * (unit.ratio() / Unit::ratio()))) { }

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

    private:

        // Components.
        Num x_, y_;
    };

    // vect2 == vect2 -> bool
    template <class Unit, typename Num>
    bool operator ==(vect2<Unit,Num> m1, vect2<Unit,Num> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // vect2 != vect2 -> bool
    template <class Unit, typename Num>
    bool operator !=(vect2<Unit,Num> m1, vect2<Unit,Num> m2)
    {
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }

    // is_equal(vect2, vect2, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(vect2<Unit,Num1> m1, vect2<Unit,Num2> m2,
		vect1<Unit,Num3> tolerance)
    {
        return static_cast<Num3>(squared_norm_value(m1 - m2))
            <= squared_norm_value(tolerance);
    }

    template <class Unit, typename Num = double>
    class point2
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Construct without values.
        explicit point2() { }

        // Construct using two numbers of the same number type.
        explicit point2(Num x, Num y): x_(x), y_(y) { }

        // Construct using an array of two numbers of the same number type.
        explicit point2(Num const values[]): x_(values[0]), y_(values[1]) { }

        // Construct using two point1s of the same unit and number type.
        explicit point2(point1<Unit,Num> x, point1<Unit,Num> y):
            x_(x.value()), y_(y.value()) { }

        // Construct using another point2 of the same unit and number type.
        template <typename Num2>
        point2(const point2<Unit,Num2>& o):
            x_(o.x_), y_(o.y_) { }

        // point2 += vect2 -> point2
        point2<Unit,Num> operator +=(vect2<Unit,Num> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            return *this;
        }

        // point2 -= vect2 -> point2
        point2<Unit,Num> operator -=(vect2<Unit,Num> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            return *this;
        }

        template <class Unit2, typename Num2>
        point2<Unit,Num> rotated_by_around(
            vect1<Unit2,Num2> a, point2<Unit,Num> rotation_center)
        {
            return rotation_center + (*this - rotation_center).rotated_by(a);
        }

        // Construct using a unit and a value.
        point2(typename Unit::magnitude unit, Num x, Num y):
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

    private:

        // Components.
        Num x_, y_;
    };

    // midpoint(point2, point2, weight) -> point2
    template <class Unit, typename Num1, typename Num2, typename Num3>
    point2<Unit,decltype((Num1()+Num2())*Num3())> midpoint(
        point2<Unit,Num1> p1, point2<Unit,Num2> p2, Num3 weight = 0.5f)
    {
        //return p1 + (p2 - p1) * weight;
        return point2<Unit,decltype((Num1()+Num2())*Num3())>(
            p1.x().value() * (1 - weight) + p2.x().value() * weight,
            p1.y().value() * (1 - weight) + p2.y().value() * weight);
    }

    // barycentric_combination(int, point2[], Num[]) -> point2
    template <class Unit, typename Num>
    point2<Unit,Num> barycentric_combination(
        int n, point2<Unit,Num> p[], Num weights[])
    {
        Num result_x = 0;
        Num result_y = 0;
        for (int i = 0; i < n; ++i)
        {
            result_x += p[i].x().value() * weights[i];
            result_y += p[i].y().value() * weights[i];
        }
        return point2<Unit,Num>(result_x, result_y);
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
    template <class Unit, typename Num>
    bool operator ==(point2<Unit,Num> m1, point2<Unit,Num> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // point2 != point2 -> bool
    template <class Unit, typename Num>
    bool operator !=(point2<Unit,Num> m1, point2<Unit,Num> m2)
    {
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }

    // is_equal(point2, point2, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(point2<Unit,Num1> m1, point2<Unit,Num2> m2,
        vect1<Unit,Num3> tolerance)
    {
        return static_cast<Num3>(squared_norm_value(m1 - m2))
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
            static_cast<Num>(sqrt(squared_norm_value(v))));
    }


    //////////////////// 3-DIMENSIONAL POINTS AND VECTORS ////////////////////

    template <class Unit, typename Num = double>
    class vect3
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Construct without values.
        explicit vect3() { }

        // Construct using three numbers of the same number type.
        explicit vect3(Num x, Num y, Num z): x_(x), y_(y), z_(z) { }

        // Construct using an array of three numbers of the same number type.
        explicit vect3(Num const values[]):
            x_(values[0]), y_(values[1]), z_(values[2]) { }

        // Construct using three vect1s of the same unit and number type.
        explicit vect3(vect1<Unit,Num> x, vect1<Unit,Num> y,
			vect1<Unit,Num> z):
            x_(x.value()), y_(y.value()), z_(z.value()) { }

        // Construct using another vect3 of the same unit and number type.
        vect3(const vect3<Unit,Num>& o): x_(o.x_), y_(o.y_), z_(o.z_) { }

        // +vect3 -> vect3
        vect3<Unit,Num> operator +() const { return *this; }

        // -vect3 -> vect3
        vect3<Unit,Num> operator -() const
			{ return vect3<Unit,Num>(-x_, -y_, -z_); }

        // vect3 += vect3 -> vect3
        vect3<Unit,Num> operator +=(vect3<Unit,Num> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            z_ += m2.z().value();
            return *this;
        }

        // vect3 -= vect3 -> vect3
        vect3<Unit,Num> operator -=(vect3<Unit,Num> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            z_ -= m2.z().value();
            return *this;
        }

        // vect3 *= N -> vect3
        template <typename Num2>
        vect3<Unit,Num> operator *=(Num2 n)
        {
            x_ *= static_cast<Num>(n);
            y_ *= static_cast<Num>(n);
            z_ *= static_cast<Num>(n);
            return *this;
        }

        // vect3 /= N -> vect3
        template <typename Num2>
        vect3<Unit,Num> operator /=(Num2 n)
        {
            x_ /= static_cast<Num>(n);
            y_ /= static_cast<Num>(n);
            z_ /= static_cast<Num>(n);
            return *this;
        }

        // Construct using a unit and a value.
        vect3(typename Unit::magnitude unit, Num x, Num y, Num z):
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
    template <class Unit, typename Num>
    bool operator ==(vect3<Unit,Num> m1, vect3<Unit,Num> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // vect3 != vect3 -> bool
    template <class Unit, typename Num>
    bool operator !=(vect3<Unit,Num> m1, vect3<Unit,Num> m2)
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
        return static_cast<Num3>(squared_norm_value(m1 - m2))
            <= squared_norm_value(tolerance);
    }

    template <class Unit, typename Num = double>
    class point3
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Construct without values.
        explicit point3() { }

        // Construct using two numbers of the same number type.
        explicit point3(Num x, Num y, Num z): x_(x), y_(y), z_(z) { }

        // Construct using an array of two numbers of the same number type.
        explicit point3(Num const values[]):
            x_(values[0]), y_(values[1]), z_(values[2]) { }

        // Construct using three point1s of the same unit and number type.
        explicit point3(point1<Unit,Num> x, point1<Unit,Num> y,
			point1<Unit,Num> z):
            x_(x.value()), y_(y.value()), z_(z.value()) { }

        // Construct using another point3 of the same unit and number type.
        point3(const point3<Unit,Num>& o):
            x_(o.x_), y_(o.y_), z_(o.z_) { }

        // point3 += vect3 -> point3
        point3<Unit,Num> operator +=(vect3<Unit,Num> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            z_ += m2.z().value();
            return *this;
        }

        // point3 -= vect3 -> point3
        point3<Unit,Num> operator -=(vect3<Unit,Num> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            z_ -= m2.z().value();
            return *this;
        }

        // Construct using a unit and a value.
        point3(typename Unit::magnitude unit, Num x, Num y, Num z):
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
        point3<Unit,Num1> p1, point3<Unit,Num2> p2, Num3 weight = 0.5f)
    {
        //return p1 + (p2 - p1) * weight;
        return point3<Unit,decltype((Num1()+Num2())*Num3())>(
            p1.x().value() * (1 - weight) + p2.x().value() * weight,
            p1.y().value() * (1 - weight) + p2.y().value() * weight,
            p1.z().value() * (1 - weight) + p2.z().value() * weight);
    }

    // barycentric_combination(int, point3[], Num[]) -> point3
    template <class Unit, typename Num>
    point3<Unit,Num> barycentric_combination(
        int n, point3<Unit,Num> p[], Num weights[])
    {
        Num result_x = 0;
        Num result_y = 0;
        Num result_z = 0;
        for (int i = 0; i < n; ++i)
        {
            result_x += p[i].x().value() * weights[i];
            result_y += p[i].y().value() * weights[i];
            result_z += p[i].z().value() * weights[i];
        }
        return point3<Unit,Num>(result_x, result_y, result_z);
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
    template <class Unit, typename Num>
    bool operator ==(point3<Unit,Num> m1, point3<Unit,Num> m2)
    {
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // point3 != point3 -> bool
    template <class Unit, typename Num>
    bool operator !=(point3<Unit,Num> m1, point3<Unit,Num> m2)
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
        return static_cast<Num3>(squared_norm_value(m1 - m2))
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
            static_cast<Num>(sqrt(squared_norm_value(v))));
    }


    //////////////////// UNIT CONVERSIONS ////////////////////

    // Scalar measures
    template <class ToUnit, class FromUnit, typename Num>
    vect1<ToUnit,Num> convert(vect1<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return vect1<ToUnit,Num>(static_cast<Num>(
            m.value() * (FromUnit::ratio() / ToUnit::ratio())));
    }

    template <class ToUnit, class FromUnit, typename Num>
    point1<ToUnit,Num> convert(point1<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return point1<ToUnit,Num>(static_cast<Num>(
            m.value() * (FromUnit::ratio() / ToUnit::ratio())
            + (FromUnit::offset() - ToUnit::offset()) / ToUnit::ratio()));
    }

    // Planar measures
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

    // Spacial measures
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
            convert<ToUnit>(point1<FromUnit>(m.value())));
    }

    template <class ToUnit, class FromUnit, typename Num>
    unsigned_azimuth<ToUnit,Num> convert(unsigned_azimuth<FromUnit,Num> m)
    {
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return unsigned_azimuth<ToUnit,Num>(
            convert<ToUnit>(point1<FromUnit>(m.value())));
    }


    //////////////////// AZIMUTHS ////////////////////

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
    // For floating-point numbers use fmod.
    template <typename Num>
    Num normalize_signed_azimuth(Num x, Num one_turn,
        typename std::enable_if<std::is_floating_point<Num>
        ::value >::type* = 0)
    {
        const Num half_turn = one_turn * 0.5f;
        if (std::abs(x) < half_turn) return x;
        Num x2 = fmod(x + half_turn, one_turn);
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
    // For floating-point numbers use fmod.
    template <typename Num>
    Num normalize_unsigned_azimuth(Num x, Num one_turn,
        typename std::enable_if<std::is_floating_point<Num>
        ::value >::type* = 0)
    {
        Num x2 = fmod(x, one_turn);
        return x2 >= 0 ? x2 : x2 + one_turn;
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
        return std::cos(convert<radians>(m).value());
    }

    // tan(vect1) -> N
    template <class Unit, typename Num>
    Num tan(vect1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return std::tan(convert<radians>(m).value());
    }

    // sin(point1) -> N
    template <class Unit, typename Num>
    Num sin(point1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return std::sin(convert<radians>(m).value());
    }

    // cos(point1) -> N
    template <class Unit, typename Num>
    Num cos(point1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return std::cos(convert<radians>(m).value());
    }

    // tan(point1) -> N
    template <class Unit, typename Num>
    Num tan(point1<Unit,Num> m)
    {
        ASSERT_IS_ANGLE(Unit)
        return std::tan(convert<radians>(m).value());
    }


    //////////////////// SIGNED AZIMUTH ////////////////////

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

        // Construct without values.
        explicit signed_azimuth() { }

        // Construct using one number of the same number type.
        explicit signed_azimuth(Num x): x_(normalize_(x)) { }

        // Construct using another signed_azimuth of the same unit
		// and number type.
        signed_azimuth(const signed_azimuth<Unit,Num>& o): x_(o.x_) { }

        // Construct using a point1 representing an angle.
        explicit signed_azimuth(point1<Unit, Num> o):
            x_(normalize_(o.value())) { }

        // Construct using an unsigned_azimuth.
        explicit signed_azimuth(unsigned_azimuth<Unit, Num> o):
            x_(normalize_(o.value())) { }

        // Construct using a vect2.
        template <class Unit2, typename Num2>
        explicit signed_azimuth(vect2<Unit2,Num2> v):
            x_(convert<Unit>(signed_azimuth<radians,Num>(
                static_cast<Num>(atan2(v.y().value(),
                v.x().value())))).value()) { }

        // Convert to a point1.
        operator point1<Unit,Num>() const { return point1<Unit,Num>(x_); }

        // Construct using a unit and a value.
        explicit signed_azimuth(typename Unit::magnitude unit, Num x):
            x_(normalize_(x * (unit.ratio() / Unit::ratio())
                + (unit.offset() - Unit::offset()) / Unit::ratio())) { }

        // Get unmutable value for the given unit.
        Num value(typename Unit::magnitude unit) const
        {
            return static_cast<Num>(x_ * (Unit::ratio() / unit.ratio())
                + (Unit::offset() - unit.offset()) / unit.ratio());
        }

        // Get unmutable value (mutable getter is not available).
        Num value() const { return x_; }

        // signed_azimuth += vect1 -> signed_azimuth
        signed_azimuth<Unit,Num> operator +=(vect1<Unit,Num> m)
			{ x_ = normalize_(x_ + m.value()); return *this; }

        // signed_azimuth -= vect1 -> signed_azimuth
        signed_azimuth<Unit,Num> operator -=(vect1<Unit,Num> m)
            { x_ = normalize_(x_ - m.value()); return *this; }

    private:

		// Returns the only value that in the current Unit represents `x`, and
		// is between minus half turn included and plus half turn excluded.
        static Num normalize_(Num x)
        {
            return normalize_signed_azimuth(
				x, static_cast<Num>(Unit::turn_fraction()));
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
        auto one_turn = Unit::turn_fraction();
        if (difference * 2 < -one_turn) difference += one_turn;
        else if (difference * 2 >= one_turn) difference -= one_turn;
        return vect1<Unit,decltype(Num1()-Num2())>(difference);
    }

    // signed_azimuth == signed_azimuth -> bool
    template <class Unit, typename Num>
    bool operator ==(signed_azimuth<Unit,Num> m1,
		signed_azimuth<Unit,Num> m2)
		{ return m1.value() == m2.value(); }

    // signed_azimuth != signed_azimuth -> bool
    template <class Unit, typename Num>
    bool operator !=(signed_azimuth<Unit,Num> m1,
		signed_azimuth<Unit,Num> m2)
		{ return m1.value() != m2.value(); }

    // angle_distance(signed_azimuth, signed_azimuth) -> vect1
    template <class Unit, typename Num>
    vect1<Unit,Num> angle_distance(
		signed_azimuth<Unit,Num> m1, signed_azimuth<Unit,Num> m2)
    {
        Num value_distance(std::abs(m1.value() - m2.value()));
        if (value_distance * 2 > Unit::turn_fraction())
            value_distance = Unit::turn_fraction() - value_distance;
        return vect1<Unit,Num>(value_distance);
    }

    // is_equal(signed_azimuth, signed_azimuth, tolerance) -> bool
    template <class Unit, typename Num>
    bool is_equal(signed_azimuth<Unit,Num> m1,
        signed_azimuth<Unit,Num> m2, vect1<Unit,Num> tolerance)
    {
        return angle_distance(m1, m2) <= tolerance;
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

    // sin(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num sin(signed_azimuth<Unit,Num> m)
		{ return std::sin(convert<radians>(m).value()); }

    // cos(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num cos(signed_azimuth<Unit,Num> m)
		{ return std::cos(convert<radians>(m).value()); }

    // tan(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num tan(signed_azimuth<Unit,Num> m)
		{ return std::tan(convert<radians>(m).value()); }

    template <class Unit = Angle::base_unit, typename Num = double>
    class unsigned_azimuth
    {
    public:
        typedef Unit unit;
        typedef Num numeric_type;

        // Construct without values.
        explicit unsigned_azimuth() { }

        // Construct using one number of the same number type.
        explicit unsigned_azimuth(Num x): x_(normalize_(x)) { }

        // Construct using another unsigned_azimuth of the same unit
        // and the same number type.
        unsigned_azimuth(const unsigned_azimuth<Unit,Num>& o): x_(o.x_) { }

        // Construct using a point1 representing an angle.
        explicit unsigned_azimuth(point1<Unit, Num> o):
            x_(normalize_(o.value())) { }

        // Construct using a signed_azimuth.
        explicit unsigned_azimuth(signed_azimuth<Unit, Num> o):
            x_(normalize_(o.value())) { }

        // Construct using a vect2.
        template <class Unit2, typename Num2>
        explicit unsigned_azimuth(vect2<Unit2,Num2> v):
            x_(convert<Unit>(unsigned_azimuth<radians,Num>(
                static_cast<Num>(atan2(v.y().value(),
                v.x().value())))).value()) { }

        // Convert to a point1.
        operator point1<Unit,Num>() const { return point1<Unit,Num>(x_); }

        // Construct using a unit and a value.
        explicit unsigned_azimuth(typename Unit::magnitude unit, Num x):
            x_(normalize_(x * (unit.ratio() / Unit::ratio())
                + (unit.offset() - Unit::offset()) / Unit::ratio())) { }

        // Get unmutable value for the given unit.
        Num value(typename Unit::magnitude unit) const
        {
            return static_cast<Num>(x_ * (Unit::ratio() / unit.ratio())
                + (Unit::offset() - unit.offset()) / unit.ratio());
        }

        // Get unmutable value (mutable getter is not available).
        Num value() const { return x_; }

        // unsigned_azimuth += vect1 -> unsigned_azimuth
        unsigned_azimuth<Unit,Num> operator +=(vect1<Unit,Num> m)
			{ x_ = normalize_(x_ + m.value()); return *this; }

        // unsigned_azimuth -= vect1 -> unsigned_azimuth
        unsigned_azimuth<Unit,Num> operator -=(vect1<Unit,Num> m)
			{ x_ = normalize_(x_ - m.value()); return *this; }

    private:

		// Returns the only value that in the current Unit represents `x`, and
		// is between 0 included and plus one turn excluded.
        static Num normalize_(Num x)
        {
            return normalize_unsigned_azimuth(x,
				static_cast<Num>(Unit::turn_fraction()));
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
        auto one_turn = Unit::turn_fraction();
        if (difference * 2 < -one_turn) difference += one_turn;
        else if (difference * 2 >= one_turn) difference -= one_turn;
        return vect1<Unit,decltype(Num1()-Num2())>(difference);
    }

    // unsigned_azimuth == unsigned_azimuth -> bool
    template <class Unit, typename Num>
    bool operator ==(unsigned_azimuth<Unit,Num> m1,
		unsigned_azimuth<Unit,Num> m2)
		{ return m1.value() == m2.value(); }

    // unsigned_azimuth != unsigned_azimuth -> bool
    template <class Unit, typename Num>
    bool operator !=(unsigned_azimuth<Unit,Num> m1,
		unsigned_azimuth<Unit,Num> m2)
		{ return m1.value() != m2.value(); }

    // angle_distance(unsigned_azimuth, unsigned_azimuth) -> vect1
    template <class Unit, typename Num>
    vect1<Unit,Num> angle_distance(unsigned_azimuth<Unit,Num> m1,
		unsigned_azimuth<Unit,Num> m2)
    {
        Num value_distance(std::abs(m1.value() - m2.value()));
        if (value_distance * 2 > Unit::turn_fraction())
            value_distance = Unit::turn_fraction() - value_distance;
        return vect1<Unit,Num>(value_distance);
    }

    // is_equal(unsigned_azimuth, unsigned_azimuth, tolerance) -> bool
    template <class Unit, typename Num>
    bool is_equal(unsigned_azimuth<Unit,Num> m1,
		unsigned_azimuth<Unit,Num> m2, vect1<Unit,Num> tolerance)
		{ return angle_distance(m1, m2) <= tolerance; }

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

    // sin(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num sin(unsigned_azimuth<Unit,Num> m)
		{ return std::sin(convert<radians>(m).value()); }

    // cos(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num cos(unsigned_azimuth<Unit,Num> m)
		{ return std::cos(convert<radians>(m).value()); }

    // tan(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num tan(unsigned_azimuth<Unit,Num> m)
		{ return std::tan(convert<radians>(m).value()); }


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

#define DEFINE_VECT_UNIT_LITERAL(Unit, Num, Operator) \
    namespace measures\
    {\
		/*operator for floating-point literals*/\
		vect1<Unit,Num> operator "" _##Operator(long double n)\
			{ return vect1<Unit,Num>(n); }\
		/*operator for integral literals*/\
		vect1<Unit,Num> operator "" _##Operator(unsigned long long n)\
			{ return vect1<Unit,Num>(n); }\
    }

#define DEFINE_POINT_UNIT_LITERAL(Unit, Num, Operator) \
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
