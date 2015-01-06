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
        long double rat = inches::ratio();
        long double off = inches::offset();
        if (spa2.offset() < 0 || suf == 0 || rat == 0 || off < 0) return 1;
    }

    { // MEASURES_ANGLE_UNIT
        Angle ang1 = degrees::id();
        degrees::magnitude ang2 = ang1;
        char const* suf = degrees::suffix();
        long double rat = degrees::ratio();
        long double off = degrees::offset();
        long double turn_fr = degrees::turn_fraction<long double>();
        turn_fr = degrees::turn_fraction<double>();
        turn_fr = degrees::turn_fraction<float>();
        if (ang2.offset() < 0 || suf == 0 || rat == 0 || off < 0 || turn_fr == 0) return 1;
    }

    { // MEASURES_MAGNITUDE
        Space u1 = Space::base_unit::id();
        Space u2(&inches_features_);
        char const* suf = u2.suffix();
        long double rat = u2.ratio();
        long double off = u2.offset();
        if (u1.offset() < 0 || suf == 0 || rat == 0 || off < 0) return 1;
    }

    { // Angle
        Angle a1 = Angle::base_unit::id();
        Angle a2(&degrees_features_);
        char const* suf = a2.suffix();
        long double rat = a2.ratio();
        long double off = a2.offset();
        long double turn_fr = a2.turn_fraction<long double>();
        turn_fr = a2.turn_fraction<double>();
        turn_fr = a2.turn_fraction<float>();
        if (a1.offset() < 0 || suf == 0 || rat == 0 || off < 0 || turn_fr == 0) return 1;
    }

    { // convert
        vect1<degrees,float> v1 = convert<degrees>(
            vect1<radians,float>(2.3f));
        point1<degrees,float> v2 = convert<degrees>(
            point1<radians,float>(2.3f));
        vect2<degrees,float> v3 = convert<degrees>(
            vect2<radians,float>(2.3f, 2.4f));
        affine_map2<degrees,float> v4 = convert<degrees>(
            make_translation(vect2<radians,float>(2.3f, 2.4f)));
        point2<degrees,float> v5 = convert<degrees>(
            point2<radians,float>(2.3f, 2.4f));
        vect3<degrees,float> v6 = convert<degrees>(
            vect3<radians,float>(2.3f, 2.4f, 2.5f));
        affine_map3<degrees,float> v7 = convert<degrees>(
            make_translation(vect3<radians,float>(2.3f, 2.4f, 2.5f)));
        point3<degrees,float> v8 = convert<degrees>(
            point3<radians,float>(2.3f, 2.4f, 2.5f));
        signed_azimuth<degrees,float> v9 = convert<degrees>(
            signed_azimuth<radians,float>(2.3f));
        unsigned_azimuth<degrees,float> v10 = convert<degrees>(
            unsigned_azimuth<radians,float>(2.3f));

        if (v1.value() < 0 && v2.value() < 0 && v3.x().value() < 0
            && v4.coeff(0, 0) < 0 && v5.x().value() < 0 && v6.x().value() < 0
            && v7.coeff(0, 0) < 0 && v8.x().value() < 0 && v9.value() < 0
            && v10.value() < 0) return 1;
    }

    { // vect1, point1
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
        v1 = normalized(v1);

        if (b && n < 0 && v1.value() < 0 && v2.value() < 0 && v3.value() < 0
            && v4.value() < 0 && v5.value() < 0) return 1;
    }

    { // 2D linear transformations
        linear_map2<float> lm1;
        lm1 = linear_map2<float>::rotation(vect1<degrees,float>(30.f));
        lm1 = make_rotation(vect1<degrees,float>(30.f));
        lm1 = linear_map2<float>::rotation_at_right();
        lm1 = make_rotation_at_right<float>();
        lm1 = linear_map2<float>::rotation_at_left();
        lm1 = make_rotation_at_left<float>();
        lm1 = linear_map2<float>::projection(point1<degrees,float>(30.f));
        lm1 = make_projection(point1<degrees,float>(30.f));
        lm1 = linear_map2<float>::projection(
            signed_azimuth<degrees,float>(30.f));
        lm1 = make_projection(
            signed_azimuth<degrees,float>(30.f));
        lm1 = linear_map2<float>::projection(
            unsigned_azimuth<degrees,float>(30.f));
        lm1 = make_projection(
            unsigned_azimuth<degrees,float>(30.f));
        lm1 = linear_map2<float>::projection(vect2<metres,float>(11.f, 12.f));
        lm1 = make_projection(vect2<metres,float>(11.f, 12.f));
        lm1 = linear_map2<float>::reflection(point1<degrees,float>(30.f));
        lm1 = make_reflection(point1<degrees,float>(30.f));
        lm1 = linear_map2<float>::reflection(
            signed_azimuth<degrees,float>(30.f));
        lm1 = make_reflection(signed_azimuth<degrees,float>(30.f));
        lm1 = linear_map2<float>::reflection(
            unsigned_azimuth<degrees,float>(30.f));
        lm1 = make_reflection(unsigned_azimuth<degrees,float>(30.f));
        lm1 = linear_map2<float>::reflection(vect2<metres,float>(11.f, 12.f));
        lm1 = make_reflection(vect2<metres,float>(11.f, 12.f));
        lm1 = linear_map2<float>::scaling(3.f, 4.f);
        lm1 = make_scaling(3.f, 4.f);
        lm1 = lm1.inverted();
        lm1 = combine(combine(combine(lm1, lm1),
            combine(lm1, lm1)), lm1);
        float a1 = lm1.coeff(0, 0);
        float a2 = lm1.coeff(0, 1);
        float a3 = lm1.coeff(1, 0);
        float a4 = lm1.coeff(1, 1);
        lm1.coeff(0, 0) = a4;
        lm1.coeff(0, 1) = a3;
        lm1.coeff(1, 0) = a2;
        lm1.coeff(1, 1) = a1;

        if (lm1.coeff(0, 0) < 0) return 1;
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

        vect2<inches> v12;
        v12 = vect2<inches,float>::unit_vector(point1<degrees>(1.2));
        v12 = vect2<inches,float>::unit_vector(signed_azimuth<degrees>(1.2));
        v12 = vect2<inches,float>::unit_vector(unsigned_azimuth<degrees>(1.2));
        v12 = make_unit_vector<inches,float>(point1<degrees>(1.2));
        v12 = make_unit_vector<inches,float>(signed_azimuth<degrees>(1.2));
        v12 = make_unit_vector<inches,float>(unsigned_azimuth<degrees>(1.2));

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
        v2 = v3.mapped_by(make_rotation(vect1<radians,float>(n)));

        b = v2 == v3;
        b = v2 != v3;
        b = is_equal(v2, v3, v1);

        if (b || v11.x().value() == 0) return 1;
    }

    { // 2D affine transformations
        point2<metres,float> fp(23.f, 24.f);
        affine_map2<metres,float> am1;
        am1 = affine_map2<metres,float>::translation(
            vect2<metres,float>(11.f, 12.f));
        am1 = make_translation(vect2<metres,float>(11.f, 12.f));
        am1 = affine_map2<metres,float>::rotation(
            fp, vect1<degrees,float>(30.f));
        am1 = make_rotation(
            fp, vect1<degrees,float>(30.f));
        am1 = affine_map2<metres,float>::rotation_at_right(fp);
        am1 = make_rotation_at_right(fp);
        am1 = affine_map2<metres,float>::rotation_at_left(fp);
        am1 = make_rotation_at_left(fp);
        am1 = affine_map2<metres,float>::projection(
            fp, point1<degrees,float>(30.f));
        am1 = make_projection(
            fp, point1<degrees,float>(30.f));
        am1 = affine_map2<metres,float>::projection(
            fp, signed_azimuth<degrees,float>(30.f));
        am1 = make_projection(
            fp, signed_azimuth<degrees,float>(30.f));
        am1 = affine_map2<metres,float>::projection(
            fp, unsigned_azimuth<degrees,float>(30.f));
        am1 = make_projection(
            fp, unsigned_azimuth<degrees,float>(30.f));
        am1 = affine_map2<metres,float>::projection(
            fp, vect2<metres,float>(11.f, 12.f));
        am1 = make_projection(
            fp, vect2<metres,float>(11.f, 12.f));
        am1 = affine_map2<metres,float>::reflection(
            fp, point1<degrees,float>(30.f));
        am1 = make_reflection(
            fp, point1<degrees,float>(30.f));
        am1 = affine_map2<metres,float>::reflection(
            fp, signed_azimuth<degrees,float>(30.f));
        am1 = make_reflection(
            fp, signed_azimuth<degrees,float>(30.f));
        am1 = affine_map2<metres,float>::reflection(
            fp, unsigned_azimuth<degrees,float>(30.f));
        am1 = make_reflection(
            fp, unsigned_azimuth<degrees,float>(30.f));
        am1 = affine_map2<metres,float>::reflection(
            fp, vect2<metres,float>(11.f, 12.f));
        am1 = make_reflection(
            fp, vect2<metres,float>(11.f, 12.f));
        am1 = affine_map2<metres,float>::scaling(
            fp, 3.f, 4.f);
        am1 = make_scaling(fp, 3.f, 4.f);
        am1 = am1.inverted();
        am1 = combine(combine(combine(am1, am1),
            combine(am1, am1)), am1);
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

        if (am1.coeff(0, 0) < 0) return 1;
    }

    {
        float n;
        float nn[2];
        double dd[2];
        bool b;
        vect1<metres,float> v1;
        vect2<metres,float> v2;
        point1<metres,float> p1;
		point2<metres,float> p2;
        point2<metres,float> p3(1.2f, 1.3f);
        point2<metres,float> p4(nn);
        point2<metres,double> p5(nn);
        point2<metres,double> p5d(dd);
        point2<metres,float> p6(p1, p1);
        point2<metres,float> p7(p2);
        auto p8 = point2<metres>();
        point2<metres,float> p9(inches::id(), 6.2f, 6.3f);
        n = p3.data()[0]; n = p3.data()[1];
        p3.data()[0] = n; p3.data()[1] = n;
        p1 = p3.x();
        p3.x() = p1;
        p1 = p3.y();
        p3.y() = p1;
        p2 += v2;
        p2 -= v2;
        p3 = p3.mapped_by(make_rotation(p2, vect1<radians,float>(n)));

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
        v2 = v2 + v2;
        v2 = v2 - v2;
        v2 = n * v2;
        v2 = v2 * n;
        v2 = v2 / n;
        n = squared_norm_value(v2);
        v1 = norm(v2);
        v2 = normalized(v2);

        if (b && n < 0 && v1.value() < 0 && v2.x().value() < 0
            && p7.x().value() < 0 && p8.x().value() < 0) return 1;
    }

    { // 3D linear transformations
        vect3<metres,float> unit_v(10.f, 11.f, 12.f);
        linear_map3<float> lm1;
        lm1 = linear_map3<float>::rotation(
            unit_v, vect1<degrees,float>(30.f));
        lm1 = make_rotation(
            unit_v, vect1<degrees,float>(30.f));
        lm1 = linear_map3<float>::rotation_at_right(unit_v);
        lm1 = make_rotation_at_right(unit_v);
        lm1 = linear_map3<float>::rotation_at_left(unit_v);
        lm1 = make_rotation_at_left(unit_v);
        lm1 = linear_map3<float>::projection_onto_line(unit_v);
        lm1 = make_projection_onto_line(unit_v);
        lm1 = linear_map3<float>::projection_onto_plane(unit_v);
        lm1 = make_projection_onto_plane(unit_v);
        lm1 = linear_map3<float>::reflection_over_line(unit_v);
        lm1 = make_reflection_over_line(unit_v);
        lm1 = linear_map3<float>::reflection_over_plane(unit_v);
        lm1 = make_reflection_over_plane(unit_v);
        lm1 = linear_map3<float>::scaling(3.f, 4.f, 5.f);
        lm1 = make_scaling(3.f, 4.f, 5.f);
        lm1 = lm1.inverted();
        lm1 = combine(combine(combine(lm1, lm1),
            combine(lm1, lm1)), lm1);
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

        if (lm1.coeff(0, 0) < 0) return 1;
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

        v2 = v2.mapped_by(make_rotation(v3, vect1<radians,float>(n)));

        b = v2 == v3;
        b = v2 != v3;
        b = is_equal(v2, v3, v1);

        if (b || v11.x().value() < 0) return 1;
    }

    { // 3D affine transformations
        point3<metres,float> fixed_p(23.f, 24.f, 25.f);
        vect3<metres,float> unit_v(10.f, 11.f, 12.f);

        affine_map3<metres,float> am1;
        am1 = affine_map3<metres,float>
            ::translation(vect3<metres,float>(11.f, 12.f, 13.f));
        am1 = make_translation(vect3<metres,float>(11.f, 12.f, 13.f));
        am1 = affine_map3<metres,float>::rotation(
            fixed_p, unit_v, vect1<degrees,float>(30.f));
        am1 = make_rotation(
            fixed_p, unit_v, vect1<degrees,float>(30.f));
        am1 = affine_map3<metres,float>
            ::rotation_at_right(fixed_p, unit_v);
        am1 = make_rotation_at_right(fixed_p, unit_v);
        am1 = affine_map3<metres,float>
            ::rotation_at_left(fixed_p, unit_v);
        am1 = make_rotation_at_left(fixed_p, unit_v);
        am1 = affine_map3<metres,float>::projection_onto_line(
            fixed_p, unit_v);
        am1 = make_projection_onto_line(
            fixed_p, unit_v);
        am1 = affine_map3<metres,float>::projection_onto_plane(
            fixed_p, unit_v);
        am1 = make_projection_onto_plane(
            fixed_p, unit_v);

        am1 = affine_map3<metres,float>::reflection_over_line(
            fixed_p, unit_v);
        am1 = make_reflection_over_line(
            fixed_p, unit_v);
        am1 = affine_map3<metres,float>::reflection_over_plane(
            fixed_p, unit_v);
        am1 = make_reflection_over_plane(
            fixed_p, unit_v);

        am1 = affine_map3<metres,float>::scaling(
            fixed_p, 3.f, 4.f, 5.f);
        am1 = make_scaling(
            fixed_p, 3.f, 4.f, 5.f);

        am1 = am1.inverted();
        am1 = combine(combine(combine(am1, am1),
            combine(am1, am1)), am1);
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

        if (am1.coeff(0, 0) < 0) return 1;
    }

    {
        float n;
        float nnn[3];
        double ddd[3];
        bool b;
        vect1<metres,float> v1;
        vect3<metres,float> v2;
        point1<metres,float> p1;
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

        p2 = p2.mapped_by(make_rotation(p2, v2, vect1<radians,float>(n)));

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
        v2 = v2 + v2;
        v2 = v2 - v2;
        v2 = n * v2;
        v2 = v2 * n;
        v2 = v2 / n;
        n = squared_norm_value(v2);
        v1 = norm(v2);
        v2 = normalized(v2);

        if (b && n < 0 && v1.value() < 0 && v2.x().value() < 0
            && p8.x().value() < 0) return 1;
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

        if (b && n < 0 && v1.value() < 0 && v2.x().value() < 0) return 1;
    }

    { // Trigonometry
        float n;
        vect1<degrees,float> v;
        point1<degrees,float> p;
        signed_azimuth<degrees,float> s;
        unsigned_azimuth<degrees,float> u;
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

        if (n < 0) return 1;
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

        if (v1.value() < 0 && v2.value() < 0 && v3.coeff(0, 0) < 0
            && v4.x().value() < 0 && v5.coeff(0, 0) < 0 && v6.x().value() < 0
            && v7.coeff(0, 0) < 0 && v8.x().value() < 0 && v9.coeff(0, 0) < 0
            && v10.x().value() < 0 && v11.value() < 0 && v12.value() < 0)
        {
            return 1;
        }
    }

    { // MEASURES_DERIVED_1_1
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

    { // MEASURES_DERIVED_SQ_1
        vect1<inches,float> space1;
        vect1<square_inches,float> area1;

        area1 = space1 * space1;
        area1 = sqr(space1);
        space1 = area1 / space1;
        space1 = sqrt(area1);

        if (space1.value() < 0 && area1.value() < 0) return 1;
    }

    { // MEASURES_DERIVED_1_2
        vect1<seconds,float> time1;
        vect2<metres,float> space2;
        vect2<metres_per_second,float> speed2;

        space2 = time1 * speed2;
        space2 = speed2 * time1;
        speed2 = space2 / time1;

        if (time1.value() < 0 && space2.x().value() < 0
            && speed2.x().value() < 0) return 1;
    }

    { // MEASURES_DERIVED_1_3
        vect1<seconds,float> time1;
        vect3<metres,float> space3;
        vect3<metres_per_second,float> speed3;

        space3 = time1 * speed3;
        space3 = speed3 * time1;
        speed3 = space3 / time1;

        if (time1.value() < 0 && space3.x().value() < 0
            && speed3.x().value() < 0) return 1;
    }

    { // MEASURES_DERIVED_2_2
        vect1<joules,float> energy1;
        vect1<newton_metres,float> torque1;
        vect2<newtons,float> force2;
        vect2<metres,float> space2;

        energy1 = force2 * space2;
        energy1 = space2 * force2;
        torque1 = cross_product(force2, space2);
        torque1 = cross_product(space2, force2);

        if (energy1.value() < 0 && torque1.value() < 0
            && force2.x().value() < 0 && space2.x().value() < 0
            ) return 1;
    }

    { // MEASURES_DERIVED_3_3
        vect1<joules,float> energy1;
        vect3<newtons,float> force3;
        vect3<metres,float> space3;
        vect3<newton_metres,float> torque3;

        energy1 = force3 * space3;
        energy1 = space3 * force3;
        torque3 = cross_product(force3, space3);
        torque3 = cross_product(space3, force3);

        if (energy1.value() < 0
            && force3.x().value() < 0 && space3.x().value() < 0
            && torque3.x().value() < 0) return 1;
    }

    { // MEASURES_DERIVED_SQ_2
        vect1<square_metres,float> area1;
        vect2<metres,float> space2;

        area1 = space2 * space2;
        area1 = sqr(space2);
        area1 = cross_product(space2, space2);

        if (area1.value() < 0
            && space2.x().value() < 0) return 1;
    }

    { // MEASURES_DERIVED_SQ_3
        vect1<square_metres,float> area1;
        vect3<metres,float> space3;
        vect3<square_metres,float> area3;

        area1 = space3 * space3;
        area1 = sqr(space3);
        area3 = cross_product(space3, space3);

        if (area1.value() < 0 && space3.x().value() < 0
            && area3.x().value() < 0) return 1;
    }
    
    {
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
