#include "measures.hpp"
using namespace measures;
	
DEFINE_ANGLE_UNIT(degrees, "^", 360, 0)
	
DEFINE_MAGNITUDE(Space, metres, " m")
DEFINE_MAGNITUDE(Area, square_metres, " m2")
DEFINE_MAGNITUDE(Force, newtons, " N")
DEFINE_MAGNITUDE(Energy, joules, " J")
DEFINE_MAGNITUDE(Time, seconds, " s")
DEFINE_MAGNITUDE(Speed, metres_per_second, " m/s")
DEFINE_MAGNITUDE(AngularSpeed, radians_per_second, " rad/s")
DEFINE_UNIT(inches, Space, " in", 0.0254, 0)
DEFINE_UNIT(square_inches, Area, " in2", 0.0254*0.0254, 0)
DEFINE_DERIVED_UNIT_SCALAR_SCALAR(seconds, radians_per_second, radians)
DEFINE_DERIVED_UNIT_SQUARED_SCALAR(inches, square_inches)
DEFINE_DERIVED_UNIT_SCALAR_VECTOR(seconds, metres_per_second, metres)
DEFINE_DERIVED_UNIT_VECTOR_VECTOR(newtons, metres, joules)
DEFINE_DERIVED_UNIT_SQUARED_VECTOR(metres, square_metres)
	
int main()
{	
@PERFECT
// Including all expressions using float.
	{
		float n;
		bool b;

		vect1<metres,float> v1;
		vect1<metres,float> v2(2.3f);
		vect1<metres,float> v3(v1);
		vect1<metres,float> v4(inches::id(), 3.4f);

		n = v3.value(inches::id());
		n = v3.value();
		v3.value() = n;
		v2 = +v1;
		v2 = -v1;
		v2 += v1;
		v2 -= v1;
		v2 *= n;
		v2 /= n;

		b = v1 == v2;
		b = v1 != v2;
		b = v1 < v2;
		b = v1 <= v2;
		b = v1 > v2;
		b = v1 >= v2;
		b = is_equal(v1, v2, v3);
		b = is_less(v1, v2, v3);
		b = is_less_or_equal(v1, v2, v3);
		
		point1<metres,float> p1;
		point1<metres,float> p2(1.2f);
		point1<metres,float> p3(p2);
		point1<metres,float> p4(inches::id(), 3.4f);
		point1<degrees,float> p5(signed_azimuth<degrees,float>(3.5f));
		point1<degrees,float> p6(unsigned_azimuth<degrees,float>(3.6f));
		
		n = p3.value(inches::id());
		n = p3.value();
		p3.value() = n;
		p2 += v1;
		p2 -= v1;

		p1 = midpoint(p1, p2);
		p2 = midpoint(p1, p2, 0.3f);
		point1<metres,float> pp[] = { p1, p2, p3 };
		float ww[] = { 0.2f, 0.3f, 0.5f };
		p1 = barycentric_combination(3, pp, ww);
		v2 = p1 - p2;
		b = p1 == p2;
		b = p1 != p2;
		b = p1 < p2;
		b = p1 <= p2;
		b = p1 > p2;
		b = p1 >= p2;
		b = is_equal(p1, p2, v1);
		b = is_less(p1, p2, v1);
		b = is_less_or_equal(p1, p2, v1);
		
		p2 = p1 + v1;
		p2 = p1 - v1;
		v2 = v1 + v1;
		v2 = v1 - v1;
		v2 = n * v1;
		v2 = v1 * n;
		v2 = v1 / n;
		n = v1 / v2;
		n = squared_norm_value(v1);
		v2 = norm(v1);
		
		if (b && n < 0 && v1.value() < 0 && v2.value() < 0 && v3.value() < 0
			&& v4.value() < 0) return 1;
	}
	
	{
		float n;
		float nn[2];
		bool b;
		vect1<metres,float> v1;
		point1<metres,float> p1;
		point1<radians,float> a1;
		signed_azimuth<radians,float> a2;
		unsigned_azimuth<radians,float> a3;
		
		vect2<metres,float> v2;
		vect2<metres,float> v3(2.3f, 2.4f);
		vect2<metres,float> v4(nn);
		vect2<metres,float> v5(v1, v1);
		vect2<metres,float> v6(v2);
		vect2<metres,float> v7(a1);
		vect2<metres,float> v8(a2);
		vect2<metres,float> v9(a3);
		vect2<metres,float> v10(inches::id(), 2.5f, 2.6f);

		n = v4.data()[0]; n = v4.data()[1];
		v4.data()[0] = n; v4.data()[1] = n;
		v1 = v4.x();
		v4.x() = v1;
		v1 = v4.y();
		v4.y() = v1;
		v2 = +v3;
		v2 = -v3;
		v2 += v3;
		v2 -= v3;
		v2 *= n;
		v2 /= n;
		v2 = v3.rotated_by(vect1<radians,float>(n));
		v2 = v3.rotated_left();
		v2 = v3.rotated_right();

		b = v2 == v3;
		b = v2 != v3;
		b = is_equal(v2, v3, v1);
		
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

		if (b && n < 0 && v1.value() < 0 && v2.x().value() < 0
			&& v3.x().value() < 0
			&& v4.x().value() < 0 && v5.x().value() < 0 && v6.x().value() < 0
			&& v7.x().value() < 0 && v8.x().value() < 0 && v9.x().value() < 0
			&& v10.x().value() < 0) return 1;
	}
	
	{
		float n;
		float nn[3];
		bool b;
		vect1<metres,float> v1;
		point1<metres,float> p1;
		
		vect3<metres,float> v2;
		vect3<metres,float> v3(2.3f, 2.4f, 2.5f);
		vect3<metres,float> v4(nn);
		vect3<metres,float> v5(v1, v1, v1);
		vect3<metres,float> v6(v2);
		vect3<metres,float> v10(inches::id(), 2.5f, 2.6f, 2.7f);

		n = v4.data()[0]; n = v4.data()[1]; n = v4.data()[2];
		v4.data()[0] = n; v4.data()[1] = n; v4.data()[2] = n;
		v1 = v4.x();
		v4.x() = v1;
		v1 = v4.y();
		v4.y() = v1;
		v1 = v4.z();
		v4.z() = v1;
		v2 = +v3;
		v2 = -v3;
		v2 += v3;
		v2 -= v3;
		v2 *= n;
		v2 /= n;

		b = v2 == v3;
		b = v2 != v3;
		b = is_equal(v2, v3, v1);	

		point3<metres,float> p2;
		point3<metres,float> p3(1.2f, 1.3f, 1.4f);
		point3<metres,float> p4(nn);
		point3<metres,float> p5(p1, p1, p1);
		point3<metres,float> p6(p2);
		point3<metres,float> p7(inches::id(), 6.2f, 6.3f, 6.4f);
		
		n = p4.data()[0]; n = p4.data()[1]; n = p4.data()[2];
		p4.data()[0] = n; p4.data()[1] = n; p4.data()[2] = n;
		p1 = p4.x();
		p4.x() = p1;
		p1 = p4.y();
		p4.y() = p1;
		p1 = p4.z();
		p4.z() = p1;
		p2 += v2;
		p2 -= v2;
		
		p2 = midpoint(p2, p3);
		p2 = midpoint(p2, p3, 0.3f);
		point3<metres,float> pp[] = { p2, p3, p4 };
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

		if (b && n < 0 && v1.value() < 0 && v2.x().value() < 0
			&& v3.x().value() < 0
			&& v4.x().value() < 0 && v5.x().value() < 0 && v6.x().value() < 0
			&& v10.x().value() < 0) return 1;
	}
	
	{
		point1<degrees,float> p1(5.8f);
		unsigned_azimuth<degrees, float> u1(8.8f);
		vect1<degrees,float> v1(7.2f);
		vect2<metres,float> v2(3.6f, 3.7f);
		float n;
		bool b;
		
		signed_azimuth<> a1;
		signed_azimuth<degrees> a2;
		signed_azimuth<degrees,float> a3(2.3f);
		signed_azimuth<degrees,float> a4(a3);
		signed_azimuth<degrees,float> a5(p1);
		signed_azimuth<degrees,float> a6(u1);
		signed_azimuth<degrees,float> a7(v2);
		signed_azimuth<degrees,float> a8(radians::id(), 3.5f);

		n = a3.value(radians::id());
		n = a3.value();
		a4 = a3 += v1;
		a4 = a3 -= v1;
	
		v1 = a3 - a4;
		b = a3 == a4;
		b = a3 != a4;
		b = is_equal(a3, a4, v1);
		v1 = angle_distance(a3, a4);
		a5 = a3 + v1;
		a5 = a3 - v1;
		n = sin(a3);
		n = cos(a3);
		n = tan(a3);

		if (b && n < 0 && v1.value() < 0 && v2.x().value() < 0) return 1;
	}

	{
		point1<degrees,float> p1(5.8f);
		signed_azimuth<degrees, float> s1(8.8f);
		vect1<degrees,float> v1(7.2f);
		vect2<metres,float> v2(3.6f, 3.7f);
		float n;
		bool b;
		
		unsigned_azimuth<> a1;
		unsigned_azimuth<degrees> a2;
		unsigned_azimuth<degrees,float> a3(2.3f);
		unsigned_azimuth<degrees,float> a4(a3);
		unsigned_azimuth<degrees,float> a5(p1);
		unsigned_azimuth<degrees,float> a6(s1);
		unsigned_azimuth<degrees,float> a7(v2);
		unsigned_azimuth<degrees,float> a8(radians::id(), 3.5f);

		n = a3.value(radians::id());
		n = a3.value();
		a4 = a3 += v1;
		a4 = a3 -= v1;
	
		v1 = a3 - a4;
		b = a3 == a4;
		b = a3 != a4;
		b = is_equal(a3, a4, v1);
		v1 = angle_distance(a3, a4);
		a5 = a3 + v1;
		a5 = a3 - v1;
		n = sin(a3);
		n = cos(a3);
		n = tan(a3);

		if (b && n < 0 && v1.value() < 0 && v2.x().value() < 0) return 1;
	}

	{
		vect1<degrees,float> v1 = convert<degrees>(
			vect1<radians,float>(2.3f));
		point1<degrees,float> v2 = convert<degrees>(
			point1<radians,float>(2.3f));
		vect2<degrees,float> v3 = convert<degrees>(
			vect2<radians,float>(2.3f, 2.4f));
		point2<degrees,float> v4 = convert<degrees>(
			point2<radians,float>(2.3f, 2.4f));
		vect3<degrees,float> v5 = convert<degrees>(
			vect3<radians,float>(2.3f, 2.4f, 2.5f));
		point3<degrees,float> v6 = convert<degrees>(
			point3<radians,float>(2.3f, 2.4f, 2.5f));
		signed_azimuth<degrees,float> v7 = convert<degrees>(
			signed_azimuth<radians,float>(2.3f));
		unsigned_azimuth<degrees,float> v8 = convert<degrees>(
			unsigned_azimuth<radians,float>(2.3f));

		if (v1.value() < 0 && v2.value() < 0
			&& v3.x().value() < 0
			&& v4.x().value() < 0 && v5.x().value() < 0 && v6.x().value() < 0
			&& v7.value() < 0 && v8.value() < 0) return 1;
	}

	{
		vect1<degrees,float> v1 = cast<float>(
			vect1<degrees,long double>(2.3f));
		point1<degrees,float> v2 = cast<float>(
			point1<degrees,long double>(2.3f));
		vect2<degrees,float> v3 = cast<float>(
			vect2<degrees,long double>(2.3f, 2.4f));
		point2<degrees,float> v4 = cast<float>(
			point2<degrees,long double>(2.3f, 2.4f));
		vect3<degrees,float> v5 = cast<float>(
			vect3<degrees,long double>(2.3f, 2.4f, 2.5f));
		point3<degrees,float> v6 = cast<float>(
			point3<degrees,long double>(2.3f, 2.4f, 2.5f));
		signed_azimuth<degrees,float> v7 = cast<float>(
			signed_azimuth<degrees,long double>(2.3f));
		unsigned_azimuth<degrees,float> v8 = cast<float>(
			unsigned_azimuth<degrees,long double>(2.3f));

		if (v1.value() < 0 && v2.value() < 0
			&& v3.x().value() < 0
			&& v4.x().value() < 0 && v5.x().value() < 0 && v6.x().value() < 0
			&& v7.value() < 0 && v8.value() < 0) return 1;
	}
	
	{ // SCALAR_SCALAR
		vect1<seconds,float> time1(1.1f);
		vect1<radians_per_second,float> angspeed1(1.2f);
		vect1<radians,float> angle1(1.3f);
		
		angle1 = time1 * angspeed1;
		angle1 = angspeed1 * time1;
		time1 = angle1 / angspeed1;
		angspeed1 = angle1 / time1;

		if (time1.value() < 0 && angspeed1.value() < 0
			&& angle1.value() < 0) return 1;
	}

	{ // SQUARED_SCALAR
		vect1<inches,float> space1(1.1f);
		vect1<square_inches,float> area1(1.1f);
		
		area1 = space1 * space1;
		area1 = sqr(space1);
		space1 = area1 / space1;
		space1 = sqrt(area1);

		if (space1.value() < 0 && area1.value() < 0) return 1;
	}

	{ // SCALAR_VECTOR
		vect1<seconds,float> time1(1.1f);
		vect1<metres,float> space1(2.2f);
		vect2<metres,float> space2(2.3f, 2.4f);
		vect3<metres,float> space3(2.5f, 2.6f, 2.7f);
		vect1<metres_per_second,float> speed1(3.1f);
		vect2<metres_per_second,float> speed2(3.2f, 3.3f);
		vect3<metres_per_second,float> speed3(3.4f, 3.5f, 3.6f);
		
		space1 = time1 * speed1;
		space1 = speed1 * time1;
		speed1 = space1 / time1;
		time1 = space1 / speed1;
		
		space2 = time1 * speed2;
		space2 = speed2 * time1;
		speed2 = space2 / time1;
		
		space3 = time1 * speed3;
		space3 = speed3 * time1;
		speed3 = space3 / time1;

		if (time1.value() < 0 && space1.value() < 0
			&& space2.x().value() < 0 && space3.x().value() < 0
			&& speed1.value() < 0 && speed2.x().value() < 0
			&& speed3.x().value() < 0) return 1;
	}

	{ // VECTOR_VECTOR
		vect1<newtons,float> force1(1.1f); 
		vect1<metres,float> space1(1.2f);
		vect1<joules,float> energy1(1.3f);
		vect2<newtons,float> force2(1.4f, 1.5f); 
		vect2<metres,float> space2(1.6f, 1.7f);
		vect3<newtons,float> force3(1.8f, 1.9f, 2.0f); 
		vect3<metres,float> space3(2.1f, 2.2f, 2.3f);
		vect3<joules,float> energy3(2.4f, 2.5f, 2.6f);
		
		energy1 = force1 * space1;
		energy1 = space1 * force1;
		force1 = energy1 / space1;
		space1 = energy1 / force1;
		
		energy1 = force2 * space2;
		energy1 = space2 * force2;
		energy1 = cross_product(force2, space2);
		energy1 = cross_product(space2, force2);
		
		energy1 = force3 * space3;
		energy1 = space3 * force3;
		energy3 = cross_product(force3, space3);
		energy3 = cross_product(space3, force3);

		if (force1.value() < 0 && space1.value() < 0
			&& energy1.value() < 0
			&& force2.x().value() < 0 && space2.x().value() < 0
			&& force3.x().value() < 0 && space3.x().value() < 0
			&& energy3.x().value() < 0) return 1;
	}
	
	{ // SQUARED_VECTOR
		vect1<metres,float> space1(1.1f);
		vect1<square_metres,float> area1(1.1f);
		vect2<metres,float> space2(1.1f, 1.2f);
		vect2<square_metres,float> area2(1.1f, 1.2f);
		vect3<metres,float> space3(1.1f, 1.2f, 1.3f);
		vect3<square_metres,float> area3(1.1f, 1.2f, 1.3f);
		
		area1 = space1 * space1;
		area1 = sqr(space1);
		space1 = area1 / space1;
		space1 = sqrt(area1);		
		
		area1 = space2 * space2;
		area1 = sqr(space2);
		area1 = cross_product(space2, space2);
		
		area1 = space3 * space3;
		area1 = sqr(space3);
		area3 = cross_product(space3, space3);

		if (space1.value() < 0 && area1.value() < 0
			&& space2.x().value() < 0 && area2.x().value() < 0
			&& space3.x().value() < 0 && area3.x().value() < 0) return 1;
	}
@END

@ALLOWED
// Including all expressions assigning a double to a float.
	{
		float n;
		double d = 5.6;
		bool b;

		vect1<metres,float> v1;
		vect1<metres> vd;
		vect1<metres,float> v2(2.3);
		vect1<metres,float> v3(vd);
		vect1<metres,float> v4(inches::id(), 3.4);

		n = vd.value(inches::id());
		n = vd.value();
		v4.value() = d;
		v2 = +vd;
		v2 = -vd;
		v2 += vd;
		v2 -= vd;
		v2 *= d;
		v2 /= d;

		b = v1 == vd;
		b = v1 != vd;
		b = v1 < vd;
		b = v1 <= vd;
		b = v1 > vd;
		b = v1 >= vd;
		b = is_equal(v1, vd, v3);
		b = is_equal(v1, v2, vd);
		b = is_less(v1, vd, v3);
		b = is_less(v1, v2, vd);
		b = is_less_or_equal(v1, vd, v3);
		b = is_less_or_equal(v1, v2, vd);
		
		point1<metres,float> p1;
		point1<metres> pd;
		point1<metres,float> p2(1.2);
		point1<metres,float> p3(pd);
		point1<metres,float> p4(inches::id(), 3.4);
		point1<degrees,float> p5(signed_azimuth<degrees>(3.5));
		point1<degrees,float> p6(unsigned_azimuth<degrees>(3.6));
		
		n = pd.value(inches::id());
		n = pd.value();
		p4.value() = d;
		p2 += vd;
		p2 -= vd;

		p1 = midpoint(p1, pd);
		p2 = midpoint(p1, pd, 0.3f);
		p2 = midpoint(p1, p2, 0.3);
		p2 = midpoint(p1, pd, 0.3);
		point1<metres> pp[] = { pd, pd, pd };
		float ww[] = { 0.2f, 0.3f, 0.5f };
		p1 = barycentric_combination(3, pp, ww);
		v2 = p1 - pd;
		b = p1 == pd;
		b = p1 != pd;
		b = p1 < pd;
		b = p1 <= pd;
		b = p1 > pd;
		b = p1 >= pd;
		b = is_equal(p1, pd, v1);
		b = is_less(p1, pd, v1);
		b = is_less_or_equal(p1, pd, v1);
		p2 = p1 + vd;
		p2 = p1 - vd;
		v2 = v1 + vd;
		v2 = v1 - vd;
		v2 = n * vd;
		v2 = v1 * d;
		v2 = v1 / d;
		n = v1 / vd;
		n = squared_norm_value(vd);
		v2 = norm(vd);
	}
	
	{
		float n;
		double d = 8.1;
		float nn[2];
		double dd[2];
		bool b;
		vect1<metres,float> v1;
		vect1<metres> v1d;
		point1<metres,float> p1;
		point1<metres> pd;
		point1<radians> a1;
		signed_azimuth<radians> a2;
		unsigned_azimuth<radians> a3;
		
		vect2<metres,float> v2;
		vect2<metres> v2d;
		vect2<metres,float> v3(2.3, 2.4);
		vect2<metres,float> v4(dd);
		vect2<metres,float> v5(v1d, v1d);
		vect2<metres,float> v6(v2d);
		vect2<metres,float> v7(a1);
		vect2<metres,float> v8(a2);
		vect2<metres,float> v9(a3);
		vect2<metres,float> v10(inches::id(), 2.5, 2.6);

		n = v2d.data()[0]; n = v2d.data()[1];
		v4.data()[0] = d; v4.data()[1] = d;
		v1 = v2d.x();
		v4.x() = v1d;
		v1 = v2d.y();
		v4.y() = v1d;
		v2 = +v2d;
		v2 = -v2d;
		v2 += v2d;
		v2 -= v2d;
		v2 *= d;
		v2 /= d;
		v2 = v3.rotated_by(vect1<radians>(n));
		v2 = v2d.rotated_by(vect1<radians>(n));
		v2 = v2d.rotated_left();
		v2 = v2d.rotated_right();

		b = v2 == v2d;
		b = v2 != v2d;
		b = is_equal(v2, v2d, v1);
		b = is_equal(v2, v3, v1d);
		
		point2<metres,float> p2;
		point2<metres> p2d;
		point2<metres,float> p3(1.2, 1.3);
		point2<metres,float> p4(dd);
		point2<metres,float> p5(pd, pd);
		point2<metres,float> p6(p2d);		
		point2<metres,float> p7(inches::id(), 6.2, 6.3);
		
		n = p2d.data()[0]; n = p2d.data()[1];
		p4.data()[0] = d; p4.data()[1] = d;
		p1 = p2d.x();
		p4.x() = pd;
		p1 = p2d.y();
		p4.y() = pd;
		p2 += v2d;
		p2 -= v2d;
		p3.rotated_by_around(vect1<radians>(d), p2);
		p3.rotated_by_around(vect1<radians,float>(n), p2d);
		p2d.rotated_by_around(vect1<radians,float>(n), p2);
		p3.rotated_left_around(p2d);
		p3.rotated_right_around(p2d);
		
		p2 = midpoint(p2, p2d);
		p2 = midpoint(p2, p2d, 0.3f);
		p2 = midpoint(p2, p3, 0.3);
		p2 = midpoint(p2, p2d, 0.3);
		point2<metres,float> ppf[] = { p2, p3, p4 };
		point2<metres> ppd[] = { p2d, p2d, p2d };
		float wwf[] = { 0.2f, 0.3f, 0.5f };
		double wwd[] = { 0.2, 0.3, 0.5 };
		p2 = barycentric_combination(3, ppf, wwd);
		p2 = barycentric_combination(3, ppd, wwf);
		p2 = barycentric_combination(3, ppd, wwd);
		v2 = p2 - p2d;
		b = p2 == p2d;
		b = p2 != p2d;
		b = is_equal(p2, p3, v1d);
		b = is_equal(p2, p2d, v1);
		b = is_equal(p2, p2d, v1d);
		p3 = p2 + v2d;
		p3 = p2 - v2d;
		v4 = v2 + v2d;
		v4 = v2 - v2d;
		v3 = n * v2d;
		v3 = d * v2;
		v3 = d * v2d;
		v3 = v2d * n;
		v3 = v2 * d;
		v3 = v2d * d;
		v3 = v2d / n;
		v3 = v2 / d;
		v3 = v2d / d;
		n = squared_norm_value(v2d);
		v1 = norm(v2d);	
	}
	
	{
		float n;
		double d = 8.1;
		float nn[2];
		double dd[2];
		bool b;
		vect1<metres,float> v1;
		vect1<metres> v1d;
		point1<metres,float> p1;
		point1<metres> pd;
		
		vect3<metres,float> v2;
		vect3<metres> v2d;
		vect3<metres,float> v3(2.3, 2.4, 2.5);
		vect3<metres,float> v4(dd);
		vect3<metres,float> v5(v1d, v1d, v1d);
		vect3<metres,float> v6(v2d);
		vect3<metres,float> v10(inches::id(), 2.5, 2.6, 2.7);

		n = v2d.data()[0]; n = v2d.data()[1]; n = v2d.data()[2];
		v4.data()[0] = d; v4.data()[1] = d; v4.data()[2] = d;
		v1 = v2d.x();
		v4.x() = v1d;
		v1 = v2d.y();
		v4.y() = v1d;
		v1 = v2d.z();
		v4.z() = v1d;
		v2 = +v2d;
		v2 = -v2d;
		v2 += v2d;
		v2 -= v2d;
		v2 *= d;
		v2 /= d;

		b = v2 == v2d;
		b = v2 != v2d;
		b = is_equal(v2, v2d, v1);	
		b = is_equal(v2, v3, v1d);	

		point3<metres,float> p2;
		point3<metres> p2d;
		point3<metres,float> p3(1.2, 1.3, 1.4);
		point3<metres,float> p4(dd);
		point3<metres,float> p5(pd, pd, pd);
		point3<metres,float> p6(p2d);
		point3<metres,float> p7(inches::id(), 6.2, 6.3, 6.4);

		n = p2d.data()[0]; n = p2d.data()[1]; n = p2d.data()[2];
		p4.data()[0] = d; p4.data()[1] = d; p4.data()[2] = d;
		p1 = p2d.x();
		p4.x() = pd;
		p1 = p2d.y();
		p4.y() = pd;
		p1 = p2d.z();
		p4.z() = pd;
		p2 += v2d;
		p2 -= v2d;
		
		p2 = midpoint(p2, p2d);
		p2 = midpoint(p2, p2d, 0.3f);
		p2 = midpoint(p2, p3, 0.3);
		p2 = midpoint(p2, p2d, 0.3);
		point3<metres,float> ppf[] = { p2, p3, p4 };
		point3<metres> ppd[] = { p2d, p2d, p2d };
		float wwf[] = { 0.2f, 0.3f, 0.5f };
		double wwd[] = { 0.2, 0.3, 0.5 };
		p2 = barycentric_combination(3, ppf, wwd);
		p2 = barycentric_combination(3, ppd, wwf);
		p2 = barycentric_combination(3, ppd, wwd);
		v2 = p2 - p2d;
		b = p2 == p2d;
		b = p2 != p2d;
		b = is_equal(p2, p3, v1d);
		b = is_equal(p2, p2d, v1);
		b = is_equal(p2, p2d, v1d);
		p3 = p2 + v2d;
		p3 = p2 - v2d;
		v4 = v2 + v2d;
		v4 = v2 - v2d;
		v3 = n * v2d;
		v3 = d * v2;
		v3 = d * v2d;
		v3 = v2d * n;
		v3 = v2 * d;
		v3 = v2d * d;
		v3 = v2d / n;
		v3 = v2 / d;
		v3 = v2d / d;
		n = squared_norm_value(v2d);
		v1 = norm(v2d);	
	}
	
	{
		point1<degrees> p1(5.8);
		unsigned_azimuth<degrees> u1(8.8);
		vect1<degrees,float> v1f(7.2f);
		vect1<degrees> v1d(7.2);
		vect2<metres> v2(3.6, 3.7);
		float n;
		float d;
		bool b;
		
		signed_azimuth<> a1;
		signed_azimuth<degrees> ad;
		signed_azimuth<degrees,float> a2;
		signed_azimuth<degrees,float> a3(2.3);
		signed_azimuth<degrees,float> a4(ad);
		signed_azimuth<degrees,float> a5(p1);
		signed_azimuth<degrees,float> a6(u1);
		signed_azimuth<degrees,float> a7(v2);
		signed_azimuth<degrees,float> a8(radians::id(), 3.5);

		n = ad.value(radians::id());
		n = ad.value();
		a4 = a3 += v1d;
		a4 = a3 -= v1d;
	
		v1f = a3 - ad;
		b = a3 == ad;
		b = a3 != ad;
		b = is_equal(a3, ad, v1f);
		b = is_equal(a3, a4, v1d);
		b = is_equal(a3, ad, v1d);
		v1f = angle_distance(a3, ad);
		a5 = a3 + v1d;
		a5 = a3 - v1d;
		n = sin(ad);
		n = cos(ad);
		n = tan(ad);
	}

	{
		point1<degrees> p1(5.8);
		signed_azimuth<degrees> s1(8.8);
		vect1<degrees,float> v1f(7.2f);
		vect1<degrees> v1d(7.2);
		vect2<metres> v2(3.6, 3.7);
		float n;
		float d;
		bool b;
		
		unsigned_azimuth<> a1;
		unsigned_azimuth<degrees> ad;
		unsigned_azimuth<degrees,float> a2;
		unsigned_azimuth<degrees,float> a3(2.3);
		unsigned_azimuth<degrees,float> a4(ad);
		unsigned_azimuth<degrees,float> a5(p1);
		unsigned_azimuth<degrees,float> a6(s1);
		unsigned_azimuth<degrees,float> a7(v2);
		unsigned_azimuth<degrees,float> a8(radians::id(), 3.5);

		n = ad.value(radians::id());
		n = ad.value();
		a4 = a3 += v1d;
		a4 = a3 -= v1d;
	
		v1f = a3 - ad;
		b = a3 == ad;
		b = a3 != ad;
		b = is_equal(a3, ad, v1f);
		b = is_equal(a3, a4, v1d);
		b = is_equal(a3, ad, v1d);
		v1f = angle_distance(a3, ad);
		a5 = a3 + v1d;
		a5 = a3 - v1d;
		n = sin(ad);
		n = cos(ad);
		n = tan(ad);
	}




	{
		vect1<degrees,float> v1 = convert<degrees>(
			vect1<radians>(2.3));
		point1<degrees,float> v2 = convert<degrees>(
			point1<radians>(2.3));
		vect2<degrees,float> v3 = convert<degrees>(
			vect2<radians>(2.3, 2.4));
		point2<degrees,float> v4 = convert<degrees>(
			point2<radians>(2.3, 2.4));
		vect3<degrees,float> v5 = convert<degrees>(
			vect3<radians>(2.3, 2.4, 2.5));
		point3<degrees,float> v6 = convert<degrees>(
			point3<radians>(2.3, 2.4, 2.5));
		signed_azimuth<degrees,float> v7 = convert<degrees>(
			signed_azimuth<radians>(2.3));
		unsigned_azimuth<degrees,float> v8 = convert<degrees>(
			unsigned_azimuth<radians>(2.3));
	}
	
	{ // SCALAR_SCALAR
		vect1<seconds,float> time1f(1.1f);
		vect1<radians_per_second,float> angspeed1f(1.2f);
		vect1<radians,float> angle1f(1.3f);

		vect1<seconds> time1d(1.1);
		vect1<radians_per_second> angspeed1d(1.2);
		vect1<radians> angle1d(1.3);
		
		
		angle1f = time1f * angspeed1d;
		angle1f = angspeed1f * time1d;
		time1f = angle1f / angspeed1d;
		angspeed1f = angle1f / time1d;
		
		angle1f = time1d * angspeed1f;
		angle1f = angspeed1d * time1f;
		time1f = angle1d / angspeed1f;
		angspeed1f = angle1d / time1f;
		
		angle1f = time1d * angspeed1d;
		angle1f = angspeed1d * time1d;
		time1f = angle1d / angspeed1d;
		angspeed1f = angle1d / time1d;
	}

	{ // SQUARED_SCALAR
		vect1<inches,float> space1f(1.1f);
		vect1<square_inches,float> area1f(1.1f);

		vect1<inches> space1d(1.1);
		vect1<square_inches> area1d(1.1);
		
		
		area1f = space1f * space1d;
		area1f = sqr(space1d);
		space1f = area1f / space1d;
		space1f = sqrt(area1d);
		
		area1f = space1d * space1f;
		space1f = area1d / space1f;
		
		area1f = space1d * space1d;
		space1f = area1d / space1d;
	}

	{ // SCALAR_VECTOR
		vect1<seconds,float> time1f(1.1f);
		vect1<metres,float> space1f(2.2f);
		vect2<metres,float> space2f(2.3f, 2.4f);
		vect3<metres,float> space3f(2.5f, 2.6f, 2.7f);
		vect1<metres_per_second,float> speed1f(3.1f);
		vect2<metres_per_second,float> speed2f(3.2f, 3.3f);
		vect3<metres_per_second,float> speed3f(3.4f, 3.5f, 3.6f);

		vect1<seconds,float> time1d(1.1);
		vect1<metres,float> space1d(2.2);
		vect2<metres,float> space2d(2.3, 2.4);
		vect3<metres,float> space3d(2.5, 2.6, 2.7);
		vect1<metres_per_second,float> speed1d(3.1);
		vect2<metres_per_second,float> speed2d(3.2, 3.3);
		vect3<metres_per_second,float> speed3d(3.4, 3.5, 3.6);
		
		
		space1f = time1f * speed1d;
		space1f = speed1f * time1d;
		speed1f = space1f / time1d;
		time1f = space1f / speed1d;
		
		space2f = time1f * speed2d;
		space2f = speed2f * time1d;
		speed2f = space2f / time1d;
		
		space3f = time1f * speed3d;
		space3f = speed3f * time1d;
		speed3f = space3f / time1d;

		
		space1f = time1d * speed1f;
		space1f = speed1d * time1f;
		speed1f = space1d / time1f;
		time1f = space1d / speed1f;
		
		space2f = time1d * speed2f;
		space2f = speed2d * time1f;
		speed2f = space2d / time1f;
		
		space3f = time1d * speed3f;
		space3f = speed3d * time1f;
		speed3f = space3d / time1f;
		
		
		space1f = time1d * speed1d;
		space1f = speed1d * time1d;
		speed1f = space1d / time1d;
		time1f = space1d / speed1d;
		
		space2f = time1d * speed2d;
		space2f = speed2d * time1d;
		speed2f = space2d / time1d;
		
		space3f = time1d * speed3d;
		space3f = speed3d * time1d;
		speed3f = space3d / time1d;
	}

	{ // VECTOR_VECTOR
		vect1<newtons,float> force1f(1.1f); 
		vect1<metres,float> space1f(1.2f);
		vect1<joules,float> energy1f(1.3f);
		vect2<newtons,float> force2f(1.4f, 1.5f); 
		vect2<metres,float> space2f(1.6f, 1.7f);
		vect3<newtons,float> force3f(1.8f, 1.9f, 2.0f); 
		vect3<metres,float> space3f(2.1f, 2.2f, 2.3f);
		vect3<joules,float> energy3f(2.4f, 2.5f, 2.6f);

		vect1<newtons> force1d(1.1);
		vect1<metres> space1d(1.2);
		vect1<joules> energy1d(1.3);
		vect2<newtons> force2d(1.4, 1.5); 
		vect2<metres> space2d(1.6, 1.7);
		vect3<newtons> force3d(1.8, 1.9, 2.0); 
		vect3<metres> space3d(2.1, 2.2, 2.3);
		vect3<joules> energy3d(2.4, 2.5, 2.6);
		
		
		energy1f = force1f * space1d;
		energy1f = space1f * force1d;
		force1f = energy1f / space1d;
		space1f = energy1f / force1d;
		
		energy1f = force2f * space2d;
		energy1f = space2f * force2d;
		energy1f = cross_product(force2f, space2d);
		energy1f = cross_product(space2f, force2d);
		
		energy1f = force3f * space3d;
		energy1f = space3f * force3d;
		energy3f = cross_product(force3f, space3d);
		energy3f = cross_product(space3f, force3d);
		
		
		energy1f = force1d * space1f;
		energy1f = space1d * force1f;
		force1f = energy1d / space1f;
		space1f = energy1d / force1f;
		
		energy1f = force2d * space2f;
		energy1f = space2d * force2f;
		energy1f = cross_product(force2d, space2f);
		energy1f = cross_product(space2d, force2f);
		
		energy1f = force3d * space3f;
		energy1f = space3d * force3f;
		energy3f = cross_product(force3d, space3f);
		energy3f = cross_product(space3d, force3f);
		
		
		energy1f = force1d * space1d;
		energy1f = space1d * force1d;
		force1f = energy1d / space1d;
		space1f = energy1d / force1d;
		
		energy1f = force2d * space2d;
		energy1f = space2d * force2d;
		energy1f = cross_product(force2d, space2d);
		energy1f = cross_product(space2d, force2d);
		
		energy1f = force3d * space3d;
		energy1f = space3d * force3d;
		energy3f = cross_product(force3d, space3d);
		energy3f = cross_product(space3d, force3d);
	}
	
	{ // SQUARED_VECTOR
		vect1<metres,float> space1f(1.1f);
		vect1<square_metres,float> area1f(1.1f);
		vect2<metres,float> space2f(1.1f, 1.2f);
		vect2<square_metres,float> area2f(1.1f, 1.2f);
		vect3<metres,float> space3f(1.1f, 1.2f, 1.3f);
		vect3<square_metres,float> area3f(1.1f, 1.2f, 1.3f);
		
		vect1<metres> space1d(1.1);
		vect1<square_metres> area1d(1.1);
		vect2<metres> space2d(1.1, 1.2);
		vect2<square_metres> area2d(1.1, 1.2);
		vect3<metres> space3d(1.1, 1.2, 1.3);
		vect3<square_metres> area3d(1.1, 1.2, 1.3);
		
		
		area1f = space1f * space1d;
		area1f = sqr(space1d);
		space1f = area1f / space1d;
		space1f = sqrt(area1d);		
		
		area1f = space2f * space2d;
		area1f = sqr(space2d);
		area1f = cross_product(space2f, space2d);
		
		area1f = space3f * space3d;
		area1f = sqr(space3d);
		area3f = cross_product(space3f, space3d);
		
		
		area1f = space1d * space1f;
		space1f = area1d / space1f;
		
		area1f = space2d * space2f;
		area1f = cross_product(space2d, space2f);
		
		area1f = space3d * space3f;
		area3f = cross_product(space3d, space3f);
		
		
		area1f = space1d * space1d;
		space1f = area1d / space1d;
		
		area1f = space2d * space2d;
		area1f = cross_product(space2d, space2d);
		
		area1f = space3d * space3d;
		area3f = cross_product(space3d, space3d);
	}
@END
}
