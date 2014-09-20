Tutorial
========

Here are step-by-step instructions for using the “measures” library.

Absolute and relative measures
------------------------------

Put in a directory the files `measures.hpp`, `measures_io.hpp`,
and create a C++ source file having the following content:

    #include "measures_io.hpp"
    int main()
    {
        std::cout << measures::vect1<measures::radians>(2.3) * 2 << std::endl;
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
Notice that absolute values are output surrounded by square brackets.

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
It should output `[3.7] rad`, meaning that the first absolute angle
has been incremented by a relative value,
so obtaining an absolute value of 3.7 radians.

And the following statement:

    cout << point1<radians>(2.3) - point1<radians>(1.4) << endl;

can be compiled and run, outputting `0.9 rad`,
meaning that to go from the second absolute angle to the first one,
you need to increment it by a relative angle of 0.9 radians.

Putting it all together, a `point` may be incremented
or decremented by a `vect`, obtaining another `point`.
An the difference between two `point`s may be computed,
obtaining a `vect`;
instead, a `vect` may be added to or subtracted from
another `vect`, and it may be multiplied or divided by a number,
obtaining another `vect` in all these cases.

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
        cout << v << endl;
    }

It should output first `[4.1] rad`, and then `0.6 rad`.

Definition of magnitudes and units
----------------------------------

Up to now, we used only the angle physical magnitude
and only the radians unit of measurement,
because they are respectively the only magnitude
and the only unit predefined by the *measures* library.

But you can, and should, define other magnitudes and units though.

Just before the main line, add the following line:

    DEFINE_MAGNITUDE(Length, metres, " m")

This statement calls a macro that defines two classes:
`Length`, to be used as a physical magnitude,
and `metres` to be used as the *base* (or *default*)
unit of measurement of such magnitude.

This statement defines also `" m"` as the suffix to use when outputting
measures in metres for debugging or logging purposes.

In the previous section, we could handle angles measured in radians,
because the core library already defined the `Angle` magnitude,
its `radians` base unit, and its `" rad"` suffix.

As you can see, we use Pascal naming convention (like `ThisIsAName`)
for magnitudes, and snake naming convention (like `this_is_a_name`)
for all other identifiers, but it is not required.

After having defined in the above way a new magnitude and its base unit,
you can use them, writing the following program:

    #include "measures_io.hpp"
    using namespace measures;
    using namespace std;
    DEFINE_MAGNITUDE(Length, metres, " m")
    int main()
    {
        cout << vect1<metres>(2.3) * 2 << endl;
        cout << point1<radians>(7) << endl;
    }

It should output:

    4.6 m
    [7] rad

If you prefer, you can define magnitudes and units in other languages,
as long as they are valid C++ class names.
For example, to define the speed magnitude, with miles per hour
as its base unit, a Spanish language speaker could write:

    DEFINE_MAGNITUDE(Velocidad, millas_por_hora, " millas/h")

As you can see, you are not constrained to use SI units.

If you need more than one unit for a magnitude,
you can add them in addition to the one already defined
with the magnitude.

For example, the following program adds two length units:
inches and centimetres. 

    #include "measures_io.hpp"
    using namespace measures;
    using namespace std;
    DEFINE_MAGNITUDE(Length, kilometres, " Km")
    DEFINE_UNIT(inches, Length, "\"", 0.0000254, 0)
    DEFINE_UNIT(centimetres, Length, " cm", 0.00001, 0)
    int main()
    {
        cout << vect1<kilometres>(3) << endl;
        cout << vect1<inches>(4) << endl;
        cout << point1<centimetres>(5) << endl;
    }

It should output:

    3 Km
    4"
    [5] cm

The `DEFINE_UNIT` macro creates the C++ corresponding to its first
argument, and references the class corresponding to its second argument.
To specify a non-existent magnitude as as second argument
generates a compilation error.

The `DEFINE_UNIT` macro has three other arguments: the suffix, the factor,
and the offset.
The suffix is just the same of the `DEFINE_MAGNITUDE` macro.
The last two arguements are need to convert a measure from a unit
to another unit of the same magnitude.

Remember that the *measures* library has no knowledge of which units
of measurement exists (except for radians), let alone to how to convert
measures between them.

The only notion wired in the library is that for every couple of units
of the same magnitude there is a ratio and possibly an offset.
For example, one kilometre is 1000 times as long as a metre,
while an inch is 2.54 times as long as a centimetre.

The way the measure library specify ratios between units is by specifying
how many base units are contained in the unit that is in course
of definition.
For example, having defined kilometres as the base unit of length,
to define a ratio, or factor, for the newly defined inches unit,
it is required to specify how many kilometres is long an inch,
or, put in another way, how many kilometres are contained in an inch.
Therefore, the required value is 0.0000254.

For centimers, the reasoning is the same, giving that in a centimetre
there are 0.00001 kilometres.

For most measures, a ratio is all that is needed to make unit conversions,
but not for all.
Thermometric scales do not differ only for the size of a degree,
but also for the zero point.
The Celsius and the Kelvin scales have the same degree size,
and therefore a ratio of 1, but they differ because the temperature
of 0°C corresponds to 273.15°K.

Therefore another information is required: the offset.
Here is how to define Kelvin degrees as the base temperature unit,
and the Celsius and the Fahrenheit degrees as secondary units.

    DEFINE_MAGNITUDE(Temperature, kelvin, "°K")
    DEFINE_UNIT(celsius, Temperature, "°C", 1, 273.15)
    DEFINE_UNIT(fahrenheit, Temperature, "°F", 5./9., 273.15-32.*5./9.)

The last argument of the `DEFINE_UNIT` macro must specify the position
of the origin (a.k.a. *zero point*) of the new unit,
with respect to the origin of the base unit, specified using
the base unit.
For example, the offset of the `fahrenheit` unit is defined using a formula
whose value equals arroximaively 255.37, as the temperature of 0°F equals
the temperature of 255.37°K.

For all the units whose origin is the same as the base units,
the offset must be zero.

Conversions
-----------

Multidimensional measures
-------------------------

Azimuths
--------

Dynamically-specified units
---------------------------

operazioni derivate
