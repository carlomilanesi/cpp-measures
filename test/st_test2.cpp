#define MEASURES_USE_ALL
#include "measures.hpp"
using namespace measures;
using namespace std;

// Define alternative angle units.
MEASURES_ANGLE_UNIT(degrees, "^", 360, 0)
MEASURES_ANGLE_UNIT(gradians, " gon", 400, 0)
MEASURES_ANGLE_UNIT(turns, " rev", 1, 0)
	
// Define solid angles with three units.
MEASURES_MAGNITUDE(SolidAngle, steradians, " sr")
MEASURES_UNIT(allrounds, SolidAngle, " all-round",\
    4*3.1415926535897932384626433832795, 0)
MEASURES_UNIT(square_degrees, SolidAngle, "^^",\
    3.1415926535897932384626433832795/180*3.1415926535897932384626433832795/180, 0)
	
// Define unitless measures.
MEASURES_MAGNITUDE(Amount, units, "")
	
// Define space and time units and some of their derived units.
MEASURES_MAGNITUDE(Space, metres, " m")
MEASURES_UNIT(inches, Space, "\"", 0.0254, 0)
MEASURES_UNIT(mm, Space, " mm", 0.001, 0)
MEASURES_UNIT(km, Space, " Km", 1000, 0)

MEASURES_MAGNITUDE(Time, seconds, " s")
MEASURES_UNIT(minutes, Time, " min", 60, 0)
MEASURES_UNIT(hours, Time, " h", 3600, 0)

MEASURES_MAGNITUDE(Speed, metres_per_second, " m/s")
MEASURES_UNIT(km_per_hour, Speed, " Km/h", 1/3.6, 0)

MEASURES_MAGNITUDE(Acceleration, metres_per_squared_second, " m/s2")
MEASURES_UNIT(km_per_hours_per_second, Acceleration, " Km/h/s", 1/3.6, 0)

MEASURES_MAGNITUDE(SquaredTime, squared_seconds, " s2")

// Define the combined operations among the defined units.
MEASURES_DERIVED_SQ_1(seconds, squared_seconds)
MEASURES_DERIVED_1_3(seconds, metres_per_second, metres)
MEASURES_DERIVED_1_3(seconds, metres_per_squared_second, metres_per_second)
MEASURES_DERIVED_1_3(squared_seconds, metres_per_squared_second, metres)

// Define the units derived from time and angle units.
MEASURES_MAGNITUDE(AngularSpeed, radians_per_second, " rad/s")
MEASURES_UNIT(turns_per_minute, AngularSpeed, " rpm", 60 / (2 * 3.14159265358979323846), 0)
MEASURES_MAGNITUDE(AngularAcceleration, radians_per_squared_second, " rad/s2")

// Define the combined operations among angle and derived units.
MEASURES_DERIVED_1_1(seconds, radians_per_second, Angle)
MEASURES_DERIVED_1_1(seconds, radians_per_squared_second, radians_per_second)
MEASURES_DERIVED_1_1(squared_seconds, radians_per_squared_second, radians)

// Define unitless combined operations.
MEASURES_DERIVED_SQ_3(units, units)

// Define vector combined operations.
MEASURES_MAGNITUDE(Force, newtons, " N")
MEASURES_MAGNITUDE(Energy, joules, " J")
MEASURES_DERIVED_3_3(newtons, metres, joules)
	
MEASURES_DERIVED_SQ_1(radians, steradians)
	
int main()
{
// vect2 construction.
@ILLEGAL
    // Measures are templates.
    vect2 v;
@END
@ILLEGAL
    // Measures need a unit.
    vect2<> v;
@END
@ILLEGAL
    // Measures need a defined unit.
    vect2<days> v;
@END
@ILLEGAL
    // Measures need a numeric type, i.e. capable of arithmetics.
    vect2<seconds,void> v;
@END
@ILLEGAL
    // 2D measures cannot be initialized using 1 number.
    vect2<seconds> v(2);
@END
@ILLEGAL
    // 2D measures cannot be initialized using 3 numbers.
    vect2<seconds> v(2, 3, 4);
@END
@ILLEGAL
    // 2D measures cannot be initialized using 1 1D measure.
    vect1<seconds> v1(1);
    vect2<seconds> v2(v1);
@END
@ILLEGAL
    // 2D measures cannot be initialized using 3 1D measures.
    vect1<seconds> v1(1);
    vect2<seconds> v2(v1, v1, v1);
@END
@ILLEGAL
    // 2D measures cannot be initialized using 2 2D measures.
    vect2<seconds> v1(1, 2);
    vect2<seconds> v2(v1, v1);
@END
@ILLEGAL
    // 2D measures cannot be initialized using 3 2D measures.
    vect2<seconds> v1(1, 2);
    vect2<seconds> v2(v1, v1, v1);
@END
@ILLEGAL
    // A vect cannot be initialized using a point.
    point2<seconds> v1(1, 2);
    vect2<seconds> v2(v1);
@END
@ILLEGAL
    // Measure assignment cannot mix units.
	vect2<metres> v1(1, 2);
    vect2<inches> v2(2, 3);
    v1 = v2;
@END
@ILLEGAL
    // A point angle cannot be used to construct a vect2.
	point1<degrees> a;
	vect2<degrees> v7(a);
@END
@ILLEGAL
    // An azimuth cannot be used to construct a vect2.
	signed_azimuth<degrees> a;
	vect2<degrees> v7(a);
@END
@ILLEGAL
    // An azimuth cannot be used to construct a vect2.
	unsigned_azimuth<degrees> a;
	vect2<degrees> v7(a);
@END

// vect2 construction with conversion.
@ILLEGAL
    // Units are classes, not values.
	vect2<seconds> v(seconds, 1, 2);
@END
@ILLEGAL
    // To specify a unit, do not instantiate it,
    // but use its `id` member function.
	vect2<seconds> v(seconds(), 1, 2);
@END
@ILLEGAL
    // To specify a unit, don't use the address of its `id` member function,
    // but call it.
	vect2<seconds> v(seconds::id, 1, 2);
@END
@ILLEGAL
    // A measure cannot be constructed by using only a unit.
	vect2<metres> v(inches::id());
@END
@ILLEGAL
    // A 2D measure cannot be constructed by using a unit and 1 number.
	vect2<metres> v(inches::id(), 1);
@END
@ILLEGAL
    // A 2D measure cannot be constructed by using a unit and 3 numbers.
	vect2<metres> v(inches::id(), 1, 2, 3);
@END
@ILLEGAL
    // A measures cannot be constructed by converting
    // a unit of a different magnitude.
	vect2<metres> v(seconds::id(), 1, 2);
@END

// make_unit_vector function.
@ILLEGAL
    // `make_unit_vector` is a static member function.
    vect2<inches>(1.2).make_unit_vector(point1<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` is a static member function.
    vect2<inches>(1.2).make_unit_vector(signed_azimuth<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` is a static member function.
    vect2<inches>(1.2).make_unit_vector(unsigned_azimuth<degrees>(1.2));
@END

@ILLEGAL
    // `make_unit_vector` does not belong to 1D measure classes.
    vect1<inches>::make_unit_vector(point1<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` does not belong to 1D measure classes.
    vect1<inches>::make_unit_vector(signed_azimuth<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` does not belong to 1D measure classes.
    vect1<inches>::make_unit_vector(unsigned_azimuth<degrees>(1.2));
@END

@ILLEGAL
    // `make_unit_vector` does not belong to 3D measure classes.
    vect3<inches>::make_unit_vector(point1<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` does not belong to 3D measure classes.
    vect3<inches>::make_unit_vector(signed_azimuth<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` does not belong to 3D measure classes.
    vect3<inches>::make_unit_vector(unsigned_azimuth<degrees>(1.2));
@END

@ILLEGAL
    // `make_unit_vector` does not belong to pointX classes.
    point2<inches>::make_unit_vector(point1<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` does not belong to pointX classes.
    point2<inches>::make_unit_vector(signed_azimuth<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` does not belong to pointX classes.
    point2<inches>::make_unit_vector(unsigned_azimuth<degrees>(1.2));
@END

@ILLEGAL
    // `make_unit_vector` cannot be passed a vectX.
    vect2<inches>::make_unit_vector(vect1<degrees>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` cannot be passed a 2D measure.
    vect2<inches>::make_unit_vector(point2<degrees>(1.2, 2.3));
@END
@ILLEGAL
    // `make_unit_vector` cannot be passed a 3D measure.
    vect2<inches>::make_unit_vector(point3<degrees>(1.2, 2.3, 3.4));
@END

@ILLEGAL
    // `make_unit_vector` cannot be passed a measure having
    // a non-angle unit.
    vect2<inches>::make_unit_vector(point1<inches>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` cannot be passed a measure having
    // a non-angle unit.
    vect2<inches>::make_unit_vector(signed_azimuth<inches>(1.2));
@END
@ILLEGAL
    // `make_unit_vector` cannot be passed a measure having
    // a non-angle unit.
    vect2<inches>::make_unit_vector(unsigned_azimuth<inches>(1.2));
@END

@ILLEGAL  
    // `make_unit_vector` does not return a 1D measure.
    vect1<inches> v12(vect2<inches,float>::make_unit_vector(point1<degrees>(1.2)));
@END
@ILLEGAL
    // `make_unit_vector` does not return a 1D measure.
    vect1<inches> v13(vect2<inches,float>::make_unit_vector(signed_azimuth<degrees>(1.2)));
@END
@ILLEGAL
    // `make_unit_vector` does not return a 1D measure.
    vect1<inches> v14(vect2<inches,float>::make_unit_vector(unsigned_azimuth<degrees>(1.2)));
@END

@ILLEGAL
    // `make_unit_vector` does not return a pointX.
    point2<inches> v12(vect2<inches,float>::make_unit_vector(point1<degrees>(1.2)));
@END
@ILLEGAL
    // `make_unit_vector` does not return a pointX.
    point2<inches> v13(vect2<inches,float>::make_unit_vector(signed_azimuth<degrees>(1.2)));
@END
@ILLEGAL
    // `make_unit_vector` does not return a pointX.
    point2<inches> v14(vect2<inches,float>::make_unit_vector(unsigned_azimuth<degrees>(1.2)));
@END

@ILLEGAL
    // When `make_unit_vector` is called from a measure in inches,
    // it does not return a measure in metres.
    vect2<metres> v12(vect2<inches,float>::make_unit_vector(point1<degrees>(1.2)));
@END
@ILLEGAL
    // When `make_unit_vector` is called from a measure in inches,
    // it does not return a measure in metres.
    vect2<metres> v13(vect2<inches,float>::make_unit_vector(signed_azimuth<degrees>(1.2)));
@END
@ILLEGAL
    // When `make_unit_vector` is called from a measure in inches,
    // it does not return a measure in metres.
    vect2<metres> v14(vect2<inches,float>::make_unit_vector(unsigned_azimuth<degrees>(1.2)));
@END

// vect2 access with conversion.
@ILLEGAL
    // vect2 has no `value` function.
	vect2<metres> v(1, 2);
    v.value(inches::id());
@END

// vect2 access to x component.
@ILLEGAL
    // `x` returns a 1D measure.
    vect2<metres> v(1, 2);
	v = vect2<metres>(3, 4).x();
@END
@ILLEGAL
    // `x` returns a measure having the same unit of the object.
    vect1<inches> v(1);
	v = vect2<metres>(2, 3).x();
@END
@ILLEGAL
    // `x` returns a measure, not a number.
    double d;
	d = vect2<metres>(1, 2).x();
@END
@ILLEGAL
    // `x` returns an l-value that cannot receive a 2D measure.
    vect2<metres> v(1, 2);
	v.x() = vect2<metres>(3, 4);
@END
@ILLEGAL
    // `x` returns an l-value that cannot receive a measure
    // having a different unit.
    vect2<metres> v(1, 2);
	v.x() = vect1<inches>(3);
@END
@ILLEGAL
    // `x` returns an l-value that cannot receive a number.
    vect2<metres> v(1, 2);
	v.x() = 4;
@END

// vect2 access to y component.
@ILLEGAL
    // `y` returns a 1D measure.
    vect2<metres> v(1, 2);
	v = vect2<metres>(3, 4).y();
@END
@ILLEGAL
    // `y` returns a measure having the same unit of the object.
    vect1<inches> v(1);
	v = vect2<metres>(2, 3).y();
@END
@ILLEGAL
    // `y` returns a measure, not a number.
    double d;
	d = vect2<metres>(1, 2).y();
@END
@ILLEGAL
    // `y` returns an l-value that cannot receive a 2D measure.
    vect2<metres> v(1, 2);
	v.y() = vect2<metres>(3, 4));
@END
@ILLEGAL
    // `y` returns an l-value that cannot receive a measure
    // having a different unit.
    vect2<metres> v(1, 2);
	v.y() = vect1<inches>(3);
@END
@ILLEGAL
    // `y` returns an l-value that cannot receive a number.
    vect2<metres> v(1, 2);
	v.y() = 4;
@END

// vect2 other access.
@ILLEGAL
    // vect2 has no `value` function.
	vect2<metres> v(1, 2);
    v.value();
@END
@ILLEGAL
    // 2D-measures have no `z` function.
    vect2<metres> v(1, 2);
	v.z();
@END

@ILLEGAL
    // A vectX cannot be incremented by a pointX.
    vect2<metres> v(1, 2);
	v += point2<metres>(3, 4);
@END
@ILLEGAL
    // 2D-measures cannot be incremented by a 1D-measure.
    vect2<metres> v(1, 2);
	v += vect1<metres>(3);
@END
@ILLEGAL
    // 2D-measures cannot be incremented by a 3D-measure.
    vect2<metres> v(1, 2);
	v += vect3<metres>(3, 4, 5);
@END
@ILLEGAL
    // Measures cannot be incremented by a measure in a different unit.
    vect2<metres> v(1, 2);
	v += vect2<inches>(3, 4);
@END

@ILLEGAL
    // A vectX cannot be decremented by a pointX.
    vect2<metres> v(1, 2);
	v -= point2<metres>(3, 4);
@END
@ILLEGAL
    // 2D-measures cannot be decremented by a 1D-measure.
    vect2<metres> v(1, 2);
	v -= vect1<metres>(3);
@END
@ILLEGAL
    // 2D-measures cannot be decremented by a 3D-measure.
    vect2<metres> v(1, 2);
	v -= vect3<metres>(3, 4, 5);
@END
@ILLEGAL
    // Measures cannot be decremented by a measure in a different unit.
    vect2<metres> v(1, 2);
	v -= vect2<inches>(3, 4);
@END

@ILLEGAL
    // Measures cannot be scaled up by a measure.
    vect2<metres> v(1, 2);
	v *= vect1<metres>(3);
@END
@ILLEGAL
    // Measures cannot be scaled up by a measure.
    vect2<metres> v(1, 2);
	v *= vect2<metres>(3, 4);
@END
@ILLEGAL
    // Measures cannot be scaled down by a measure.
    vect2<metres> v(1, 2);
	v /= vect1<metres>(3);
@END
@ILLEGAL
    // Measures cannot be scaled down by a measure.
    vect2<metres> v(1, 2);
	v /= vect2<metres>(3, 4);
@END

@ILLEGAL
    // point can't rotate.
    point2<metres> v;
	v.rotated_by(vect1<radians>());
@END
@ILLEGAL
    // can't rotate by point
    vect2<metres> v;
	v.rotated_by(point1<radians>());
@END
@ILLEGAL
    // can't rotate by vect2
    vect2<metres> v;
	v.rotated_by(vect2<radians>());
@END
@ILLEGAL
    // can't rotate by metres.
    vect2<metres> v;
	v.rotated_by(vect1<metres>());
@END

@ILLEGAL
    // point can't rotate.
    point2<metres> v;
	v.rotated_right();
@END
@ILLEGAL
    // point can't rotate.
    point2<metres> v;
	v.rotated_left();
@END

// vect2 comparisons.
@ILLEGAL
    // A vect1 cannot be compared with a vect2.
	vect1<metres>() == vect2<metres>();
@END
@ILLEGAL
    // A vect1 cannot be compared with a vect3.
	vect1<metres>() == vect3<metres>();
@END
@ILLEGAL
    
	vect2<metres>() == vect2<inches>();
@END
@ILLEGAL
	vect2<metres>() != vect1<metres>();
@END
@ILLEGAL
	vect2<metres>() != vect2<inches>();
@END

@ILLEGAL
    // `is_equal` requires a tolerance.
	is_equal(vect2<metres>(), vect2<metres>());
@END
@ILLEGAL
    // The tolerance must by a 1D measure.
	is_equal(vect2<metres>(), vect2<metres>(), vect2<metres>());
@END
@ILLEGAL
    // The tolerance must have the same unit.
	is_equal(vect2<metres>(), vect2<metres>(), vect1<inches>());
@END
@ILLEGAL
    // The measures to compare must have the same unit.
	is_equal(vect2<metres>(), vect2<inches>(), vect1<metres>());

/*

		point2<metres,float> p2;
		point2<metres,float> p3(1.2f, 1.3f);
		point2<metres,float> p4(nn);
		point2<metres,float> p5(p1, p1);
		point2<metres,float> p6(p2);
		
		point2<metres,float> p7(inches::id(), 6.2f, 6.3f);
		n = p4.data()[0]; n = p4.data()[1];
		p4.data()[0] = n; p4.data()[1] = n;
		p1 = p4.x();
		p4.x() = p1;
		p1 = p4.y();
		p4.y() = p1;
		p2 += v2;
		p2 -= v2;
		p3.rotated_by_around(vect1<radians,float>(n), p2);
		p3.rotated_left_around(p2);
		p3.rotated_right_around(p2);
		
		p2 = midpoint(p2, p3);
		p2 = midpoint(p2, p3, 0.3f);
		point2<metres,float> pp[] = { p2, p3, p4 };
		float ww[] = { 0.2f, 0.3f, 0.5f };
		p2 = barycentric_combination(3, pp, ww);
		v2 = p2 - p3;
		b = p2 == p3;
		b = p2 != p3;
		b = is_equal(p2, p3, v1);
		p3 = p2 + v2;
		p3 = p2 - v2;
		v4 = v2 + v3;
		v4 = v2 - v3;
		v3 = n * v2;
		v3 = v2 * n;
		v3 = v2 / n;
		n = squared_norm_value(v2);
		v1 = norm(v2);	
*/
@END
}
