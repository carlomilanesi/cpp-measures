#include <sstream>
#include <cassert>
using namespace std;

void f1(double, stringstream&);
void f2(double, stringstream&);

int main()
{
    stringstream ss;
	f1(4, ss);
	f2(25, ss);
    assert(ss.str() ==
        "8 10 12 Km"
        "1000 1250 1500 m"
        "6 5.5 5 Km"
        "6000 5500 5000 m"
        "12.5 12 11.5 Km"
        "12500 12000 11500 m"
        );
}
