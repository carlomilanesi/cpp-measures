#include "measures.hpp"
using namespace measures;

DEFINE_MAGNITUDE(Length, meters, " m")
DEFINE_UNIT(mm, Length, " mm", 0.001, 0)
DEFINE_MAGNITUDE(Area, square_meters, " m2")
DEFINE_UNIT(square_mm, Area, " mm2", 0.000001, 0)
DEFINE_DERIVED_MAGNITUDE_SQUARED_VECTOR(Length, Area)
DEFINE_DERIVED_UNIT_SQUARED_VECTOR(mm, square_mm)

#include <ctime>
#include <cstdio>
using namespace std;

int main()
{
	clock_t start;
	const int n_iterations = 2000000000;
	const double dyn_factor = 6.0;

	printf("> ");
	start = clock();
	for (int i1 = 0; i1 < int(n_iterations / dyn_factor); ++i1)
	{
		dyn_vect2<Length> a(mm::id(), i1, i1 + 1);
		dyn_vect2<Length> b(mm::id(), i1 + 2, i1 + 3);
		if ((a * b).value() < 0) return 1;
	}
	printf("%f ns per iteration\n", (clock() - start) * 1.e9 / (n_iterations / dyn_factor) / CLOCKS_PER_SEC);
	
	printf("> ");
	start = clock();
	for (int i1 = 0; i1 < n_iterations; ++i1)
	{
		vect2<mm> a(i1, i1 + 1);
		vect2<mm> b(i1 + 2, i1 + 3);
		if ((a * b).value() < 0) return 1;
	}
	printf("%f ns per iteration\n", (clock() - start) * 1.e9 / n_iterations / CLOCKS_PER_SEC);
	
	printf("> ");
	start = clock();
	for (int i1 = 0; i1 < n_iterations; ++i1)
	{
		double ax = i1;
		double ay = i1 + 1;
		double bx = i1 + 2;
		double by = i1 + 3;
		if (ax * bx + ay * by < 0) return 1;
	}
	printf("%f ns per iteration\n", (clock() - start) * 1.e9 / n_iterations / CLOCKS_PER_SEC);
}
