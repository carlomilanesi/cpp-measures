Tutorial
========

Here are step-by-step instructions for using the “measures” library.

First steps
-----------

Put in a directory the files `measures.hpp`, `measures_io.hpp`,
and a C++ source file having the following content:

    #include "measures_io.hpp"
    int main()
    {
        std::cout << measures::vect1<radians>(2.3) * 2 << std::endl;
    }

If you compile and run it, it should output `4.6 rad`.
It means a relative angle of 4.6 radians, obtained multiplying by two
a relative angle of 2.3 radians.
To avoid having to specify namespaces throughout, use the following directives:

    using namespace measures; 
    using namespace std;

Now replace `vect1` with `point1`, obtaining the following file:

    #include "measures_io.hpp"
    using namespace measures;
    using namespace std;
    int main()
    {
        cout << point1<radians>(2.3) * 2 << endl;
    }

It should generate a compilation error, as absolute angles cannot be multiplied.

Now remove the multiplication by 2, so having the following statement:

    cout << point1<radians>(2.3) << endl;

It should print `[2.3] rad`.
It means an absolute angle of 2.3 radians.

Now replace the previous statement with the following:

    cout << point1<radians>(2.3) + point1<radians>(1.4) << endl;

It gives a compilation error, as absolute angles
cannot be added to other absolute angles.

Even the following statement gives a compilation error:

    cout << vect1<radians>(2.3) + point1<radians>(1.4) << endl;

It is because absolute measures (i.e. points) cannot be added
to relative measures (i.e. vects).

Instead, the following statement is valid:

    cout << point1<radians>(2.3) + vect1<radians>(1.4) << endl;

Actually you can add a relative measure to an absolute measure of the same unit.
It should print `[3.7] rad`, meaning that the first absolute angle
has been incremented by a relative value,
so obtaining an absolute value of 3.7 radians.

And if you write the following statement:

    cout << point1<radians>(2.3) - point1<radians>(1.4) << endl;

You can compile and run it, and it should print `0.9 rad`,
meaning that to go from the second absolute angle to the first one,
you need to increment it by a relative angle of 0.9 radians.

Putting it all together, a point may be incremented
ore decremented by a vect, obtaining another point.
An the difference between two points may be computed,
obtaining a vect;
instead, a vect may be added to or subtracted from
another vect, and it may be multiplied or divided by a number,
obtaining another vect in all these cases.

These operazions are available also as assignements,
like in the following valid program:

    #include "measures_io.hpp"
    using namespace measures;
    using namespace std;
    int main()
    {
        point1<radians> p(2.3);
        vect1<radians> v(1.8);
        p += v;
        cout << p << endl;
        v /= 3;
        cout << p << endl;
    }

It should print first `[4.1] rad`, and then `0.6 rad`.

Definition of magnitudes and units
----------------------------------

Up to now, we used only angles and only radians,
because they are respectively the only magnitude
and the only unit predefined by the *measures* library.

But you can, and should, define other magnitudes and units though.

Just before the main line, add the following line:

    DEFINE_MAGNITUDE(Length, metres, " m")

This statement calls a macro that defines two classes:
`Length`, to be used as a physical magnitude,
and `metres` to be used as the *base* (or *default*)
unit of measurement of such magnitude.

This statement defines also `" m"` as the suffix to use when printing measures
in metres for debugging or logging purposes.

Before, we could handle angles measured in radians,
because the core library already defined the `Angle` magnitude,
its base unit as `radians`, and its suffix as `" rad"`.

As you can see, we use Pascal naming convention (like `ThisIsAName`)
for magnitudes, and snake naming convention (like `this_is_a_name`)
for all other identifiers.

After having defined in the above way a new magnitude and its base unit,
you can use them, writing the following program:

    #include "measures_io.h"
    using namespace measures;
    using namespace std;
    DEFINE_MAGNITUDE(Length, metres, " m")
    int main()
    {
        cout << vect1<metres>(2.3) * 2 << endl;
        cout << point1<radians>(7) << endl;
    }

It should print:

    4.6 m
    7 rad

Conversions
-----------

Multidimensional measures
-------------------------

Azimuths
--------

Dynamically-specified units
---------------------------

operazioni derivate
