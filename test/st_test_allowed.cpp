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
        point1<metres,float> ppf[] = { p2, p2, p2 };
        point1<metres> ppd[] = { pd, pd, pd };
        float wwf[] = { 0.2f, 0.3f, 0.5f };
        double wwd[] = { 0.2, 0.3, 0.5 };
        p1 = barycentric_combination(3, ppf, wwd);
        p1 = barycentric_combination(3, ppd, wwf);
        p1 = barycentric_combination(3, ppd, wwd);
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
        double dd[2];
        bool b;
        vect1<metres,float> v1;
        vect1<metres> v1d;
        point1<metres,float> p1;
        point1<metres> pd;
        
        vect2<metres,float> v2;
        vect2<metres> v2d;
        vect2<metres,float> v3(2.3, 2.4);
        vect2<metres,float> v4(dd);
        vect2<metres,float> v5(v1d, v1d);
        vect2<metres,float> v6(v2d);
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
		v2 = v3.mapped_by(linear_map2<>::rotation(
            vect1<radians>(n)));
		v2 = v2d.mapped_by(linear_map2<>::rotation(
            vect1<radians>(n)));
        v2 = v2d.mapped_by(linear_map2<>::rotation_at_left());
        v2 = v2d.mapped_by(linear_map2<>::rotation_at_right());

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
        p3.mapped_by(affine_map2<metres>::rotation(p2, vect1<radians>(d)));
        p3.mapped_by(affine_map2<metres>::rotation(p2d, vect1<radians,float>(n)));
        p2d.mapped_by(affine_map2<metres>::rotation(p2, vect1<radians,float>(n)));
        p3.mapped_by(affine_map2<metres>::rotation_at_left(p2d));
        p3.mapped_by(affine_map2<metres>::rotation_at_right(p2d));
        
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
        double ddd[3];
        bool b;
        vect1<metres,float> v1;
        vect1<metres> v1d;
        point1<metres,float> p1;
        point1<metres> pd;
        
        vect3<metres,float> v2;
        vect3<metres> v2d;
        vect3<metres,float> v3(2.3, 2.4, 2.5);
        vect3<metres,float> v4(ddd);
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
        point3<metres,float> p4(ddd);
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
            vect1<radians>(1.2));
        point1<degrees,float> v2 = convert<degrees>(
            point1<radians>(1.2));
        vect2<degrees,float> v3 = convert<degrees>(
            vect2<radians>(1.2, 1.3));
        point2<degrees,float> v4 = convert<degrees>(
            point2<radians>(1.2, 1.3));
        vect3<degrees,float> v5 = convert<degrees>(
            vect3<radians>(1.2, 1.3, 1.4));
        point3<degrees,float> v6 = convert<degrees>(
            point3<radians>(1.2, 1.3, 1.4));
        signed_azimuth<degrees,float> v7 = convert<degrees>(
            signed_azimuth<radians>(1.2));
        unsigned_azimuth<degrees,float> v8 = convert<degrees>(
            unsigned_azimuth<radians>(1.2));
    }

    { // SCALAR_SCALAR
        vect1<seconds,float> time1f;
        vect1<radians_per_second,float> angspeed1f;
        vect1<radians,float> angle1f;

        vect1<seconds> time1d;
        vect1<radians_per_second> angspeed1d;
        vect1<radians> angle1d;
        
        
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
        vect1<inches,float> space1f;
        vect1<square_inches,float> area1f;

        vect1<inches> space1d;
        vect1<square_inches> area1d;
        
        
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
        vect1<seconds,float> time1f;
        vect1<metres,float> space1f;
        vect2<metres,float> space2f;
        vect3<metres,float> space3f;
        vect1<metres_per_second,float> speed1f;
        vect2<metres_per_second,float> speed2f;
        vect3<metres_per_second,float> speed3f;

        vect1<seconds,float> time1d;
        vect1<metres,float> space1d;
        vect2<metres,float> space2d;
        vect3<metres,float> space3d;
        vect1<metres_per_second,float> speed1d;
        vect2<metres_per_second,float> speed2d;
        vect3<metres_per_second,float> speed3d;
        
        
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
        vect1<newtons,float> force1f;
        vect1<metres,float> space1f;
        vect1<joules,float> energy1f;
        vect1<newton_metres,float> torque1f;
        vect2<newtons,float> force2f;
        vect2<metres,float> space2f;
        vect3<newtons,float> force3f;
        vect3<metres,float> space3f;
        vect3<newton_metres,float> torque3f;

        vect1<newtons> force1d;
        vect1<metres> space1d;
        vect1<joules> energy1d;
        vect1<newton_metres> torque1d;
        vect2<newtons> force2d;
        vect2<metres> space2d;
        vect3<newtons> force3d;
        vect3<metres> space3d;
        vect3<newton_metres> torque3d;
        
        
        energy1f = force1f * space1d;
        energy1f = space1f * force1d;
        force1f = energy1f / space1d;
        space1f = energy1f / force1d;
        
        energy1f = force2f * space2d;
        energy1f = space2f * force2d;
        torque1f = cross_product(force2f, space2d);
        torque1f = cross_product(space2f, force2d);
        
        energy1f = force3f * space3d;
        energy1f = space3f * force3d;
        torque3f = cross_product(force3f, space3d);
        torque3f = cross_product(space3f, force3d);
        
        
        energy1f = force1d * space1f;
        energy1f = space1d * force1f;
        force1f = energy1d / space1f;
        space1f = energy1d / force1f;
        
        energy1f = force2d * space2f;
        energy1f = space2d * force2f;
        torque1f = cross_product(force2d, space2f);
        torque1f = cross_product(space2d, force2f);
        
        energy1f = force3d * space3f;
        energy1f = space3d * force3f;
        torque3f = cross_product(force3d, space3f);
        torque3f = cross_product(space3d, force3f);
        
        
        energy1f = force1d * space1d;
        energy1f = space1d * force1d;
        force1f = energy1d / space1d;
        space1f = energy1d / force1d;
        
        energy1f = force2d * space2d;
        energy1f = space2d * force2d;
        torque1f = cross_product(force2d, space2d);
        torque1f = cross_product(space2d, force2d);
        
        energy1f = force3d * space3d;
        energy1f = space3d * force3d;
        torque3f = cross_product(force3d, space3d);
        torque3f = cross_product(space3d, force3d);
    }

    { // SQUARED_VECTOR
        vect1<metres,float> space1f;
        vect1<square_metres,float> area1f;
        vect2<metres,float> space2f;
        vect2<square_metres,float> area2f;
        vect3<metres,float> space3f;
        vect3<square_metres,float> area3f;
        
        vect1<metres> space1d;
        vect1<square_metres> area1d;
        vect2<metres> space2d;
        vect2<square_metres> area2d;
        vect3<metres> space3d;
        vect3<square_metres> area3d;
        
        
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
}
