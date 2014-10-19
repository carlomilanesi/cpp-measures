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
@ILLEGAL
	vect1 v;
@END
@ILLEGAL
	vect1<> v;
@END
@ILLEGAL
	vect1<days> v;
@END
@ILLEGAL
	vect1<seconds,void> v;
@END
@ILLEGAL
	vect1<seconds> v(1.2, 2.3);
@END
@ILLEGAL
	vect1<seconds> v1(1.2);
    vect1<seconds> v2(v1, v1);
@END
@ILLEGAL
	vect1<metres> v1(1.2);
    vect1<inches> v2(2.3);
    v1 = v2;
@END
@ILLEGAL
	vect1<seconds> v(metres::id(), 3.4);
@END
@ILLEGAL
	vect1<seconds> v(seconds, 3.4);
@END
@ILLEGAL
	vect1<seconds> v(seconds(), 3.4);
@END
@ILLEGAL
	vect1<seconds> v(seconds::id, 3.4);
@END
@ILLEGAL
	vect1<seconds> v(seconds::id(), 3.4, 4.5);
@END

@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.value(metres::id());
@END
@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.value(seconds::id);
@END
@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.value(seconds());
@END
@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.value(seconds);
@END
@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.value;
@END
@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.x();
@END
@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.x;
@END
@ILLEGAL
	vect1<seconds> v1(4);
    double n = v1.x_;
@END
@ILLEGAL
	vect1<seconds> v1(4);
    v1.value = 3;
@END
@ILLEGAL
	vect1<seconds> v1(4);
    v1.x = 3;
@END
@ILLEGAL
	vect1<seconds> v1(4);
    v1.x_ = 3;
@END
@ILLEGAL
	vect1<seconds> v1(4);
    v1.x() = 3;
@END

@ILLEGAL
	vect1<metres> v1(4);
    v1 += vect1<inches>(3);
@END
@ILLEGAL
	vect1<metres> v1(4);
    v1 -= vect1<inches>(3);
@END
@ILLEGAL
	vect1<metres> v1(4);
    v1 += point1<metres>(3);
@END
@ILLEGAL
	vect1<metres> v1(4);
    v1 -= point1<metres>(3);
@END
@ILLEGAL
	vect1<metres> v1(4);
    v1 *= vect1<metres>(3);
@END
@ILLEGAL
	vect1<metres> v1(4);
    v1 /= vect1<metres>(3);
@END

@ILLEGAL
	vect1<metres>(4) == vect1<inches>(3);
@END
@ILLEGAL
	vect1<metres>(4) != vect1<inches>(3);
@END
@ILLEGAL
	vect1<metres>(4) < vect1<inches>(3);
@END
@ILLEGAL
	vect1<metres>(4) <= vect1<inches>(3);
@END
@ILLEGAL
	vect1<metres>(4) > vect1<inches>(3);
@END
@ILLEGAL
	vect1<metres>(4) >= vect1<inches>(3);
@END
@ILLEGAL
	is_equal(vect1<metres>(4), vect1<inches>(3), vect1<inches>(1));
@END
@ILLEGAL
	is_equal(vect1<metres>(4), vect1<inches>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_equal(vect1<metres>(4), vect1<metres>(3), vect1<inches>(1));
@END
@ILLEGAL
	is_equal(vect1<inches>(4), vect1<metres>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_equal(vect1<inches>(4), vect1<inches>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_equal(vect1<inches>(4), vect1<metres>(3), vect1<inches>(1));
@END

@ILLEGAL	
	is_less(vect1<metres>(4), vect1<inches>(3), vect1<inches>(1));
@END
@ILLEGAL
	is_less(vect1<metres>(4), vect1<inches>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_less(vect1<metres>(4), vect1<metres>(3), vect1<inches>(1));
@END
@ILLEGAL
	is_less(vect1<inches>(4), vect1<metres>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_less(vect1<inches>(4), vect1<inches>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_less(vect1<inches>(4), vect1<metres>(3), vect1<inches>(1));
@END

@ILLEGAL
	is_less_or_equal(vect1<metres>(4), vect1<inches>(3), vect1<inches>(1));
@END
@ILLEGAL
	is_less_or_equal(vect1<metres>(4), vect1<inches>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_less_or_equal(vect1<metres>(4), vect1<metres>(3), vect1<inches>(1));
@END
@ILLEGAL
	is_less_or_equal(vect1<inches>(4), vect1<metres>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_less_or_equal(vect1<inches>(4), vect1<inches>(3), vect1<metres>(1));
@END
@ILLEGAL
	is_less_or_equal(vect1<inches>(4), vect1<metres>(3), vect1<inches>(1));
@END

@ILLEGAL
	point1 p;
@END
@ILLEGAL
	point1<> p;
@END
@ILLEGAL
	point<days> p;
@END
@ILLEGAL
	point1<seconds,void> p;	
@END
@ILLEGAL
	point1<seconds> p(1.2, 2.3);
@END
@ILLEGAL
	point1<seconds> p1(1.2);
    point1<seconds> p2(p1, p1);
@END
@ILLEGAL
	point1<metres> p1(1.2);
    point1<inches> p2(2.3);
    p1 = p2;
@END
@ILLEGAL
	point1<seconds> p(metres::id(), 3.4);
@END
@ILLEGAL
	point1<seconds> p(seconds::id(), 3.4, 4.5);
@END
@ILLEGAL
	point1<seconds> p(seconds::id, 3.4);
@END
@ILLEGAL
	point1<seconds> p(seconds(), 3.4);
@END
@ILLEGAL
	point1<seconds> p(seconds, 3.4);
@END
@ILLEGAL
	point1<radians> p(signed_azimuth<degrees>(3.5);
@END
@ILLEGAL
	point1<radians> p(unsigned_azimuth<degrees>(3.5);
@END

@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.value(metres::id());
@END
@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.value(seconds::id);
@END
@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.value(seconds());
@END
@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.value(seconds);
@END
@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.value;
@END
@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.x();
@END
@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.x;
@END
@ILLEGAL
	point1<seconds> p1(4);
    double n = p1.x_;
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1.value = 3;
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1.x = 3;
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1.x_ = 3;
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1.x() = 3;
@END

@ILLEGAL
	point1<seconds> p1(4);
    p1 += point1<seconds>(5);
@END
@ILLEGAL
	point1<metres> p1(4);
    p1 += vect1<inches>(5);
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1 -= point1<seconds>(5);
@END
@ILLEGAL
	point1<metres> p1(4);
    p1 -= vect1<inches>(5);
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1 *= point1<seconds>(5);
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1 *= vect1<seconds>(5);
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1 /= point1<seconds>(5);
@END
@ILLEGAL
	point1<seconds> p1(4);
    p1 /= vect1<seconds>(5);
@END

@ILLEGAL	
	midpoint(vect1<metres>(3), vect1<metres>(4));
@END
@ILLEGAL
	midpoint(point1<metres>(3), vect1<metres>(4));
@END
@ILLEGAL
	midpoint(vect1<metres>(3), point1<metres>(4));
@END
@ILLEGAL
	midpoint(point1<metres>(3), point1<inches>(4));
@END
@ILLEGAL
	point1<inches> p(midpoint(point1<metres>(3), point1<metres>(4)));
@END
@ILLEGAL
	midpoint(vect1<metres>(3), vect1<metres>(4), 0.4);
@END
@ILLEGAL
	midpoint(point1<metres>(3), vect1<metres>(4), 0.4);
@END
@ILLEGAL
	midpoint(vect1<metres>(3), point1<metres>(4), 0.4);
@END
@ILLEGAL
	midpoint(point1<metres>(3), point1<inches>(4), 0.4);
@END
@ILLEGAL
	point1<inches> p(midpoint(point1<metres>(3), point1<metres>(4)), 0.4);
@END

@ILLEGAL    
    vect1<metres> v(0.4);
    vect1<metres,float> vv[] = { v, v, v };
	float ww[] = { 0.2f, 0.3f, 0.5f };
	barycentric_combination(3, vv, ww);
@END
@ILLEGAL    
    point1<metres> p(0.4);
    point1<metres,float> pp[] = { p, p, p };
	float ww[] = { 0.2f, 0.3f, 0.5f };
	vect1<metres> v(barycentric_combination(3, pp, ww));
@END

/*		v2 = p1 - p2;
		b = p1 == p2;
		b = p1 != p2;
		b = p1 < p2;
		b = p1 <= p2;
		b = p1 > p2;
		b = p1 >= p2;
		b = is_equal(p1, p2, v1);
		b = is_less(p1, p2, v1);
		b = is_less_or_equal(p1, p2, v1);
*/
}
