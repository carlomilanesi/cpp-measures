@echo off
rem This script must be launched from "cpp-measures\test" directory.

setlocal
set gendir=..\..\gen\
set unixgendir=../../gen/
rd /S /Q %gendir% 2>NUL
mkdir %gendir%

rem copy measures.hpp %gendir%
rem copy measures_io.hpp %gendir%
rem copy st_test1.cpp %gendir%
rem copy st_test2.cpp %gendir%

rem goto dyntest

rem TODO benchmark
rem TODO program using several source files
cl /nologo /EHsc /Ox /I..\ /Fo%gendir% /c pr1.cpp
cl /nologo /EHsc /Ox /I..\ /Fo%gendir% /c pr2.cpp
cl /nologo /EHsc /Ox /I..\ /Fo%gendir% /c pr3.cpp
cl /nologo /Ox /Fe%gendir% %gendir%pr1.obj %gendir%pr2.obj %gendir%pr3.obj

rem cl /nologo /EHsc /Fo%gendir% /Fe%gendir%st_test_gen.exe st_test_gen.cpp
rem g++ -std=c++11 -Wfatal-errors -pedantic -o %gendir%st_test_gen.exe st_test_gen.cpp
rem %gendir%st_test_gen.exe >%gendir%st_test_generated.cpp

rem 
rem goto skip_vcpp_test
echo.
echo ==== Visual C++ static test ====
cl /nologo /EHsc /Ox /Fo%gendir% /Fe%gendir%static_tester.exe static_tester.cpp

set REDIRECTION_COMMAND="2>NUL >NUL"
set COMPILER_VERSION_COMMAND=cl
set COMPILER_BASE_ERRORS_COMMAND=cl /nologo /EHsc /Za /Zs /I..\ /Fo%gendir%
set COMPILER_BASE_WARNINGS_COMMAND=cl /nologo /EHsc /Za /Zs /W4 /WX /I..\ /Fo%gendir%
%gendir%static_tester /t%gendir% /pst_test_perfect.cpp /ast_test_allowed.cpp /pst_test_core.cpp /pst_test_2d.cpp /pst_test_3d.cpp /pst_test_angles.cpp /pst_test_io.cpp
if errorlevel 1 goto test_errors
del %gendir%_* 2>NUL >NUL
:skip_vcpp_test

rem goto skip_gcc_test
echo.
echo ==== GCC static test ====
g++ -std=c++11 -Wall -W -Wfatal-errors -pedantic -o %unixgendir%static_tester.exe static_tester.cpp

set REDIRECTION_COMMAND="2>/dev/null >/dev/null"
set COMPILER_VERSION_COMMAND=g++ --version
rem SET COMPILER_BASE_ERRORS_COMMAND=g++ -std=c++11 -fsyntax-only -Wfatal-errors -pedantic
set COMPILER_BASE_ERRORS_COMMAND=g++ -std=c++11 -Wfatal-errors -pedantic -I../ -o%unixgendir%_test.o -c
rem SET COMPILER_BASE_WARNINGS_COMMAND=g++ -std=c++11 -fsyntax-only -Wfatal-errors -pedantic -Werror
set COMPILER_BASE_WARNINGS_COMMAND=g++ -std=c++11 -Wfatal-errors -pedantic -Wconversion -Wall -W -Werror -I../ -o%unixgendir%_test.o -c
%gendir%static_tester -t%unixgendir% -pst_test_perfect.cpp -ast_test_allowed.cpp /pst_test_core.cpp /pst_test_2d.cpp /pst_test_3d.cpp /pst_test_angles.cpp /pst_test_io.cpp
if errorlevel 1 goto test_errors
del %gendir%_* 2>NUL >NUL
:skip_gcc_test

:dyntest
echo.
echo ==== Dynamic tests ====

rem Use the following command when using GTest for Windows x32 (32-bit) 
rem cl /nologo /Ox /EHsc /I ..\gtest-1.7.0\include /D _VARIADIC_MAX=10 dyn_tests.cpp /Fo%gendir% /Fe%gendir%dyn_tests.exe /link /LIBPATH:..\gtest-1.7.0\msvc\gtest\Release gtest.lib

rem Use the following command when using GTest for Windows x64 (64-bit)
cl /nologo /Ox /EHsc /I..\ /I..\..\gtest-1.7.0\include /D _VARIADIC_MAX=10 dyn_tests.cpp /Fo%gendir% /Fe%gendir%dyn_tests.exe /link /LIBPATH:..\..\gtest-1.7.0\msvc\x64\Release gtest.lib

if errorlevel 1 goto test_errors
%gendir%dyn_tests.exe
:fine

echo OK

:test_errors
