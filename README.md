cpp-measures
============

Header-only C++ library to handle physical measures

This library is aimed at who writes C++14 software, which handles values provided of unit of measurement, like lengths measured in meters, inches, or kilometers, durations measured in seconds, hours, or days, angles measured in degrees, radians, or turns, but also derived units, like revolutions per minute.
Such library is useful for:
- making code more readable;
- making code more concise;
- spotting logic errors at compile-time;
- easing unit conversions.
Two kinds of measures are handled:
- Values having a unit of mesurement defined at compile-time (for example, when a programmer defines that an expression represents a length in millimeters).
- Values having a physical magnitude defined at compile-time, but unit of measurement defined at run-time (for example, when a programmer defines that an expression represents a length that could be measured in millimeters, in centimeters, or in inches, and at run-time one of those three units will be assigned to it).
The first kind of measures guarantees virtually zero-runtime-overhead, with respect to using primitive types, provided an optimized compilation is performed.

Let's see some example.

To define a variable named "a", representing a length of 6 millimeters, and keeping it in a "double" primitive object, it can be written:

    vect1<mm,double> a(6);

"vect" indicates that such value is a mathematical vector, i.e. it may be added to or subtracted from another object of the same type, and it may be multiplied or divided by a number.
"1" indicates that such measure in monodimensional.
"mm" is the unit of measurement.

To define a variable named "b" representing a point in 3D space having cartesian coordinates x=7, y=8, z=9, expressed in inches, and stored in three "double" primitive objects, it can be written:

    point3<inches,double> a(7, 8, 9);

"point" indicates that such measure is an element of an affine space and not of a vector space, i.e. it cannot be added to or subtracted from another object of the same type, and it cannot be multiplied or divided by a number.

In mechanics, the physical work performed by a constant force moved by a displacement is by definition the dot product between such force and such displacement.
Assuming we are constrained in a plane, and so we use only 2D vectors, we can write:

    vect1<joule> work = vect2<newton>(12, 13) * vect2<meters>(3, 5);

Here, no numeric type is specified, and the default "double" is used.
The "*" operator performs the dot product.
The type "vect2<newton>" represents a force laying in a plane, measured in Newtons.
The type "vect2<meters>" represents a displacement in a plane, measured in meters.
The resulting value should be a scalar (i.e. one-dimensional vect), measured in Joule.
I wrote "should" as such units of measurement are not part of the library, but are defined by the application programmer according with needs.

To convert between thermometric scales, the following statements can be written:

    vect1<celsius> t = convert<celsius>(vect1<fahrenheit>(90));
    point1<celsius> t = convert<celsius>(point1<fahrenheit>(90));

The first one of such statements converts temperature changes, and therefore it does not takes into account the difference between the scale zeros, but only the ratio between the degrees. The resulting value is a change of 50 Celsius degrees.
Conversely, the second statement converts temperature points, and therefore it takes into account both the ratio between the degrees and the difference between the scale zeros. The resulting value is a temperature of 32.222 Celsius degrees.

Particular attention is devoted to angles.
Actually, differing from other magnitudes and units, that must be defined by the application programmer,
the Angle magnitude is predefined, as two of its units are, turns and radians.
In addition, beyond arbitrary-valued angles, both of "vect1" and of "point1" kind, the following two statements may be written:

    signed_azimuth<radians> a(2);
    unsigned_azimuth<radians> b(3);

Both of them define angular positions modulo one turn, i.e. plane directions, i.e. positions on a circumpherence, but the first statement forces its value to lay between minus half turn and plus half turn, and so it is "signed", while the second statement forces its value to lay between zero and one turn, and so it is "unsigned".

At last, measures whose unit of measurement is defined only at run-time may de defined, with statements like the following ones:

    dyn_vect1<Temperature> a(celsius::id(), 5);
    dyn_vect3<Length> b(celsius::mm(), 6, 7, 8);

"a" at compile-time represents only a temperature change, and at run-time it gets the value of 5 Celsius degrees.
"b" at compile-time represents only a displacement in 3D space, and at run-time it gets the values x=6, y=7, z=8 millimeters.
Such kind of measures are much less efficient than the static ones, but they are required when the unit of measurement is not known at compile-time.

Here is a benchmark, made using Visual C++ for Windows, both with no optimizations and with full standard optimizations (/Ox).
Using native types, the loop body is:

    double ax = i, ay = i + 1;
    double bx = i + 2, by = i + 3;
    if (ax * bx + ay * by < 0) return 1;

The meaning of such code is that two 2D vectors are created using the loop counter "i"; the dot product of such vectors is computed; and its value is compared to zero (even if it is always positive).

In one computer, the times per iteration are:
- 3.4 ns with optimizations.
- 9 ns without optimizations;

Using the library with compile-time units, such code becomes:

    vect2<mm> a(i, i + 1);
    vect2<mm> b(i + 2, i + 3);
    if ((a * b).value() < 0) return 1;

The times per iteration are:
- 3.4 ns with optimizations.
- 76 ns without optimizations;

Using the library with run-time units, the code becomes:

    dyn_vect2<Length> a(mm::id(), i, i + 1);
    dyn_vect2<Length> b(mm::id(), i + 2, i + 3);
    if ((a * b).value() < 0) return 1;

The times per iteration are:
- 18 to 20 ns with optimizations.
- 440 to 660 ns without optimizations;

Some comments.
When optimizations are enabled, double and vect1 have the same performance, while dyn_vect1 takes 6 times as much time.
When optimizations are disabled, the slowdown factor is only 2.6 for doubles, while it is around 22 for both vect1s and dyn_vect1s.
