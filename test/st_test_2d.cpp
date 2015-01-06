#define MEASURES_USE_2D
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
MEASURES_DERIVED_1_2(seconds, metres_per_second, metres)
MEASURES_DERIVED_2_2(newtons, metres, joules, newton_metres)
MEASURES_DERIVED_SQ_2(metres, square_metres, square_metres)

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
        vect2<inches,float> v3 = convert<inches>(
            vect2<metres,float>(2.3f, 2.4f));
        affine_map2<inches,float> v4 = convert<inches>(
            make_translation(vect2<metres,float>(2.3f, 2.4f)));
        point2<inches,float> v5 = convert<inches>(
            point2<metres,float>(2.3f, 2.4f));

        if (v1.value() < 0 && v2.value() < 0 && v3.x().value() < 0
            && v4.coeff(0, 0) < 0 && v5.x().value() < 0) return 1;
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

    { // linear_map2
        linear_map2<float> lm1;
        lm1 = linear_map2<float>::projection(vect2<metres,float>(11.f, 12.f));
        lm1 = make_projection(vect2<metres,float>(11.f, 12.f));
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

    { // vect2
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
        v2 = v3.mapped_by(make_projection(vect2<metres,float>(n, n)));

        b = v2 == v3;
        b = v2 != v3;
        b = is_equal(v2, v3, v1);

        if (b || v11.x().value() == 0) return 1;
    }

    { // affine_map2
        point2<metres,float> fp(23.f, 24.f);
        affine_map2<metres,float> am1;
        am1 = affine_map2<metres,float>::translation(
            vect2<metres,float>(11.f, 12.f));
        am1 = make_translation(vect2<metres,float>(11.f, 12.f));
        am1 = affine_map2<metres,float>::projection(
            fp, vect2<metres,float>(11.f, 12.f));
        am1 = make_projection(
            fp, vect2<metres,float>(11.f, 12.f));
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

    { // point2
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
        p3 = p3.mapped_by(make_projection(p2, vect2<metres,float>(n, n)));

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

    { // cast
        vect1<inches,float> v1 = cast<float>(
            vect1<inches,long double>(2.3f));
        point1<inches,float> v2 = cast<float>(
            point1<inches,long double>(2.3f));
        linear_map2<float> v3 = cast<float>(
            make_projection(vect2<inches,long double>(12, 13)));
        vect2<inches,float> v4 = cast<float>(
            vect2<inches,long double>(2.3f, 2.4f));
        affine_map2<inches,float> v5 = cast<float>(
            make_translation(vect2<inches,long double>(12, 13)));
        point2<inches,float> v6 = cast<float>(
            point2<inches,long double>(2.3f, 2.4f));

        if (v1.value() < 0 && v2.value() < 0 && v3.coeff(0, 0) < 0
            && v4.x().value() < 0 && v5.coeff(0, 0) < 0 && v6.x().value() < 0)
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

    { // MEASURES_DERIVED_SQ_2
        vect1<square_metres,float> area1;
        vect2<metres,float> space2;

        area1 = space2 * space2;
        area1 = sqr(space2);
        area1 = cross_product(space2, space2);

        if (area1.value() < 0
            && space2.x().value() < 0) return 1;
    }
}
