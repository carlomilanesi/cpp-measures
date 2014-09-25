#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

/*
void f1(int);
void f2(int);
void f3();
void f4();
*/

const double const_pi() { return std::atan(1) * 4; } // 2.7, 2.8
//double pi = std::atan(1) * 4; // 2.7, 2.8
const double pi = std::atan(1) * 4; // 2.7, 2.8
const double pix(int ii) { return std::atan(1) * ii; } // 2.7, 2.8
// empty: 1.4

int main()
{
//	f1(1);
//	f2(1);
//	f3();
//	f4();
	clock_t start = clock();
	for (int i = 0; i < 2000000000; ++i)
//		if (const_pi() * i < 0) return 1;
//		if (pi * i < 0) return 1;
//		if (pix(i) < 0) return 1;
		if (i < 0) return 1;
	cout << (clock() - start) * 1.0 / CLOCKS_PER_SEC << " s" << endl;
	return 0;
}
