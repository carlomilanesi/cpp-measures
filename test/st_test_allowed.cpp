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
MEASURES_DERIVED_1_1(seconds, metres_per_second, metres)
MEASURES_DERIVED_1_2(seconds, metres_per_second, metres)
MEASURES_DERIVED_1_3(seconds, metres_per_second, metres)
MEASURES_DERIVED_1_1(newtons, metres, joules)
MEASURES_DERIVED_2_2(newtons, metres, joules, newton_metres)
MEASURES_DERIVED_3_3(newtons, metres, joules, newton_metres)
MEASURES_DERIVED_SQ_1(metres, square_metres)
MEASURES_DERIVED_SQ_2(metres, square_metres, square_metres)
MEASURES_DERIVED_SQ_3(metres, square_metres, square_metres)

int main()
{
    { // sqr
        sqr(3);
        sqr(3.14);
    }

    { // MEASURES_UNIT
        Space spa1 = inches::id();
        inches::magnitude spa2 = spa1;
        char const* suf = inches::suffix();
        double rat = inches::ratio();
        double off = inches::offset();
    }

    { // MEASURES_ANGLE_UNIT
        Angle ang1 = degrees::id();
        degrees::magnitude ang2 = ang1;
        char const* suf = degrees::suffix();
        double rat = degrees::ratio();
        double off = degrees::offset();
        double turn_fr = degrees::turn_fraction<double>();
    }

    { // MEASURES_MAGNITUDE
        Space u1 = Space::base_unit::id();
        Space u2(&inches_features_);
        char const* suf = u2.suffix();
        double rat = u2.ratio();
        double off = u2.offset();
    }

    { // Angle
        Angle a1 = Angle::base_unit::id();
        Angle a2(&degrees_features_);
        char const* suf = a2.suffix();
        double rat = a2.ratio();
        double off = a2.offset();
        double turn_fr = a2.turn_fraction<double>();
    }

    { // convert
        vect1<degrees,float> v1 = convert<degrees>(
            vect1<radians>(2.3));
        point1<degrees,float> v2 = convert<degrees>(
            point1<radians>(2.3));
        vect2<degrees,float> v3 = convert<degrees>(
            vect2<radians>(2.3, 2.4));
        affine_map2<degrees,float> v4 = convert<degrees>(
            make_translation(vect2<radians,float>(2.3, 2.4)));
        point2<degrees,float> v5 = convert<degrees>(
            point2<radians>(2.3, 2.4));
        vect3<degrees,float> v6 = convert<degrees>(
            vect3<radians>(2.3, 2.4, 2.5));
        affine_map3<degrees,float> v7 = convert<degrees>(
            make_translation(vect3<radians,float>(2.3, 2.4, 2.5)));
        point3<degrees,float> v8 = convert<degrees>(
            point3<radians>(2.3, 2.4, 2.5));
        signed_azimuth<degrees,float> v9 = convert<degrees>(
            signed_azimuth<radians>(2.3));
        unsigned_azimuth<degrees,float> v10 = convert<degrees>(
            unsigned_azimuth<radians>(2.3));
    }

    { // vect1, point1
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
        v2 = normalized(vd);
        vd = normalized(vd);

    }

    { // linear_map2
        linear_map2<float> lm1;
        lm1 = linear_map2<float>::rotation(vect1<degrees,double>(30.));
        lm1 = make_rotation(vect1<degrees,float>(30.));
        lm1 = linear_map2<float>::rotation_at_right();
        lm1 = make_rotation_at_right<float>();
        lm1 = linear_map2<float>::rotation_at_left();
        lm1 = make_rotation_at_left<float>();
        lm1 = linear_map2<float>::projection(point1<degrees,double>(30.));
        lm1 = make_projection(point1<degrees,float>(30.));
        lm1 = linear_map2<float>::projection(
            signed_azimuth<degrees,double>(30.));
        lm1 = make_projection(
            signed_azimuth<degrees,float>(30.));
        lm1 = linear_map2<float>::projection(
            unsigned_azimuth<degrees,double>(30.));
        lm1 = make_projection(
            unsigned_azimuth<degrees,float>(30.));
        lm1 = linear_map2<float>::projection(vect2<metres,double>(11., 12.));
        lm1 = make_projection(vect2<metres,float>(11., 12.));
        lm1 = linear_map2<float>::reflection(point1<degrees,double>(30.));
        lm1 = make_reflection(point1<degrees,float>(30.));
        lm1 = linear_map2<float>::reflection(
            signed_azimuth<degrees,double>(30.));
        lm1 = make_reflection(signed_azimuth<degrees,float>(30.));
        lm1 = linear_map2<float>::reflection(
            unsigned_azimuth<degrees,double>(30.));
        lm1 = make_reflection(unsigned_azimuth<degrees,float>(30.));
        lm1 = linear_map2<float>::reflection(vect2<metres,double>(11., 12.));
        lm1 = make_reflection(vect2<metres,float>(11., 12.));
        lm1 = linear_map2<float>::scaling(3., 4.);
        lm1 = make_scaling<float>(3.f, 4.f);
        make_scaling<float>(3., 4.);
        lm1 = lm1.inverted();
        lm1 = combine(combine(combine(lm1, lm1),
            combine(lm1, lm1)), lm1);
        linear_map2<double> lm2;
        float a1 = lm2.coeff(0, 0);
        float a2 = lm2.coeff(0, 1);
        float a3 = lm2.coeff(1, 0);
        float a4 = lm2.coeff(1, 1);
        lm2.coeff(0, 0) = a4;
        lm2.coeff(0, 1) = a3;
        lm2.coeff(1, 0) = a2;
        lm2.coeff(1, 1) = a1;
    }

    { // vect2
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

        vect2<inches> v12;
        v12 = vect2<inches,float>::unit_vector(point1<degrees>(1.2));
        v12 = vect2<inches,float>::unit_vector(signed_azimuth<degrees>(1.2));
        v12 = vect2<inches,float>::unit_vector(unsigned_azimuth<degrees>(1.2));
        v12 = make_unit_vector<inches,float>(point1<degrees>(1.2));
        v12 = make_unit_vector<inches,float>(signed_azimuth<degrees>(1.2));
        v12 = make_unit_vector<inches,float>(unsigned_azimuth<degrees>(1.2));

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
    }

    { // affine_map2
        point2<metres,float> fpf(23.f, 24.f);
        point2<metres,double> fpd(23., 24.);
        affine_map2<metres,float> am1;
        am1 = affine_map2<metres,float>::translation(
            vect2<metres,double>(11., 12.));
        am1 = make_translation(vect2<metres,float>(11., 12.));
        am1 = affine_map2<metres,float>::rotation(
            fpd, vect1<degrees,double>(30.));
        am1 = make_rotation<metres,float>(
            fpd, vect1<degrees,float>(30.));
        am1 = affine_map2<metres,float>::rotation_at_right(fpd);
        am1 = make_rotation_at_right<metres,float>(fpd);
        am1 = affine_map2<metres,float>::rotation_at_left(fpd);
        am1 = make_rotation_at_left<metres,float>(fpd);
        am1 = affine_map2<metres,float>::projection(
            fpd, point1<degrees,double>(30.));
        am1 = make_projection(
            fpf, point1<degrees,double>(30.));
        am1 = affine_map2<metres,float>::projection(
            fpd, signed_azimuth<degrees,double>(30.));
        am1 = make_projection<metres,float>(
            fpd, signed_azimuth<degrees,double>(30.));
        am1 = affine_map2<metres,float>::projection(
            fpd, unsigned_azimuth<degrees,double>(30.));
        am1 = make_projection<metres,float>(
            fpd, unsigned_azimuth<degrees,double>(30.));
        am1 = affine_map2<metres,float>::projection(
            fpd, vect2<metres,double>(11., 12.));
        am1 = make_projection<metres,float>(
            fpd, vect2<metres,double>(11., 12.));
        am1 = affine_map2<metres,float>::reflection(
            fpd, point1<degrees,double>(30.));
        am1 = make_reflection<metres,float>(
            fpd, point1<degrees,double>(30.));
        am1 = affine_map2<metres,float>::reflection(
            fpd, signed_azimuth<degrees,double>(30.));
        am1 = make_reflection<metres,float>(
            fpd, signed_azimuth<degrees,double>(30.));
        am1 = affine_map2<metres,float>::reflection(
            fpd, unsigned_azimuth<degrees,double>(30.));
        am1 = make_reflection<metres,float>(
            fpd, unsigned_azimuth<degrees,double>(30.));
        am1 = affine_map2<metres,float>::reflection(
            fpd, vect2<metres,double>(11., 12.));
        am1 = make_reflection<metres,float>(
            fpd, vect2<metres,double>(11., 12.));
        am1 = affine_map2<metres,float>::scaling(
            fpd, 3., 4.);
        am1 = make_scaling(fpf, 3., 4.);
        am1 = am1.inverted();
        am1 = combine(combine(combine(am1, am1),
            combine(am1, am1)), am1);
        affine_map2<metres,double> am2;
        float a1 = am2.coeff(0, 0);
        float a2 = am2.coeff(0, 1);
        float a3 = am2.coeff(0, 2);
        float a4 = am2.coeff(1, 0);
        float a5 = am2.coeff(1, 1);
        float a6 = am2.coeff(1, 2);
        am2.coeff(0, 0) = a6;
        am2.coeff(0, 1) = a5;
        am2.coeff(0, 2) = a4;
        am2.coeff(1, 0) = a3;
        am2.coeff(1, 1) = a2;
        am2.coeff(1, 2) = a1;
    }

    { // point2
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
        v2 = v2 + v2d;
        v2 = v2 - v2d;
        v2 = n * v2d;
        v2 = d * v2;
        v2 = d * v2d;
        v2 = v2d * n;
        v2 = v2 * d;
        v2 = v2d * d;
        v2 = v2d / n;
        v2 = v2 / d;
        v2 = v2d / d;
        n = squared_norm_value(v2d);
        v1 = norm(v2d);
        v2 = normalized(v2d);
    }

    { // linear_map3
        vect3<metres,float> unit_vf(10., 11., 12.);
        vect3<metres,double> unit_vd(10.f, 11.f, 12.f);
        linear_map3<float> lmf;
        linear_map3<double> lmd;
        lmf = linear_map3<float>::rotation(
            unit_vd, vect1<degrees,double>(30.));
        lmf = make_rotation(
            unit_vf, vect1<degrees,double>(30.));
        lmf = linear_map3<float>::rotation_at_right(unit_vd);
        lmf = make_rotation_at_right(unit_vf);
        lmf = linear_map3<float>::rotation_at_left(unit_vd);
        lmf = make_rotation_at_left(unit_vf);
        lmf = linear_map3<float>::projection_onto_line(unit_vd);
        lmf = make_projection_onto_line(unit_vf);
        lmf = linear_map3<float>::projection_onto_plane(unit_vd);
        lmf = make_projection_onto_plane(unit_vf);
        lmf = linear_map3<float>::reflection_over_line(unit_vd);
        lmf = make_reflection_over_line(unit_vf);
        lmf = linear_map3<float>::reflection_over_plane(unit_vd);
        lmf = make_reflection_over_plane(unit_vf);
        lmf = linear_map3<float>::scaling(3., 4., 5.);
        lmf = make_scaling(3.f, 4.f, 5.f);
        lmf = lmf.inverted();
        lmf = combine(combine(combine(lmf, lmf),
            combine(lmf, lmf)), lmf);
        double a1 = lmf.coeff(0, 0);
        double a2 = lmf.coeff(0, 1);
        double a3 = lmf.coeff(0, 2);
        double a4 = lmf.coeff(1, 0);
        double a5 = lmf.coeff(1, 1);
        double a6 = lmf.coeff(1, 2);
        double a7 = lmf.coeff(2, 0);
        double a8 = lmf.coeff(2, 1);
        double a9 = lmf.coeff(2, 2);
        lmf.coeff(0, 0) = a1;
        lmf.coeff(0, 1) = a2;
        lmf.coeff(0, 2) = a3;
        lmf.coeff(1, 0) = a4;
        lmf.coeff(1, 1) = a5;
        lmf.coeff(1, 2) = a6;
        lmf.coeff(2, 0) = a7;
        lmf.coeff(2, 1) = a8;
        lmf.coeff(2, 2) = a9;
    }

    { // vect3
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

        v2 = v2.mapped_by(make_rotation(
            vect3<metres,double>(1, 2, 3), vect1<radians,double>(n)));

        b = v2 == v2d;
        b = v2 != v2d;
        b = is_equal(v2, v2d, v1);
        b = is_equal(v2, v3, v1d);
    }

    { // affine_map3
        point3<metres,float> fixed_pf(23., 24., 25.);
        point3<metres,double> fixed_pd(23.f, 24.f, 25.f);
        vect3<metres,float> unit_vf(10., 11., 12.);
        vect3<metres,double> unit_vd(10.f, 11.f, 12.f);

        affine_map3<metres,float> am1;
        affine_map3<metres,double> am2;
        am1 = affine_map3<metres,float>
            ::translation(vect3<metres,double>(11., 12., 13.));
        am1 = make_translation(vect3<metres,float>(11., 12., 13.));
        am1 = affine_map3<metres,float>::rotation(
            fixed_pd, unit_vd, vect1<degrees,double>(30.));
        am1 = make_rotation(
            fixed_pf, unit_vd, vect1<degrees,double>(30.));
        am1 = affine_map3<metres,float>
            ::rotation_at_right(fixed_pd, unit_vd);
        am1 = make_rotation_at_right(fixed_pf, unit_vd);
        am1 = affine_map3<metres,float>
            ::rotation_at_left(fixed_pd, unit_vd);
        am1 = make_rotation_at_left(fixed_pf, unit_vd);
        am1 = affine_map3<metres,float>::projection_onto_line(
            fixed_pd, unit_vd);
        am1 = make_projection_onto_line(
            fixed_pf, unit_vd);
        am1 = affine_map3<metres,float>::projection_onto_plane(
            fixed_pd, unit_vd);
        am1 = make_projection_onto_plane(
            fixed_pf, unit_vd);

        am1 = affine_map3<metres,float>::reflection_over_line(
            fixed_pd, unit_vd);
        am1 = make_reflection_over_line(
            fixed_pf, unit_vd);
        am1 = affine_map3<metres,float>::reflection_over_plane(
            fixed_pd, unit_vd);
        am1 = make_reflection_over_plane(
            fixed_pf, unit_vd);

        am1 = affine_map3<metres,float>::scaling(
            fixed_pd, 3., 4., 5.);
        am1 = make_scaling(
            fixed_pf, 3., 4., 5.);

        am1 = am1.inverted();
        am1 = combine(combine(combine(am1, am1),
            combine(am1, am1)), am1);
        double a1 = am1.coeff(0, 0);
        double a2 = am1.coeff(0, 1);
        double a3 = am1.coeff(0, 2);
        double a4 = am1.coeff(1, 0);
        double a5 = am1.coeff(1, 1);
        double a6 = am1.coeff(1, 2);
        am1.coeff(0, 0) = a6;
        am1.coeff(0, 1) = a5;
        am1.coeff(0, 2) = a4;
        am1.coeff(1, 0) = a3;
        am1.coeff(1, 1) = a2;
        am1.coeff(1, 2) = a1;
    }

    { // point3
        float n;
        double d;
        double ddd[3];
        bool b;
        vect1<metres,float> v1;
        vect1<metres,double> v1d;
        vect3<metres,float> v2;
        vect3<metres,double> v2d;
        point1<metres,float> p1;
        point1<metres,double> pd;

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

        p2 = p2.mapped_by(make_rotation(p2d, v2d, vect1<radians,double>(n)));

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
        v2 = v2 + v2d;
        v2 = v2 - v2d;
        v2 = n * v2d;
        v2 = d * v2;
        v2 = d * v2d;
        v2 = v2d * n;
        v2 = v2 * d;
        v2 = v2d * d;
        v2 = v2d / n;
        v2 = v2 / d;
        v2 = v2d / d;
        n = squared_norm_value(v2d);
        v1 = norm(v2d);
        v2 = normalized(v2);
    }

    { // signed_azimuth
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
    }

    { // unsigned_azimuth
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
    }

    { // sin, cos, tan
        float n;
        vect1<degrees> v;
        point1<degrees> p;
        signed_azimuth<degrees> s;
        unsigned_azimuth<degrees> u;
        n = sin(v);
        n = cos(v);
        n = tan(v);

        n = sin(p);
        n = cos(p);
        n = tan(p);

        n = sin(s);
        n = cos(s);
        n = tan(s);

        n = sin(u);
        n = cos(u);
        n = tan(u);
    }

    { // cast
        vect1<degrees,float> v1 = cast<float>(
            vect1<degrees,long double>(2.3f));
        point1<degrees,float> v2 = cast<float>(
            point1<degrees,long double>(2.3f));
        linear_map2<float> v3 = cast<float>(
            make_rotation(vect1<degrees,long double>(12)));
        vect2<degrees,float> v4 = cast<float>(
            vect2<degrees,long double>(2.3f, 2.4f));
        affine_map2<degrees,float> v5 = cast<float>(
            make_translation(vect2<degrees,long double>(12, 13)));
        point2<degrees,float> v6 = cast<float>(
            point2<degrees,long double>(2.3f, 2.4f));
        linear_map3<float> v7 = cast<float>(
            make_rotation(vect3<degrees,long double>(12, 13, 14),
                vect1<degrees,long double>(12)));
        vect3<degrees,float> v8 = cast<float>(
            vect3<degrees,long double>(2.3f, 2.4f, 2.5f));
        affine_map3<degrees,float> v9 = cast<float>(
            make_translation(vect3<degrees,long double>(12, 13, 14)));
        point3<degrees,float> v10 = cast<float>(
            point3<degrees,long double>(2.3f, 2.4f, 2.5f));
        signed_azimuth<degrees,float> v11 = cast<float>(
            signed_azimuth<degrees,long double>(1.2));
        unsigned_azimuth<degrees,float> v12 = cast<float>(
            unsigned_azimuth<degrees,long double>(2.3));
    }

    { // MEASURES_DERIVED_1_1
        vect1<seconds,float> time1f;
        vect1<seconds,double> time1d;
        vect1<radians_per_second,float> angspeed1f;
        vect1<radians_per_second,double> angspeed1d;
        vect1<radians,float> angle1f;
        vect1<radians,double> angle1d;

        angle1f = time1d * angspeed1d;
        angle1f = time1f * angspeed1d;
        angle1f = time1d * angspeed1f;
        angle1f = angspeed1d * time1d;
        angle1f = angspeed1f * time1d;
        angle1f = angspeed1d * time1f;
        time1f = angle1d / angspeed1d;
        time1f = angle1f / angspeed1d;
        time1f = angle1d / angspeed1f;
        angspeed1f = angle1d / time1d;
        angspeed1f = angle1f / time1d;
        angspeed1f = angle1d / time1f;
    }

    { // MEASURES_DERIVED_SQ_1
        vect1<inches,float> space1f;
        vect1<inches,double> space1d;
        vect1<square_inches,float> area1f;
        vect1<square_inches,double> area1d;

        area1f = space1d * space1d;
        area1f = space1f * space1d;
        area1f = space1d * space1f;
        area1f = sqr(space1d);
        space1f = area1d / space1d;
        space1f = area1f / space1d;
        space1f = area1d / space1f;
        space1f = sqrt(area1d);
    }

    { // MEASURES_DERIVED_1_2
        vect1<seconds,float> time1f;
        vect1<seconds,double> time1d;
        vect2<metres,float> space2f;
        vect2<metres,double> space2d;
        vect2<metres_per_second,float> speed2f;
        vect2<metres_per_second,double> speed2d;

        space2f = time1d * speed2d;
        space2f = time1f * speed2d;
        space2f = time1d * speed2f;
        space2f = speed2d * time1d;
        space2f = speed2f * time1d;
        space2f = speed2d * time1f;
        speed2f = space2d / time1d;
        speed2f = space2f / time1d;
        speed2f = space2d / time1f;
    }

    { // MEASURES_DERIVED_1_3
        vect1<seconds,float> time1f;
        vect1<seconds,double> time1d;
        vect3<metres,float> space3f;
        vect3<metres,double> space3d;
        vect3<metres_per_second,float> speed3f;
        vect3<metres_per_second,double> speed3d;

        space3f = time1d * speed3d;
        space3f = time1f * speed3d;
        space3f = time1d * speed3f;
        space3f = speed3d * time1d;
        space3f = speed3f * time1d;
        space3f = speed3d * time1f;
        speed3f = space3d / time1d;
        speed3f = space3f / time1d;
        speed3f = space3d / time1f;
    }

    { // MEASURES_DERIVED_2_2
        vect1<joules,float> energy1f;
        vect1<newton_metres,float> torque1f;
        vect2<newtons,float> force2f;
        vect2<newtons,double> force2d;
        vect2<metres,float> space2f;
        vect2<metres,double> space2d;

        energy1f = force2d * space2d;
        energy1f = force2f * space2d;
        energy1f = force2d * space2f;
        energy1f = space2d * force2d;
        energy1f = space2f * force2d;
        energy1f = space2d * force2f;
        torque1f = cross_product(force2d, space2d);
        torque1f = cross_product(force2f, space2d);
        torque1f = cross_product(force2d, space2f);
        torque1f = cross_product(space2d, force2d);
        torque1f = cross_product(space2f, force2d);
        torque1f = cross_product(space2d, force2f);
    }

    { // MEASURES_DERIVED_3_3
        vect3<newtons,float> force3f;
        vect3<newtons,double> force3d;
        vect3<metres,float> space3f;
        vect3<metres,double> space3d;
        vect1<joules,float> energy1f;
        vect3<newton_metres,float> torque3f;

        energy1f = force3d * space3d;
        energy1f = force3f * space3d;
        energy1f = force3d * space3f;
        energy1f = space3d * force3d;
        energy1f = space3f * force3d;
        energy1f = space3d * force3f;
        torque3f = cross_product(force3d, space3d);
        torque3f = cross_product(force3f, space3d);
        torque3f = cross_product(force3d, space3f);
        torque3f = cross_product(space3d, force3d);
        torque3f = cross_product(space3f, force3d);
        torque3f = cross_product(space3d, force3f);
    }

    { // MEASURES_DERIVED_SQ_2
        vect1<square_metres,float> area1f;
        vect2<metres,float> space2f;
        vect2<metres,double> space2d;

        area1f = space2d * space2d;
        area1f = space2f * space2d;
        area1f = space2d * space2f;
        area1f = sqr(space2d);
        area1f = cross_product(space2d, space2d);
        area1f = cross_product(space2f, space2d);
        area1f = cross_product(space2d, space2f);
    }

    { // MEASURES_DERIVED_SQ_3
        vect3<metres,float> space3f;
        vect3<metres,double> space3d;
        vect1<square_metres,float> area1f;
        vect3<square_metres,float> area3f;

        area1f = space3d * space3d;
        area1f = space3f * space3d;
        area1f = space3d * space3f;
        area1f = sqr(space3d);
        area3f = cross_product(space3d, space3d);
        area3f = cross_product(space3f, space3d);
        area3f = cross_product(space3d, space3f);
    }

    { // operator >>, operator <<
        vect1<metres,float> v1f;
        vect1<metres,double> v1d;
        point1<metres,float> p1f;
        point1<metres,double> p1d;
        vect2<metres,float> v2f;
        vect2<metres,double> v2d;
        linear_map2<float> lm2f;
        linear_map2<double> lm2d;
        point2<metres,float> p2f;
        point2<metres,double> p2d;
        affine_map2<metres,float> am2f;
        affine_map2<metres,double> am2d;
        vect3<metres,float> v3f;
        vect3<metres,double> v3d;
        linear_map3<float> lm3f;
        linear_map3<double> lm3d;
        point3<metres,float> p3f;
        point3<metres,double> p3d;
        affine_map3<metres,float> am3f;
        affine_map3<metres,double> am3d;
        signed_azimuth<degrees,float> saf;
        signed_azimuth<degrees,double> sad;
        unsigned_azimuth<degrees,float> uaf;
        unsigned_azimuth<degrees,double> uad;

        std::cin >> v1f >> v1d;
        std::cout << v1f << v1d;
        std::cin >> p1f >> p1d;
        std::cout << p1f << p1d;
        std::cin >> v2f >> v2d;
        std::cout << v2f << v2d;
        std::cin >> lm2f >> lm2d;
        std::cout << lm2f << lm2d;
        std::cin >> p2f >> p2d;
        std::cout << p2f << p2d;
        std::cin >> am2f >> am2d;
        std::cout << am2f << am2d;
        std::cin >> v3f >> v3d;
        std::cout << v3f << v3d;
        std::cin >> lm3f >> lm3d;
        std::cout << lm3f << lm3d;
        std::cin >> p3f >> p3d;
        std::cout << p3f << p3d;
        std::cin >> am3f >> am3d;
        std::cout << am3f << am3d;
        std::cin >> saf >> sad;
        std::cout << saf << sad;
        std::cin >> uaf >> uad;
        std::cout << uaf << uad;
    }
}
