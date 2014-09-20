Introduction
============

Purpose and Architecture 
------------------------

The purpose of this library is to define and use values that are typed
by their unit of measurement, with the following goals:
- to ease unit conversion;
- to prevent some kinds of programming errors;
- to favor readability and maintenance;
- to avoid significant runtime overhead.

It is a “base library”, defining an infrastructure that is mostly unusable
“as is” by application code.
Before being used, it should be “customized” by some lines of code,
that are named “custom library”.
After having parsed the base library and the custom library,
application code may be parsed successfully.

Users and documentation
-----------------------

The "measure" library has the following kinds of users:

- Base library developers,
  i.e. those who change the base library.
- Custom library developers,
  i.e. those who write or change a custom library that uses the base library.
- Application software developers,
  i.e. those who write or change some application software that uses
  a custom library.
- Application software users,
  i.e. those who use application software.

Let's say right away that application software users should be unaware
that such a library has been used by software developers,
and therefore they are not interested in any kind of documentation
regarding this library.

The documentation of the library comprises the following documents:
- introduction.md: It is this document.
  It describes the purpose and general architecture of the library.
- motivation.md: It is an academic paper explaining how a useful measure library
  should be designed.
- requirements.md: Following the purposes delineated by the motivation document,
  this document specifies exactly what should be performed by the library.
  It should be used and maintained by the base library developers.
- tutorial.md: It describes step-by-step how use the library,
  by building a custom library and then the ensuing application software.
  It should be maintained by the base library developers,
  and should be read by custom library developers and by application software
  developers before starting using the library in production code.
- reference.md: It is a list of the features of the library,
  with examples showing how to use them.
  It should be maintained by the base library developers,
  and should be used by custom library developers and by application software
  developers, when a doubt arises about how to use a feature of the library.
