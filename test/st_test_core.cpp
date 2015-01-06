#include "measures.hpp"
using namespace measures;

MEASURES_MAGNITUDE(Space, metres, " m")
MEASURES_MAGNITUDE(Area, square_metres, " m2")
MEASURES_MAGNITUDE(Time, seconds, " s")
MEASURES_MAGNITUDE(Speed, metres_per_second, " m/s")
MEASURES_UNIT(inches, Space, " in", 0.0254, 0)
MEASURES_UNIT(square_inches, Area, " in2", 0.0254*0.0254, 0)
MEASURES_DERIVED_SQ_1(inches, square_inches)
MEASURES_DERIVED_1_1(seconds, metres_per_second, metres)

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

        if (v1.value() < 0 && v2.value()) return 1;
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

    { // cast
        vect1<inches,float> v1 = cast<float>(
            vect1<inches,long double>(2.3f));
        point1<inches,float> v2 = cast<float>(
            point1<inches,long double>(2.3f));

        if (v1.value() < 0 && v2.value() < 0) return 1;
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
}
