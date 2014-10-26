#include "gtest/gtest.h"
#include <iostream>
#include <limits>
#include <complex>
using namespace std;

#include "measures_io.hpp"
using namespace measures;

DEFINE_MAGNITUDE(Space, metres, " m")
DEFINE_ANGLE_UNIT(degrees, "°", 360, 0)
DEFINE_ANGLE_UNIT(turns, " rev", 1, 0)
#define AZIMUTH_TOLERANCE 0.00002f
#define MIN_THRESHOLD 1e-14
#define MAX_THRESHOLD 1e14

template <typename Num1, typename Num2>
decltype(Num1()/Num2()) modulo(Num1 a, Num2 b)
{
  const auto result = fmod(a, b);
  return result >= 0 ? result : result + b;
}

template <class T>
class measureTest: public testing::Test { };

typedef testing::Types<int,long,long long,float,double,long double> Implementations;
TYPED_TEST_CASE(measureTest, Implementations);

template <typename T>
struct test_values
{
	static const int count = 11;
	static const T v[count];
};

template <typename T>
const T test_values<T>::v[] = {
	numeric_limits<T>::lowest() + 1,
	(numeric_limits<T>::lowest() + 1) / 17,
	-static_cast<T>(sqrt(numeric_limits<T>::max() / 3)),
	-1 - numeric_limits<T>::epsilon(),
	numeric_limits<T>::is_integer ? -1 : -numeric_limits<T>::min(),
	0,
	numeric_limits<T>::is_integer ? 1 : numeric_limits<T>::min(),
	1 + numeric_limits<T>::epsilon(),
	static_cast<T>(sqrt(numeric_limits<T>::max() / 3)),
	numeric_limits<T>::max() / 17,
	numeric_limits<T>::max(),
};

TEST(measureTest, turn)
{
	EXPECT_FLOAT_EQ(360, degrees::turn_fraction<int>());
	EXPECT_FLOAT_EQ(2 * pi, radians::turn_fraction<double>());
	EXPECT_FLOAT_EQ(1, turns::turn_fraction<int>());
	EXPECT_FLOAT_EQ(360, degrees::id().turn_fraction<int>());
	EXPECT_FLOAT_EQ(2 * pi, radians::id().turn_fraction<double>());
	EXPECT_FLOAT_EQ(1, turns::id().turn_fraction<int>());
}

//#if 0
TYPED_TEST(measureTest, vect1)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<metres,TypeParam>(epsilon * 2);
	auto zero = vect1<metres,TypeParam>(0);
	
	// Try several numeric values.
	for (int i = 0; i < test_values<TypeParam>::count; ++i)
	{
		TypeParam val1 = test_values<TypeParam>::v[i];
		TypeParam val2 = static_cast<TypeParam>(2.19);
		TypeParam val3 = 3;
		auto m1 = vect1<metres,TypeParam>(val1);
		auto m2 = vect1<metres,TypeParam>(val2);

		// Construction and "value".
		EXPECT_EQ(val1, m1.value());
		EXPECT_EQ(val2, m2.value());

		// Operator +=.
		// All test-values except the largest one,
		// as it would cause overflow by incrementing.
		if (i < test_values<TypeParam>::count - 1)
		{
			m1 = vect1<metres,TypeParam>(val1);
			auto m3 = m1 += m2;
			EXPECT_EQ(val1 + val2, m1.value());
			EXPECT_EQ(val2, m2.value());
			EXPECT_EQ(val1 + val2, m3.value());
		}

		// Operator -=.
		// All test-values except the lowest one,
		// as it would cause overflow by decrementing.
		if (0 < i)
		{
			m1 = vect1<metres,TypeParam>(val1);
			auto m3 = m1 -= m2;
			EXPECT_EQ(val1 - val2, m1.value());
			EXPECT_EQ(val2, m2.value());
			EXPECT_EQ(val1 - val2, m3.value());
		}

		// Operator *=.
		// All test-values except the lowest and the largest ones,
		// as they would cause overflow by multiplying.
		if (0 < i && i < test_values<TypeParam>::count - 1)
		{
			m1 = vect1<metres,TypeParam>(val1);
			auto m3 = m1 *= val2;
			EXPECT_EQ(val1 * val2, m1.value());
			EXPECT_EQ(val1 * val2, m3.value());
			auto m4 = m1 *= val3;
			EXPECT_EQ(val1 * val2 * val3, m1.value());
			EXPECT_EQ(val1 * val2 * val3, m4.value());
		}
		
		// Operator /=.
		// All test-values.
		if (val2 != 0)
		{
			m1 = vect1<metres,TypeParam>(val1);
			auto m3 = m1 /= val2;
			EXPECT_EQ(val1 / val2, m1.value());
			EXPECT_EQ(val1 / val2, m3.value());
			if (val3 != 0)
			{
				auto m4 = m1 /= val3;
				EXPECT_EQ(val1 / val2 / val3, m1.value());
				EXPECT_EQ(val1 / val2 / val3, m4.value());
			}
		}

		// Relational operators.
		
		// Comparing equal measures.
		m1 = vect1<metres,TypeParam>(val1);
		m2 = m1;
		EXPECT_TRUE(m1 == m2);
		EXPECT_FALSE(m1 != m2);
		EXPECT_FALSE(m1 < m2);
		EXPECT_TRUE(m1 <= m2);
		EXPECT_FALSE(m1 > m2);
		EXPECT_TRUE(m1 >= m2);
		EXPECT_TRUE(is_equal(m1, m2, zero));
		EXPECT_FALSE(is_less(m1, m2, zero));
		EXPECT_TRUE(is_less_or_equal(m1, m2, zero));

		// Comparing equal measures with a tolerance.
		// Avoid extreme test-values with a non-zero tolerance,
		// as they would cause overflow by incrementing or decrementing.
		if (0 < i && i < test_values<TypeParam>::count - 1)
		{
			EXPECT_TRUE(is_equal(m1, m2, tolerance));
			EXPECT_FALSE(is_less(m1, m2, tolerance));
			EXPECT_TRUE(is_less_or_equal(m1, m2, tolerance));
		}

		// Comparing a measure with its half, if it is large,
		// with its double if it is small.
		if (m1.value() < -1 || m1.value() > 1) m2 /= 2;
		else m1 *= 2;

		// Consider separately positive and negative cases.
		// Ignore zero case, as its half is the same as itself.
		if (m2.value() > 0)
		{
			EXPECT_FALSE(m1 == m2);
			EXPECT_TRUE(m1 != m2);
			EXPECT_FALSE(m1 < m2);
			EXPECT_FALSE(m1 <= m2);
			EXPECT_TRUE(m1 > m2);
			EXPECT_TRUE(m1 >= m2);
			EXPECT_FALSE(is_equal(m1, m2, zero));
			EXPECT_FALSE(is_less(m1, m2, zero));
			EXPECT_FALSE(is_less_or_equal(m1, m2, zero));
		}
		else if (m2.value() < 0)
		{
			EXPECT_FALSE(m1 == m2);
			EXPECT_TRUE(m1 != m2);
			EXPECT_TRUE(m1 < m2);
			EXPECT_TRUE(m1 <= m2);
			EXPECT_FALSE(m1 > m2);
			EXPECT_FALSE(m1 >= m2);
			EXPECT_FALSE(is_equal(m1, m2, zero));
			EXPECT_TRUE(is_less(m1, m2, zero));
			EXPECT_TRUE(is_less_or_equal(m1, m2, zero));
		}
	}
	
	// Comparing 1 with a bit more than 1.
	auto m3 = vect1<metres,TypeParam>(1);
	auto m4 = vect1<metres,TypeParam>(1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(m3 < m4);
	EXPECT_TRUE(m3 <= m4);
	EXPECT_FALSE(m3 > m4);
	EXPECT_FALSE(m3 >= m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));
	EXPECT_FALSE(is_less(m3, m4, tolerance));
	EXPECT_TRUE(is_less_or_equal(m3, m4, tolerance));

	// Comparing -1 with a bit less than -1.
	auto m5 = vect1<metres,TypeParam>(-1);
	auto m6 = vect1<metres,TypeParam>(-1 - epsilon);
	EXPECT_FALSE(m5 == m6);
	EXPECT_TRUE(m5 != m6);
	EXPECT_FALSE(m5 < m6);
	EXPECT_FALSE(m5 <= m6);
	EXPECT_TRUE(m5 > m6);
	EXPECT_TRUE(m5 >= m6);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));
	EXPECT_FALSE(is_less(m5, m6, tolerance));
	EXPECT_TRUE(is_less_or_equal(m5, m6, tolerance));

	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = vect1<metres,TypeParam>(45);
	auto m8 = vect1<metres,TypeParam>(48);
	auto tol1 = vect1<metres,TypeParam>(2);
	auto tol2 = vect1<metres,TypeParam>(4);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));

	EXPECT_TRUE(is_less(m7, m8, tol1));
	EXPECT_FALSE(is_less(m8, m7, tol1));
	EXPECT_FALSE(is_less(m7, m8, tol2));
	EXPECT_FALSE(is_less(m8, m7, tol2));

	EXPECT_TRUE(is_less_or_equal(m7, m8, tol1));
	EXPECT_FALSE(is_less_or_equal(m8, m7, tol1));
	EXPECT_TRUE(is_less_or_equal(m7, m8, tol2));
	EXPECT_TRUE(is_less_or_equal(m8, m7, tol2));
}

TYPED_TEST(measureTest, point1)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<metres,TypeParam>(epsilon * 2);
	auto zero = vect1<metres,TypeParam>(0);
	
	// Try several numeric values.
	for (int i = 0; i < test_values<TypeParam>::count; ++i)
	{
		TypeParam val1 = test_values<TypeParam>::v[i];
		TypeParam val2 = static_cast<TypeParam>(2.19);
		TypeParam val3 = 3;
		auto m1 = point1<metres,TypeParam>(val1);
		auto m2 = vect1<metres,TypeParam>(val2);

		// Construction and "value".
		EXPECT_EQ(val1, m1.value());
		EXPECT_EQ(val2, m2.value());

		// Operator +=.
		// All test-values except the largest one,
		// as it would cause overflow by incrementing.
		if (i < test_values<TypeParam>::count - 1)
		{
			m1 = point1<metres,TypeParam>(val1);
			auto m3 = m1 += m2;
			EXPECT_EQ(val1 + val2, m1.value());
			EXPECT_EQ(val1 + val2, m3.value());
		}

		// Operator -=.
		// All test-values except the lowest one,
		// as it would cause overflow by decrementing.
		if (0 < i)
		{
			m1 = point1<metres,TypeParam>(val1);
			auto m3 = m1 -= m2;
			EXPECT_EQ(val1 - val2, m1.value());
			EXPECT_EQ(val1 - val2, m3.value());
		}

		// Relational operators.
		
		// Comparing equal measures.
		m1 = point1<metres,TypeParam>(val1);
		auto m3 = m1;
		EXPECT_TRUE(m1 == m3);
		EXPECT_FALSE(m1 != m3);
		EXPECT_FALSE(m1 < m3);
		EXPECT_TRUE(m1 <= m3);
		EXPECT_FALSE(m1 > m3);
		EXPECT_TRUE(m1 >= m3);
		EXPECT_TRUE(is_equal(m1, m3, zero));
		EXPECT_FALSE(is_less(m1, m3, zero));
		EXPECT_TRUE(is_less_or_equal(m1, m3, zero));

		// Comparing equal measures with a tolerance.
		// Avoid extreme test-values with a non-zero tolerance,
		// as they would cause overflow by incrementing or decrementing.
		if (0 < i && i < test_values<TypeParam>::count - 1)
		{
			EXPECT_TRUE(is_equal(m1, m3, tolerance));
			EXPECT_FALSE(is_less(m1, m3, tolerance));
			EXPECT_TRUE(is_less_or_equal(m1, m3, tolerance));
		}

		// Comparing a measure with its half.
		m3 = point1<metres,TypeParam>(val1 / 2);
		
		// Consider separately positive and negative cases.
		// Ignore zero case, as its half is the same as itself.
		if (m1.value() > 0)
		{
			EXPECT_FALSE(m1 == m3);
			EXPECT_TRUE(m1 != m3);
			EXPECT_FALSE(m1 < m3);
			EXPECT_FALSE(m1 <= m3);
			EXPECT_TRUE(m1 > m3);
			EXPECT_TRUE(m1 >= m3);
			EXPECT_FALSE(is_equal(m1, m3, zero));
			EXPECT_FALSE(is_less(m1, m3, zero));
			EXPECT_FALSE(is_less_or_equal(m1, m3, zero));
		}
		else if (m1.value() > 0)
		{
			EXPECT_FALSE(m1 == m3);
			EXPECT_TRUE(m1 != m3);
			EXPECT_TRUE(m1 < m3);
			EXPECT_TRUE(m1 <= m3);
			EXPECT_FALSE(m1 > m3);
			EXPECT_FALSE(m1 >= m3);
			EXPECT_FALSE(is_equal(m1, m3, zero));
			EXPECT_FALSE(is_less(m1, m3, zero));
			EXPECT_FALSE(is_less_or_equal(m1, m3, zero));
		}
	}
	
	// Comparing 1 with a bit more than 1.
	auto m3 = point1<metres,TypeParam>(1);
	auto m4 = point1<metres,TypeParam>(1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(m3 < m4);
	EXPECT_TRUE(m3 <= m4);
	EXPECT_FALSE(m3 > m4);
	EXPECT_FALSE(m3 >= m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));
	EXPECT_FALSE(is_less(m3, m4, tolerance));
	EXPECT_TRUE(is_less_or_equal(m3, m4, tolerance));

	// Comparing -1 with a bit less than -1.
	auto m5 = point1<metres,TypeParam>(-1);
	auto m6 = point1<metres,TypeParam>(-1 - epsilon);
	EXPECT_FALSE(m5 == m6);
	EXPECT_TRUE(m5 != m6);
	EXPECT_FALSE(m5 < m6);
	EXPECT_FALSE(m5 <= m6);
	EXPECT_TRUE(m5 > m6);
	EXPECT_TRUE(m5 >= m6);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));
	EXPECT_FALSE(is_less(m5, m6, tolerance));
	EXPECT_TRUE(is_less_or_equal(m5, m6, tolerance));

	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = point1<metres,TypeParam>(45);
	auto m8 = point1<metres,TypeParam>(48);
	auto tol1 = vect1<metres,TypeParam>(2);
	auto tol2 = vect1<metres,TypeParam>(4);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));

	EXPECT_TRUE(is_less(m7, m8, tol1));
	EXPECT_FALSE(is_less(m8, m7, tol1));
	EXPECT_FALSE(is_less(m7, m8, tol2));
	EXPECT_FALSE(is_less(m8, m7, tol2));

	EXPECT_TRUE(is_less_or_equal(m7, m8, tol1));
	EXPECT_FALSE(is_less_or_equal(m8, m7, tol1));
	EXPECT_TRUE(is_less_or_equal(m7, m8, tol2));
	EXPECT_TRUE(is_less_or_equal(m8, m7, tol2));
}

TYPED_TEST(measureTest, vectpoint1)
{

	// Midpoint.
	// Try all pairs of numeric values except extremes.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1 = test_values<TypeParam>::v[i1];
		auto m1 = point1<metres,TypeParam>(val1);
		for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
		{
			TypeParam val2 = test_values<TypeParam>::v[i2];
			auto m2 = point1<metres,TypeParam>(val2);
			EXPECT_FLOAT_EQ((val1 + val2) * 0.5f, midpoint(m1, m2).value());
			EXPECT_FLOAT_EQ(val1, midpoint(m1, m2, 0).value());
			EXPECT_FLOAT_EQ(val2, midpoint(m1, m2, 1).value());
			EXPECT_FLOAT_EQ(val1 * (1 - 0.23f) + val2 * 0.23f, midpoint(m1, m2, 0.23f).value());
		}
	}

	// Barycentric combination.
	// Try all pairs of numeric values except extremes,
	// and add some other points.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1 = test_values<TypeParam>::v[i1];
		auto m1 = point1<metres,TypeParam>(val1);
		for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
		{
			TypeParam val2 = test_values<TypeParam>::v[i2];
			auto m2 = point1<metres,TypeParam>(val2);
			TypeParam val3 = 3;
			auto m3 = point1<metres,TypeParam>(val3);
			TypeParam val4 = 4;
			auto m4 = point1<metres,TypeParam>(val4);
			point1<metres,TypeParam> point1array[] = { m1, m2, m3, m4 };
			TypeParam weights[] = { 2, 3, 7, 4 };
			EXPECT_FLOAT_EQ(val1 * weights[0],
				barycentric_combination(1, point1array, weights).value());
			EXPECT_FLOAT_EQ(val1 * weights[0] + val2 * weights[1],
				barycentric_combination(2, point1array, weights).value());
			EXPECT_FLOAT_EQ(val1 * weights[0] + val2 * weights[1]
				+ val3 * weights[2],
				barycentric_combination(3, point1array, weights).value());
			EXPECT_FLOAT_EQ(val1 * weights[0] + val2 * weights[1]
				+ val3 * weights[2] + val4 * weights[3],
				barycentric_combination(4, point1array, weights).value());
		}
	}
	
	// Try all pairs of numeric values except extremes.
	TypeParam sqrt_of_max = static_cast<TypeParam>(sqrt(test_values<TypeParam>
		::v[test_values<TypeParam>::count - 1]));
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1 = test_values<TypeParam>::v[i1];
		auto v1 = vect1<metres,TypeParam>(val1);
		auto p1 = point1<metres,TypeParam>(val1);
		for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
		{
			TypeParam val2 = test_values<TypeParam>::v[i2];
			auto v2 = vect1<metres,TypeParam>(val2);
			auto p2 = point1<metres,TypeParam>(val2);

			// point1 - point1 -> vect1
			EXPECT_EQ(val1 - val2, (p1 - p2).value());

			// point1 + vect1 -> point1
			EXPECT_EQ(val1 + val2, (p1 + v2).value());

			// point1 - vect1 -> point1
			EXPECT_EQ(val1 - val2, (p1 - v2).value());

			// vect1 + vect1 -> vect1
			EXPECT_EQ(val1 + val2, (v1 + v2).value());

			// vect1 - vect1 -> vect1
			EXPECT_EQ(val1 - val2, (v1 - v2).value());
			
			if (abs(val1) < sqrt_of_max && abs(val2) < sqrt_of_max)
			{
				// N * vect1 -> vect1
				EXPECT_EQ(val1 * val2, (val1 * v2).value());

				// vect1 * N -> vect1
				EXPECT_EQ(val1 * val2, (v1 * val2).value());
			}
			
			// vect1 / N -> vect1
			if (val2 != 0) EXPECT_EQ(val1 / val2, (v1 / val2).value());

			// vect1 / vect1 -> N
			if (val2 != 0) EXPECT_EQ(val1 / val2, v1 / v2);
		}
	}
	
	for (int i = 2; i < test_values<TypeParam>::count - 2; ++i)
	{
		TypeParam val1 = test_values<TypeParam>::v[i];
		auto m1 = vect1<metres, TypeParam>(val1);
		if (abs(val1) < sqrt_of_max)
		{
			EXPECT_EQ(val1 * val1, squared_norm_value(m1));
		}
		EXPECT_EQ(abs(val1), norm(m1).value());
	}
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(measureTest, vect2)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<metres,TypeParam>(epsilon * 2);
	auto zero = vect1<metres,TypeParam>(0);
	
	// Try several numeric values.
	for (int i = 0; i < test_values<TypeParam>::count; ++i)
	{
		TypeParam val1x = test_values<TypeParam>::v[i];

		// Try several numeric values.
		for (int j = 0; j < test_values<TypeParam>::count; ++j)
		{
			TypeParam val1y = test_values<TypeParam>::v[j];
			TypeParam val2 = static_cast<TypeParam>(2.19);
			TypeParam val2x = static_cast<TypeParam>(2.27);
			TypeParam val2y = static_cast<TypeParam>(2.13);
			TypeParam val3 = 3;
			auto m1 = vect2<metres,TypeParam>(val1x, val1y);
			auto m2 = vect2<metres,TypeParam>(val2x, val2y);

			// Construction and "value".
			EXPECT_EQ(val1x, m1.x().value());
			EXPECT_EQ(val1y, m1.y().value());
			EXPECT_EQ(val2x, m2.x().value());
			EXPECT_EQ(val2y, m2.y().value());

			// Operator +=.
			// All test-values except the largest one,
			// as it would cause overflow by incrementing.
			if (i < test_values<TypeParam>::count - 1
				&& j < test_values<TypeParam>::count - 1)
			{
				m1 = vect2<metres,TypeParam>(val1x, val1y);
				auto m3 = m1 += m2;
				EXPECT_EQ(val1x + val2x, m1.x().value());
				EXPECT_EQ(val1y + val2y, m1.y().value());
				EXPECT_EQ(val2x, m2.x().value());
				EXPECT_EQ(val2y, m2.y().value());
				EXPECT_EQ(val1x + val2x, m3.x().value());
				EXPECT_EQ(val1y + val2y, m3.y().value());
			}

			// Operator -=.
			// All test-values except the lowest one,
			// as it would cause overflow by decrementing.
			if (0 < i && 0 < j)
			{
				m1 = vect2<metres,TypeParam>(val1x, val1y);
				auto m3 = m1 -= m2;
				EXPECT_EQ(val1x - val2x, m1.x().value());
				EXPECT_EQ(val1y - val2y, m1.y().value());
				EXPECT_EQ(val2x, m2.x().value());
				EXPECT_EQ(val2y, m2.y().value());
				EXPECT_EQ(val1x - val2x, m3.x().value());
				EXPECT_EQ(val1y - val2y, m3.y().value());
			}

			// Operator *=.
			// All test-values except the lowest and the largest ones,
			// as they would cause overflow by multiplying.
			if (0 < i && i < test_values<TypeParam>::count - 1
				&& 0 < j && j < test_values<TypeParam>::count - 1)
			{
				m1 = vect2<metres,TypeParam>(val1x, val1y);
				auto m3 = m1 *= val2;
				EXPECT_EQ(val1x * val2, m1.x().value());
				EXPECT_EQ(val1y * val2, m1.y().value());
				EXPECT_EQ(val1x * val2, m3.x().value());
				EXPECT_EQ(val1y * val2, m3.y().value());
				auto m4 = m1 *= val3;
				EXPECT_EQ(val1x * val2 * val3, m1.x().value());
				EXPECT_EQ(val1y * val2 * val3, m1.y().value());
				EXPECT_EQ(val1x * val2 * val3, m4.x().value());
				EXPECT_EQ(val1y * val2 * val3, m4.y().value());
			}
			
			// Operator /=.
			// All test-values.
			if (val2 != 0)
			{
				m1 = vect2<metres,TypeParam>(val1x, val1y);
				auto m3 = m1 /= val2;
				EXPECT_EQ(val1x / val2, m1.x().value());
				EXPECT_EQ(val1y / val2, m1.y().value());
				EXPECT_EQ(val1x / val2, m3.x().value());
				EXPECT_EQ(val1y / val2, m3.y().value());
				if (val3 != 0)
				{
					auto m4 = m1 /= val3;
					EXPECT_EQ(val1x / val2 / val3, m1.x().value());
					EXPECT_EQ(val1y / val2 / val3, m1.y().value());
					EXPECT_EQ(val1x / val2 / val3, m4.x().value());
					EXPECT_EQ(val1y / val2 / val3, m4.y().value());
				}
			}

			// Relational operators.

			// Comparing equal measures.
			m1 = vect2<metres,TypeParam>(val1x, val1y);
			m2 = m1;
			EXPECT_TRUE(m1 == m2);
			EXPECT_FALSE(m1 != m2);

			// All test-values except the two lowest and the two largest ones,
			// as they would cause overflow by squaring.
			if (2 <= i && i < test_values<TypeParam>::count - 2
				&& 2 <= j && j < test_values<TypeParam>::count - 2)
			{
				EXPECT_TRUE(is_equal(m1, m2, zero));
				EXPECT_TRUE(is_equal(m1, m2, tolerance));

				// Comparing a measure with its half, if it is large,
				// with its double if it is small.
				if (abs(m2.x().value()) >= MIN_THRESHOLD)
				{
					if (squared_norm_value(m2) > 1) m2 /= 2;
					else m1 *= 2;
					EXPECT_FALSE(m1 == m2);
					EXPECT_TRUE(m1 != m2);					
					EXPECT_FALSE(is_equal(m1, m2, zero));
				}
			}
		}
	}

	// Comparing 1 with a bit more than 1.
	auto m3 = vect2<metres,TypeParam>(1, 1);
	auto m4 = vect2<metres,TypeParam>(1 + epsilon, 1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));

	// Comparing -1 with a bit less than -1.
	auto m5 = vect2<metres,TypeParam>(-1, -1);
	auto m6 = vect2<metres,TypeParam>(-1 - epsilon, -1 - epsilon);
	EXPECT_FALSE(m5 == m6);
	EXPECT_TRUE(m5 != m6);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));

	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = vect2<metres,TypeParam>(4, 4);
	auto m8 = vect2<metres,TypeParam>(8, 8);
	auto tol1 = vect1<metres,TypeParam>(5);
	auto tol2 = vect1<metres,TypeParam>(6);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));
}

TYPED_TEST(measureTest, point2)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<metres,TypeParam>(epsilon * 2);
	auto zero = vect1<metres,TypeParam>(0);
	
	// Try several numeric values.
	for (int i = 0; i < test_values<TypeParam>::count; ++i)
	{
		TypeParam val1x = test_values<TypeParam>::v[i];

		// Try several numeric values.
		for (int j = 0; j < test_values<TypeParam>::count; ++j)
		{
			TypeParam val1y = test_values<TypeParam>::v[j];
			TypeParam val2 = static_cast<TypeParam>(2.19);
			TypeParam val2x = static_cast<TypeParam>(2.27);
			TypeParam val2y = static_cast<TypeParam>(2.13);
			TypeParam val3 = 3;
			auto m1 = point2<metres,TypeParam>(val1x, val1y);
			auto m2 = vect2<metres,TypeParam>(val2x, val2y);

			// Construction and "value".
			EXPECT_EQ(val1x, m1.x().value());
			EXPECT_EQ(val1y, m1.y().value());
			EXPECT_EQ(val2x, m2.x().value());
			EXPECT_EQ(val2y, m2.y().value());

			// Operator +=.
			// All test-values except the largest one,
			// as it would cause overflow by incrementing.
			if (i < test_values<TypeParam>::count - 1
				&& j < test_values<TypeParam>::count - 1)
			{
				m1 = point2<metres,TypeParam>(val1x, val1y);
				auto m3 = m1 += m2;
				EXPECT_EQ(val1x + val2x, m1.x().value());
				EXPECT_EQ(val1y + val2y, m1.y().value());
				EXPECT_EQ(val2x, m2.x().value());
				EXPECT_EQ(val2y, m2.y().value());
				EXPECT_EQ(val1x + val2x, m3.x().value());
				EXPECT_EQ(val1y + val2y, m3.y().value());
			}

			// Operator -=.
			// All test-values except the lowest one,
			// as it would cause overflow by decrementing.
			if (0 < i && 0 < j)
			{
				m1 = point2<metres,TypeParam>(val1x, val1y);
				auto m3 = m1 -= m2;
				EXPECT_EQ(val1x - val2x, m1.x().value());
				EXPECT_EQ(val1y - val2y, m1.y().value());
				EXPECT_EQ(val2x, m2.x().value());
				EXPECT_EQ(val2y, m2.y().value());
				EXPECT_EQ(val1x - val2x, m3.x().value());
				EXPECT_EQ(val1y - val2y, m3.y().value());
			}

			// Relational operators.

			// Comparing equal measures.
			m1 = point2<metres,TypeParam>(val1x, val1y);
			auto m3 = m1;
			EXPECT_TRUE(m1 == m3);
			EXPECT_FALSE(m1 != m3);

			// All test-values except the two lowest and the two largest ones,
			// as they would cause overflow by squaring.
			if (2 <= i && i < test_values<TypeParam>::count - 2
				&& 2 <= j && j < test_values<TypeParam>::count - 2)
			{
				EXPECT_TRUE(is_equal(m1, m3, zero));
				EXPECT_TRUE(is_equal(m1, m3, tolerance));

				// Comparing a measure with its half, if it is large,
				// with its double if it is small.
				if (abs(m3.x().value()) >= MIN_THRESHOLD)
				{
					if (abs(m3.x().value()) + abs(m3.y().value()) > 2)
						m3 = point2<metres,TypeParam>(m3.x().value() / 2, m3.y().value() / 2);
					else m1 = point2<metres,TypeParam>(m1.x().value() * 2, m1.y().value() * 2);
					EXPECT_FALSE(m1 == m3);
					EXPECT_TRUE(m1 != m3);					
					EXPECT_FALSE(is_equal(m1, m3, zero));
				}
			}
		}
	}

	// Comparing 1 with a bit more than 1.
	auto m3 = point2<metres,TypeParam>(1, 1);
	auto m4 = point2<metres,TypeParam>(1 + epsilon, 1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));

	// Comparing -1 with a bit less than -1.
	auto m5 = point2<metres,TypeParam>(-1, -1);
	auto m6 = point2<metres,TypeParam>(-1 - epsilon, -1 - epsilon);
	EXPECT_FALSE(m5 == m6);
	EXPECT_TRUE(m5 != m6);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));

	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = point2<metres,TypeParam>(4, 4);
	auto m8 = point2<metres,TypeParam>(8, 8);
	auto tol1 = vect1<metres,TypeParam>(5);
	auto tol2 = vect1<metres,TypeParam>(6);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));
}

TYPED_TEST(measureTest, vectpoint2)
{
	// Midpoint.
	// Try all pairs of pairs of numeric values except extremes.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1x = test_values<TypeParam>::v[i1];
		for (int j1 = 1; j1 < test_values<TypeParam>::count - 1; ++j1)
		{
			TypeParam val1y = test_values<TypeParam>::v[j1];
			auto m1 = point2<metres,TypeParam>(val1x, val1y);

			for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
			{
				TypeParam val2x = test_values<TypeParam>::v[i2];
				for (int j2 = 1; j2 < test_values<TypeParam>::count - 1; ++j2)
				{
					TypeParam val2y = test_values<TypeParam>::v[j2];
					auto m2 = point2<metres,TypeParam>(val2x, val2y);
					
					EXPECT_FLOAT_EQ((val1x + val2x) * 0.5f, midpoint(m1, m2).x().value());
					EXPECT_FLOAT_EQ((val1y + val2y) * 0.5f, midpoint(m1, m2).y().value());
					EXPECT_FLOAT_EQ(val1x, midpoint(m1, m2, 0).x().value());
					EXPECT_FLOAT_EQ(val1y, midpoint(m1, m2, 0).y().value());
					EXPECT_FLOAT_EQ(val2x, midpoint(m1, m2, 1).x().value());
					EXPECT_FLOAT_EQ(val2y, midpoint(m1, m2, 1).y().value());
					EXPECT_FLOAT_EQ(val1x * (1 - 0.23f) + val2x * 0.23f, midpoint(m1, m2, 0.23f).x().value());
					EXPECT_FLOAT_EQ(val1y * (1 - 0.23f) + val2y * 0.23f, midpoint(m1, m2, 0.23f).y().value());
				}
			}
		}
	}

	// Barycentric combination.
	// Try all pairs of pairs of numeric values except extremes,
	// and add some other points.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1x = test_values<TypeParam>::v[i1];
		for (int j1 = 1; j1 < test_values<TypeParam>::count - 1; ++j1)
		{
			TypeParam val1y = test_values<TypeParam>::v[j1];
			auto m1 = point2<metres,TypeParam>(val1x, val1y);
			for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
			{
				TypeParam val2x = test_values<TypeParam>::v[i2];
				for (int j2 = 1; j2 < test_values<TypeParam>::count - 1; ++j2)
				{
					TypeParam val2y = test_values<TypeParam>::v[j2];
					auto m2 = point2<metres,TypeParam>(val2x, val2y);

					TypeParam val3x = static_cast<TypeParam>(3.1);
					TypeParam val3y = static_cast<TypeParam>(3.2);
					auto m3 = point2<metres,TypeParam>(val3x, val3y);
					TypeParam val4x = static_cast<TypeParam>(4.3);
					TypeParam val4y = static_cast<TypeParam>(4.4);
					auto m4 = point2<metres,TypeParam>(val4x, val4y);
					point2<metres,TypeParam> point2array[] = { m1, m2, m3, m4 };
					TypeParam weights[] = { 2, 3, 7, 4 };
					EXPECT_FLOAT_EQ(val1x * weights[0],
						barycentric_combination(1, point2array, weights).x().value());
					EXPECT_FLOAT_EQ(val1y * weights[0],
						barycentric_combination(1, point2array, weights).y().value());
					EXPECT_FLOAT_EQ(val1x * weights[0] + val2x * weights[1],
						barycentric_combination(2, point2array, weights).x().value());
					EXPECT_FLOAT_EQ(val1y * weights[0] + val2y * weights[1],
						barycentric_combination(2, point2array, weights).y().value());
					EXPECT_FLOAT_EQ(val1x * weights[0] + val2x * weights[1]
						+ val3x * weights[2],
						barycentric_combination(3, point2array, weights).x().value());
					EXPECT_FLOAT_EQ(val1y * weights[0] + val2y * weights[1]
						+ val3y * weights[2],
						barycentric_combination(3, point2array, weights).y().value());
					EXPECT_FLOAT_EQ(val1x * weights[0] + val2x * weights[1]
						+ val3x * weights[2] + val4x * weights[3],
						barycentric_combination(4, point2array, weights).x().value());
					EXPECT_FLOAT_EQ(val1y * weights[0] + val2y * weights[1]
						+ val3y * weights[2] + val4y * weights[3],
						barycentric_combination(4, point2array, weights).y().value());
				}
			}
		}
	}
	
	// Try all pairs of pairs of numeric values except extremes.
	TypeParam sqrt_of_max = static_cast<TypeParam>(sqrt(test_values<TypeParam>
		::v[test_values<TypeParam>::count - 1]));
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1x = test_values<TypeParam>::v[i1];
		for (int j1 = 1; j1 < test_values<TypeParam>::count - 1; ++j1)
		{
			TypeParam val1y = test_values<TypeParam>::v[j1];
			auto v1 = vect2<metres,TypeParam>(val1x, val1y);
			auto p1 = point2<metres,TypeParam>(val1x, val1y);

			for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
			{
				TypeParam val2x = test_values<TypeParam>::v[i2];
				for (int j2 = 1; j2 < test_values<TypeParam>::count - 1; ++j2)
				{
					TypeParam val2y = test_values<TypeParam>::v[j2];
					auto v2 = vect2<metres,TypeParam>(val2x, val2y);
					auto p2 = point2<metres,TypeParam>(val2x, val2y);

					// point2 - point2 -> vect2
					EXPECT_EQ(val1x - val2x, (p1 - p2).x().value());
					EXPECT_EQ(val1y - val2y, (p1 - p2).y().value());

					// point2 + vect2 -> point2
					EXPECT_EQ(val1x + val2x, (p1 + v2).x().value());
					EXPECT_EQ(val1y + val2y, (p1 + v2).y().value());

					// point2 - vect2 -> point2
					EXPECT_EQ(val1x - val2x, (p1 - v2).x().value());
					EXPECT_EQ(val1y - val2y, (p1 - v2).y().value());

					// vect2 + vect2 -> vect2
					EXPECT_EQ(val1x + val2x, (v1 + v2).x().value());
					EXPECT_EQ(val1y + val2y, (v1 + v2).y().value());

					// vect2 - vect2 -> vect2
					EXPECT_EQ(val1x - val2x, (v1 - v2).x().value());
					EXPECT_EQ(val1y - val2y, (v1 - v2).y().value());
					
					if (abs(val1x) < sqrt_of_max && abs(val2x) < sqrt_of_max)
					{
						// N * vect2 -> vect2
						EXPECT_EQ(val1x * val2x, (val1x * v2).x().value());

						// vect2 * N -> vect2
						EXPECT_EQ(val1x * val2x, (v1 * val2x).x().value());
					}

					if (abs(val1y) < sqrt_of_max && abs(val2y) < sqrt_of_max)
					{
						// N * vect2 -> vect2
						EXPECT_EQ(val1y * val2y, (val1y * v2).y().value());

						// vect2 * N -> vect2
						EXPECT_EQ(val1y * val2y, (v1 * val2y).y().value());
					}
					
					// vect2 / N -> vect2
					if (val2x != 0) EXPECT_EQ(val1x / val2x, (v1 / val2x).x().value());
					if (val2y != 0) EXPECT_EQ(val1y / val2y, (v1 / val2y).y().value());
				}
			}
		}
	}
	
	for (int i = 2; i < test_values<TypeParam>::count - 2; ++i)
	{
		for (int j = 2; j < test_values<TypeParam>::count - 2; ++j)
		{
			TypeParam val1x = test_values<TypeParam>::v[i];
			TypeParam val1y = test_values<TypeParam>::v[j];
			auto m1 = vect2<metres, TypeParam>(val1x, val1y);
			if (abs(val1x) < sqrt_of_max && abs(val1y) < sqrt_of_max)
			{
				EXPECT_FLOAT_EQ(val1x * val1x + val1y * val1y, squared_norm_value(m1));
				EXPECT_FLOAT_EQ(static_cast<TypeParam>(
					sqrt(val1x * val1x + val1y * val1y)), norm(m1).value());
			}
		}
	}
}

TYPED_TEST(measureTest, vect3)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<metres,TypeParam>(epsilon * 2);
	auto zero = vect1<metres,TypeParam>(0);
	
	// Try several numeric values.
	for (int i = 0; i < test_values<TypeParam>::count; ++i)
	{
		TypeParam val1x = test_values<TypeParam>::v[i];

		// Try several numeric values.
		for (int j = 0; j < test_values<TypeParam>::count; ++j)
		{
			TypeParam val1y = test_values<TypeParam>::v[j];

			// Try several numeric values.
			for (int k = 0; k < test_values<TypeParam>::count; ++k)
			{
				TypeParam val1z = test_values<TypeParam>::v[k];
				TypeParam val2 = static_cast<TypeParam>(2.19);
				TypeParam val2x = static_cast<TypeParam>(2.27);
				TypeParam val2y = static_cast<TypeParam>(2.13);
				TypeParam val2z = static_cast<TypeParam>(2.15);
				TypeParam val3 = 3;
				auto m1 = vect3<metres,TypeParam>(val1x, val1y, val1z);
				auto m2 = vect3<metres,TypeParam>(val2x, val2y, val2z);

				// Construction and "value".
				EXPECT_EQ(val1x, m1.x().value());
				EXPECT_EQ(val1y, m1.y().value());
				EXPECT_EQ(val1z, m1.z().value());
				EXPECT_EQ(val2x, m2.x().value());
				EXPECT_EQ(val2y, m2.y().value());
				EXPECT_EQ(val2z, m2.z().value());

				// Operator +=.
				// All test-values except the largest one,
				// as it would cause overflow by incrementing.
				if (i < test_values<TypeParam>::count - 1
					&& j < test_values<TypeParam>::count - 1
					&& k < test_values<TypeParam>::count - 1)
				{
					m1 = vect3<metres,TypeParam>(val1x, val1y, val1z);
					auto m3 = m1 += m2;
					EXPECT_EQ(val1x + val2x, m1.x().value());
					EXPECT_EQ(val1y + val2y, m1.y().value());
					EXPECT_EQ(val1z + val2z, m1.z().value());
					EXPECT_EQ(val2x, m2.x().value());
					EXPECT_EQ(val2y, m2.y().value());
					EXPECT_EQ(val2z, m2.z().value());
					EXPECT_EQ(val1x + val2x, m3.x().value());
					EXPECT_EQ(val1y + val2y, m3.y().value());
					EXPECT_EQ(val1z + val2z, m3.z().value());
				}

				// Operator -=.
				// All test-values except the lowest one,
				// as it would cause overflow by decrementing.
				if (0 < i && 0 < j && 0 < k)
				{
					m1 = vect3<metres,TypeParam>(val1x, val1y, val1z);
					auto m3 = m1 -= m2;
					EXPECT_EQ(val1x - val2x, m1.x().value());
					EXPECT_EQ(val1y - val2y, m1.y().value());
					EXPECT_EQ(val1z - val2z, m1.z().value());
					EXPECT_EQ(val2x, m2.x().value());
					EXPECT_EQ(val2y, m2.y().value());
					EXPECT_EQ(val2z, m2.z().value());
					EXPECT_EQ(val1x - val2x, m3.x().value());
					EXPECT_EQ(val1y - val2y, m3.y().value());
					EXPECT_EQ(val1z - val2z, m3.z().value());
				}

				// Operator *=.
				// All test-values except the lowest and the largest ones,
				// as they would cause overflow by multiplying.
				if (0 < i && i < test_values<TypeParam>::count - 1
					&& 0 < j && j < test_values<TypeParam>::count - 1
					&& 0 < k && k < test_values<TypeParam>::count - 1)
				{
					m1 = vect3<metres,TypeParam>(val1x, val1y, val1z);
					auto m3 = m1 *= val2;
					EXPECT_EQ(val1x * val2, m1.x().value());
					EXPECT_EQ(val1y * val2, m1.y().value());
					EXPECT_EQ(val1z * val2, m1.z().value());
					EXPECT_EQ(val1x * val2, m3.x().value());
					EXPECT_EQ(val1y * val2, m3.y().value());
					EXPECT_EQ(val1z * val2, m3.z().value());
					auto m4 = m1 *= val3;
					EXPECT_EQ(val1x * val2 * val3, m1.x().value());
					EXPECT_EQ(val1y * val2 * val3, m1.y().value());
					EXPECT_EQ(val1z * val2 * val3, m1.z().value());
					EXPECT_EQ(val1x * val2 * val3, m4.x().value());
					EXPECT_EQ(val1y * val2 * val3, m4.y().value());
					EXPECT_EQ(val1z * val2 * val3, m4.z().value());
				}
				
				// Operator /=.
				// All test-values.
				if (val2 != 0)
				{
					m1 = vect3<metres,TypeParam>(val1x, val1y, val1z);
					auto m3 = m1 /= val2;
					EXPECT_EQ(val1x / val2, m1.x().value());
					EXPECT_EQ(val1y / val2, m1.y().value());
					EXPECT_EQ(val1z / val2, m1.z().value());
					EXPECT_EQ(val1x / val2, m3.x().value());
					EXPECT_EQ(val1y / val2, m3.y().value());
					EXPECT_EQ(val1z / val2, m3.z().value());
					if (val3 != 0)
					{
						auto m4 = m1 /= val3;
						EXPECT_EQ(val1x / val2 / val3, m1.x().value());
						EXPECT_EQ(val1y / val2 / val3, m1.y().value());
						EXPECT_EQ(val1z / val2 / val3, m1.z().value());
						EXPECT_EQ(val1x / val2 / val3, m4.x().value());
						EXPECT_EQ(val1y / val2 / val3, m4.y().value());
						EXPECT_EQ(val1z / val2 / val3, m4.z().value());
					}
				}

				// Relational operators.

				// Comparing equal measures.
				m1 = vect3<metres,TypeParam>(val1x, val1y, val1z);
				m2 = m1;
				EXPECT_TRUE(m1 == m2);
				EXPECT_FALSE(m1 != m2);

				// All test-values except the two lowest and the two largest ones,
				// as they would cause overflow by squaring.
				if (2 <= i && i < test_values<TypeParam>::count - 2
					&& 2 <= j && j < test_values<TypeParam>::count - 2
					&& 2 <= k && k < test_values<TypeParam>::count - 2)
				{
					EXPECT_TRUE(is_equal(m1, m2, zero));
					EXPECT_TRUE(is_equal(m1, m2, tolerance));

					// Comparing a measure with its half, if it is large,
					// with its double if it is small.
					if (abs(m2.x().value()) >= MIN_THRESHOLD)
					{
						if (squared_norm_value(m2) > 1) m2 /= 2;
						else m1 *= 2;
						EXPECT_FALSE(m1 == m2);
						EXPECT_TRUE(m1 != m2);
						EXPECT_FALSE(is_equal(m1, m2, zero));
					}
				}
			}
		}
	}

	// Comparing 1 with a bit more than 1.
	auto m3 = vect3<metres,TypeParam>(1, 1, 1);
	auto m4 = vect3<metres,TypeParam>(1 + epsilon, 1 + epsilon, 1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));

	// Comparing -1 with a bit less than -1.
	auto m5 = vect3<metres,TypeParam>(-1, -1, -1);
	auto m6 = vect3<metres,TypeParam>(-1 - epsilon, -1 - epsilon, -1 - epsilon);
	EXPECT_FALSE(m5 == m6);
	EXPECT_TRUE(m5 != m6);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));

	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = vect3<metres,TypeParam>(4, 4, 4);
	auto m8 = vect3<metres,TypeParam>(6, 6, 6);
	auto tol1 = vect1<metres,TypeParam>(3);
	auto tol2 = vect1<metres,TypeParam>(4);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));
}

TYPED_TEST(measureTest, point3)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<metres,TypeParam>(epsilon * 2);
	auto zero = vect1<metres,TypeParam>(0);
	
	// Try several numeric values.
	for (int i = 0; i < test_values<TypeParam>::count; ++i)
	{
		TypeParam val1x = test_values<TypeParam>::v[i];

		// Try several numeric values.
		for (int j = 0; j < test_values<TypeParam>::count; ++j)
		{
			TypeParam val1y = test_values<TypeParam>::v[j];

			// Try several numeric values.
			for (int k = 0; k < test_values<TypeParam>::count; ++k)
			{
				TypeParam val1z = test_values<TypeParam>::v[k];
				TypeParam val2 = static_cast<TypeParam>(2.19);
				TypeParam val2x = static_cast<TypeParam>(2.27);
				TypeParam val2y = static_cast<TypeParam>(2.13);
				TypeParam val2z = static_cast<TypeParam>(2.15);
				TypeParam val3 = 3;
				auto m1 = point3<metres,TypeParam>(val1x, val1y, val1z);
				auto m2 = vect3<metres,TypeParam>(val2x, val2y, val2z);

				// Construction and "value".
				EXPECT_EQ(val1x, m1.x().value());
				EXPECT_EQ(val1y, m1.y().value());
				EXPECT_EQ(val1z, m1.z().value());
				EXPECT_EQ(val2x, m2.x().value());
				EXPECT_EQ(val2y, m2.y().value());
				EXPECT_EQ(val2z, m2.z().value());

				// Operator +=.
				// All test-values except the largest one,
				// as it would cause overflow by incrementing.
				if (i < test_values<TypeParam>::count - 1
					&& j < test_values<TypeParam>::count - 1
					&& k < test_values<TypeParam>::count - 1)
				{
					m1 = point3<metres,TypeParam>(val1x, val1y, val1z);
					auto m3 = m1 += m2;
					EXPECT_EQ(val1x + val2x, m1.x().value());
					EXPECT_EQ(val1y + val2y, m1.y().value());
					EXPECT_EQ(val1z + val2z, m1.z().value());
					EXPECT_EQ(val2x, m2.x().value());
					EXPECT_EQ(val2y, m2.y().value());
					EXPECT_EQ(val2z, m2.z().value());
					EXPECT_EQ(val1x + val2x, m3.x().value());
					EXPECT_EQ(val1y + val2y, m3.y().value());
					EXPECT_EQ(val1z + val2z, m3.z().value());
				}

				// Operator -=.
				// All test-values except the lowest one,
				// as it would cause overflow by decrementing.
				if (0 < i && 0 < j && 0 < k)
				{
					m1 = point3<metres,TypeParam>(val1x, val1y, val1z);
					auto m3 = m1 -= m2;
					EXPECT_EQ(val1x - val2x, m1.x().value());
					EXPECT_EQ(val1y - val2y, m1.y().value());
					EXPECT_EQ(val1z - val2z, m1.z().value());
					EXPECT_EQ(val2x, m2.x().value());
					EXPECT_EQ(val2y, m2.y().value());
					EXPECT_EQ(val2z, m2.z().value());
					EXPECT_EQ(val1x - val2x, m3.x().value());
					EXPECT_EQ(val1y - val2y, m3.y().value());
					EXPECT_EQ(val1z - val2z, m3.z().value());
				}

				// Relational operators.

				// Comparing equal measures.
				m1 = point3<metres,TypeParam>(val1x, val1y, val1z);
				auto m3 = m1;
				EXPECT_TRUE(m1 == m3);
				EXPECT_FALSE(m1 != m3);

				// All test-values except the two lowest and the two largest ones,
				// as they would cause overflow by squaring.
				if (2 <= i && i < test_values<TypeParam>::count - 2
					&& 2 <= j && j < test_values<TypeParam>::count - 2
					&& 2 <= k && k < test_values<TypeParam>::count - 2)
				{
					EXPECT_TRUE(is_equal(m1, m3, zero));
					EXPECT_TRUE(is_equal(m1, m3, tolerance));

					// Comparing a measure with its half, if it is large,
					// with its double if it is small.
					if (abs(m3.x().value()) >= MIN_THRESHOLD)
					{
						if (abs(m3.x().value()) + abs(m3.y().value()) + abs(m3.z().value()) > 2)
							m3 = point3<metres,TypeParam>(m3.x().value() / 2, m3.y().value() / 2, m3.z().value() / 2);
						else m1 = point3<metres,TypeParam>(m1.x().value() * 2, m1.y().value() * 2, m1.z().value() * 2);
						EXPECT_FALSE(m1 == m3);
						EXPECT_TRUE(m1 != m3);					
						EXPECT_FALSE(is_equal(m1, m3, zero));
					}
				}
			}
		}
	}

	// Comparing 1 with a bit more than 1.
	auto m3 = point3<metres,TypeParam>(1, 1, 1);
	auto m4 = point3<metres,TypeParam>(1 + epsilon, 1 + epsilon, 1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));

	// Comparing -1 with a bit less than -1.
	auto m5 = point3<metres,TypeParam>(-1, -1, -1);
	auto m6 = point3<metres,TypeParam>(-1 - epsilon, -1 - epsilon, -1 - epsilon);
	EXPECT_FALSE(m5 == m6);
	EXPECT_TRUE(m5 != m6);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));

	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = point3<metres,TypeParam>(4, 4, 4);
	auto m8 = point3<metres,TypeParam>(6, 6, 6);
	auto tol1 = vect1<metres,TypeParam>(3);
	auto tol2 = vect1<metres,TypeParam>(4);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));
}

TYPED_TEST(measureTest, vectpoint3)
{
	// Midpoint.
	// Try all triples of pairs of numeric values except extremes.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1x = test_values<TypeParam>::v[i1];
		for (int j1 = 1; j1 < test_values<TypeParam>::count - 1; ++j1)
		{
			TypeParam val1y = test_values<TypeParam>::v[j1];
			for (int k1 = 1; k1 < test_values<TypeParam>::count - 1; ++k1)
			{
				TypeParam val1z = test_values<TypeParam>::v[k1];
				auto m1 = point3<metres,TypeParam>(val1x, val1y, val1z);
				for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
				{
					TypeParam val2x = test_values<TypeParam>::v[i2];
					for (int j2 = 1; j2 < test_values<TypeParam>::count - 1; ++j2)
					{
						TypeParam val2y = test_values<TypeParam>::v[j2];
						for (int k2 = 1; k2 < test_values<TypeParam>::count - 1; ++k2)
						{
							TypeParam val2z = test_values<TypeParam>::v[k2];
							auto m2 = point3<metres,TypeParam>(val2x, val2y, val2z);
							
							EXPECT_FLOAT_EQ((val1x + val2x) * 0.5f, midpoint(m1, m2, 0.5f).x().value());
							EXPECT_FLOAT_EQ((val1y + val2y) * 0.5f, midpoint(m1, m2, 0.5f).y().value());
							EXPECT_FLOAT_EQ((val1z + val2z) * 0.5f, midpoint(m1, m2, 0.5f).z().value());
							EXPECT_FLOAT_EQ(val1x, midpoint(m1, m2, 0).x().value());
							EXPECT_FLOAT_EQ(val1y, midpoint(m1, m2, 0).y().value());
							EXPECT_FLOAT_EQ(val1z, midpoint(m1, m2, 0).z().value());
							EXPECT_FLOAT_EQ(val2x, midpoint(m1, m2, 1).x().value());
							EXPECT_FLOAT_EQ(val2y, midpoint(m1, m2, 1).y().value());
							EXPECT_FLOAT_EQ(val2z, midpoint(m1, m2, 1).z().value());
							EXPECT_FLOAT_EQ(val1x * (1 - 0.23f) + val2x * 0.23f, midpoint(m1, m2, 0.23f).x().value());
							EXPECT_FLOAT_EQ(val1y * (1 - 0.23f) + val2y * 0.23f, midpoint(m1, m2, 0.23f).y().value());
							EXPECT_FLOAT_EQ(val1z * (1 - 0.23f) + val2z * 0.23f, midpoint(m1, m2, 0.23f).z().value());
						}
					}
				}
			}
		}
	}

	// Barycentric combination.
	// Try all triples of pairs of numeric values except extremes,
	// and add some other points.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1x = test_values<TypeParam>::v[i1];
		for (int j1 = 1; j1 < test_values<TypeParam>::count - 1; ++j1)
		{
			TypeParam val1y = test_values<TypeParam>::v[j1];
			for (int k1 = 1; k1 < test_values<TypeParam>::count - 1; ++k1)
			{
				TypeParam val1z = test_values<TypeParam>::v[k1];
				auto m1 = point3<metres,TypeParam>(val1x, val1y, val1z);
				for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
				{
					TypeParam val2x = test_values<TypeParam>::v[i2];
					for (int j2 = 1; j2 < test_values<TypeParam>::count - 1; ++j2)
					{
						TypeParam val2y = test_values<TypeParam>::v[j2];
						for (int k2 = 1; k2 < test_values<TypeParam>::count - 1; ++k2)
						{
							TypeParam val2z = test_values<TypeParam>::v[k2];
							auto m2 = point3<metres,TypeParam>(val2x, val2y, val2z);

							TypeParam val3x = static_cast<TypeParam>(3.1);
							TypeParam val3y = static_cast<TypeParam>(3.2);
							TypeParam val3z = static_cast<TypeParam>(3.3);
							auto m3 = point3<metres,TypeParam>(val3x, val3y, val3z);
							TypeParam val4x = static_cast<TypeParam>(4.3);
							TypeParam val4y = static_cast<TypeParam>(4.4);
							TypeParam val4z = static_cast<TypeParam>(4.5);
							auto m4 = point3<metres,TypeParam>(val4x, val4y, val4z);
							point3<metres,TypeParam> point3array[] = { m1, m2, m3, m4 };
							TypeParam weights[] = { 2, 3, 7, 4 };
							EXPECT_FLOAT_EQ(val1x * weights[0],
								barycentric_combination(1, point3array, weights).x().value());
							EXPECT_FLOAT_EQ(val1y * weights[0],
								barycentric_combination(1, point3array, weights).y().value());
							EXPECT_FLOAT_EQ(val1z * weights[0],
								barycentric_combination(1, point3array, weights).z().value());
							EXPECT_FLOAT_EQ(val1x * weights[0] + val2x * weights[1],
								barycentric_combination(2, point3array, weights).x().value());
							EXPECT_FLOAT_EQ(val1y * weights[0] + val2y * weights[1],
								barycentric_combination(2, point3array, weights).y().value());
							EXPECT_FLOAT_EQ(val1z * weights[0] + val2z * weights[1],
								barycentric_combination(2, point3array, weights).z().value());
							EXPECT_FLOAT_EQ(val1x * weights[0] + val2x * weights[1]
								+ val3x * weights[2],
								barycentric_combination(3, point3array, weights).x().value());
							EXPECT_FLOAT_EQ(val1y * weights[0] + val2y * weights[1]
								+ val3y * weights[2],
								barycentric_combination(3, point3array, weights).y().value());
							EXPECT_FLOAT_EQ(val1z * weights[0] + val2z * weights[1]
								+ val3z * weights[2],
								barycentric_combination(3, point3array, weights).z().value());
							EXPECT_FLOAT_EQ(val1x * weights[0] + val2x * weights[1]
								+ val3x * weights[2] + val4x * weights[3],
								barycentric_combination(4, point3array, weights).x().value());
							EXPECT_FLOAT_EQ(val1y * weights[0] + val2y * weights[1]
								+ val3y * weights[2] + val4y * weights[3],
								barycentric_combination(4, point3array, weights).y().value());
							EXPECT_FLOAT_EQ(val1z * weights[0] + val2z * weights[1]
								+ val3z * weights[2] + val4z * weights[3],
								barycentric_combination(4, point3array, weights).z().value());
						}
					}
				}
			}
		}
	}

	// Try all triples of pairs of numeric values except extremes.
	TypeParam sqrt_of_max = static_cast<TypeParam>(sqrt(test_values<TypeParam>
		::v[test_values<TypeParam>::count - 1]));
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1x = test_values<TypeParam>::v[i1];
		for (int j1 = 1; j1 < test_values<TypeParam>::count - 1; ++j1)
		{
			TypeParam val1y = test_values<TypeParam>::v[j1];
			for (int k1 = 1; k1 < test_values<TypeParam>::count - 1; ++k1)
			{
				TypeParam val1z = test_values<TypeParam>::v[k1];
				auto v1 = vect3<metres,TypeParam>(val1x, val1y, val1z);
				auto p1 = point3<metres,TypeParam>(val1x, val1y, val1z);

				for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
				{
					TypeParam val2x = test_values<TypeParam>::v[i2];
					for (int j2 = 1; j2 < test_values<TypeParam>::count - 1; ++j2)
					{
						TypeParam val2y = test_values<TypeParam>::v[j2];
						for (int k2 = 1; k2 < test_values<TypeParam>::count - 1; ++k2)
						{
							TypeParam val2z = test_values<TypeParam>::v[k2];
							auto v2 = vect3<metres,TypeParam>(val2x, val2y, val2z);
							auto p2 = point3<metres,TypeParam>(val2x, val2y, val2z);

							// point3 - point3 -> vect3
							EXPECT_EQ(val1x - val2x, (p1 - p2).x().value());
							EXPECT_EQ(val1y - val2y, (p1 - p2).y().value());
							EXPECT_EQ(val1z - val2z, (p1 - p2).z().value());

							// point3 + vect3 -> point3
							EXPECT_EQ(val1x + val2x, (p1 + v2).x().value());
							EXPECT_EQ(val1y + val2y, (p1 + v2).y().value());
							EXPECT_EQ(val1z + val2z, (p1 + v2).z().value());

							// point3 - vect3 -> point3
							EXPECT_EQ(val1x - val2x, (p1 - v2).x().value());
							EXPECT_EQ(val1y - val2y, (p1 - v2).y().value());
							EXPECT_EQ(val1z - val2z, (p1 - v2).z().value());

							// vect3 + vect3 -> vect3
							EXPECT_EQ(val1x + val2x, (v1 + v2).x().value());
							EXPECT_EQ(val1y + val2y, (v1 + v2).y().value());
							EXPECT_EQ(val1z + val2z, (v1 + v2).z().value());

							// vect3 - vect3 -> vect3
							EXPECT_EQ(val1x - val2x, (v1 - v2).x().value());
							EXPECT_EQ(val1y - val2y, (v1 - v2).y().value());
							EXPECT_EQ(val1z - val2z, (v1 - v2).z().value());

							if (abs(val1x) < sqrt_of_max && abs(val2x) < sqrt_of_max)
							{
								// N * vect3 -> vect3
								EXPECT_EQ(val1x * val2x, (val1x * v2).x().value());

								// vect3 * N -> vect3
								EXPECT_EQ(val1x * val2x, (v1 * val2x).x().value());
							}

							if (abs(val1y) < sqrt_of_max && abs(val2y) < sqrt_of_max)
							{
								// N * vect3 -> vect3
								EXPECT_EQ(val1y * val2y, (val1y * v2).y().value());

								// vect3 * N -> vect3
								EXPECT_EQ(val1y * val2y, (v1 * val2y).y().value());
							}

							if (abs(val1z) < sqrt_of_max && abs(val2z) < sqrt_of_max)
							{
								// N * vect3 -> vect3
								EXPECT_EQ(val1z * val2z, (val1z * v2).z().value());

								// vect3 * N -> vect3
								EXPECT_EQ(val1z * val2z, (v1 * val2z).z().value());	
							}

							// vect3 / N -> vect3
							if (val2x != 0) EXPECT_EQ(val1x / val2x, (v1 / val2x).x().value());
							if (val2y != 0) EXPECT_EQ(val1y / val2y, (v1 / val2y).y().value());
							if (val2z != 0) EXPECT_EQ(val1z / val2z, (v1 / val2z).z().value());
						}
					}
				}
			}
		}
	}
	
	for (int i = 2; i < test_values<TypeParam>::count - 2; ++i)
	{
		for (int j = 2; j < test_values<TypeParam>::count - 2; ++j)
		{
			for (int k = 2; k < test_values<TypeParam>::count - 2; ++k)
			{
				TypeParam val1x = test_values<TypeParam>::v[i];
				TypeParam val1y = test_values<TypeParam>::v[j];
				TypeParam val1z = test_values<TypeParam>::v[k];
				auto m1 = vect3<metres, TypeParam>(val1x, val1y, val1z);
				if (abs(val1x) < sqrt_of_max && abs(val1y) < sqrt_of_max && abs(val1z) < sqrt_of_max)
				{
					EXPECT_FLOAT_EQ(val1x * val1x + val1y * val1y + val1z * val1z, squared_norm_value(m1));
					EXPECT_FLOAT_EQ(static_cast<TypeParam>(
						sqrt(val1x * val1x + val1y * val1y + val1z * val1z)), norm(m1).value());
				}
			}
		}
	}
}

//// azimuths
TYPED_TEST(measureTest, signed_azimuth)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<degrees,TypeParam>(epsilon * 2);
	auto zero = vect1<degrees,TypeParam>(0);
	
	// Try several numeric values except extremes.
	for (int i = 1; i < test_values<TypeParam>::count - 1; ++i)
	{
		TypeParam val1 = test_values<TypeParam>::v[i];
		TypeParam val2 = static_cast<TypeParam>(2.19);
		if (abs(val1) < MAX_THRESHOLD && abs(val2) < MAX_THRESHOLD)
		{
			auto m1 = signed_azimuth<degrees,TypeParam>(val1);
			auto m2 = vect1<degrees,TypeParam>(val2);

			// Construction and "value".
			EXPECT_NEAR(modulo(val1 + 180, 360) - 180, m1.value(), AZIMUTH_TOLERANCE);
			EXPECT_EQ(val2, m2.value());

			// Operator +=.
			// All test-values except the largest one,
			// as it would cause overflow by incrementing.
			if (i < test_values<TypeParam>::count - 1)
			{
				m1 = signed_azimuth<degrees,TypeParam>(val1);
				auto m3 = m1 += m2;
				EXPECT_NEAR(modulo(val1 + val2 + 180, 360) - 180, m1.value(), AZIMUTH_TOLERANCE);
				EXPECT_NEAR(modulo(val1 + val2 + 180, 360) - 180, m3.value(), AZIMUTH_TOLERANCE);
			}

			// Operator -=.
			// All test-values except the lowest one,
			// as it would cause overflow by decrementing.
			if (0 < i)
			{
				m1 = signed_azimuth<degrees,TypeParam>(val1);
				auto m3 = m1 -= m2;
				EXPECT_NEAR(modulo(val1 - val2 + 180, 360) - 180, m1.value(), AZIMUTH_TOLERANCE);
				EXPECT_NEAR(modulo(val1 - val2 + 180, 360) - 180, m3.value(), AZIMUTH_TOLERANCE);
			}
		}

		// Relational operators.
		
		// Comparing equal measures.
		auto m1 = signed_azimuth<degrees,TypeParam>(val1);
		auto m3 = m1;
		EXPECT_TRUE(m1 == m3);
		EXPECT_FALSE(m1 != m3);
		EXPECT_TRUE(is_equal(m1, m3, zero));

		// Comparing equal measures with a tolerance.
		// Avoid extreme test-values with a non-zero tolerance,
		// as they would cause overflow by incrementing or decrementing.
		if (0 < i && i < test_values<TypeParam>::count - 1)
		{
			EXPECT_TRUE(is_equal(m1, m3, tolerance));
		}

		// Comparing a measure with its half, if it is large,
		// with its double if it is small.
		// Omit test if it is near zero or larger than a turn.
		if (abs(val1) >= MIN_THRESHOLD && abs(val1) <= 360)
		{
			if (abs(val1) > 2)
				m3 = signed_azimuth<degrees,TypeParam>(val1 / 2);
			else m1 = signed_azimuth<degrees,TypeParam>(val1 * 2);

			EXPECT_FALSE(m1 == m3);
			EXPECT_TRUE(m1 != m3);
			EXPECT_FALSE(is_equal(m1, m3, zero));
		}
	}
	
	// Comparing 1 with a bit more than 1.
	auto m3 = signed_azimuth<degrees,TypeParam>(1);
	auto m4 = signed_azimuth<degrees,TypeParam>(1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));

	// Comparing -1 with a bit less than -1.
	auto m5 = signed_azimuth<degrees,TypeParam>(-1);
	auto m6 = signed_azimuth<degrees,TypeParam>(-1 - epsilon);
	EXPECT_FALSE(m5 == m6);
	EXPECT_TRUE(m5 != m6);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));

	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = signed_azimuth<degrees,TypeParam>(45);
	auto m8 = signed_azimuth<degrees,TypeParam>(48);
	auto tol1 = vect1<degrees,TypeParam>(2);
	auto tol2 = vect1<degrees,TypeParam>(4);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));
	
	// Try all pairs of numeric values except extremes.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1 = test_values<TypeParam>::v[i1];
		auto v1 = vect1<degrees,TypeParam>(val1);
		auto p1 = signed_azimuth<degrees,TypeParam>(val1);
		for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
		{
			TypeParam val2 = test_values<TypeParam>::v[i2];
			if (abs(val1) < MAX_THRESHOLD && abs(val2) < MAX_THRESHOLD
				&& abs(val1) > MIN_THRESHOLD && abs(val2) > MIN_THRESHOLD)			
			{
				auto v2 = vect1<degrees,TypeParam>(val2);
				auto p2 = signed_azimuth<degrees,TypeParam>(val2);
			
				// signed_azimuth - signed_azimuth -> vect1
				EXPECT_NEAR(modulo(val1 - val2 + 180, 360) - 180,
					(p1 - p2).value(), AZIMUTH_TOLERANCE);

				// signed_azimuth + vect1 -> signed_azimuth
				EXPECT_NEAR(modulo(val1 + val2 + 180, 360) - 180,
					(p1 + v2).value(), AZIMUTH_TOLERANCE);

				// signed_azimuth - vect1 -> signed_azimuth
				EXPECT_NEAR(modulo(val1 - val2 + 180, 360) - 180,
					(p1 - v2).value(), AZIMUTH_TOLERANCE);
			}
		}
	}	
}
///////////////
TYPED_TEST(measureTest, unsigned_azimuth)
{
	auto epsilon = numeric_limits<TypeParam>::is_integer ?
		1 : numeric_limits<TypeParam>::epsilon();
	auto tolerance = vect1<degrees,TypeParam>(epsilon * 2);
	auto zero = vect1<degrees,TypeParam>(0);
	
	// Try several numeric values.
	for (int i = 0; i < test_values<TypeParam>::count; ++i)
	{
		TypeParam val1 = test_values<TypeParam>::v[i];
		TypeParam val2 = static_cast<TypeParam>(2.19);
		if (abs(val1) < MAX_THRESHOLD && abs(val2) < MAX_THRESHOLD)
		{
			auto m1 = unsigned_azimuth<degrees,TypeParam>(val1);
			auto m2 = vect1<degrees,TypeParam>(val2);

			// Construction and "value".
			EXPECT_NEAR(modulo(val1, 360), m1.value(), AZIMUTH_TOLERANCE);
			EXPECT_EQ(val2, m2.value());

			// Operator +=.
			// All test-values except the largest one,
			// as it would cause overflow by incrementing.
			if (i < test_values<TypeParam>::count - 1)
			{
				m1 = unsigned_azimuth<degrees,TypeParam>(val1);
				auto m3 = m1 += m2;
				EXPECT_NEAR(modulo(val1 + val2, 360), m1.value(), AZIMUTH_TOLERANCE);
				EXPECT_NEAR(modulo(val1 + val2, 360), m3.value(), AZIMUTH_TOLERANCE);
			}

			// Operator -=.
			// All test-values except the lowest one,
			// as it would cause overflow by decrementing.
			if (0 < i)
			{
				m1 = unsigned_azimuth<degrees,TypeParam>(val1);
				auto m3 = m1 -= m2;
				EXPECT_NEAR(modulo(val1 - val2, 360), m1.value(), AZIMUTH_TOLERANCE);
				EXPECT_NEAR(modulo(val1 - val2, 360), m3.value(), AZIMUTH_TOLERANCE);
			}
		}

		// Relational operators.
		
		// Comparing equal measures.
		auto m1 = unsigned_azimuth<degrees,TypeParam>(val1);
		auto m3 = m1;
		EXPECT_TRUE(m1 == m3);
		EXPECT_FALSE(m1 != m3);
		EXPECT_TRUE(is_equal(m1, m3, zero));

		// Comparing equal measures with a tolerance.
		// Avoid extreme test-values with a non-zero tolerance,
		// as they would cause overflow by incrementing or decrementing.
		if (0 < i && i < test_values<TypeParam>::count - 1)
		{
			EXPECT_TRUE(is_equal(m1, m3, tolerance));
		}

		// Comparing a measure with its half, if it is large,
		// with its double if it is small.
		// Omit test if it is near zero or larger than a turn.
		if (abs(val1) >= MIN_THRESHOLD && abs(val1) <= 360)
		{
			if (abs(val1) > 2)
				m3 = unsigned_azimuth<degrees,TypeParam>(val1 / 2);
			else m1 = unsigned_azimuth<degrees,TypeParam>(val1 * 2);

			EXPECT_FALSE(m1 == m3);
			EXPECT_TRUE(m1 != m3);
			EXPECT_FALSE(is_equal(m1, m3, zero));
		}
	}
	
	// Comparing 1 with a bit more than 1.
	auto m3 = unsigned_azimuth<degrees,TypeParam>(1);
	auto m4 = unsigned_azimuth<degrees,TypeParam>(1 + epsilon);
	EXPECT_FALSE(m3 == m4);
	EXPECT_TRUE(m3 != m4);
	EXPECT_TRUE(is_equal(m3, m4, tolerance));
	
	// Comparing -1 with a bit less than -1.
	auto m5 = unsigned_azimuth<degrees,TypeParam>(-1);
	auto m6 = unsigned_azimuth<degrees,TypeParam>(-1 - epsilon);
	EXPECT_TRUE(is_equal(m5, m6, tolerance));
	
	// Compare non-extreme numbers, far from zero,
	// with a small tolerance or with a large tolerance.
	auto m7 = unsigned_azimuth<degrees,TypeParam>(45);
	auto m8 = unsigned_azimuth<degrees,TypeParam>(48);
	auto tol1 = vect1<degrees,TypeParam>(2);
	auto tol2 = vect1<degrees,TypeParam>(4);
	EXPECT_FALSE(is_equal(m7, m8, tol1));
	EXPECT_FALSE(is_equal(m8, m7, tol1));
	EXPECT_TRUE(is_equal(m7, m8, tol2));
	EXPECT_TRUE(is_equal(m8, m7, tol2));
	
	// Try all pairs of numeric values except extremes.
	for (int i1 = 1; i1 < test_values<TypeParam>::count - 1; ++i1)
	{
		TypeParam val1 = test_values<TypeParam>::v[i1];
		auto v1 = vect1<degrees,TypeParam>(val1);
		auto p1 = unsigned_azimuth<degrees,TypeParam>(val1);
		for (int i2 = 1; i2 < test_values<TypeParam>::count - 1; ++i2)
		{
			TypeParam val2 = test_values<TypeParam>::v[i2];
			if (abs(val1) < MAX_THRESHOLD && abs(val2) < MAX_THRESHOLD
				&& abs(val1) > MIN_THRESHOLD && abs(val2) > MIN_THRESHOLD)
			{
				auto v2 = vect1<degrees,TypeParam>(val2);
				
				auto p2 = unsigned_azimuth<degrees,TypeParam>(val2);

				// unsigned_azimuth - unsigned_azimuth -> vect1
				EXPECT_NEAR(modulo(val1 - val2 + 180, 360) - 180,
					(p1 - p2).value(), AZIMUTH_TOLERANCE);

				// unsigned_azimuth + vect1 -> unsigned_azimuth
				EXPECT_NEAR(modulo(val1 + val2, 360),
					(p1 + v2).value(), AZIMUTH_TOLERANCE);

				// unsigned_azimuth - vect1 -> unsigned_azimuth
				EXPECT_NEAR(modulo(val1 - val2, 360),
					(p1 - v2).value(), AZIMUTH_TOLERANCE);
			}
		}
	}	
}

//#endif

DEFINE_UNIT(km, Space, " Km", 1000, 0)
DEFINE_UNIT(inches, Space, "\"", 0.0254, 0)

DEFINE_MAGNITUDE(Time, seconds, " s")
DEFINE_UNIT(hours, Time, " h", 3600, 0)
DEFINE_UNIT(days, Time, " d", 86400, 0)

DEFINE_MAGNITUDE(Speed, metres_per_second, " m/s")
DEFINE_UNIT(km_per_hour, Speed, " Km/h", 1 / 3.6, 0)
DEFINE_UNIT(inches_per_day, Speed, "\"/day", 86400 / 0.0254, 0)

DEFINE_MAGNITUDE(Area, square_metres, " m2")
DEFINE_UNIT(square_km, Area, " Km2", 1000000, 0)
DEFINE_UNIT(square_inches, Area, "\"2", 0.0254 * 0.0254, 0)

DEFINE_MAGNITUDE(Temperature, kelvin, "°K")
DEFINE_UNIT(celsius, Temperature, "°C", 1, 273.15)
DEFINE_UNIT(fahrenheit, Temperature, "°F", 5. / 9., 273.15 - 32. * 5. / 9.)

TEST(unitTest, magnitudes)
{
	ASSERT_STREQ(" rad", radians::id().suffix());
	EXPECT_EQ(1, radians::id().ratio());
	EXPECT_EQ(0, radians::id().offset());

	ASSERT_STREQ(" rev", turns::id().suffix());
	EXPECT_NEAR(2 * pi, turns::id().ratio(), AZIMUTH_TOLERANCE);
	EXPECT_EQ(0, turns::id().offset());

	ASSERT_STREQ(" m", metres::id().suffix());
	EXPECT_EQ(1, metres::id().ratio());
	EXPECT_EQ(0, metres::id().offset());

	ASSERT_STREQ("°", degrees::id().suffix());
	EXPECT_NEAR(pi / 180, degrees::id().ratio(), AZIMUTH_TOLERANCE);
	EXPECT_EQ(0, degrees::id().offset());

	ASSERT_STREQ(" Km", km::id().suffix());
	EXPECT_EQ(1000, km::id().ratio());
	EXPECT_EQ(0, km::id().offset());

	ASSERT_STREQ("\"", inches::id().suffix());
	EXPECT_EQ(0.0254, inches::id().ratio());
	EXPECT_EQ(0, inches::id().offset());

	ASSERT_STREQ(" s", seconds::id().suffix());
	EXPECT_EQ(1, metres::id().ratio());
	EXPECT_EQ(0, metres::id().offset());

	ASSERT_STREQ(" h", hours::id().suffix());
	EXPECT_EQ(3600, hours::id().ratio());
	EXPECT_EQ(0, hours::id().offset());

	ASSERT_STREQ(" d", days::id().suffix());
	EXPECT_EQ(86400, days::id().ratio());
	EXPECT_EQ(0, days::id().offset());
		
	ASSERT_STREQ(" m/s", metres_per_second::id().suffix());
	EXPECT_EQ(1, metres_per_second::id().ratio());
	EXPECT_EQ(0, metres_per_second::id().offset());

	ASSERT_STREQ(" Km/h", km_per_hour::id().suffix());
	EXPECT_FLOAT_EQ(1 / 3.6, km_per_hour::id().ratio());
	EXPECT_EQ(0, km_per_hour::id().offset());

	ASSERT_STREQ("\"/day", inches_per_day::id().suffix());
	EXPECT_EQ(86400 / 0.0254, inches_per_day::id().ratio());
	EXPECT_EQ(0, inches_per_day::id().offset());

	ASSERT_STREQ(" m2", square_metres::id().suffix());
	EXPECT_EQ(1, square_metres::id().ratio());
	EXPECT_EQ(0, square_metres::id().offset());

	ASSERT_STREQ(" Km2", square_km::id().suffix());
	EXPECT_EQ(1000000, square_km::id().ratio());
	EXPECT_EQ(0, square_km::id().offset());

	ASSERT_STREQ("\"2", square_inches::id().suffix());
	EXPECT_EQ(0.0254 * 0.0254, square_inches::id().ratio());
	EXPECT_EQ(0, square_inches::id().offset());

	ASSERT_STREQ("°K", kelvin::id().suffix());
	EXPECT_EQ(1, kelvin::id().ratio());
	EXPECT_EQ(0, kelvin::id().offset());

	ASSERT_STREQ("°C", celsius::id().suffix());
	EXPECT_EQ(1, celsius::id().ratio());
	EXPECT_EQ(273.15, celsius::id().offset());

	ASSERT_STREQ("°F", fahrenheit::id().suffix());
	EXPECT_EQ(5. / 9., fahrenheit::id().ratio());
	EXPECT_EQ(273.15 - 32. * 5. / 9., fahrenheit::id().offset());
}

TEST(unitTest, units)
{
	ASSERT_STREQ(" rad", radians::suffix());
	EXPECT_EQ(1, radians::ratio());
	EXPECT_EQ(0, radians::offset());

	ASSERT_STREQ(" rev", turns::suffix());
	EXPECT_NEAR(2 * pi, turns::ratio(), AZIMUTH_TOLERANCE);
	EXPECT_EQ(0, turns::offset());

	ASSERT_STREQ(" m", metres::suffix());
	EXPECT_EQ(1, metres::ratio());
	EXPECT_EQ(0, metres::offset());

	ASSERT_STREQ("°", degrees::suffix());
	EXPECT_NEAR(pi / 180, degrees::ratio(), AZIMUTH_TOLERANCE);
	EXPECT_EQ(0, degrees::offset());

	ASSERT_STREQ(" Km", km::suffix());
	EXPECT_EQ(1000, km::ratio());
	EXPECT_EQ(0, km::offset());

	ASSERT_STREQ("\"", inches::suffix());
	EXPECT_EQ(0.0254, inches::ratio());
	EXPECT_EQ(0, inches::offset());

	ASSERT_STREQ(" s", seconds::suffix());
	EXPECT_EQ(1, metres::ratio());
	EXPECT_EQ(0, metres::offset());

	ASSERT_STREQ(" h", hours::suffix());
	EXPECT_EQ(3600, hours::ratio());
	EXPECT_EQ(0, hours::offset());

	ASSERT_STREQ(" d", days::suffix());
	EXPECT_EQ(86400, days::ratio());
	EXPECT_EQ(0, days::offset());
		
	ASSERT_STREQ(" m/s", metres_per_second::suffix());
	EXPECT_EQ(1, metres_per_second::ratio());
	EXPECT_EQ(0, metres_per_second::offset());

	ASSERT_STREQ(" Km/h", km_per_hour::suffix());
	EXPECT_FLOAT_EQ(1 / 3.6, km_per_hour::ratio());
	EXPECT_EQ(0, km_per_hour::offset());

	ASSERT_STREQ("\"/day", inches_per_day::suffix());
	EXPECT_EQ(86400 / 0.0254, inches_per_day::ratio());
	EXPECT_EQ(0, inches_per_day::offset());

	ASSERT_STREQ(" m2", square_metres::suffix());
	EXPECT_EQ(1, square_metres::ratio());
	EXPECT_EQ(0, square_metres::offset());

	ASSERT_STREQ(" Km2", square_km::suffix());
	EXPECT_EQ(1000000, square_km::ratio());
	EXPECT_EQ(0, square_km::offset());

	ASSERT_STREQ("\"2", square_inches::suffix());
	EXPECT_EQ(0.0254 * 0.0254, square_inches::ratio());
	EXPECT_EQ(0, square_inches::offset());

	ASSERT_STREQ("°K", kelvin::suffix());
	EXPECT_EQ(1, kelvin::ratio());
	EXPECT_EQ(0, kelvin::offset());

	ASSERT_STREQ("°C", celsius::suffix());
	EXPECT_EQ(1, celsius::ratio());
	EXPECT_EQ(273.15, celsius::offset());

	ASSERT_STREQ("°F", fahrenheit::suffix());
	EXPECT_EQ(5. / 9., fahrenheit::ratio());
	EXPECT_EQ(273.15 - 32. * 5. / 9., fahrenheit::offset());
}

TEST(unitTest, vect_conversions)
{
	EXPECT_FLOAT_EQ(101. / 2 / pi, convert<turns>(vect1<radians>(101)).value());
	EXPECT_FLOAT_EQ(101. * 2 * pi, convert<radians>(vect1<turns>(101)).value());
	EXPECT_FLOAT_EQ(101. / 2 / pi * 360, convert<degrees>(vect1<radians>(101)).value());
	EXPECT_FLOAT_EQ(101. * 2 * pi / 360, convert<radians>(vect1<degrees>(101)).value());
	EXPECT_FLOAT_EQ(101. / 360, convert<turns>(vect1<degrees>(101)).value());
	EXPECT_FLOAT_EQ(101. * 360, convert<degrees>(vect1<turns>(101)).value());
	
	EXPECT_FLOAT_EQ(101. * 1000 / 0.0254, convert<inches>(vect1<km>(101)).value());
	EXPECT_FLOAT_EQ(101. / 1000 * 0.0254, convert<km>(vect1<inches>(101)).value());

	EXPECT_FLOAT_EQ(101., convert<celsius>(vect1<kelvin>(101)).value());
	EXPECT_FLOAT_EQ(101., convert<kelvin>(vect1<celsius>(101)).value());
	EXPECT_FLOAT_EQ(101. * 9 / 5, convert<fahrenheit>(vect1<kelvin>(101)).value());
	EXPECT_FLOAT_EQ(101. * 5 / 9, convert<kelvin>(vect1<fahrenheit>(101)).value());
	EXPECT_FLOAT_EQ(101. / 9 * 5, convert<celsius>(vect1<fahrenheit>(101)).value());
	EXPECT_FLOAT_EQ(101. / 5 * 9, convert<fahrenheit>(vect1<celsius>(101)).value());

	auto a1 = convert<inches>(vect2<metres>(101, 102));
	EXPECT_FLOAT_EQ(101. / 0.0254, a1.x().value());
	EXPECT_FLOAT_EQ(102. / 0.0254, a1.y().value());
	
	auto a2 = convert<metres>(vect2<inches>(101, 102));
	EXPECT_FLOAT_EQ(101. * 0.0254, a2.x().value());
	EXPECT_FLOAT_EQ(102. * 0.0254, a2.y().value());

	auto a3 = convert<inches>(vect3<metres>(101, 102, 103));
	EXPECT_FLOAT_EQ(101. / 0.0254, a3.x().value());
	EXPECT_FLOAT_EQ(102. / 0.0254, a3.y().value());
	EXPECT_FLOAT_EQ(103. / 0.0254, a3.z().value());
	
	auto a4 = convert<metres>(vect3<inches>(101, 102, 103));
	EXPECT_FLOAT_EQ(101. * 0.0254, a4.x().value());
	EXPECT_FLOAT_EQ(102. * 0.0254, a4.y().value());
	EXPECT_FLOAT_EQ(103. * 0.0254, a4.z().value());
}

TEST(unitTest, point_conversions)
{
	EXPECT_FLOAT_EQ(101. / 2 / pi, convert<turns>(point1<radians>(101)).value());
	EXPECT_FLOAT_EQ(101. * 2 * pi, convert<radians>(point1<turns>(101)).value());
	EXPECT_FLOAT_EQ(101. / 2 / pi * 360, convert<degrees>(point1<radians>(101)).value());
	EXPECT_FLOAT_EQ(101. * 2 * pi / 360, convert<radians>(point1<degrees>(101)).value());
	EXPECT_FLOAT_EQ(101. / 360, convert<turns>(point1<degrees>(101)).value());
	EXPECT_FLOAT_EQ(101. * 360, convert<degrees>(point1<turns>(101)).value());
	
	EXPECT_FLOAT_EQ(101. * 1000 / 0.0254, convert<inches>(point1<km>(101)).value());
	EXPECT_FLOAT_EQ(101. / 1000 * 0.0254, convert<km>(point1<inches>(101)).value());

	EXPECT_FLOAT_EQ(101. - 273.15, convert<celsius>(point1<kelvin>(101)).value());
	EXPECT_FLOAT_EQ(101. + 273.15, convert<kelvin>(point1<celsius>(101)).value());
	EXPECT_FLOAT_EQ((101. - 273.15) * 9 / 5 + 32, convert<fahrenheit>(point1<kelvin>(101)).value());
	EXPECT_FLOAT_EQ((101. - 32) * 5 / 9 + 273.15, convert<kelvin>(point1<fahrenheit>(101)).value());
	EXPECT_FLOAT_EQ((101. - 32) / 9 * 5, convert<celsius>(point1<fahrenheit>(101)).value());
	EXPECT_FLOAT_EQ(101. / 5 * 9 + 32, convert<fahrenheit>(point1<celsius>(101)).value());

	auto a1 = convert<inches>(point2<metres>(101, 102));
	EXPECT_FLOAT_EQ(101. / 0.0254, a1.x().value());
	EXPECT_FLOAT_EQ(102. / 0.0254, a1.y().value());
	
	auto a2 = convert<metres>(point2<inches>(101, 102));
	EXPECT_FLOAT_EQ(101. * 0.0254, a2.x().value());
	EXPECT_FLOAT_EQ(102. * 0.0254, a2.y().value());

	auto a3 = convert<inches>(point3<metres>(101, 102, 103));
	EXPECT_FLOAT_EQ(101. / 0.0254, a3.x().value());
	EXPECT_FLOAT_EQ(102. / 0.0254, a3.y().value());
	EXPECT_FLOAT_EQ(103. / 0.0254, a3.z().value());
	
	auto a4 = convert<metres>(point3<inches>(101, 102, 103));
	EXPECT_FLOAT_EQ(101. * 0.0254, a4.x().value());
	EXPECT_FLOAT_EQ(102. * 0.0254, a4.y().value());
	EXPECT_FLOAT_EQ(103. * 0.0254, a4.z().value());
}

TEST(unitTest, azimuth_conversions)
{
	EXPECT_FLOAT_EQ(fmod(1010.3 / 2 / pi + 0.5, 1.) - 0.5, convert<turns>(signed_azimuth<radians>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 * 2 * pi + pi, 2. * pi) - pi, convert<radians>(signed_azimuth<turns>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 / 2 / pi * 360 + 180, 360.) - 180, convert<degrees>(signed_azimuth<radians>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 * 2 * pi / 360 + pi, 2. * pi) - pi, convert<radians>(signed_azimuth<degrees>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 / 360 + 0.5, 1.) - 0.5, convert<turns>(signed_azimuth<degrees>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 * 360 + 180, 360.) - 180, convert<degrees>(signed_azimuth<turns>(1010.3)).value());

	EXPECT_FLOAT_EQ(fmod(1010.3 / 2 / pi, 1.), convert<turns>(unsigned_azimuth<radians>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 * 2 * pi, 2. * pi), convert<radians>(unsigned_azimuth<turns>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 / 2 / pi * 360, 360.), convert<degrees>(unsigned_azimuth<radians>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 * 2 * pi / 360, 2. * pi), convert<radians>(unsigned_azimuth<degrees>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 / 360, 1.), convert<turns>(unsigned_azimuth<degrees>(1010.3)).value());
	EXPECT_FLOAT_EQ(fmod(1010.3 * 360, 360.), convert<degrees>(unsigned_azimuth<turns>(1010.3)).value());
}

DEFINE_MAGNITUDE(Volume, cubic_metres, " m3")
DEFINE_MAGNITUDE(Density, kg_per_cubic_metre, " Kg/m3")
DEFINE_MAGNITUDE(Mass, kg, " Kg")
DEFINE_MAGNITUDE(Force, newtons, " N")
DEFINE_MAGNITUDE(Energy, joules, " J")
DEFINE_MAGNITUDE(Unitless, units, " u.")
DEFINE_UNIT(dozens, Unitless, " doz.", 12, 0)
DEFINE_MAGNITUDE(MagneticField, tesla, " T")
DEFINE_MAGNITUDE(ElectricField, volts_per_metre, " V/m")

DEFINE_DERIVED_UNIT_SCALAR_SCALAR(cubic_metres, kg_per_cubic_metre, kg)
DEFINE_DERIVED_UNIT_SQUARED_SCALAR(metres, square_metres)
DEFINE_DERIVED_UNIT_SCALAR_VECTOR(seconds, metres_per_second, metres)
DEFINE_DERIVED_UNIT_VECTOR_VECTOR(newtons, metres, joules)
DEFINE_DERIVED_UNIT_VECTOR_VECTOR(metres_per_second, tesla, volts_per_metre)
DEFINE_DERIVED_UNIT_SQUARED_VECTOR(units, units)

DEFINE_DERIVED_UNIT_SQUARED_SCALAR(inches, square_inches)
DEFINE_DERIVED_UNIT_SCALAR_VECTOR(hours, km_per_hour, km)

TEST(unitTest, derived_operations)
{
	EXPECT_FLOAT_EQ(12.3 * 23.47, (vect1<hours>(12.3) * vect1<km_per_hour>(23.47)).value());
	EXPECT_FLOAT_EQ(12.3 * 23.47, (vect1<km_per_hour>(12.3) * vect1<hours>(23.47)).value());
	EXPECT_FLOAT_EQ(12.3 / 23.47, (vect1<km>(12.3) / vect1<hours>(23.47)).value());
	EXPECT_FLOAT_EQ(12.3 / 23.47, (vect1<km>(12.3) / vect1<km_per_hour>(23.47)).value());
	EXPECT_FLOAT_EQ(12.3 * 23.47, (vect1<inches>(12.3) * vect1<inches>(23.47)).value());
	EXPECT_FLOAT_EQ(12.3 / 23.47, (vect1<square_inches>(12.3) / vect1<inches>(23.47)).value());
	EXPECT_FLOAT_EQ(sqrt(23.47), sqrt(vect1<square_inches>(23.47)).value());

	auto a1 = vect1<hours>(12.3) * vect2<km_per_hour>(23.47, 34.51);
	EXPECT_FLOAT_EQ(12.3 * 23.47, a1.x().value());
	EXPECT_FLOAT_EQ(12.3 * 34.51, a1.y().value());

	auto a2 = vect2<km_per_hour>(23.47, 34.51) * vect1<hours>(12.3);
	EXPECT_FLOAT_EQ(23.47 * 12.3, a2.x().value());
	EXPECT_FLOAT_EQ(34.51 * 12.3, a2.y().value());

	auto a3 = vect2<km>(23.47, 34.51) / vect1<hours>(12.3);
	EXPECT_FLOAT_EQ(23.47 / 12.3, a3.x().value());
	EXPECT_FLOAT_EQ(34.51 / 12.3, a3.y().value());

	auto a4 = vect1<hours>(12.3) * vect3<km_per_hour>(23.47, 34.51, 45.19);
	EXPECT_FLOAT_EQ(12.3 * 23.47, a4.x().value());
	EXPECT_FLOAT_EQ(12.3 * 34.51, a4.y().value());
	EXPECT_FLOAT_EQ(12.3 * 45.19, a4.z().value());

	auto a5 = vect3<km_per_hour>(23.47, 34.51, 45.19) * vect1<hours>(12.3);
	EXPECT_FLOAT_EQ(23.47 * 12.3, a5.x().value());
	EXPECT_FLOAT_EQ(34.51 * 12.3, a5.y().value());
	EXPECT_FLOAT_EQ(45.19 * 12.3, a5.z().value());

	auto a6 = vect3<km>(23.47, 34.51, 45.19) / vect1<hours>(12.3);
	EXPECT_FLOAT_EQ(23.47 / 12.3, a6.x().value());
	EXPECT_FLOAT_EQ(34.51 / 12.3, a6.y().value());
	EXPECT_FLOAT_EQ(45.19 / 12.3, a6.z().value());

	EXPECT_FLOAT_EQ(12.3 * 34.5 + 23.4 * 45.6,
		(vect2<newtons>(12.3, 23.4) * vect2<metres>(34.5, 45.6)).value());
	EXPECT_FLOAT_EQ(12.3 * 34.5 + 23.4 * 45.6,
		(vect2<metres>(12.3, 23.4) * vect2<newtons>(34.5, 45.6)).value());

	EXPECT_FLOAT_EQ(12.3 * 45.6 - 23.4 * 34.5,
		cross_product(vect2<metres_per_second>(12.3, 23.4), vect2<tesla>(34.5, 45.6)).value());
	EXPECT_FLOAT_EQ(12.3 * 45.6 - 23.4 * 34.5,
		cross_product(vect2<tesla>(12.3, 23.4), vect2<metres_per_second>(34.5, 45.6)).value());
	
	EXPECT_FLOAT_EQ(12.3 * 45.6 + 23.4 * 56.7 + 34.5 * 67.8,
		(vect3<newtons>(12.3, 23.4, 34.5) * vect3<metres>(45.6, 56.7, 67.8)).value());
	EXPECT_FLOAT_EQ(12.3 * 45.6 + 23.4 * 56.7 + 34.5 * 67.8,
		(vect3<metres>(12.3, 23.4, 34.5) * vect3<newtons>(45.6, 56.7, 67.8)).value());
		
	auto a7 = cross_product(
		vect3<metres_per_second>(12.3, 23.4, 34.5),
		vect3<tesla>(45.6, 56.7, 67.8));
	EXPECT_FLOAT_EQ(23.4 * 67.8 - 34.5 * 56.7, a7.x().value());
	EXPECT_FLOAT_EQ(34.5 * 45.6 - 12.3 * 67.8, a7.y().value());
	EXPECT_FLOAT_EQ(12.3 * 56.7 - 23.4 * 45.6, a7.z().value());
	
	auto a8 = cross_product(
		vect3<tesla>(12.3, 23.4, 34.5),
		vect3<metres_per_second>(45.6, 56.7, 67.8));
	EXPECT_FLOAT_EQ(23.4 * 67.8 - 34.5 * 56.7, a8.x().value());
	EXPECT_FLOAT_EQ(34.5 * 45.6 - 12.3 * 67.8, a8.y().value());
	EXPECT_FLOAT_EQ(12.3 * 56.7 - 23.4 * 45.6, a8.z().value());

	EXPECT_FLOAT_EQ(12.3 * 34.5 + 23.4 * 45.6,
		(vect2<units>(12.3, 23.4) * vect2<units>(34.5, 45.6)).value());
	
	EXPECT_FLOAT_EQ(12.3 * 45.6 - 23.4 * 34.5,
		(cross_product(vect2<units>(12.3, 23.4), vect2<units>(34.5, 45.6))).value());
	
	EXPECT_FLOAT_EQ(12.3 * 45.6 + 23.4 * 56.7 + 34.5 * 67.8,
		(vect3<units>(12.3, 23.4, 34.5) * vect3<units>(45.6, 56.7, 67.8)).value());

	auto a9 = cross_product(
		vect3<units>(12.3, 23.4, 34.5),
		vect3<units>(45.6, 56.7, 67.8));
	EXPECT_FLOAT_EQ(23.4 * 67.8 - 34.5 * 56.7, a9.x().value());
	EXPECT_FLOAT_EQ(34.5 * 45.6 - 12.3 * 67.8, a9.y().value());
	EXPECT_FLOAT_EQ(12.3 * 56.7 - 23.4 * 45.6, a9.z().value());
}

/*
operazioni da testare:
	trigonometriche
	azimuth
*/

/*
//// STATIC-UNIT 1-DIMENSION MEASURES

//EXPECT_EQ(m5.value(), m4.value());

//// STATIC-UNIT 2-DIMENSIONS MEASURES
//// STATIC-UNIT 3-DIMENSIONS MEASURES
//// STATIC-UNIT AZIMUTHS

*/

int main(int argc, char** argv) 
{
	::testing::InitGoogleTest(&argc, argv);	
    auto result = RUN_ALL_TESTS();
	return result;
}
