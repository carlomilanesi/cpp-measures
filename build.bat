@echo off
rem This script must be launched from "cpp-measure" directory.

setlocal

rd /S /Q ..\gen 2>NUL
mkdir ..\gen

cl /nologo /EHsc /Ox /Fo..\gen\ /Fe..\gen\static_tester.exe static_tester.cpp
copy measures.hpp ..\gen\
copy measures_io.hpp ..\gen\

rem TODO benchmark
rem TODO program using several source files

cl /nologo /EHsc /Fo..\gen\ /Fe..\gen\st_test_gen.exe st_test_gen.cpp
..\gen\st_test_gen.exe >..\gen\st_test_generated.cpp

echo.
echo ==== Visual C++ static test ====
set REDIRECTION_COMMAND="2>NUL >NUL"
set COMPILER_VERSION_COMMAND=cl
set COMPILER_BASE_ERRORS_COMMAND=cl /nologo /EHsc /Za /Zs /Fo..\gen\
set COMPILER_BASE_WARNINGS_COMMAND=cl /nologo /EHsc /Za /Zs /W4 /WX /Fo..\gen\
..\gen\static_tester /t..\gen\ st_test_generated.cpp
if errorlevel 1 goto test_errors
del ..\gen\_* 2>NUL >NUL

echo.
echo ==== GCC static test ====
set CYGWIN=nodosfilewarning
set REDIRECTION_COMMAND="2>NUL >NUL"
set COMPILER_VERSION_COMMAND=g++ --version
rem SET COMPILER_BASE_ERRORS_COMMAND=g++ -std=c++11 -fsyntax-only -Wfatal-errors -pedantic
set COMPILER_BASE_ERRORS_COMMAND=g++ -std=c++11 -Wfatal-errors -pedantic
rem SET COMPILER_BASE_WARNINGS_COMMAND=g++ -std=c++11 -fsyntax-only -Wfatal-errors -pedantic -Werror
set COMPILER_BASE_WARNINGS_COMMAND=g++ -std=c++11 -Wfatal-errors -pedantic -Werror
..\gen\static_tester /t..\gen\ st_test_generated.cpp
if errorlevel 1 goto test_errors
del ..\gen\_* 2>NUL >NUL

echo.
echo ==== Dynamic tests ====
cl /nologo /Ox /EHsc /I ..\gtest-1.7.0\include /D _VARIADIC_MAX=10 dyn_tests.cpp /Fo..\gen\ /Fe..\gen\dyn_tests.exe /link /LIBPATH:..\gtest-1.7.0\msvc\gtest\Release gtest.lib
if errorlevel 1 goto test_errors
..\gen\dyn_tests.exe
:fine

echo OK

:test_errors
