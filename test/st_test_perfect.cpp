#define MEASURES_USE_ALL
#include "measures.hpp"
using namespace measures;
	
MEASURES_ANGLE_UNIT(degrees, "^", 360, 0)
	
MEASURES_MAGNITUDE(Space, metres, " m")
MEASURES_MAGNITUDE(Area, square_metres, " m2")
MEASURES_MAGNITUDE(Force, newtons, " N")
MEASURES_MAGNITUDE(Energy, joules, " J")
MEASURES_MAGNITUDE(Torque, newton_metres, " Nm")
MEASURES_MAGNITUDE(Time, seconds, " s")
MEASURES_MAGNITUDE(Speed, metres_per_second, " m/s")
MEASURES_MAGNITUDE(AngularSpeed, radians_per_second, " rad/s")
MEASURES_UNIT(inches, Space, " in", 0.0254, 0)
MEASURES_UNIT(square_inches, Area, " in2", 0.0254*0.0254, 0)
MEASURES_DERIVED_1_1(seconds, radians_per_second, radians)
MEASURES_DERIVED_SQ_1(inches, square_inches)
MEASURES_DERIVED_1_3_ALL(seconds, metres_per_second, metres)
MEASURES_DERIVED_3_3_ALL(newtons, metres, joules, newton_metres)
MEASURES_DERIVED_SQ_3_ALL(metres, square_metres, square_metres)

int main()
{
    {
		float n;
		bool b;

		vect1<metres,float> v1;
		vect1<metres,float> v2(2.3f);
		vect1<metres,float> v3(v1);
		vect1<metres,float> v4(inches::id(), 3.4f);
        auto v5 = vect1<metres>();

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
		p1 = barycentric_combination(30000, pp, ww);
		p1 = barycentric_combination(-5, pp, ww);
		p1 = barycentric_combination(0,
            static_cast<point1<metres,float>*>(nullptr),
            static_cast<float*>(nullptr));
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
			&& v4.value() < 0 && v5.value() < 0) return 1;
	}

    { // 2D linear transformations
        linear_map2<float> lm10
            = linear_map2<float>::rotation(vect1<degrees,float>(30.f));
        linear_map2<float> lm20
            = linear_map2<float>::projection(point1<degrees,float>(30.f));
        linear_map2<float> lm21
            = linear_map2<float>::projection(
            signed_azimuth<degrees,float>(30.f));
        linear_map2<float> lm22
            = linear_map2<float>::projection(
            unsigned_azimuth<degrees,float>(30.f));
        linear_map2<float> lm23
            = linear_map2<float>::projection(vect2<metres,float>(11.f, 12.f));
        linear_map2<float> lm30
            = linear_map2<float>::reflection(point1<degrees,float>(30.f));
        linear_map2<float> lm31
            = linear_map2<float>::reflection(
            signed_azimuth<degrees,float>(30.f));
        linear_map2<float> lm32
            = linear_map2<float>::reflection(
            unsigned_azimuth<degrees,float>(30.f));
        linear_map2<float> lm33
            = linear_map2<float>::reflection(vect2<metres,float>(11.f, 12.f));
        linear_map2<float> lm40
            = linear_map2<float>::scaling(3.f, 4.f);
        linear_map2<float> lm41 = lm10.inverted();
        linear_map2<float> lm42 = combine(combine(combine(lm10, lm20),
            combine(lm30, lm31)), lm40);
        float a1 = lm10.coeff(0, 0);
        float a2 = lm10.coeff(0, 1);
        float a3 = lm10.coeff(1, 0);
        float a4 = lm10.coeff(1, 1);
        lm10.coeff(0, 0) = a4;
        lm10.coeff(0, 1) = a3;
        lm10.coeff(1, 0) = a2;
        lm10.coeff(1, 1) = a1;

		if (lm10.coeff(0, 0) < 0
            || lm20.coeff(0, 0) < 0
            || lm21.coeff(0, 0) < 0
            || lm22.coeff(0, 0) < 0
            || lm23.coeff(0, 0) < 0
            || lm30.coeff(0, 0) < 0
            || lm31.coeff(0, 0) < 0
            || lm32.coeff(0, 0) < 0
            || lm33.coeff(0, 0) < 0
            || lm40.coeff(0, 0) < 0
            || lm41.coeff(0, 0) < 0
            || lm42.coeff(0, 0) < 0
            ) return 1;
    }

    { // 2D affine transformations
        point2<metres,float> fp(23.f, 24.f);
        affine_map2<metres,float> am10 = affine_map2<metres,float>
            ::translation(vect2<metres,float>(11.f, 12.f));
        affine_map2<metres,float> am20 = affine_map2<metres,float>::rotation(
            fp, vect1<degrees,float>(30.f));
        affine_map2<metres,float> am30
            = affine_map2<metres,float>::projection(
            fp, point1<degrees,float>(30.f));
        affine_map2<metres,float> am31 = affine_map2<metres,float>::projection(
            fp, signed_azimuth<degrees,float>(30.f));
        affine_map2<metres,float> am32 = affine_map2<metres,float>::projection(
            fp, unsigned_azimuth<degrees,float>(30.f));
        affine_map2<metres,float> am33 = affine_map2<metres,float>::projection(
            fp, vect2<metres,float>(11.f, 12.f));
        affine_map2<metres,float> am40 = affine_map2<metres,float>::reflection(
            fp, point1<degrees,float>(30.f));
        affine_map2<metres,float> am41 = affine_map2<metres,float>::reflection(
            fp, signed_azimuth<degrees,float>(30.f));
        affine_map2<metres,float> am42 = affine_map2<metres,float>::
            reflection(fp, unsigned_azimuth<degrees,float>(30.f));
        affine_map2<metres,float> am43 = affine_map2<metres,float>
            ::reflection(fp, vect2<metres,float>(11.f, 12.f));
        affine_map2<metres,float> am50 = affine_map2<metres,float>::scaling(
            fp, 3.f, 4.f);
        affine_map2<metres,float> am60 = am10.inverted();
        affine_map2<metres,float> am61 = combine(combine(combine(am10, am20),
            combine(am30, am40)), am50);
        float a1 = am10.coeff(0, 0);
        float a2 = am10.coeff(0, 1);
        float a3 = am10.coeff(0, 2);
        float a4 = am10.coeff(1, 0);
        float a5 = am10.coeff(1, 1);
        float a6 = am10.coeff(1, 2);
        am10.coeff(0, 0) = a6;
        am10.coeff(0, 1) = a5;
        am10.coeff(0, 2) = a4;
        am10.coeff(1, 0) = a3;
        am10.coeff(1, 1) = a2;
        am10.coeff(1, 2) = a1;

		if (am10.coeff(0, 0) < 0
            || am20.coeff(0, 0) < 0
            || am30.coeff(0, 0) < 0
            || am31.coeff(0, 0) < 0
            || am32.coeff(0, 0) < 0
            || am33.coeff(0, 0) < 0
            || am40.coeff(0, 0) < 0
            || am41.coeff(0, 0) < 0
            || am42.coeff(0, 0) < 0
            || am43.coeff(0, 0) < 0
            || am50.coeff(0, 0) < 0
            || am60.coeff(0, 0) < 0
            || am61.coeff(0, 0) < 0
            ) return 1;
    }

	{
		float n;
		float nn[2];
		double dd[2];
		bool b;
		vect1<metres,float> v1;
		point1<metres,float> p1;
		
		vect2<metres,float> v2;
		vect2<metres,float> v3(2.3f, 2.4f);
		vect2<metres,float> v4(nn);
		vect2<metres,double> v4b(nn);
		vect2<metres,double> v4c(dd);
		vect2<metres,float> v5(v1, v1);
		vect2<metres,float> v6(v2);
		vect2<metres,float> v10(inches::id(), 2.5f, 2.6f);
        auto v11 = vect2<metres>();

        vect2<inches> v12(vect2<inches,float>::make_unit_vector(point1<degrees>(1.2)));
        vect2<inches> v13(vect2<inches,float>::make_unit_vector(signed_azimuth<degrees>(1.2)));
        vect2<inches> v14(vect2<inches,float>::make_unit_vector(unsigned_azimuth<degrees>(1.2)));

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
		v2 = v3.mapped_by(linear_map2<float>::rotation(
            vect1<radians,float>(n)));
		v2 = v3.mapped_by(linear_map2<float>::rotation_at_left());
		v2 = v3.mapped_by(linear_map2<float>::rotation_at_right());

		b = v2 == v3;
		b = v2 != v3;
		b = is_equal(v2, v3, v1);
		
		point2<metres,float> p2;
		point2<metres,float> p3(1.2f, 1.3f);
		point2<metres,float> p4(nn);
		point2<metres,double> p4b(nn);
		point2<metres,double> p4c(dd);
		point2<metres,float> p5(p1, p1);
		point2<metres,float> p6(p2);
        auto p7 = point2<metres>();

		point2<metres,float> p8(inches::id(), 6.2f, 6.3f);
		n = p4.data()[0]; n = p4.data()[1];
		p4.data()[0] = n; p4.data()[1] = n;
		p1 = p4.x();
		p4.x() = p1;
		p1 = p4.y();
		p4.y() = p1;
		p2 += v2;
		p2 -= v2;
		p3.mapped_by(affine_map2<metres,float>::rotation(p2, vect1<radians,float>(n)));
		p3.mapped_by(affine_map2<metres,float>::rotation_at_left(p2));
		p3.mapped_by(affine_map2<metres,float>::rotation_at_right(p2));
		
		p2 = midpoint(p2, p3);
		p2 = midpoint(p2, p3, 0.3f);
		point2<metres,float> pp[] = { p2, p3, p4 };
		float ww[] = { 0.2f, 0.3f, 0.5f };
		p2 = barycentric_combination(30000, pp, ww);
		p2 = barycentric_combination(-5, pp, ww);
		p2 = barycentric_combination(0,
            static_cast<point2<metres,float>*>(nullptr),
            static_cast<float*>(nullptr));
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
			&& v3.x().value() < 0 && v4.x().value() < 0
            && v5.x().value() < 0 && v6.x().value() < 0
			&& v10.x().value() < 0 && v11.x().value() < 0
            && v12.x().value() < 0 && v13.x().value() < 0
            && v14.x().value() < 0 && p7.x().value() < 0) return 1;
	}

    { // 3D linear transformations
        vect3<metres,float> unit_v(10.f, 11.f, 12.f);

        linear_map3<float> lm1
            = linear_map3<float>::rotation(
            unit_v, vect1<degrees,float>(30.f));

        linear_map3<float> lm2
            = linear_map3<float>::projection_onto_line(
            unit_v);
        linear_map3<float> lm3
            = linear_map3<float>::projection_onto_plane(
            unit_v);

        linear_map3<float> lm4
            = linear_map3<float>::reflection_over_line(
            unit_v);
        linear_map3<float> lm5
            = linear_map3<float>::reflection_over_plane(
            unit_v);

        linear_map3<float> lm6
            = linear_map3<float>::scaling(3.f, 4.f, 5.f);
        
        linear_map3<float> lm7 = lm1.inverted();
        linear_map3<float> lm8 = combine(combine(combine(lm1, lm2),
            combine(lm3, lm4)), lm5);
        float a1 = lm1.coeff(0, 0);
        float a2 = lm1.coeff(0, 1);
        float a3 = lm1.coeff(0, 2);
        float a4 = lm1.coeff(1, 0);
        float a5 = lm1.coeff(1, 1);
        float a6 = lm1.coeff(1, 2);
        float a7 = lm1.coeff(2, 0);
        float a8 = lm1.coeff(2, 1);
        float a9 = lm1.coeff(2, 2);
        lm1.coeff(0, 0) = a1;
        lm1.coeff(0, 1) = a2;
        lm1.coeff(0, 2) = a3;
        lm1.coeff(1, 0) = a4;
        lm1.coeff(1, 1) = a5;
        lm1.coeff(1, 2) = a6;
        lm1.coeff(2, 0) = a7;
        lm1.coeff(2, 1) = a8;
        lm1.coeff(2, 2) = a9;

		if (lm1.coeff(0, 0) < 0
            || lm2.coeff(0, 0) < 0
            || lm3.coeff(0, 0) < 0
            || lm4.coeff(0, 0) < 0
            || lm5.coeff(0, 0) < 0
            || lm6.coeff(0, 0) < 0
            || lm7.coeff(0, 0) < 0
            || lm8.coeff(0, 0) < 0
            ) return 1;
    }

    { // 3D affine transformations
        point3<metres,float> fixed_p(23.f, 24.f, 25.f);
        vect3<metres,float> unit_v(10.f, 11.f, 12.f);
        
        affine_map3<metres,float> am1 = affine_map3<metres,float>
            ::translation(vect3<metres,float>(11.f, 12.f, 13.f));
            
        affine_map3<metres,float> am2 = affine_map3<metres,float>::rotation(
            fixed_p, unit_v, vect1<degrees,float>(30.f));

        affine_map3<metres,float> am3 = affine_map3<metres,float>
            ::rotation_at_right(fixed_p, unit_v);
        
        affine_map3<metres,float> am4 = affine_map3<metres,float>
            ::rotation_at_left(fixed_p, unit_v);
            
        affine_map3<metres,float> am5
            = affine_map3<metres,float>::projection_onto_line(
            fixed_p, unit_v);
        affine_map3<metres,float> am6
            = affine_map3<metres,float>::projection_onto_plane(
            fixed_p, unit_v);

        affine_map3<metres,float> am7
            = affine_map3<metres,float>::reflection_over_line(
            fixed_p, unit_v);
        affine_map3<metres,float> am8
            = affine_map3<metres,float>::reflection_over_plane(
            fixed_p, unit_v);

        affine_map3<metres,float> am9 = affine_map3<metres,float>::scaling(
            fixed_p, 3.f, 4.f, 5.f);
        
        affine_map3<metres,float> am10 = am1.inverted();
        affine_map3<metres,float> am11 = combine(combine(combine(am1, am2),
            combine(am3, am4)), am5);
        float a1 = am1.coeff(0, 0);
        float a2 = am1.coeff(0, 1);
        float a3 = am1.coeff(0, 2);
        float a4 = am1.coeff(1, 0);
        float a5 = am1.coeff(1, 1);
        float a6 = am1.coeff(1, 2);
        am1.coeff(0, 0) = a6;
        am1.coeff(0, 1) = a5;
        am1.coeff(0, 2) = a4;
        am1.coeff(1, 0) = a3;
        am1.coeff(1, 1) = a2;
        am1.coeff(1, 2) = a1;

		if (am1.coeff(0, 0) < 0
            || am2.coeff(0, 0) < 0
            || am3.coeff(0, 0) < 0
            || am4.coeff(0, 0) < 0
            || am5.coeff(0, 0) < 0
            || am6.coeff(0, 0) < 0
            || am7.coeff(0, 0) < 0
            || am8.coeff(0, 0) < 0
            || am9.coeff(0, 0) < 0
            || am10.coeff(0, 0) < 0
            || am11.coeff(0, 0) < 0
            ) return 1;
    }
	
	{
		float n;
		float nnn[3];
		double ddd[3];
		bool b;
		vect1<metres,float> v1;
		point1<metres,float> p1;
		
		vect3<metres,float> v2;
		vect3<metres,float> v3(2.3f, 2.4f, 2.5f);
		vect3<metres,float> v4(nnn);
		vect3<metres,double> v4b(nnn);
		vect3<metres,double> v4c(ddd);
		vect3<metres,float> v5(v1, v1, v1);
		vect3<metres,float> v6(v2);
		vect3<metres,float> v10(inches::id(), 2.5f, 2.6f, 2.7f);
        auto v11 = vect3<metres>();

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
		point3<metres,float> p4(nnn);
		point3<metres,double> p4b(nnn);
		point3<metres,double> p4c(ddd);
		point3<metres,float> p5(p1, p1, p1);
		point3<metres,float> p6(p2);
		point3<metres,float> p7(inches::id(), 6.2f, 6.3f, 6.4f);
        auto p8 = point3<metres>();
		
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
		p2 = barycentric_combination(30000, pp, ww);
		p2 = barycentric_combination(-5, pp, ww);
		p2 = barycentric_combination(0,
            static_cast<point3<metres,float>*>(nullptr),
            static_cast<float*>(nullptr));
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
			&& v10.x().value() < 0 && v11.x().value() < 0 && p8.x().value() < 0) return 1;
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
			signed_azimuth<degrees,long double>(1.2));
		unsigned_azimuth<degrees,float> v8 = cast<float>(
			unsigned_azimuth<degrees,long double>(2.3));

		if (v1.value() < 0 && v2.value() < 0
			&& v3.x().value() < 0
			&& v4.x().value() < 0 && v5.x().value() < 0 && v6.x().value() < 0
			&& v7.value() < 0 && v8.value() < 0) return 1;
	}
	
	{ // SCALAR_SCALAR
		vect1<seconds,float> time1;
		vect1<radians_per_second,float> angspeed1;
		vect1<radians,float> angle1;
		
		angle1 = time1 * angspeed1;
		angle1 = angspeed1 * time1;
		time1 = angle1 / angspeed1;
		angspeed1 = angle1 / time1;

		if (time1.value() < 0 && angspeed1.value() < 0
			&& angle1.value() < 0) return 1;
	}

	{ // SQUARED_SCALAR
		vect1<inches,float> space1;
		vect1<square_inches,float> area1;
		
		area1 = space1 * space1;
		area1 = sqr(space1);
		space1 = area1 / space1;
		space1 = sqrt(area1);

		if (space1.value() < 0 && area1.value() < 0) return 1;
	}

	{ // SCALAR_VECTOR
		vect1<seconds,float> time1;
		vect1<metres,float> space1;
		vect2<metres,float> space2;
		vect3<metres,float> space3;
		vect1<metres_per_second,float> speed1;
		vect2<metres_per_second,float> speed2;
		vect3<metres_per_second,float> speed3;
		
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
		vect1<newtons,float> force1;
		vect1<metres,float> space1;
		vect1<joules,float> energy1;
		vect1<newton_metres,float> torque1;
		vect2<newtons,float> force2;
		vect2<metres,float> space2;
		vect3<newtons,float> force3;
		vect3<metres,float> space3;
		vect3<newton_metres,float> torque3;
		
		energy1 = force1 * space1;
		energy1 = space1 * force1;
		force1 = energy1 / space1;
		space1 = energy1 / force1;
		
		energy1 = force2 * space2;
		energy1 = space2 * force2;
		torque1 = cross_product(force2, space2);
		torque1 = cross_product(space2, force2);
		
		energy1 = force3 * space3;
		energy1 = space3 * force3;
		torque3 = cross_product(force3, space3);
		torque3 = cross_product(space3, force3);

		if (force1.value() < 0 && space1.value() < 0
			&& energy1.value() < 0 && torque1.value() < 0
			&& force2.x().value() < 0 && space2.x().value() < 0
			&& force3.x().value() < 0 && space3.x().value() < 0
			&& torque3.x().value() < 0) return 1;
	}
	
	{ // SQUARED_VECTOR
		vect1<metres,float> space1;
		vect1<square_metres,float> area1;
		vect2<metres,float> space2;
		vect2<square_metres,float> area2;
		vect3<metres,float> space3;
		vect3<square_metres,float> area3;
		
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
}
