#include "measures_io.hpp"
#include <sstream>
using namespace measures;
using namespace std;

MEASURES_MAGNITUDE(Length, metres, " m")
MEASURES_UNIT(cm, Length, " cm", 0.01, 0)
MEASURES_UNIT(km, Length, " Km", 1000, 0)

void f2(double a, stringstream& ss)
{
	vect3<km> m1(a, a - 1, a - 2);
	ss << m1 / 2.0;
	vect3<metres> m2(km::id(), a, a - 1, a - 2);
	ss << m2 / 2.0;
}
