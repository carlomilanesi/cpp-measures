#include "measures.hpp"
using namespace measures;
using namespace std;

// Define alternative angle units.
DEFINE_ANGLE_UNIT(degrees, "^", 360, 0)
DEFINE_ANGLE_UNIT(gradians, " gon", 400, 0)
DEFINE_ANGLE_UNIT(turns, " rev", 1, 0)
	
// Define solid angles with three units.
DEFINE_MAGNITUDE(SolidAngle, steradians, " sr")
DEFINE_UNIT(allrounds, SolidAngle, " all-round",\
    4*3.1415926535897932384626433832795, 0)
DEFINE_UNIT(square_degrees, SolidAngle, "^^",\
    3.1415926535897932384626433832795/180*3.1415926535897932384626433832795/180, 0)
	
// Define unitless measures.
DEFINE_MAGNITUDE(Amount, units, "")
	
// Define space and time units and some of their derived units.
DEFINE_MAGNITUDE(Space, metres, " m")
DEFINE_UNIT(inches, Space, "\"", 0.0254, 0)
DEFINE_UNIT(mm, Space, " mm", 0.001, 0)
DEFINE_UNIT(km, Space, " Km", 1000, 0)

DEFINE_MAGNITUDE(Time, seconds, " s")
DEFINE_UNIT(minutes, Time, " min", 60, 0)
DEFINE_UNIT(hours, Time, " h", 3600, 0)

DEFINE_MAGNITUDE(Speed, metres_per_second, " m/s")
DEFINE_UNIT(km_per_hour, Speed, " Km/h", 1/3.6, 0)

DEFINE_MAGNITUDE(Acceleration, metres_per_squared_second, " m/s2")
DEFINE_UNIT(km_per_hours_per_second, Acceleration, " Km/h/s", 1/3.6, 0)

DEFINE_MAGNITUDE(SquaredTime, squared_seconds, " s2")

// Define the combined operations among the defined units.
DEFINE_DERIVED_UNIT_SQUARED_SCALAR(seconds, squared_seconds)
DEFINE_DERIVED_UNIT_SCALAR_VECTOR(seconds, metres_per_second, metres)
DEFINE_DERIVED_UNIT_SCALAR_VECTOR(seconds, metres_per_squared_second, metres_per_second)
DEFINE_DERIVED_UNIT_SCALAR_VECTOR(squared_seconds, metres_per_squared_second, metres)

// Define the units derived from time and angle units.
DEFINE_MAGNITUDE(AngularSpeed, radians_per_second, " rad/s")
DEFINE_UNIT(turns_per_minute, AngularSpeed, " rpm", 60 / (2 * 3.14159265358979323846), 0)
DEFINE_MAGNITUDE(AngularAcceleration, radians_per_squared_second, " rad/s2")

// Define the combined operations among angle and derived units.
DEFINE_DERIVED_UNIT_SCALAR_SCALAR(seconds, radians_per_second, Angle)
DEFINE_DERIVED_UNIT_SCALAR_SCALAR(seconds, radians_per_squared_second, radians_per_second)
DEFINE_DERIVED_UNIT_SCALAR_SCALAR(squared_seconds, radians_per_squared_second, radians)

// Define unitless combined operations.
DEFINE_DERIVED_UNIT_SQUARED_VECTOR(units, units)

// Define vector combined operations.
DEFINE_MAGNITUDE(Force, newtons, " N")
DEFINE_MAGNITUDE(Energy, joules, " J")
DEFINE_DERIVED_UNIT_VECTOR_VECTOR(newtons, metres, joules)
	
DEFINE_DERIVED_UNIT_SQUARED_SCALAR(radians, steradians)
	
int main()
{
// vect1 construction.
@ILLEGAL
    // Measures are templates.
	vect1 v;
@END
@ILLEGAL
    // Measures need a unit.
	vect1<> v;
@END
@ILLEGAL
    // Measures need a defined unit.
	vect1<days> v;
@END
@ILLEGAL
    // Measures need a numeric type, i.e. capable of arithmetics.
	vect1<seconds,void> v;
@END
@ILLEGAL
    // 1D measures cannot be initialized using 2 numbers.
	vect1<seconds> v(1, 2);
@END
@ILLEGAL
    // 1D measures cannot be initialized using 3 numbers.
	vect1<seconds> v(1, 2, 3);
@END
@ILLEGAL
    // 1D measures cannot be initialized using 2 1D measures.
	vect1<seconds> v1(4);
    vect1<seconds> v2(v1, v1);
@END
@ILLEGAL
    // 1D measures cannot be initialized using 3 1D measures.
	vect1<seconds> v1(4);
    vect1<seconds> v2(v1, v1, v1);
@END
@ILLEGAL
    // 1D measures cannot be initialized using a 2D measure.
	vect2<seconds> v1(1, 2);
    vect1<seconds> v2(v1);
@END
@ILLEGAL
    // 1D measures cannot be initialized using a 3D measure.
	vect3<seconds> v1(1, 2, 3);
    vect1<seconds> v2(v1);
@END
@ILLEGAL
    // A vect cannot be initialized using a point.
	point1<seconds> p(1);
    vect1<seconds> v(p);
@END
@ILLEGAL
    // Measure assignment cannot mix units.
	vect1<metres> v1(1.2);
    vect1<inches> v2(2.3);
    v1 = v2;
@END

// vect1 construction with conversion.
@ILLEGAL
    // Units are classes, not values.
	vect1<seconds> v(seconds, 3.4);
@END
@ILLEGAL
    // To specify a unit, do not instantiate it,
    // but use its `id` member function.
	vect1<seconds> v(seconds(), 3.4);
@END
@ILLEGAL
    // To specify a unit, don't use the address of its `id` member function,
    // but call it.
	vect1<seconds> v(seconds::id, 3.4);
@END
@ILLEGAL
    // A measure cannot be constructed by using only a unit.
	vect1<metres> v(inches::id());
@END
@ILLEGAL
    // A 1D measure cannot be constructed by using a unit and 2 numbers.
	vect1<metres> v(inches::id(), 1, 2);
@END
@ILLEGAL
    // A 1D measure cannot be constructed by using a unit and 3 numbers.
	vect1<metres> v(inches::id(), 1, 2, 3);
@END
@ILLEGAL
    // A measures cannot be constructed by converting
    // a unit of a different magnitude.
	vect1<metres> v(seconds::id(), 1);
@END

// vect1 access with conversion.
@ILLEGAL
    // Units are types, not values.
	vect1<seconds> v1(1);
    double n = v1.value(seconds);
@END
@ILLEGAL
    // To specify a run-time unit, it shouldn't be instantiated,
    // but its `id` function must be called.
	vect1<seconds> v1(1);
    double n = v1.value(seconds());
@END
@ILLEGAL
    // To specify a run-time unit,
    // the `id` function must be called, not passed.
	vect1<seconds> v1(1);
    double n = v1.value(seconds::id);
@END
@ILLEGAL
    // The value of measures cannot be extracted by converting it
    // to a unit of a different magnitude.
	vect1<seconds> v1(1);
    double n = v1.value(metres::id());
@END

// vect1 access.
@ILLEGAL
    // `value` is a member function, not a member variable.
	vect1<seconds> v(1);
    double n = v.value;
@END
@ILLEGAL
    // 1D measures, have no `x` member function.
	vect1<seconds> v(1);
    double n = v.x();
@END
@ILLEGAL
    // Measures, have no `x` member variable.
	vect1<seconds> v(1);
    double n = v.x;
@END
@ILLEGAL
    // 1D measures, have a `x_` member variable, but it is private.
	vect1<seconds> v(1);
    double n = v.x_;
@END
@ILLEGAL
    // `value` is a member function, not a member variable.
	vect1<seconds> v(1);
    v.value = 3;
@END
@ILLEGAL
    // Measures, have no `x` member variable.
	vect1<seconds> v(1);
    v.x = 3;
@END
@ILLEGAL
    // 1D measures, have a `x_` member variable, but it is private.
	vect1<seconds> v(1);
    v.x_ = 3;
@END
@ILLEGAL
    // 1D measures, have no `x` member function.
	vect1<seconds> v(1);
    v.x() = 3;
@END

// Member arithmetic operations.
@ILLEGAL
    // Measure increment cannot mix units.
	vect1<metres> v(1);
    v += vect1<inches>(2);
@END
@ILLEGAL
    // Measure decrement cannot mix units.
	vect1<metres> v(1);
    v -= vect1<inches>(2);
@END
@ILLEGAL
    // Cannot add a pointX to a vectX.
	vect1<metres> v(1);
    v += point1<metres>(2);
@END
@ILLEGAL
    // Cannot subtract a pointX from a vectX.
	vect1<metres> v(1);
    v -= point1<metres>(2);
@END
@ILLEGAL
    // Cannot scale up a vectX by a vectX.
	vect1<metres> v(1);
    v *= vect1<metres>(2);
@END
@ILLEGAL
    // Cannot scale down a vectX by a vectX.
	vect1<metres> v(1);
    v /= vect1<metres>(2);
@END

@ILLEGAL
    // Cannot compare measures with different units.
	vect1<metres>(1) == vect1<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
	vect1<metres>(1) != vect1<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
	vect1<metres>(1) < vect1<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
	vect1<metres>(1) <= vect1<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
	vect1<metres>(1) > vect1<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
	vect1<metres>(1) >= vect1<inches>(2);
@END
@ILLEGAL
    // `is_equal` requires a tolerance.
	is_equal(vect1<metres>(1), vect1<metres>(2));
@END
@ILLEGAL
    // `is_equal` arguments must have the same unit.
	is_equal(vect1<metres>(1), vect1<inches>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_equal` arguments must have the same unit.
	is_equal(vect1<metres>(1), vect1<inches>(2), vect1<metres>(3));
@END
@ILLEGAL
    // `is_equal` tolerance must have the same unit as arguments.
	is_equal(vect1<metres>(1), vect1<metres>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_equal` tolerance must be a vectX.
	is_equal(vect1<metres>(1), vect1<metres>(2), point1<inches>(3));
@END
@ILLEGAL
    // `is_equal` tolerance must be a 1D measure.
	is_equal(vect1<metres>(1), vect1<metres>(2), vect2<inches>(3));
@END

@ILLEGAL
    // `is_less` requires a tolerance.
	is_less(vect1<metres>(1), vect1<metres>(2));
@END
@ILLEGAL	
    // `is_less` arguments must have the same unit.
	is_less(vect1<metres>(1), vect1<inches>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less` arguments must have the same unit.
	is_less(vect1<metres>(1), vect1<inches>(2), vect1<metres>(3));
@END
@ILLEGAL
    // `is_less` tolerance must have the same unit as arguments.
	is_less(vect1<metres>(1), vect1<metres>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less` tolerance must be a vectX.
	is_less(vect1<metres>(1), vect1<metres>(2), point1<inches>(3));
@END
@ILLEGAL
    // `is_less` tolerance must be a 1D measure.
	is_less(vect1<metres>(1), vect1<metres>(2), vect2<inches>(3));
@END

@ILLEGAL
    // `is_less_or_equal` requires a tolerance.
	is_less_or_equal(vect1<metres>(1), vect1<metres>(2));
@END
@ILLEGAL
    // `is_less_or_equal` arguments must have the same unit.
	is_less_or_equal(vect1<metres>(1), vect1<inches>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less_or_equal` arguments must have the same unit.
	is_less_or_equal(vect1<metres>(1), vect1<inches>(2), vect1<metres>(3));
@END
@ILLEGAL
    // `is_less_or_equal` tolerance must have the same unit as arguments.
	is_less_or_equal(vect1<metres>(1), vect1<metres>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less_or_equal` tolerance must be a vectX.
	is_less_or_equal(vect1<metres>(1), vect1<metres>(2), point1<inches>(3));
@END
@ILLEGAL
    // `is_less_or_equal` tolerance must be a 1D measure.
	is_less_or_equal(vect1<metres>(1), vect1<metres>(2), vect2<inches>(3));
@END

@ILLEGAL
    // Measures are templates.
	point1 p(1);
@END
@ILLEGAL
    // Measures need a unit.
	point1<> p(1);
@END
@ILLEGAL
    // Measures need a defined unit.
	point<days> p(1);
@END
@ILLEGAL
    // Measures need a numeric type, i.e. capable of arithmetics.
	point1<seconds,void> p(1);	
@END
@ILLEGAL
    // 1D measures cannot be initialized using 2 numbers.
	point1<seconds> p(1, 2);
@END
@ILLEGAL
    // 1D measures cannot be initialized using 2 measures.
	point1<seconds> p1(1);
    point1<seconds> p2(p1, p1);
@END
@ILLEGAL
    // Measure assignment cannot mix units.
	point1<metres> p1(1);
    point1<inches> p2(2);
    p1 = p2;
@END
@ILLEGAL
    // A measures cannot be constructed by converting
    // a unit of a different magnitude.
	point1<seconds> p(metres::id(), 1);
@END
@ILLEGAL
    // Units are classes, not values.
	point1<seconds> p(seconds, 1);
@END
@ILLEGAL
    // To specify a unit, do not instantiate it,
    // but use its `id` member function.
	point1<seconds> p(seconds(), 1);
@END
@ILLEGAL
    // To specify a unit, don't use the address of its `id` member function,
    // but call it.
	point1<seconds> p(seconds::id, 1);
@END
@ILLEGAL
    // A 1D measures cannot be constructed by converting two numbers.
	point1<seconds> p(seconds::id(), 1, 2);
@END
@ILLEGAL
    // An angle point cannot be constructed
    // using an azimuth of a different unit.
	point1<radians> p(signed_azimuth<degrees>(1));
@END
@ILLEGAL
    // An angle point cannot be constructed
    // using an azimuth of a different unit.
	point1<radians> p(unsigned_azimuth<degrees>(1));
@END

@ILLEGAL
    // The value of measures cannot be extracting converting it
    // to a unit of a different magnitude.
	point1<seconds> p(1);
    double n = p.value(metres::id());
@END
@ILLEGAL
    // To specify a run-time unit,
    // the `id` function must be called, not passed.
	point1<seconds> p(1);
    double n = p.value(seconds::id);
@END
@ILLEGAL
    // To specify a run-time unit, it shouldn't be instantiated,
    // but its `id` function must be called.
	point1<seconds> p(1);
    double n = p.value(seconds());
@END
@ILLEGAL
    // Units are types, not values.
	point1<seconds> p(1);
    double n = p.value(seconds);
@END
@ILLEGAL
    // `value` is a member function, not a member variable.
	point1<seconds> p(1);
    double n = p.value;
@END
@ILLEGAL
    // 1D measures, have no `x` member function.
	point1<seconds> p(1);
    double n = p.x();
@END
@ILLEGAL
    // Measures, have no `x` member variable.
	point1<seconds> p(1);
    double n = p.x;
@END
@ILLEGAL
    // 1D measures, have a `x_` member variable, but it is private.
	point1<seconds> p(1);
    double n = p.x_;
@END
@ILLEGAL
    // `value` is a member function, not a member variable.
	point1<seconds> p(1);
    p.value = 3;
@END
@ILLEGAL
    // Measures, have no `x` member variable.
	point1<seconds> p(1);
    p.x = 3;
@END
@ILLEGAL
    // 1D measures, have a `x_` member variable, but it is private.
	point1<seconds> p(1);
    p.x_ = 3;
@END
@ILLEGAL
    // 1D measures, have no `x` member function.
	point1<seconds> p(1);
    p.x() = 3;
@END

@ILLEGAL
    // Cannot add a pointX to a pointX.
	point1<seconds> p(1);
    p += point1<seconds>(2);
@END
@ILLEGAL
    // Measure increment cannot mix units.
	point1<metres> p(1);
    p += vect1<inches>(2);
@END
@ILLEGAL
    // Cannot subtract a pointX from a pointX.
	point1<seconds> p(1);
    p -= point1<seconds>(2);
@END
@ILLEGAL
    // Measure decrement cannot mix units.
	point1<metres> p(1);
    p -= vect1<inches>(2);
@END
@ILLEGAL
    // Cannot scale up a pointX by a pointX.
	point1<seconds> p(1);
    p *= point1<seconds>(2);
@END
@ILLEGAL
    // Cannot scale up a pointX by a vectX.
	point1<seconds> p(1);
    p *= vect1<seconds>(2);
@END
@ILLEGAL
    // Cannot scale up a pointX by a vectX.
	point1<seconds> p(1);
    p *= 1.3;
@END
@ILLEGAL
    // Cannot scale down a pointX by a pointX.
	point1<seconds> p(1);
    p /= point1<seconds>(2);
@END
@ILLEGAL
    // Cannot scale down a pointX by a vectX.
	point1<seconds> p(1);
    p /= vect1<seconds>(2);
@END
@ILLEGAL
    // Cannot scale down a pointX by a number.
	point1<seconds> p(1);
    p /= 1.3;
@END

@ILLEGAL
    // Cannot compute the midpoint of two vectX
    // (or if you need it, you can do it yourself).
	midpoint(vect1<metres>(1), vect1<metres>(2));
@END
@ILLEGAL
    // Cannot compute the midpoint of a pointX and a vectX.
	midpoint(point1<metres>(1), vect1<metres>(2));
@END
@ILLEGAL
    // Cannot compute the midpoint of a vectX and a pointX.
	midpoint(vect1<metres>(1), point1<metres>(2));
@END
@ILLEGAL
    // Cannot compute the midpoint between measure with different unit.
	midpoint(point1<metres>(1), point1<inches>(2));
@END
@ILLEGAL
    // The resulting midpoint between measures has the same unit
    // as the arguments.
	point1<inches> p(midpoint(point1<metres>(1), point1<metres>(2)));
@END
@ILLEGAL
    // Cannot compute the weighted midpoint of two vectX
    // (or if you need it, you can do it yourself).
	midpoint(vect1<metres>(1), vect1<metres>(2), 0.4);
@END
@ILLEGAL
    // Cannot compute the weighted midpoint of a pointX and a vectX.
	midpoint(point1<metres>(1), vect1<metres>(2), 0.4);
@END
@ILLEGAL
    // Cannot compute the weighted midpoint of a vectX and a pointX.
	midpoint(vect1<metres>(), point1<metres>(), 0.4);
@END
@ILLEGAL
    // Cannot compute the weighted midpoint between measure
    // with different unit.
	midpoint(point1<metres>(1), point1<inches>(2), 0.4);
@END
@ILLEGAL
    // The resulting weighted midpoint between measures has the same unit
    // as the arguments.
	point1<inches> p(midpoint(point1<metres>(1), point1<metres>(2), 0.4));
@END
@ILLEGAL
    // Cannot compute a barycentric combination among vectX.
    vect1<metres> v(0.4);
    vect1<metres,float> vv[] = { v, v, v };
	float ww[] = { 0.2f, 0.3f, 0.5f };
	barycentric_combination(3, vv, ww);
@END
@ILLEGAL
    // The result of a barycentric combination is a pointX.
    point1<metres> p(0.4);
    point1<metres> pp[] = { p, p, p };
	double ww[] = { 0.2, 0.3, 0.5 };
	vect1<metres> v(barycentric_combination(3, pp, ww));
@END
@ILLEGAL
    // Cannot subtract pointX having different units.
    point1<metres>(1) - point1<inches>(2);
@END
@ILLEGAL
    // The result of subtracting a pointX from a pointX is a vectX.
    point1<metres> p = point1<metres>(1) - point1<metres>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
    point1<metres>(1) == p2<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
    point1<metres>(1) != p2<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
    point1<metres>(1) < p2<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
    point1<metres>(1) <= p2<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
    point1<metres>(1) > p2<inches>(2);
@END
@ILLEGAL
    // Cannot compare measures with different units.
    point1<metres>(1) >= p2<inches>(2);
@END
@ILLEGAL
    // `is_equal` requires a tolerance.
    is_equal(point1<metres>(1), point1<metres>(2));
@END
@ILLEGAL
    // `is_equal` arguments must have the same unit.
    is_equal(point1<metres>(1), point1<inches>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_equal` arguments must have the same unit.
    is_equal(point1<metres>(1), point1<inches>(2), vect1<metres>(3));
@END
@ILLEGAL
    // `is_equal` tolerance must have the same unit as arguments.
    is_equal(point1<metres>(1), point1<metres>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_equal` tolerance must be a vectX.
    is_equal(point1<metres>(1), point1<metres>(2), point1<metres>(3));
@END
@ILLEGAL
    // `is_equal` tolerance must be a 1D measure.
    is_equal(point1<metres>(1), point1<metres>(2), vect2<metres>(3));
@END
@ILLEGAL
    // `is_equal` arguments to compare must be of the same kind.
    is_equal(point1<metres>(1), vect1<metres>(2), vect1<metres>(3));
@END

@ILLEGAL
    // `is_less` requires a tolerance.
    is_less(point1<metres>(1), point1<metres>(2));
@END
@ILLEGAL
    // `is_less` arguments must have the same unit.
    is_less(point1<metres>(1), point1<inches>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less` arguments must have the same unit.
    is_less(point1<metres>(1), point1<inches>(2), vect1<metres>(3));
@END
@ILLEGAL
    // `is_less` tolerance must have the same unit as arguments.
    is_less(point1<metres>(1), point1<metres>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less` tolerance must be a vectX.
    is_less(point1<metres>(1), point1<metres>(2), point1<metres>(3));
@END
@ILLEGAL
    // `is_less` tolerance must be a 1D measure.
    is_less(point1<metres>(1), point1<metres>(2), vect2<metres>(3));
@END
@ILLEGAL
    // `is_less` arguments to compare must be of the same kind.
    is_less(point1<metres>(1), vect1<metres>(2), vect1<metres>(3));
@END

@ILLEGAL
    // `is_less_or_equal` requires a tolerance.
    is_less_or_equal(point1<metres>(1), point1<metres>(2));
@END
@ILLEGAL
    // `is_less_or_equal` arguments must have the same unit.
    is_less_or_equal(point1<metres>(1), point1<inches>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less_or_equal` arguments must have the same unit.
    is_less_or_equal(point1<metres>(1), point1<inches>(2), vect1<metres>(3));
@END
@ILLEGAL
    // `is_less_or_equal` tolerance must have the same unit as arguments.
    is_less_or_equal(point1<metres>(1), point1<metres>(2), vect1<inches>(3));
@END
@ILLEGAL
    // `is_less_or_equal` tolerance must be a vectX.
    is_less_or_equal(point1<metres>(1), point1<metres>(2), point1<metres>(3));
@END
@ILLEGAL
    // `is_less_or_equal` tolerance must be a 1D measure.
    is_less_or_equal(point1<metres>(1), point1<metres>(2), vect2<metres>(3));
@END
@ILLEGAL
    // `is_less_or_equal` arguments to compare must be of the same kind.
    is_less_or_equal(point1<metres>(1), vect1<metres>(2), vect1<metres>(3));
@END

@ILLEGAL
    // Measure addition cannot mix units.
    point1<metres>(1) + vect1<inches>(2);
@END
@ILLEGAL
    // Measure increment has the same unit than operands.
    point1<inches>(point1<metres>(1) + vect1<metres>(2));
@END
@ILLEGAL
    // When a vectX is added to a pointX, a pointX results.
    vect1<metres>(point1<metres>(1) + vect1<metres>(2));
@END

@ILLEGAL
    // Measure subtraction cannot mix units.
    point1<metres>(1) - vect1<inches>(2);
@END
@ILLEGAL
    // Measure decrement has the same unit than operands.
    point1<inches>(point1<metres>(1) - vect1<metres>(2));
@END
@ILLEGAL
    // When a vectX is added to a pointX, a pointX results.
    vect1<metres>(point1<metres>(1) - vect1<metres>(2));
@END

@ILLEGAL
    // Measure addition cannot mix units.
    vect1<metres>(1) + vect1<inches>(2);
@END
@ILLEGAL
    // Measure addition has the same unit than operands.
    vect1<inches>(vect1<metres>(1) + vect1<metres>(2));
@END
@ILLEGAL
    // When a vectX is added to a vectX, a vectX results.
    point1<metres>(vect1<metres>(1) + vect1<metres>(2));
@END

@ILLEGAL
    // Measure subtraction cannot mix units.
    vect1<metres>(1) - vect1<inches>(2);
@END
@ILLEGAL
    // Measure subtraction has the same unit than operands.
    vect1<inches>(vect1<metres>(1) - vect1<metres>(2));
@END
@ILLEGAL
    // When a vectX is subtracted from to a vectX, a vectX results.
    point1<metres>(vect1<metres>(1) - vect1<metres>(2));
@END

@ILLEGAL
    // A number cannot by multiplied by a pointX.
    1.3 * point1<inches>(1);
@END
@ILLEGAL
    // When a number is multiplied by a vectX,
    // the result has the same unit than the operand.
    vect1<inches>(1.3 * vect1<metres>(1));
@END
@ILLEGAL
    // When a number is multiplied by a vectX, the result is a vectX.
    point1<metres>(1.3 * vect1<metres>(1));
@END

@ILLEGAL
    // A pointX cannot by multiplied by a number.
    point1<inches>(1) * 1.3;
@END
@ILLEGAL
    // When a vectX is multiplied by a number,
    // the result has the same unit than the operand.
    vect1<inches>(vect1<metres>(1) * 1.3);
@END
@ILLEGAL
    // When a vectX is multiplied by a number, the result is a vectX.
    point1<metres>(vect1<metres>(1) * 1.3);
@END

@ILLEGAL
    // A number cannot be divided by a vectX.
    1.3 / vect1<inches>(1);
@END
@ILLEGAL
    // A number cannot be divided by a pointX.
    1.3 / point1<inches>(1);
@END
@ILLEGAL
    // A pointX cannot be divided by a number.
    point1<inches>(1) / 1.3;
@END
@ILLEGAL
    // When a vectX is dividied by a number,
    // the result has the same unit than the operand.
    vect1<inches>(vect1<metres>(1) / 1.3));
@END
@ILLEGAL
    // When a vectX is dividied by a number, the result is a vectX.
    point1<metres>(vect1<metres>(1) / 1.3);
@END

@ILLEGAL
    // A pointX cannot be divided.
    point1<inches>(1) / point1<inches>(2);
@END
@ILLEGAL
    // A vectX cannot be divided by a pointX.
    vect1<inches>(1) / point1<inches>(2);
@END
@ILLEGAL
    // A pointX cannot be divided.
    point1<inches>(1) / vect1<inches>(2);
@END
@ILLEGAL
    // A vectX cannot be divided by a measure having a different unit.
    vect1<metres>(1) / vect1<inches>(2);
@END
@ILLEGAL
    // The result of a division between measures is a number, not a measure.
    vect1<metres> v = vect1<metres>(1) / vect1<metres>(2);
@END
@ILLEGAL
    // The result of a division between measures is a number, not a measure.
    point1<metres> p = vect1<metres>(1) / vect1<metres>(2);
@END

@ILLEGAL
    // The squared norm of a pointX cannot be computed.
    squared_norm_value(point1<metres>(1));
@END
@ILLEGAL
    // The squared norm is a number, not a measure.
    vect1<metres> v = squared_norm_value(vect1<metres>(1));
@END
@ILLEGAL
    // The squared norm is a number, not a measure.
    point1<metres> v = squared_norm_value(vect1<metres>(1));
@END
@ILLEGAL
    // The norm of a pointX cannot be computed.
    norm(point1<metres>(1));
@END
@ILLEGAL
    // The norm of a measure has the same unit.
    vect1<inches> p = norm(vect1<metres>(1));
@END
@ILLEGAL
    // The norm of a measure is a vectX, not a pointX.
    point1<metres> p = norm(vect1<metres>(1));
@END

@ILLEGAL
    // The norm of a measure is a vectX, not a pointX.
    point1<metres> p = norm(vect1<metres>(1));
		v2 = v3.rotated_by(vect1<radians,float>(n));
		v2 = v3.rotated_left();
		v2 = v3.rotated_right();
		p3.rotated_by_around(vect1<radians,float>(n), p2);
		p3.rotated_left_around(p2);
		p3.rotated_right_around(p2);
@END
@ILLEGAL
    // 1D measures cannot rotate.
    vect1<metres> v(1);
	v.rotated_by(vect1<radians>(n));
@END
@ILLEGAL
    // 1D measures cannot rotate.
    vect1<metres> v(1);
	v.rotated_left();
@END
@ILLEGAL
    // 1D measures cannot rotate.
    vect1<metres> v(1);
	v.rotated_right();
@END
@ILLEGAL
    // 1D measures cannot rotate.
    point1<metres> p(1);
    point2<metres> p2(2, 3);
	p.rotated_by_around(vect1<radians>(n), p2);
@END
@ILLEGAL
    // 1D measures cannot rotate.
    point1<metres> p(1);
    point2<metres> p2(2, 3);
	p.rotated_left_around(p2);
@END
@ILLEGAL
    // 1D measures cannot rotate.
    point1<metres> p(1);
    point2<metres> p2(2, 3);
	p.rotated_right_around(p2);
@END
}
