# cpp-measures

Header-only C++ library to handle physical measures

**License: This project is released under the Mozilla Public License 2.0.**

## Purpose

This library is aimed at those who use C++11 language
to write software which uses values provided of unit of measurement,
like, for example, lengths measured in meters, inches, or kilometers,
durations measured in seconds, hours, or days,
angles measured in degrees, radians, or turns,
but also derived units, like revolutions per minute.

Specifically, it addresses software for technical drafting (CAD),
engineering simulation (CAE), industrial automation planning (CAM),
embedded or real-time industrial automation (CNC), and also
several kinds of vector graphics applications, like computer games.

On the other side, this library does not address:
* Commercial/financial systems.
* Systems of advanced theoretical physics modeling.
* Physics learning environments.
* Video-editing and image-editing systems.
* Graphical-User-Interface handling.

Such library is useful for:
* making code more readable
  (i.e. self-explanatory and self-documented);
* making code more concise;
* spotting logic errors at compile-time;
* easing unit conversions;
* avoiding the run-time overhead added by other similar libraries.

The main features of this library are shown in the examples.

## Limitations

This library has the following limitations by design:
* The only predefined magnitude is `Angle`,
  and the only predefined unit of measurement is `radians`.
  The application programmers should define
  every other magnitude and every other unit of measurement
  that are needed by their applications,
  even if that is easy and well documented.
* No more than three dimensions are supported for points and vectors.
* Only monometric orthogonal cartesian coordinate systems are supported,
  i.e. (X,Y) and (X,Y,Z) coordinates having the same unit.
* Non-decimal measures (like hours + minutes) are not supported.
* End-user-defined magnitudes and units are not supported
  (but application-programmer-defined magnitudes and units are supported,
  nd end-user-chosen units are supported).

## Examples of use

To define a variable named "a", representing a length of 6 millimeters,
and keeping it in a "double" primitive object, it can be written:

    vect1<mm,double> a(6);

"vect" indicates that such value is a mathematical vector,
i.e. it may be added to or subtracted from another object of the same type,
and it may be multiplied or divided by a number.
"1" indicates that such measure in monodimensional.
"mm" is the unit of measurement.

To define a variable named "b" representing a point in 3D space having
cartesian coordinates x=7, y=8, z=9, expressed in inches,
and stored in three "double" primitive objects, it can be written:

    point3<inches,double> b(7, 8, 9);

"point" indicates that such measure is an element of an affine space,
and not of a vector space, i.e. it cannot be added to or subtracted
from another object of the same type,
and it cannot be multiplied or divided by a number.

In mechanics, the physical work performed by a constant force moved
by a displacement is by definition the dot product between such force
and such displacement.
Assuming we are constrained in a plane, we can write:

    vect1<joules> work = vect2<newtons>(12, 13) * vect2<metres>(3, 5);

Here, no numeric type is specified, and so the default "double" is used.
The "*" operator performs the dot product.
The type "vect2<newtons>" represents a force laying in a plane,
measured in Newtons.
The type "vect2<metres>" represents a displacement in a plane,
measured in metres.
The resulting value should be a scalar (i.e. a one-dimensional vect),
measured in Joules.
I wrote "should" as such units of measurement are not
part of the library, but are defined by the application programmer
according with the needs of the application software.

To convert between thermometric scales,
the following statements can be written:

    vect1<celsius> t = convert<celsius>(vect1<fahrenheit>(90));
    point1<celsius> t = convert<celsius>(point1<fahrenheit>(90));

The first one of such statements converts temperature changes,
and therefore it does not takes into account the difference
between the scale zeros, but only the ratio between the degrees.
The resulting value is a change of 50 Celsius degrees.

Conversely, the second statement converts temperature points,
and therefore it takes into account both the ratio between
the degrees and the difference between the scale zeros.
The resulting value is a temperature of 32.222 Celsius degrees.

Particular attention is devoted to angles.
Actually, differing from other magnitudes and units,
that must be defined by the application programmer,
the Angle magnitude is predefined, as one of its units is, radians.
In addition, beyond arbitrary-valued angles, both of "vect1"
and of "point1" kind, the following two statements may be written:

    signed_azimuth<radians> a(2);
    unsigned_azimuth<radians> b(3);

Both of them define angular positions modulo one turn, i.e. plane directions,
i.e. positions on a circumpherence; but the first statement forces its value
to lay between minus half turn and plus half turn, and so it is "signed",
while the second statement forces its value to lay between zero and one turn,
and so it is "unsigned".

At last, measures whose unit of measurement is defined only at run-time
may de defined, with statements like the following ones:

    dyn_vect1<Temperature> a(celsius::id(), 5);
    dyn_vect3<Length> b(mm::id(), 6, 7, 8);

"a" at compile-time represents only a temperature change,
without a defined unit, and at run-time it gets the value of 5 Celsius degrees.
"b" at compile-time represents only a displacement in 3D space,
and at run-time it gets the values x=6, y=7, z=8 millimeters.
Such kind of measures are much less efficient than the static ones,
but they are required when the unit of measurement
is not known at compile-time.

Here is a benchmark, made using Visual C++ for Windows,
both with no optimizations and with full standard optimizations (/Ox).
Using native types, the loop body is:

    double ax = i, ay = i + 1;
    double bx = i + 2, by = i + 3;
    if (ax * bx + ay * by < 0) return 1;

The meaning of such code is that two 2D vectors are created
using the loop counter "i"; the dot product of such vectors is computed;
and its value is compared to zero (even if it is always positive).

In one computer, the times in nanoseconds per iteration are:
- 3.4 with optimizations.
- 9 without optimizations;

Using this library with compile-time units, such code becomes:

    vect2<mm> a(i, i + 1);
    vect2<mm> b(i + 2, i + 3);
    if ((a * b).value() < 0) return 1;

The times per iteration are:
- 3.4 with optimizations.
- 76 without optimizations;

Using the library with run-time units, the code becomes:

    dyn_vect2<Length> a(mm::id(), i, i + 1);
    dyn_vect2<Length> b(mm::id(), i + 2, i + 3);
    if ((a * b).value() < 0) return 1;

The times per iteration are:
- 18 to 20 with optimizations.
- 440 to 660 without optimizations;

Some comments.
When optimizations are enabled, `double` and `vect1` have the same performance,
while `dyn_vect1` takes 6 times as much time.
When optimizations are disabled, the slowdown factor is only 2.6 for `double`s,
while it is around 22 for vect1s and even larger for dyn_vect1s.
