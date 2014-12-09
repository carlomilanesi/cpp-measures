#define MEASURES_USE_3D
#define MEASURES_USE_IOSTREAM
#include "measures.hpp"
#include <sstream>
using namespace measures;
using namespace std;

MEASURES_MAGNITUDE(Length, metres, " m")
MEASURES_UNIT(km, Length, " Km", 1000, 0)
MEASURES_UNIT(cm, Length, " cm", 0.01, 0)

void f2(double, stringstream&);

void f1(double a, stringstream& ss)
{
	vect3<km> m1(a, a + 1, a + 2);
	ss << m1 * 2;
	vect3<metres> m2(km::id(), a, a + 1, a + 2);
	ss << m2 / 4;
	f2(a * 3, ss);
}
