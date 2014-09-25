#include "measures_io.hpp"
using namespace measures;

#include <iostream>
using namespace std;

DEFINE_MAGNITUDE(Length, meters, " m")
DEFINE_UNIT(km, Length, " Km", 1000, 0)
DEFINE_UNIT(inches, Length, "\"", 0.0254, 0)

void f2(int);

void f1(int n)
{
	cout << "Inizio f1" << endl;
	dyn_vect1<Length> v1(km::id(), 4);
	cout << v1 << endl;
	cout << convert<>(meters::id(), v1) << endl;
	cout << convert<>(km::id(), v1) << endl;
	cout << convert<>(inches::id (), v1) << endl;
	if (n > 0) f2(n - 1);
	cout << "Fine f1" << endl;
}

void g1(dyn_vect1<Length> v)
{
	cout << v << endl;
}

void g2(dyn_vect1<Length> v);

void f3()
{
	g1(dyn_vect1<Length>(km::id(), 8));
	g2(dyn_vect1<Length>(km::id(), 9));
	cout << endl << (Length(1) == meters::id()) << endl;
	cout << (Length(1) == km::id()) << endl;
	cout << (Length(1) == inches::id()) << endl << endl;
		cout << (Length(2) == meters::id()) << endl;
	cout << (Length(2) == km::id()) << endl;
	cout << (Length(2) == inches::id()) << endl << endl;
	cout << (Length(3) == meters::id()) << endl;
		cout << (Length(3) == km::id()) << endl;
	cout << (Length(3) == inches::id()) << endl << endl;
	cout << (Length(4) == meters::id()) << endl;
	cout << (Length(4) == km::id()) << endl;
		cout << (Length(4) == inches::id()) << endl << endl;
}
