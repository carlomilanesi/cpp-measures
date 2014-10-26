# This script must be launched from "cpp-measure/test" directory.

gendir=../../gen/
rm -f -r $gendir
mkdir $gendir

clang++ -std=c++11 -O2 -I../ -o${gendir}pr1.o -c pr1.cpp
clang++ -std=c++11 -O2 -I../ -o${gendir}pr2.o -c pr2.cpp
clang++ -std=c++11 -O2 -I../ -o${gendir}pr3.o -c pr3.cpp
clang++ -O2 -o${gendir}pr1.exe ${gendir}pr1.o ${gendir}pr2.o ${gendir}pr3.o

clang++ -std=c++11 -s -O -Wall -W -Wconversion -o${gendir}static_tester.exe static_tester.cpp

echo
echo ==== Clang static test ====
REDIRECTION_COMMAND="2>/dev/null >/dev/null"
export REDIRECTION_COMMAND
COMPILER_VERSION_COMMAND="clang --version"
export COMPILER_VERSION_COMMAND
COMPILER_BASE_ERRORS_COMMAND="clang -std=c++11 -fsyntax-only -Wfatal-errors -pedantic -I../"
export COMPILER_BASE_ERRORS_COMMAND
COMPILER_BASE_WARNINGS_COMMAND="clang -std=c++11 -fsyntax-only -Wfatal-errors -pedantic -Werror -I../"
export COMPILER_BASE_WARNINGS_COMMAND
${gendir}static_tester.exe -t${gendir} st_test_legal.cpp st_test1.cpp st_test2.cpp
# rm ${gendir}_*

echo
echo ==== GCC 4.7 static test ====
# Some errors are not catched by GCC with the option -fsyntax-only,
# therefore the output file is generated and discarded.
REDIRECTION_COMMAND="2>/dev/null >/dev/null"
export REDIRECTION_COMMAND
COMPILER_VERSION_COMMAND="g++-4.7 --version"
export COMPILER_VERSION_COMMAND
COMPILER_BASE_ERRORS_COMMAND="g++-4.7 -std=c++11 -o /dev/null -Wfatal-errors -pedantic -I../"
export COMPILER_BASE_ERRORS_COMMAND
COMPILER_BASE_WARNINGS_COMMAND="g++-4.7 -std=c++11 -o /dev/null -Wfatal-errors -pedantic -Werror -I../"
export COMPILER_BASE_WARNINGS_COMMAND
${gendir}static_tester.exe -t${gendir} st_test_legal.cpp st_test1.cpp st_test2.cpp
# rm ${gendir}_*

echo
echo ==== Dynamic tests ====

# Use the following command when using GTest for Windows x32 (32-bit) 
# cl /nologo /Ox /EHsc /I ..\gtest-1.7.0\include /D _VARIADIC_MAX=10 dyn_tests.cpp /Fo..\gen\ /Fe..\gen\dyn_tests.exe /link /LIBPATH:..\gtest-1.7.0\msvc\gtest\Release gtest.lib

# Use the following command when using GTest for Windows x64 (64-bit)
# cl /nologo /Ox /EHsc /I ..\gtest-1.7.0\include /D _VARIADIC_MAX=10 dyn_tests.cpp /Fo..\gen\ /Fe..\gen\dyn_tests.exe /link /LIBPATH:..\gtest-1.7.0\msvc\x64\Release gtest.lib

g++-4.7 -std=c++11 -g -O -I../ -isystem ../../gtest-1.7.0/include -Wall -Wextra -Werror dyn_tests.cpp -lpthread ../../gtest-1.7.0/make/gtest_main.a -Wfatal-errors -o${gendir}dyn_tests.exe

${gendir}dyn_tests.exe

echo OK
