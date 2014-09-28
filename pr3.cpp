#include "measures_io.hpp"
using namespace measures;

#include <iostream>
using namespace std;

DEFINE_MAGNITUDE(Length, meters, " m")
DEFINE_UNIT(inches, Length, "\"", 0.0254, 0)
DEFINE_UNIT(km, Length, " Km", 1000, 0)

void f2(double a)
{
	vect3<inches> m1(a, a - 1, a - 2);
	cout << m1 / 2.0 << endl;
	dyn_vect3<Length> m2(inches::id(), a, a - 1, a - 2);
	cout << m2 / 2.0 << endl;
}
