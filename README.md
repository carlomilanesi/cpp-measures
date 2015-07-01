# cpp-measures


Header-only C++11 library to handle physical measures

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
embedded or real-time industrial automation, and also
several kinds of vector graphics applications, like computer games.

On the other side, this library does not address:
* Commercial/financial systems.
* Systems of advanced theoretical physics modeling.
* Physics learning environments.
* Video-editing and image-editing systems.
* Graphical-User-Interface handling.

Such library is useful for:
* making source code more readable
  (i.e. self-explanatory and self-documented);
* making source code more concise;
* detecting logic errors at compile-time;
* easing unit conversions;
* avoiding the run-time overhead added by other similar libraries.

The main features of this library are shown in the examples.


## Examples of use

To define a variable named "a", representing a length of 6 millimeters,
and keeping it in a "double" primitive object, it can be written:

    vect1<mm,double> a(6);

"vect" indicates that such value is a mathematical vector,
i.e. it may be added to or subtracted from another object of the same type,
and it may be multiplied or divided by a number;
"1" indicates that such measure in one-dimensional;
"mm" is the unit of measurement.

To define a variable named "b" representing a point in 3D space having
cartesian coordinates x=7, y=8, z=9, expressed in inches,
and stored in three "float" primitive objects, it can be written:

    point3<inches,float> b(7, 8, 9);

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
the Angle magnitude is predefined, and also one of its units is, radians.
In addition, beyond arbitrary-valued angles, both of "vect1"
and of "point1" kinds, the following two statements may be written:

    signed_azimuth<radians> a(2);
    unsigned_azimuth<radians> b(3);

Both of them define angular positions modulo one turn, i.e. plane directions,
or positions on a circumpherence; but the first statement forces its value
to lay between minus half turn and plus half turn, and so it is "signed",
while the second statement forces its value to lay between zero and one turn,
and so it is "unsigned".


## Guarantees for resource-limited applications

This library provides optional overloads of the
`>>` and `<<` operators of C++ stream,
for logging or debugging purposes.
If such operators are used, the requirements of the application
become those of supporting the `cin` and `cout`
standard console streams.

As long as its I/O functions are not used,
this library does not introduces nor requires:
* Virtual functions.
* Dynamic memory allocation.
* Exception handling.
* Run-Time Type Information.
* Run-time assertions.
* Per-value additional memory.
  For example, if `sizeof (double) == 8`,
  then `sizeof array<vect1<seconds,double>,1000> == 8000`.


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
  and end-user-chosen units are supported).


## Documentation

The documentation provided is the following:

* Tutorial: It is the first document to read. It is a step-by-step
  introductory guide to use the library.
* Reference: A detailed description of all the features of the library.
* Motivation: It is an academic paper that describes the reasons
  why design decisions have been taken.
* Guidelines: A set of rules to follow for who wants to change
  the library itself.
