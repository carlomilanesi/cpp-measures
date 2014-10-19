@echo off
rem This script must be launched from "cpp-measure" directory.

setlocal

rd /S /Q ..\gen 2>NUL
mkdir ..\gen

copy measures.hpp ..\gen\
copy measures_io.hpp ..\gen\
copy st_test1.cpp ..\gen\
copy st_test2.cpp ..\gen\

rem goto dyntest

rem TODO benchmark
rem TODO program using several source files

rem cl /nologo /EHsc /Fo..\gen\ /Fe..\gen\st_test_gen.exe st_test_gen.cpp
rem g++ -std=c++11 -Wfatal-errors -pedantic -o ../gen/st_test_gen.exe st_test_gen.cpp
rem ..\gen\st_test_gen.exe >..\gen\st_test_generated.cpp

rem 
rem goto skip_vcpp_test
echo.
echo ==== Visual C++ static test ====
cl /nologo /EHsc /Ox /Fo..\gen\ /Fe..\gen\static_tester.exe static_tester.cpp

set REDIRECTION_COMMAND="2>NUL >NUL"
set COMPILER_VERSION_COMMAND=cl
set COMPILER_BASE_ERRORS_COMMAND=cl /nologo /Za /Zs /Fo..\gen\
set COMPILER_BASE_WARNINGS_COMMAND=cl /nologo /Za /Zs /W4 /WX /Fo..\gen\
..\gen\static_tester /t..\gen\ st_test1.cpp st_test2.cpp
if errorlevel 1 goto test_errors
del ..\gen\_* 2>NUL >NUL
:skip_vcpp_test

rem goto skip_gcc_test
echo.
echo ==== GCC static test ====
g++ -std=c++11 -Wall -W -Wfatal-errors -pedantic -o ../gen/static_tester.exe static_tester.cpp

set REDIRECTION_COMMAND="2>/dev/null >/dev/null"
set COMPILER_VERSION_COMMAND=g++ --version
rem SET COMPILER_BASE_ERRORS_COMMAND=g++ -std=c++11 -fsyntax-only -Wfatal-errors -pedantic
set COMPILER_BASE_ERRORS_COMMAND=g++ -std=c++11 -Wfatal-errors -pedantic -c
rem SET COMPILER_BASE_WARNINGS_COMMAND=g++ -std=c++11 -fsyntax-only -Wfatal-errors -pedantic -Werror
set COMPILER_BASE_WARNINGS_COMMAND=g++ -std=c++11 -Wfatal-errors -pedantic -Wconversion -Wall -W -Werror -c
..\gen\static_tester /t../gen/ st_test1.cpp st_test2.cpp
if errorlevel 1 goto test_errors
del ..\gen\_* 2>NUL >NUL
:skip_gcc_test

:dyntest
echo.
echo ==== Dynamic tests ====

rem Use the following command when using GTest for Windows x32 (32-bit) 
rem cl /nologo /Ox /EHsc /I ..\gtest-1.7.0\include /D _VARIADIC_MAX=10 dyn_tests.cpp /Fo..\gen\ /Fe..\gen\dyn_tests.exe /link /LIBPATH:..\gtest-1.7.0\msvc\gtest\Release gtest.lib

rem Use the following command when using GTest for Windows x64 (64-bit)
cl /nologo /Ox /EHsc /I ..\gtest-1.7.0\include /D _VARIADIC_MAX=10 dyn_tests.cpp /Fo..\gen\ /Fe..\gen\dyn_tests.exe /link /LIBPATH:..\gtest-1.7.0\msvc\x64\Release gtest.lib

if errorlevel 1 goto test_errors
..\gen\dyn_tests.exe
:fine

echo OK

:test_errors
