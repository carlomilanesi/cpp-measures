// cpp-benchmark

// criteria
// test the performance of float, double, complex<long double>
// test the performance of conversions to the same unit, and to different unit
// test the performance of casts to the same numeric type, and to different numeric type
// test the performance of relative and absolute temperature conversion
#include "measures_io.hpp"
#include <ctime>
#include <complex>
#include <cstdlib>
#include <cstring>
using namespace measures;
using namespace std;

DEFINE_MAGNITUDE(Temperature, kelvin, "^K")
DEFINE_UNIT(celsius, Temperature, "^C", 1, 273.15) 
DEFINE_UNIT(fahrenheit, Temperature, "^F", 5. / 9, 273.15 - 32 * 5 / 9)

#if 0
template <template <class Unit, typename Num> class Measure, class Unit1, class Unit2, typename Num>
void measures_conversion(int j)
{
    int const size = 1000;
    static Measure<Unit1,Num> cells1[size];
    static Measure<Unit2,Num> cells2[size];
    if (j < 0)
    {
        srand(0);
        for (int i = 0; i < size; ++i)
        {
            cells1[i] = Measure<Unit1,Num>(rand());
        }
    }
    for (int i = 0; i < size; ++i)
    {
        cells2[i] += (convert<Unit2>(cells1[size - 1 - i]) - cells2[i]) * Num(0.01 * j);//1030
    }
/*
    for (int i = size; i >= 0; --i)
    {
        if (convert<Unit2>(cells2[i]).value() == Num(-1e9)) exit(1);
    }
*/
}

template <class Unit1, class Unit2, typename Num>
void native_conversion_vect1(int j)
{
    int const size = 1000;
    static Num cells1[size];
    static Num cells2[size];
    if (j < 0)
    {
        srand(0);
        for (int i = 0; i < size; ++i)
        {
            cells1[i] = static_cast<Num>(rand());
        }
    }
    for (int i = 0; i < size; ++i)
    {
        cells2[i] += (cells1[size - 1 - i] * Num(Unit1::ratio() / Unit2::ratio()) - cells2[i]) * Num(0.01 * j);
        //cells1[i] = cells2[size - 1 - i];
    }
/*
    for (int i = size; i >= 0; --i)
    {
        if (cells2[i] * Num(Unit2::ratio() / Unit1::ratio()) == Num(-1e9)) exit(1);
    }
*/
}

template <class Unit1, class Unit2, typename Num>
void native_conversion_point1(int j)
{
    int const size = 1000;
    static Num cells1[size];
    static Num cells2[size];
    if (j < 0)
    {
        srand(0);
        for (int i = 0; i < size; ++i)
        {
            cells1[i] = static_cast<Num>(rand());
        }
    }
    for (int i = 0; i < size; ++i)
    {
        cells2[i] += (cells1[size - 1 - i] * Num(Unit1::ratio() / Unit2::ratio())
            + Num((Unit1::offset() - Unit2::offset()) / Unit2::ratio()) - cells2[i]) * Num(0.01 * j);
    }
    for (int i = size; i >= 0; --i)
    {
        if (cells2[i] * Num(Unit2::ratio() / Unit1::ratio())
            + Num((Unit2::offset() - Unit1::offset()) / Unit1::ratio()) == Num(-1e9)) exit(1);
    }
}

/*
test_point1_conversion<celsius,celsius,float>
test_point1_conversion<celsius,celsius,double>
test_point1_conversion<celsius,celsius,complex<long double>>
test_point1_conversion<celsius,fahrenheit>
test_vect1_conversion<celsius,celsius>
test_vect1_conversion<celsius,fahrenheit>
test_point3_cast<double,double>
test_point3_cast<double,float>
test_vect3_cast<double,double>
test_vect3_cast<double,float>
*/

int main()
{
    int const n_iter = 1000000;
    int const n_iterc = 100000;
    
/*
    // point1, same unit, float
    {
        measures_conversion<point1,celsius,celsius,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            measures_conversion<point1,celsius,celsius,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }
    {
        native_conversion_point1<celsius,celsius,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            native_conversion_point1<celsius,celsius,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }

    // point1, different unit, float
    {
        measures_conversion<point1,celsius,fahrenheit,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            measures_conversion<point1,celsius,fahrenheit,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }
    {
        native_conversion_point1<celsius,fahrenheit,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            native_conversion_point1<celsius,fahrenheit,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }
*/

    // vect1, same unit, float
    {
        measures_conversion<vect1,celsius,celsius,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            measures_conversion<vect1,celsius,celsius,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }
    {
        native_conversion_vect1<celsius,celsius,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            native_conversion_vect1<celsius,celsius,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }
    
/*
    // vect1, different unit, float
    {
        measures_conversion<vect1,celsius,fahrenheit,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            measures_conversion<vect1,celsius,fahrenheit,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }
    {
        native_conversion_vect1<celsius,fahrenheit,float>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iter; ++i)
        {
            native_conversion_vect1<celsius,fahrenheit,float>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
    }

    {
        measures_conversion<point1,celsius,celsius,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            measures_conversion<point1,celsius,celsius,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }
    {
        native_conversion_point1<celsius,celsius,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            native_conversion_point1<celsius,celsius,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }

    {
        measures_conversion<point1,celsius,fahrenheit,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            measures_conversion<point1,celsius,fahrenheit,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }
    {
        native_conversion_point1<celsius,fahrenheit,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            native_conversion_point1<celsius,fahrenheit,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }

    {
        measures_conversion<vect1,celsius,celsius,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            measures_conversion<vect1,celsius,celsius,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }
    {
        native_conversion_vect1<celsius,celsius,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            native_conversion_vect1<celsius,celsius,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }

    {
        measures_conversion<vect1,celsius,fahrenheit,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            measures_conversion<vect1,celsius,fahrenheit,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }
    {
        native_conversion_vect1<celsius,fahrenheit,complex<long double>>(-1);
        clock_t start = clock();
        for (int i = 0; i < n_iterc; ++i)
        {
            native_conversion_vect1<celsius,fahrenheit,complex<long double>>(i);
        }
        cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iterc << " ns per iteration." << endl;
    }
*/
}
#endif

DEFINE_MAGNITUDE(Space, mm, " mm")
DEFINE_MAGNITUDE(Area, mm2, " mm2")
DEFINE_DERIVED_UNIT_SQUARED_SCALAR(mm, mm2)

bool is_equal(double a, double b, double tol)
{
    return std::abs(a - b) <= tol;
}

int const size = 10;

void bench(double A[size][size], double B[size][size], double C[size][size])
{
    double D[size][size];
        for (int j = 0; j < size; ++j)
    {
    for (int i = 0; i < size; ++i)
        {
            double sum = 0;
            for (int k = 0; k < size; ++k)
            { sum += A[i][k] * B[k][j]; }
            D[i][j] = sum;
        }
    }
        for (int j = 0; j < size; ++j)
    {
    for (int i = 0; i < size; ++i)
        {
            B[i][j] = sqrt(D[i][j] + C[i][j] * 3);
        }
    }
    double tol = 0.2;
    int n_near = 0;
        for (int j = 0; j < size; ++j)
    {
    for (int i = 0; i < size; ++i)
        {
            if (is_equal(A[i][j], B[i][j], tol)) ++n_near;
        }
    }
    //cout << n_near << endl;
    if (n_near == 100) exit(1);
}

void bench_measure(vect1<mm> A[size][size], vect1<mm> B[size][size], vect1<mm2> C[size][size])
{
    vect1<mm2> D[size][size];
        for (int j = 0; j < size; ++j)
    {
    for (int i = 0; i < size; ++i)
        {
            vect1<mm2> sum(0);
            for (int k = 0; k < size; ++k)
            { sum += A[i][k] * B[k][j]; }
            D[i][j] = sum;
        }
    }
        for (int j = 0; j < size; ++j)
    {
    for (int i = 0; i < size; ++i)
        {
            B[i][j] = sqrt(D[i][j] + C[i][j] * 3);
        }
    }
    vect1<mm> tol(0.2);
    int n_near = 0;
        for (int j = 0; j < size; ++j)
    {
    for (int i = 0; i < size; ++i)
        {
            if (is_equal(A[i][j], B[i][j], tol)) ++n_near;
        }
    }
    //cout << n_near << endl;
    if (n_near == 100) exit(1);
}

int main()
{
    double A[size][size];
    double B[size][size];
    double C[size][size];
    srand(0);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            A[i][j] = rand() / (double)RAND_MAX;
            B[i][j] = rand() / (double)RAND_MAX;
            C[i][j] = rand() / (double)RAND_MAX;
        }
    }
    int const n_iter = 2000000;
    clock_t start = clock();
    for (int iter = 0; iter < n_iter; ++iter)
    {
///* // cl:1330 gcc:1200  gcc:180 cl:175 - gcc:905 cl:821
// gcc:1240 1209 cl:1299 1292
        double A2[size][size];
        double B2[size][size];
        double C2[size][size];
        memcpy(A2, A, sizeof A);
        memcpy(B2, B, sizeof B);
        memcpy(C2, C, sizeof C);
        bench(A2, B2, C2);
//*/
/* // cl: 1920 gcc: 1300  - gcc:413 cl:854 - gcc:905 cl: 883
// gcc:1326 1318 1201 cl:1794 1833
        vect1<mm> A2[size][size];
        vect1<mm> B2[size][size];
        vect1<mm2> C2[size][size];
        memcpy(A2, A, sizeof A);
        memcpy(B2, B, sizeof B);
        memcpy(C2, C, sizeof C);
        bench_measure(A2, B2, C2);
*/
    }
    cout << (clock() - start) * 1e9 / CLOCKS_PER_SEC / n_iter << " ns per iteration." << endl;
}
