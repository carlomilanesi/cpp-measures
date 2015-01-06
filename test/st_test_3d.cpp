#define MEASURES_USE_3D
#include "measures.hpp"
using namespace measures;

MEASURES_MAGNITUDE(Space, metres, " m")
MEASURES_MAGNITUDE(Area, square_metres, " m2")
MEASURES_MAGNITUDE(Force, newtons, " N")
MEASURES_MAGNITUDE(Energy, joules, " J")
MEASURES_MAGNITUDE(Torque, newton_metres, " Nm")
MEASURES_MAGNITUDE(Time, seconds, " s")
MEASURES_MAGNITUDE(Speed, metres_per_second, " m/s")
MEASURES_UNIT(inches, Space, " in", 0.0254, 0)
MEASURES_UNIT(square_inches, Area, " in2", 0.0254*0.0254, 0)
MEASURES_DERIVED_SQ_1(inches, square_inches)
MEASURES_DERIVED_1_1(seconds, metres_per_second, metres)
MEASURES_DERIVED_1_3(seconds, metres_per_second, metres)
MEASURES_DERIVED_3_3(newtons, metres, joules, newton_metres)
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

    { // MEASURES_MAGNITUDE
        Space u1 = Space::base_unit::id();
        Space u2(&inches_features_);
        char const* suf = u2.suffix();
        long double rat = u2.ratio();
        long double off = u2.offset();
        if (u1.offset() < 0 || suf == 0 || rat == 0 || off < 0) return 1;
    }

    { // convert
        vect1<inches,float> v1 = convert<inches>(
            vect1<metres,float>(2.3f));
        point1<inches,float> v2 = convert<inches>(
            point1<metres,float>(2.3f));
        vect3<inches,float> v6 = convert<inches>(
            vect3<metres,float>(2.3f, 2.4f, 2.5f));
        affine_map3<inches,float> v7 = convert<inches>(
            make_translation(vect3<metres,float>(2.3f, 2.4f, 2.5f)));
        point3<inches,float> v8 = convert<inches>(
            point3<metres,float>(2.3f, 2.4f, 2.5f));

        if (v1.value() < 0 && v2.value() < 0 && v6.x().value() < 0
            && v7.coeff(0, 0) < 0 && v8.x().value() < 0) return 1;
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

    { // linear_map3
        vect3<metres,float> unit_v(10.f, 11.f, 12.f);
        linear_map3<float> lm1;
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

    { // vect3
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

        v2 = v2.mapped_by(make_projection_onto_line(v3));

        b = v2 == v3;
        b = v2 != v3;
        b = is_equal(v2, v3, v1);

        if (b || v11.x().value() < 0) return 1;
    }

    { // affine_map3
        point3<metres,float> fixed_p(23.f, 24.f, 25.f);
        vect3<metres,float> unit_v(10.f, 11.f, 12.f);

        affine_map3<metres,float> am1;
        am1 = affine_map3<metres,float>
            ::translation(vect3<metres,float>(11.f, 12.f, 13.f));
        am1 = make_translation(vect3<metres,float>(11.f, 12.f, 13.f));
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

    { // point3
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

        p2 = p2.mapped_by(make_projection_onto_line(p2, v2));

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

    { // cast
        vect1<inches,float> v1 = cast<float>(
            vect1<inches,long double>(2.3f));
        point1<inches,float> v2 = cast<float>(
            point1<inches,long double>(2.3f));
        linear_map3<float> v7 = cast<float>(
            make_projection_onto_line(vect3<inches,long double>(12, 13, 14)));
        vect3<inches,float> v8 = cast<float>(
            vect3<inches,long double>(2.3f, 2.4f, 2.5f));
        affine_map3<inches,float> v9 = cast<float>(
            make_translation(vect3<inches,long double>(12, 13, 14)));
        point3<inches,float> v10 = cast<float>(
            point3<inches,long double>(2.3f, 2.4f, 2.5f));

        if (v1.value() < 0 && v2.value() < 0
            && v7.coeff(0, 0) < 0 && v8.x().value() < 0 && v9.coeff(0, 0) < 0
            && v10.x().value() < 0)
        {
            return 1;
        }
    }

    { // MEASURES_DERIVED_1_1
        vect1<seconds,float> time1;
        vect1<metres_per_second,float> angspeed1;
        vect1<metres,float> angle1;

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
   
}
