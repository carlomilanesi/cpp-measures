// measures.hpp
// Released under Mozilla Public Licence 2.0
// by Carlo Milanesi (carlo.milanesi@tiscali.it)
#ifndef MEASURES_HPP
#define MEASURES_HPP
#include <cassert>
#include <cmath>
#include <vector>
//#include <type_traits>
#include <unordered_map>

///////////////////////// STATIC ASSERTS FOR MAGNITUDE /////////////////////////
template <typename T>
inline void assert_same_type(T, T) { }
#define ASSERT_HAVE_SAME_MAGNITUDE(U1,U2)\
	assert_same_type(typename U1::magnitude(0), typename U2::magnitude(0));
#define ASSERT_IS_ANGLE(U)\
	assert_same_type(typename U::magnitude(0), Angle(0));
#define ASSERT_IS_NUMERIC(Num) \
	static_assert(std::is_arithmetic<Num>::value, "The '" #Num "' type must be arithmetic");\
	static_assert(sizeof(Num) >= sizeof(int), "The '" #Num "' type must be not shorter than an int");

///////////////////////// UNIT FEATURES /////////////////////////

namespace measures
{
	template <typename Num>
	Num sqr(Num x)
	{
		ASSERT_IS_NUMERIC(Num)\
		return x * x;
	}
	
	struct UnitFeatures
	{
		explicit UnitFeatures(const char* symbol, double ratio,
			double offset, double turn_fraction = 0):
			symbol(symbol), ratio(ratio), offset(offset), turn_fraction(turn_fraction) { }
		const char* symbol;
		double ratio;
		double offset;
		double turn_fraction;
	};
	
	static std::vector<UnitFeatures> unit_features;
}


///////////////////////// MAGNITUDE AND UNIT DEFINITIONS /////////////////////////

// Every unit class is never instanced.
// It is used only as a template parameter
// or as a collection of static member functions.
#define DEFINE_UNIT(UnitClass,MagnitudeClass,Symbol,Ratio,Offset)\
    namespace measures\
    {\
		static unsigned int const UnitClass##id_ = []() {\
			unit_features.push_back(UnitFeatures(Symbol, Ratio, Offset)); \
			return unsigned int(unit_features.size() - 1);\
		}();\
		\
		class UnitClass\
		{\
		public:\
			typedef MagnitudeClass magnitude;\
			static MagnitudeClass const id() { return MagnitudeClass(UnitClass##id_); }\
			static char const * symbol() { return Symbol; }\
			static double ratio() { return Ratio; }\
			static double offset() { return Offset; }\
		};\
	}

#define DEFINE_ANGLE_UNIT(UnitClass,Symbol,TurnFraction,Offset)\
    namespace measures\
    {\
		static unsigned int const UnitClass##id_ = []() {\
			unit_features.push_back(UnitFeatures(Symbol, 2 * pi / (TurnFraction), Offset, (TurnFraction))); \
			return unsigned int(unit_features.size() - 1);\
		}();\
		\
		class UnitClass\
		{\
		public:\
			typedef Angle magnitude;\
			static Angle const id() { return Angle(UnitClass##id_); }\
			static char const * symbol() { return Symbol; }\
			static double ratio() { return 2 * pi / (TurnFraction); }\
			static double offset() { return Offset; }\
			static double turn_fraction() { return TurnFraction; }\
		};\
	}

// Every magnitude class is used as a template parameter
// or it may be instanced to represent a dynamic unit.
#define DEFINE_MAGNITUDE(MagnitudeClass,MainUnitClass,MainUnitSymbol)\
	namespace measures\
	{\
		class MainUnitClass;\
		class MagnitudeClass\
		{\
		public:\
			typedef MainUnitClass base_unit;\
			explicit MagnitudeClass(unsigned int unit_id): unit_id_(unit_id) { }\
			operator unsigned int() const { return unit_id_; }\
			const char* symbol() const { return unit_features[unit_id_].symbol; }\
			double ratio() const { return unit_features[unit_id_].ratio; }\
			double offset() const { return unit_features[unit_id_].offset; }\
		private:\
			unsigned int unit_id_;\
		};\
	}\
	DEFINE_UNIT(MainUnitClass, MagnitudeClass, MainUnitSymbol, 1, 0)


///////////////////////// PREDEFINED MAGNITUDES AND UNITS /////////////////////////

// The "Angle" magnitude and its "radians" and "turns" units are required.
//DEFINE_MAGNITUDE(Angle, radians, " rad")
namespace measures
{
	static const double pi = atan(1) * 4;
	class radians;
	class Angle
	{
	public:
		typedef radians base_unit;
		explicit Angle(int unit_id): unit_id_(unit_id) { }
		operator int() const { return unit_id_; }
		const char* symbol() const { return unit_features[unit_id_].symbol; }
		double ratio() const { return unit_features[unit_id_].ratio; }
		double offset() const { return unit_features[unit_id_].offset; }
		double turn_fraction() const { return unit_features[unit_id_].turn_fraction; }
	private:
		int unit_id_;
	};
}
DEFINE_ANGLE_UNIT(radians, " rad", 2 * pi, 0)


///////////////////////// DERIVED MAGNITUDES AND UNITS /////////////////////////

/*
The following different-unit operations are supported:
Scalar * Scalar -> Scalar // simple product
Scalar / Scalar -> Scalar // simple division
sqroot(Scalar) -> Scalar // simple square root
Scalar * Vector -> Vector // vector external product
Vector * Scalar -> Vector // vector external product, commutated
Vector / Scalar -> Vector // vector external product by the reciprocal
Vector * Vector -> Scalar // vector dot product
cross_product(Vector, Vector) -> Vector // vector cross product
*/

// U1 (Scalar) * U2 (Scalar) == U3 (Scalar)
// with U1 != U2
#define DEFINE_DERIVED_UNIT_SCALAR_SCALAR(U1,U2,U3)\
    namespace measures\
    {\
		static bool init_derived_##U1##_x_##U2##_is_##U3 = []() {\
			DerivationRules1.emplace(\
				std::make_pair(U2::id(), U3::id()), U1::id());\
			DerivationRules2.emplace(\
				std::make_pair(U1::id(), U3::id()), U2::id());\
			DerivationRules3.emplace(\
				std::make_pair(U1::id(), U2::id()), U3::id());\
			return true;\
		}();\
	}\
	DEFINE_DERIVED_SIMPLE_UNIT_SCALAR_SCALAR(U1,U2,U3)

// U1 (Scalar) * U1 (Scalar) == U2 (Scalar)
#define DEFINE_DERIVED_UNIT_SQUARED_SCALAR(U1,U2)\
    namespace measures\
    {\
		static bool init_derived_##U1##_x_##U1##_is_##U2 = []() {\
			DerivationRules1.emplace(\
				std::make_pair(U1::id(), U2::id()), U1::id());\
			DerivationRules2.emplace(\
				std::make_pair(U1::id(), U2::id()), U1::id());\
			DerivationRules3.emplace(\
				std::make_pair(U1::id(), U1::id()), U2::id());\
			DerivationRulesSquared.emplace(\
				U2::id(), U1::id());\
			return true;\
		}();\
	}\
	DEFINE_DERIVED_SIMPLE_UNIT_SQUARED_SCALAR(U1,U2)
	
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_UNIT_SCALAR_VECTOR(U1,U2,U3)\
    DEFINE_DERIVED_UNIT_SCALAR_SCALAR(U1,U2,U3)\
	DEFINE_DERIVED_OTHER_UNIT_SCALAR_VECTOR(U1,U2,U3)
	
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_UNIT_VECTOR_VECTOR(U1,U2,U3)\
    DEFINE_DERIVED_UNIT_SCALAR_SCALAR(U1,U2,U3)\
	DEFINE_DERIVED_OTHER_UNIT_VECTOR_VECTOR(U1,U2,U3)

// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U2 (Vector)
#define DEFINE_DERIVED_UNIT_SQUARED_VECTOR(U1,U2)\
    DEFINE_DERIVED_UNIT_SQUARED_SCALAR(U1,U2)\
	DEFINE_DERIVED_OTHER_UNIT_SQUARED_VECTOR(U1,U2)

// Functions to use unordered_map with a pair as key.
namespace std
{
	template <class T>
	inline void hash_combine(std::size_t & seed, const T & v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	template<typename S, typename T>
	struct hash<pair<S,T>>
	{
		inline size_t operator()(const pair<S,T> & v) const
		{
			size_t seed = 0;
			std::hash_combine(seed, v.first);
			std::hash_combine(seed, v.second);
			return seed;
		}
	};
}

namespace measures
{
	static std::unordered_map<std::pair<int,int>,int> DerivationRules1;
	static std::unordered_map<std::pair<int,int>,int> DerivationRules2;
	static std::unordered_map<std::pair<int,int>,int> DerivationRules3;
	static std::unordered_map<int,int> DerivationRulesSquared;
}

// M1 (Scalar) * M2 (Scalar) == M3 (Scalar)
// with M1 != M2
#define DEFINE_DERIVED_MAGNITUDE_SCALAR_SCALAR(M1,M2,M3) \
	DEFINE_DERIVED_SIMPLE_MAGNITUDE_SCALAR_SCALAR(M1,M2,M3)\
	DEFINE_DERIVED_SIMPLE_UNIT_SCALAR_SCALAR(M1::base_unit, M2::base_unit, M3::base_unit)

// M1 (Scalar) * M1 (Scalar) == M2 (Scalar)
#define DEFINE_DERIVED_MAGNITUDE_SQUARED_SCALAR(M1,M2)\
	DEFINE_DERIVED_SIMPLE_MAGNITUDE_SQUARED_SCALAR(M1,M2)\
	DEFINE_DERIVED_SIMPLE_UNIT_SQUARED_SCALAR(M1::base_unit, M2::base_unit)

// M1 (Scalar) * M2 (Vector) == M3 (Vector)
// with M1 != M2
#define DEFINE_DERIVED_MAGNITUDE_SCALAR_VECTOR(M1,M2,M3)\
	DEFINE_DERIVED_SIMPLE_MAGNITUDE_SCALAR_SCALAR(M1,M2,M3)\
    namespace measures\
    {\
		/* dyn_vect1 * dyn_vect2 -> dyn_vect2 */\
        template <typename Num1, typename Num2>\
        dyn_vect2<M3,decltype(Num1()*Num2())> operator *(\
            dyn_vect1<M1,Num1> m1,\
            dyn_vect2<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect2<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m1.value() * m2.x().value(),\
					m1.value() * m2.y().value());\
			}\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect2<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
                val1 * m2_base.x().value(),\
                val1 * m2_base.y().value());\
        }\
        \
		/* dyn_vect2 * dyn_vect1 -> dyn_vect2 */\
        template <typename Num1, typename Num2>\
        dyn_vect2<M3,decltype(Num1()*Num2())> operator *(\
            dyn_vect2<M2,Num1> m2,\
            dyn_vect1<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect2<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m2.x().value() * m1.value(),\
					m2.y().value() * m1.value());\
			}\
			auto m2_base = convert(M2::base_unit::id(), m2);\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
            return dyn_vect2<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
				m2_base.x().value() * val1,\
				m2_base.y().value() * val1);\
        }\
        \
		/* dyn_vect2 / dyn_vect1 -> dyn_vect2 */\
        template <typename Num1, typename Num2>\
        dyn_vect2<M2,decltype(Num1()/Num2())> operator /(\
            dyn_vect2<M3,Num1> m3,\
            dyn_vect1<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules2.find(std::make_pair(m1.unit(), m3.unit()));\
			if (result != DerivationRules2.end()) {\
				return dyn_vect2<M2,decltype(Num1()/Num2())>(\
					M2(result->second),\
					m3.x().value() / m1.value(),\
					m3.y().value() / m1.value());\
			}\
			auto m3_base = convert(M3::base_unit::id(), m3);\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
            return dyn_vect2<M2,decltype(Num1()/Num2())>(\
				M2::base_unit::id(),\
				m3_base.x().value() / val1,\
				m3_base.y().value() / val1);\
			}\
        \
		/* dyn_vect1 * dyn_vect3 -> dyn_vect3 */\
        template <typename Num1, typename Num2>\
        dyn_vect3<M3,decltype(Num1()*Num2())> operator *(\
            dyn_vect1<M1,Num1> m1,\
            dyn_vect3<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect3<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m1.value() * m2.x().value(),\
					m1.value() * m2.y().value(),\
					m1.value() * m2.z().value());\
			}\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect3<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
                val1 * m2_base.x().value(),\
                val1 * m2_base.y().value(),\
                val1 * m2_base.z().value());\
        }\
        \
		/* dyn_vect3 * dyn_vect1 -> dyn_vect3 */\
        template <typename Num1, typename Num2>\
        dyn_vect3<M3,decltype(Num1()*Num2())> operator *(\
            dyn_vect3<M2,Num1> m2,\
            dyn_vect1<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect3<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m2.x().value() * m1.value(),\
					m2.y().value() * m1.value(),\
					m2.z().value() * m1.value());\
			}\
			auto m2_base = convert(M2::base_unit::id(), m2);\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
            return dyn_vect3<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
				m2_base.x().value() * val1,\
				m2_base.y().value() * val1,\
				m2_base.z().value() * val1);\
        }\
        \
		/* dyn_vect3 * dyn_vect1 -> dyn_vect3 */\
        template <typename Num1, typename Num2>\
        dyn_vect3<M2,decltype(Num1()/Num2())> operator /(\
            dyn_vect3<M3,Num1> m3,\
            dyn_vect1<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules2.find(std::make_pair(m1.unit(), m3.unit()));\
			if (result != DerivationRules2.end()) {\
				return dyn_vect3<M2,decltype(Num1()/Num2())>(\
					M2(result->second),\
					m3.x().value() / m1.value(),\
					m3.y().value() / m1.value(),\
					m3.z().value() / m1.value());\
			}\
			auto m3_base = convert(M3::base_unit::id(), m3);\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
            return dyn_vect3<M2,decltype(Num1()/Num2())>(\
				M2::base_unit::id(),\
				m3_base.x().value() / val1,\
				m3_base.y().value() / val1,\
				m3_base.z().value() / val1);\
		}\
    }\
	DEFINE_DERIVED_SIMPLE_UNIT_SCALAR_VECTOR(M1::base_unit, M2::base_unit, M3::base_unit)

// M1 (Vector) * M2 (Vector) == M3 (Scalar)
// M1 (Vector) X M2 (Vector) == M3 (Vector)
// with M1 != M2
#define DEFINE_DERIVED_MAGNITUDE_VECTOR_VECTOR(M1,M2,M3) \
    DEFINE_DERIVED_SIMPLE_MAGNITUDE_SCALAR_SCALAR(M1,M2,M3) \
    namespace measures\
    {\
		/* dyn_vect2 * dyn_vect2 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M3,decltype(Num1()*Num2())> operator *(\
            dyn_vect2<M1,Num1> m1,\
            dyn_vect2<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m1.x().value() * m2.x().value() +\
					m1.y().value() * m2.y().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect1<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
				m1_base.x().value() * m2_base.x().value() +\
				m1_base.y().value() * m2_base.y().value());\
		}\
        \
		/* dyn_vect2 * dyn_vect2 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M3,decltype(Num2()*Num1())> operator *(\
            dyn_vect2<M2,Num1> m2,\
            dyn_vect2<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num2()*Num1())>(\
					M3(result->second),\
					m2.x().value() * m1.x().value() +\
					m2.y().value() * m1.y().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect1<M3,decltype(Num2()*Num1())>(\
				M3::base_unit::id(),\
				m2_base.x().value() * m1_base.x().value() +\
				m2_base.y().value() * m1_base.y().value());\
		}\
        \
		/* cross_product(dyn_vect2, dyn_vect2) -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M3,decltype(Num1()*Num2())> cross_product(\
            dyn_vect2<M1,Num1> m1,\
            dyn_vect2<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m1.x().value() * m2.y().value() -\
					m1.y().value() * m2.x().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect1<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
				m1_base.x().value() * m2_base.y().value() -\
				m1_base.y().value() * m2_base.x().value());\
		}\
        \
		/* cross_product(dyn_vect2, dyn_vect2) -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M3,decltype(Num2()*Num1())> cross_product(\
            dyn_vect2<M2,Num1> m2,\
            dyn_vect2<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num2()*Num1())>(\
					M3(result->second),\
					m2.x().value() * m1.y().value() -\
					m2.y().value() * m1.x().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect1<M3,decltype(Num2()*Num1())>(\
				M3::base_unit::id(),\
				m2_base.x().value() * m1_base.y().value() -\
				m2_base.y().value() * m1_base.x().value());\
		}\
		\
		/* dyn_vect3 * dyn_vect3 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M3,decltype(Num1()*Num2())> operator *(\
            dyn_vect3<M1,Num1> m1,\
            dyn_vect3<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m1.x().value() * m2.x().value() +\
					m1.y().value() * m2.y().value() +\
					m1.z().value() * m2.z().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect1<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
				m1_base.x().value() * m2_base.x().value() +\
				m1_base.y().value() * m2_base.y().value() +\
				m1_base.z().value() * m2_base.z().value());\
		}\
        \
		/* dyn_vect3 * dyn_vect3 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M3,decltype(Num2()*Num1())> operator *(\
            dyn_vect3<M2,Num1> m2,\
            dyn_vect3<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num2()*Num1())>(\
					M3(result->second),\
					m2.x().value() * m1.x().value() +\
					m2.y().value() * m1.y().value() +\
					m2.z().value() * m1.z().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect1<M3,decltype(Num2()*Num1())>(\
				M3::base_unit::id(),\
				m2_base.x().value() * m1_base.x().value() +\
				m2_base.y().value() * m1_base.y().value() +\
				m2_base.z().value() * m1_base.z().value());\
		}\
		\
		/* cross_product(dyn_vect3, dyn_vect3) -> dyn_vect3 */\
        template <typename Num1, typename Num2>\
        dyn_vect3<M3,decltype(Num1()*Num2())> cross_product(\
            dyn_vect3<M1,Num1> m1,\
            dyn_vect3<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect3<M3,decltype(Num1()*Num2())>(\
					M3(result->second),\
					m1.y().value() * m2.z().value() -\
					m1.z().value() * m2.y().value(),\
					m1.z().value() * m2.x().value() -\
					m1.x().value() * m2.z().value(),\
					m1.x().value() * m2.y().value() -\
					m1.y().value() * m2.x().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect3<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(),\
				m1_base.y().value() * m2_base.z().value() -\
				m1_base.z().value() * m2_base.y().value(),\
				m1_base.z().value() * m2_base.x().value() -\
				m1_base.x().value() * m2_base.z().value(),\
				m1_base.x().value() * m2_base.y().value() -\
				m1_base.y().value() * m2_base.x().value());\
		}\
        \
		/* cross_product(dyn_vect3, dyn_vect3) -> dyn_vect3 */\
        template <typename Num1, typename Num2>\
        dyn_vect3<M3,decltype(Num2()*Num1())> cross_product(\
            dyn_vect3<M2,Num1> m2,\
            dyn_vect3<M1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect3<M3,decltype(Num2()*Num1())>(\
					M3(result->second),\
					m2.y().value() * m1.z().value() -\
					m2.z().value() * m1.y().value(),\
					m2.z().value() * m1.x().value() -\
					m2.x().value() * m1.z().value(),\
					m2.x().value() * m1.y().value() -\
					m2.y().value() * m1.x().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M2::base_unit::id(), m2);\
            return dyn_vect3<M3,decltype(Num2()*Num1())>(\
				M3::base_unit::id(),\
				m2_base.y().value() * m1_base.z().value() -\
				m2_base.z().value() * m1_base.y().value(),\
				m2_base.z().value() * m1_base.x().value() -\
				m2_base.x().value() * m1_base.z().value(),\
				m2_base.x().value() * m1_base.y().value() -\
				m2_base.y().value() * m1_base.x().value());\
		}\
    }\
	DEFINE_DERIVED_SIMPLE_UNIT_VECTOR_VECTOR(M1::base_unit, M2::base_unit, M3::base_unit)

// M1 (Vector) * M1 (Vector) == M2 (Scalar)
// M1 (Vector) X M1 (Vector) == M2 (Vector)
#define DEFINE_DERIVED_MAGNITUDE_SQUARED_VECTOR(M1,M2) \
    DEFINE_DERIVED_SIMPLE_MAGNITUDE_SQUARED_SCALAR(M1,M2) \
    namespace measures\
    {\
		/* dyn_vect2 * dyn_vect2 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M2,decltype(Num1()*Num2())> operator *(\
            dyn_vect2<M1,Num1> m1,\
            dyn_vect2<M1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M2,decltype(Num1()*Num2())>(\
					M2(result->second),\
					m1.x().value() * m2.x().value() +\
					m1.y().value() * m2.y().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M1::base_unit::id(), m2);\
            return dyn_vect1<M2,decltype(Num1()*Num2())>(\
				M2::base_unit::id(),\
				m1_base.x().value() * m2_base.x().value() +\
				m1_base.y().value() * m2_base.y().value());\
		}\
        \
		/* cross_product(dyn_vect2, dyn_vect2) -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M2,decltype(Num1()*Num2())> cross_product(\
            dyn_vect2<M1,Num1> m1,\
            dyn_vect2<M1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M2,decltype(Num1()*Num2())>(\
					M2(result->second),\
					m1.x().value() * m2.y().value() -\
					m1.y().value() * m2.x().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M1::base_unit::id(), m2);\
            return dyn_vect1<M2,decltype(Num1()*Num2())>(\
				M2::base_unit::id(),\
				m1_base.x().value() * m2_base.y().value() -\
				m1_base.y().value() * m2_base.x().value());\
		}\
        \
		/* dyn_vect3 * dyn_vect3 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M2,decltype(Num1()*Num2())> operator *(\
            dyn_vect3<M1,Num1> m1,\
            dyn_vect3<M1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M2,decltype(Num1()*Num2())>(\
					M2(result->second),\
					m1.x().value() * m2.x().value() +\
					m1.y().value() * m2.y().value() +\
					m1.z().value() * m2.z().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M1::base_unit::id(), m2);\
            return dyn_vect1<M2,decltype(Num1()*Num2())>(\
				M2::base_unit::id(),\
				m1_base.x().value() * m2_base.x().value() +\
				m1_base.y().value() * m2_base.y().value() +\
				m1_base.z().value() * m2_base.z().value());\
		}\
        \
		/* cross_product(dyn_vect3, dyn_vect3) -> dyn_vect3 */\
        template <typename Num1, typename Num2>\
        dyn_vect3<M2,decltype(Num1()*Num2())> cross_product(\
            dyn_vect3<M1,Num1> m1,\
            dyn_vect3<M1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect3<M2,decltype(Num1()*Num2())>(\
					M2(result->second),\
					m1.y().value() * m2.z().value() -\
					m1.z().value() * m2.y().value(),\
					m1.z().value() * m2.x().value() -\
					m1.x().value() * m2.z().value(),\
					m1.x().value() * m2.y().value() -\
					m1.y().value() * m2.x().value());\
			}\
			auto m1_base = convert(M1::base_unit::id(), m1);\
			auto m2_base = convert(M1::base_unit::id(), m2);\
            return dyn_vect3<M2,decltype(Num1()*Num2())>(\
				M2::base_unit::id(),\
				m1_base.y().value() * m2_base.z().value() -\
				m1_base.z().value() * m2_base.y().value(),\
				m1_base.z().value() * m2_base.x().value() -\
				m1_base.x().value() * m2_base.z().value(),\
				m1_base.x().value() * m2_base.y().value() -\
				m1_base.y().value() * m2_base.x().value());\
		}\
    }\
	DEFINE_DERIVED_SIMPLE_UNIT_SQUARED_VECTOR(M1::base_unit, M2::base_unit)

// Private
// U1 (Scalar) * U2 (Scalar) == U3 (Scalar)
// with U1 != U2
#define DEFINE_DERIVED_SIMPLE_UNIT_SCALAR_SCALAR(U1,U2,U3)\
    namespace measures\
    {\
		/* vect1 * vect1 -> vect1 */\
		template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect1<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num1()*Num2())>(m1.value() * m2.value());\
        }\
        \
		/* vect1 * vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U2,Num1> m2, vect1<U1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num1()*Num2())>(m2.value() * m1.value());\
        }\
        \
		/* vect1 / vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()/Num2())> operator /(\
            vect1<U3,Num1> m3, vect1<U1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U2,decltype(Num1()/Num2())>(m3.value() / m1.value());\
        }\
        \
		/* vect1 / vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U1,decltype(Num1()/Num2())> operator /(\
            vect1<U3,Num1> m3, vect1<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U1,decltype(Num1()/Num2())>(m3.value() / m2.value());\
        }\
    }

// Private
// U1 (Scalar) * U1 (Scalar) == U2 (Scalar)
//
#define DEFINE_DERIVED_SIMPLE_UNIT_SQUARED_SCALAR(U1,U2)\
    namespace measures\
    {\
		/* vect1 * vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect1<U1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U2,decltype(Num1()*Num2())>(\
				m1.value() * m2.value());\
        }\
        \
		/* vect1 / vect1 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U1,decltype(Num1()/Num2())> operator /(\
            vect1<U2,Num1> m2, vect1<U1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U1,decltype(Num1()/Num2())>(\
				m2.value() / m1.value());\
        }\
        \
		/* sqroot(vect1) -> vect1 */\
        template <typename Num>\
        vect1<U1,Num> sqroot(vect1<U2,Num> m2)\
        {\
			ASSERT_IS_NUMERIC(Num)\
            return vect1<U1,Num>(\
				static_cast<Num>(sqrt(m2.value())));\
        }\
    }

// Private
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_OTHER_UNIT_SCALAR_VECTOR(U1,U2,U3)\
    namespace measures\
    {\
		/* vect1 * vect2 -> vect2 */\
        template <typename Num1, typename Num2>\
        vect2<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect2<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect2<U3,decltype(Num1()*Num2())>(\
                m1.value() * m2.x().value(),\
                m1.value() * m2.y().value());\
        }\
        \
		/* vect2 * vect1 -> vect2 */\
        template <typename Num2, typename Num1>\
        vect2<U3,decltype(Num2()*Num1())> operator *(\
            vect2<U2,Num2> m2, vect1<U1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect2<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.value(),\
                m2.y().value() * m1.value());\
        }\
        \
		/* vect2 / vect1 -> vect2 */\
        template <typename Num3, typename Num1>\
        vect2<U2,decltype(Num3()/Num1())> operator /(\
            vect2<U3,Num3> m3, vect1<U1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num3)\
            return vect2<U2,decltype(Num3()/Num1())>(\
                m3.x().value() / m1.value(),\
                m3.y().value() / m1.value());\
        }\
        \
		/* vect1 * vect3 -> vect3 */\
        template <typename Num1, typename Num2>\
        vect3<U3,decltype(Num1()*Num2())> operator *(\
            vect1<U1,Num1> m1, vect3<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect3<U3,decltype(Num1()*Num2())>(\
                m1.value() * m2.x().value(),\
                m1.value() * m2.y().value(),\
                m1.value() * m2.z().value());\
        }\
        \
		/* vect3 * vect1 -> vect3 */\
        template <typename Num2, typename Num1>\
        vect3<U3,decltype(Num2()*Num1())> operator *(\
            vect3<U2,Num2> m2, vect1<U1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect3<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.value(),\
                m2.y().value() * m1.value(),\
                m2.z().value() * m1.value());\
        }\
        \
		/* vect3 / vect1 -> vect3 */\
        template <typename Num3, typename Num1>\
        vect3<U2,decltype(Num3()*Num1())> operator /(\
            vect3<U3,Num3> m3, vect1<U1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num3)\
            return vect3<U2,decltype(Num3()*Num1())>(\
                m3.x().value() / m1.value(),\
                m3.y().value() / m1.value(),\
                m3.z().value() / m1.value());\
        }\
    }

// Private
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_OTHER_UNIT_VECTOR_VECTOR(U1,U2,U3)\
    namespace measures\
    {\
		/* vect2 * vect2 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect2<U1,Num1> m1, vect2<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() +\
                m1.y().value() * m2.y().value());\
        }\
        \
		/* vect2 * vect2 -> vect1 */\
        template <typename Num2, typename Num1>\
        vect1<U3,decltype(Num2()*Num1())> operator *(\
            vect2<U2,Num1> m2, vect2<U1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.x().value() +\
                m2.y().value() * m1.y().value());\
        }\
        \
		/* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> cross_product(\
            vect2<U1,Num1> m1, vect2<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
        \
		/* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num2, typename Num1>\
        vect1<U3,decltype(Num2()*Num1())> cross_product(\
            vect2<U2,Num1> m2, vect2<U1,Num2> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.y().value() -\
                m2.y().value() * m1.x().value());\
        }\
		\
		/* vect3 * vect3 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U3,decltype(Num1()*Num2())> operator *(\
            vect3<U1,Num1> m1, vect3<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() +\
                m1.y().value() * m2.y().value() +\
                m1.z().value() * m2.z().value());\
        }\
        \
		/* vect3 * vect3 -> vect1 */\
        template <typename Num2, typename Num1>\
        vect1<U3,decltype(Num2()*Num1())> operator *(\
            vect3<U2,Num2> m2, vect3<U1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U3,decltype(Num2()*Num1())>(\
                m2.x().value() * m1.x().value() +\
                m2.y().value() * m1.y().value() +\
                m2.z().value() * m1.z().value());\
        }\
		\
		/* cross_product(vect3, vect3) -> vect3 */\
        template <typename Num1, typename Num2>\
        vect3<U3,decltype(Num1()*Num2())> cross_product(\
            vect3<U1,Num1> m1, vect3<U2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect3<U3,decltype(Num1()*Num2())>(\
                m1.y().value() * m2.z().value() -\
                m1.z().value() * m2.y().value(),\
                m1.z().value() * m2.x().value() -\
                m1.x().value() * m2.z().value(),\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
        \
		/* cross_product(vect3, vect3) -> vect3 */\
        template <typename Num2, typename Num1>\
        vect3<U3,decltype(Num2()*Num1())> cross_product(\
            vect3<U2,Num2> m2, vect3<U1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect3<U3,decltype(Num2()*Num1())>(\
                m2.y().value() * m1.z().value() -\
                m2.z().value() * m1.y().value(),\
                m2.z().value() * m1.x().value() -\
                m2.x().value() * m1.z().value(),\
                m2.x().value() * m1.y().value() -\
                m2.y().value() * m1.x().value());\
        }\
	}

// Private
// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U2 (Vector)
#define DEFINE_DERIVED_OTHER_UNIT_SQUARED_VECTOR(U1,U2)\
	namespace measures\
    {\
		/* vect2 * vect2 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> operator *(\
            vect2<U1,Num1> m1, vect2<U1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U2,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() + \
                m1.y().value() * m2.y().value());\
        }\
		\
		/* cross_product(vect2, vect2) -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> cross_product(\
            vect2<U1,Num1> m1, vect2<U1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U2,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
        \
		/* vect3 * vect3 -> vect1 */\
        template <typename Num1, typename Num2>\
        vect1<U2,decltype(Num1()*Num2())> operator *(\
            vect3<U1,Num1> m1, vect3<U1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect1<U2,decltype(Num1()*Num2())>(\
                m1.x().value() * m2.x().value() + \
                m1.y().value() * m2.y().value() + \
                m1.z().value() * m2.z().value());\
        }\
        \
		/* cross_product(vect3, vect3) -> vect3 */\
        template <typename Num1, typename Num2>\
        vect3<U2,decltype(Num1()*Num2())> cross_product(\
            vect3<U1,Num1> m1, vect3<U1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
            return vect3<U2,decltype(Num1()*Num2())>(\
                m1.y().value() * m2.z().value() -\
                m1.z().value() * m2.y().value(),\
                m1.z().value() * m2.x().value() -\
                m1.x().value() * m2.z().value(),\
                m1.x().value() * m2.y().value() -\
                m1.y().value() * m2.x().value());\
        }\
    }

// Private
// U1 (Scalar) * U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_SIMPLE_UNIT_SCALAR_VECTOR(U1,U2,U3)\
	DEFINE_DERIVED_SIMPLE_UNIT_SCALAR_SCALAR(U1,U2,U3)\
	DEFINE_DERIVED_OTHER_UNIT_SCALAR_VECTOR(U1,U2,U3)

// Private
// U1 (Vector) * U2 (Vector) == U3 (Scalar)
// U1 (Vector) X U2 (Vector) == U3 (Vector)
// with U1 != U2
#define DEFINE_DERIVED_SIMPLE_UNIT_VECTOR_VECTOR(U1,U2,U3)\
    DEFINE_DERIVED_SIMPLE_UNIT_SCALAR_SCALAR(U1,U2,U3)\
	DEFINE_DERIVED_OTHER_UNIT_VECTOR_VECTOR(U1,U2,U3)

// Private
// U1 (Vector) * U1 (Vector) == U2 (Scalar)
// U1 (Vector) X U1 (Vector) == U2 (Vector)
#define DEFINE_DERIVED_SIMPLE_UNIT_SQUARED_VECTOR(U1,U2)\
    DEFINE_DERIVED_SIMPLE_UNIT_SQUARED_SCALAR(U1,U2)\
	DEFINE_DERIVED_OTHER_UNIT_SQUARED_VECTOR(U1,U2)

// Private
// M1 (Scalar) * M2 (Scalar) == M3 (Scalar)
// with M1 != M2
#define DEFINE_DERIVED_SIMPLE_MAGNITUDE_SCALAR_SCALAR(M1,M2,M3)\
    namespace measures\
    {\
		/* dyn_vect1 * dyn_vect1 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M3,decltype(Num1()*Num2())> operator *(\
            dyn_vect1<M1,Num1> m1,\
            dyn_vect1<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num1()*Num2())>(\
					M3(result->second), m1.value() * m2.value());\
			}\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
			auto val2 = convert(M2::base_unit::id(), m2).value();\
            return dyn_vect1<M3,decltype(Num1()*Num2())>(\
				M3::base_unit::id(), val1 * val2);\
        }\
        \
		/* dyn_vect1 * dyn_vect1 -> dyn_vect1 */\
        template <typename Num2, typename Num1>\
        dyn_vect1<M3,decltype(Num2()*Num1())> operator *(\
            dyn_vect1<M2,Num2> m2,\
            dyn_vect1<M1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M3,decltype(Num2()*Num1())>(\
					M3(result->second), m2.value() * m1.value());\
			}\
			auto val2 = convert(M2::base_unit::id(), m2).value();\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
            return dyn_vect1<M3,decltype(Num2()*Num1())>(\
				M3::base_unit::id(), val2 * val1);\
        }\
        \
		/* dyn_vect1 / dyn_vect1 -> dyn_vect1 */\
        template <typename Num3, typename Num1>\
        dyn_vect1<M2,decltype(Num3()/Num1())> operator /(\
            dyn_vect1<M3,Num3> m3,\
            dyn_vect1<M1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num3)\
			auto result = DerivationRules2.find(\
				std::make_pair(m1.unit(), m3.unit()));\
			if (result != DerivationRules2.end()) {\
				return dyn_vect1<M2,decltype(Num3()/Num1())>(\
					M2(result->second), m3.value() / m1.value());\
			}\
			auto val3 = convert(M3::base_unit::id(), m3).value();\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
            return dyn_vect1<M2,decltype(Num3()/Num1())>(\
				M2::base_unit::id(), val3 / val1);\
        }\
        \
		/* dyn_vect1 / dyn_vect1 -> dyn_vect1 */\
        template <typename Num3, typename Num2>\
        dyn_vect1<M1,decltype(Num3()/Num2())> operator /(\
            dyn_vect1<M3,Num3> m3,\
            dyn_vect1<M2,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num2)\
			ASSERT_IS_NUMERIC(Num3)\
			auto result = DerivationRules1.find(\
				std::make_pair(m2.unit(), m3.unit()));\
			if (result != DerivationRules1.end()) {\
				return dyn_vect1<M1,decltype(Num3()/Num2())>(\
					M1(result->second), m3.value() / m2.value());\
			}\
			auto val3 = convert(M3::base_unit::id(), m3).value();\
			auto val2 = convert(M2::base_unit::id(), m2).value();\
            return dyn_vect1<M1,decltype(Num3()/Num2())>(\
				M1::base_unit::id(), val3 / val2);\
        }\
    }

// Private
// M1 (Scalar) * M1 (Scalar) == M2 (Scalar)
#define DEFINE_DERIVED_SIMPLE_MAGNITUDE_SQUARED_SCALAR(M1,M2)\
    namespace measures\
    {\
		/* dyn_vect1 * dyn_vect1 -> dyn_vect1 */\
        template <typename Num1, typename Num2>\
        dyn_vect1<M2,decltype(Num1()*Num2())> operator *(\
            dyn_vect1<M1,Num1> m1,\
            dyn_vect1<M1,Num2> m2)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules3.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules3.end()) {\
				return dyn_vect1<M2,decltype(Num1()*Num2())>(\
					M2(result->second), m1.value() * m2.value());\
			}\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
			auto val2 = convert(M1::base_unit::id(), m2).value();\
            return dyn_vect1<M2,decltype(Num1()*Num2())>(\
				M2::base_unit::id(), val1 * val2);\
        }\
        \
		/* dyn_vect1 / dyn_vect1 -> dyn_vect1 */\
        template <typename Num2, typename Num1>\
        dyn_vect1<M1,decltype(Num2()/Num1())> operator /(\
            dyn_vect1<M2,Num2> m2,\
            dyn_vect1<M1,Num1> m1)\
        {\
			ASSERT_IS_NUMERIC(Num1)\
			ASSERT_IS_NUMERIC(Num2)\
			auto result = DerivationRules2.find(\
				std::make_pair(m1.unit(), m2.unit()));\
			if (result != DerivationRules2.end()) {\
				return dyn_vect1<M1,decltype(Num2()/Num1())>(\
					M1(result->second), m2.value() / m1.value());\
			}\
			auto val2 = convert(M2::base_unit::id(), m2).value();\
			auto val1 = convert(M1::base_unit::id(), m1).value();\
            return dyn_vect1<M1,decltype(Num2()/Num1())>(\
				M1::base_unit::id(), val2 / val1);\
        }\
		\
		/* sqroot(dyn_vect1) -> dyn_vect1 */\
        template <typename Num>\
        dyn_vect1<M1,Num> sqroot(dyn_vect1<M2,Num> m2)\
        {\
			ASSERT_IS_NUMERIC(Num)\
			auto result = DerivationRulesSquared.find(m2.unit());\
			if (result != DerivationRulesSquared.end()) {\
				return dyn_vect1<M1,Num>(\
					M1(result->second), static_cast<Num>(sqrt(m2.value())));\
			}\
			auto val2 = convert(M2::base_unit::id(), m2).value();\
            return dyn_vect1<M1,Num>(\
				M1::base_unit::id(), static_cast<Num>(sqrt(val2)));\
        }\
    }

	
namespace measures
{
    ///////////////////////// STATIC-UNIT SCALAR AND 1-DIMENSIONAL VECTORS AND POINTS /////////////////////////

    template <class Unit, typename Num = double>
    class vect1
    {
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;
	
        // Construct without values.
        explicit vect1() { }

        // Construct using one number of the same number type.
        explicit vect1(Num x): x_(x) { }

        // Construct using another vect1 of the same unit
		// and the same number type.
        vect1(const vect1<Unit,Num>& o):
            x_(o.value()) { }

        // Get unmutable value.
        Num value() const { return x_; }

        // Get mutable value.
        Num& value() { return x_; }

        // +vect1 -> vect1
        vect1<Unit,Num> operator +() const
        {
            return *this;
        }

        // -vect1 -> vect1
        vect1<Unit,Num> operator -() const
        {
            return vect1<Unit,Num>(-x_);
        }

        // vect1 += vect1 -> vect1
        vect1<Unit,Num> operator +=(vect1<Unit,Num> m2)
        {
            x_ += m2.value();
            return *this;
        }

        // vect1 -= vect1 -> vect1
        vect1<Unit,Num> operator -=(vect1<Unit,Num> m2)
        {
            x_ -= m2.value();
            return *this;
        }

        // vect1 *= N -> vect1
		template <typename Num2>
        vect1<Unit,Num> operator *=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ *= static_cast<Num>(n);
            return *this;
        }

        // vect1 /= N -> vect1
		template <typename Num2>
        vect1<Unit,Num> operator /=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ /= static_cast<Num>(n);
            return *this;
        }

    private:
        Num x_;
    };

    // vect1 == vect1 -> bool
    template <class Unit, typename Num>
    bool operator ==(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() == m2.value();
    }

    // vect1 != vect1 -> bool
    template <class Unit, typename Num>
    bool operator !=(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() != m2.value();
    }

    // vect1 < vect1 -> bool
    template <class Unit, typename Num>
    bool operator <(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() < m2.value();
    }

    // vect1 <= vect1 -> bool
    template <class Unit, typename Num>
    bool operator <=(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() <= m2.value();
    }

    // vect1 > vect1 -> bool
    template <class Unit, typename Num>
    bool operator >(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() > m2.value();
    }

    // vect1 >= vect1 -> bool
    template <class Unit, typename Num>
    bool operator >=(vect1<Unit,Num> m1, vect1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() >= m2.value();
    }

    // is_equal(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2,
		vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(abs(m1.value() - m2.value()))
			<= tolerance.value();
    }

    // is_less(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(m1.value())
			< static_cast<Num3>(m2.value()) - tolerance.value();
    }

    // is_less_or_equal(vect1, vect1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(m1.value())
			<= static_cast<Num3>(m2.value()) + tolerance.value();
    }
	
    template <class Unit, typename Num = double>
    class point1
    {
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;

        // Construct without values.
        explicit point1() { }

        // Construct using one number of the same number type.
        explicit point1(Num x): x_(x) { }
		
        // Construct using another point1 of the same unit
		// and the same number type.
        point1(const point1<Unit,Num>& o):
            x_(o.value()) { }

        // Get unmutable value.
        Num value() const { return x_; }

        // Get mutable value.
        Num& value() { return x_; }

        // point1 += vect1 -> point1
        point1<Unit,Num> operator +=(vect1<Unit,Num> m2)
		{
			x_ += m2.value();
			return *this;
		}

        // point1 -= vect1 -> point1
        point1<Unit,Num> operator -=(vect1<Unit,Num> m2)
		{
			x_ -= m2.value();
			return *this;
		}

    private:
        Num x_;
    };

	// midpoint(point1, point1, weight) -> point1
    template <class Unit, typename Num1, typename Num2, typename Num3>
	point1<Unit,decltype((Num1()+Num2())*Num3())> midpoint(
		point1<Unit,Num1> p1, point1<Unit,Num2> p2, Num3 weight = 0.5f)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		//return p1 + (p2 - p1) * weight;
		return point1<Unit,decltype((Num1()+Num2())*Num3())>(
			p1.value() * (1 - weight) + p2.value() * weight);
	}

	// barycentric_combination(int, point1[], Num[]) -> point1
    template <class Unit, typename Num>
	point1<Unit,Num> barycentric_combination(
		int n, point1<Unit,Num> p[], Num weights[])
	{
		ASSERT_IS_NUMERIC(Num)
		Num result = 0;
		for (int i = 0; i < n; ++i)
		{
			result += p[i].value() * weights[i];
		}
		return point1<Unit,Num>(result);
	}
	
	// point1 - point1 -> vect1
    template <class Unit, typename Num1, typename Num2>
	vect1<Unit,decltype(Num1()-Num2())> operator -(
		point1<Unit,Num1> m1, point1<Unit,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		return vect1<Unit,decltype(Num1()-Num2())>(
			m1.value() - m2.value());
	}
	
    // point1 == point1 -> bool
    template <class Unit, typename Num>
    bool operator ==(point1<Unit,Num> m1, point1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() == m2.value();
    }

    // point1 != point1 -> bool
    template <class Unit, typename Num>
    bool operator !=(point1<Unit,Num> m1, point1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() != m2.value();
    }

    // point1 < point1 -> bool
    template <class Unit, typename Num>
    bool operator <(point1<Unit,Num> m1, point1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() < m2.value();
    }

    // point1 <= point1 -> bool
    template <class Unit, typename Num>
    bool operator <=(point1<Unit,Num> m1, point1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() <= m2.value();
    }

    // point1 > point1 -> bool
    template <class Unit, typename Num>
    bool operator >(point1<Unit,Num> m1, point1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() > m2.value();
    }

    // point1 >= point1 -> bool
    template <class Unit, typename Num>
    bool operator >=(point1<Unit,Num> m1, point1<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() >= m2.value();
    }

    // is_equal(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(point1<Unit,Num1> m1, point1<Unit,Num2> m2,
		vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(abs(m1.value() - m2.value()))
			<= tolerance.value();
    }

    // is_less(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less(point1<Unit,Num1> m1, point1<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(m1.value())
			< static_cast<Num3>(m2.value()) - tolerance.value();
    }

    // is_less_or_equal(point1, point1, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(point1<Unit,Num1> m1, point1<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(m1.value())
			<= static_cast<Num3>(m2.value()) + tolerance.value();
    }
	
    // point1 + vect1 -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype(Num1()+Num2())> operator +(point1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return point1<Unit,decltype(Num1()+Num2())>(m1.value() + m2.value());
    }

    // point1 - vect1 -> point1
    template <class Unit, typename Num1, typename Num2>
    point1<Unit,decltype(Num1()-Num2())> operator -(
		point1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return point1<Unit,decltype(Num1()-Num2())>(
			m1.value() - m2.value());
    }

    // vect1 + vect1 -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()+Num2())> operator +(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect1<Unit,decltype(Num1()+Num2())>(m1.value() + m2.value());
    }

    // vect1 - vect1 -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()-Num2())> operator -(
		vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect1<Unit,decltype(Num1()-Num2())>(
			m1.value() - m2.value());
    }

    // N * vect1 -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()*Num2())> operator *(Num1 n, vect1<Unit,Num2> m)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect1<Unit,decltype(Num1()*Num2())>(n * m.value());
    }

    // vect1 * N -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()*Num2())> operator *(vect1<Unit,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect1<Unit,decltype(Num1()*Num2())>(m.value() * n);
    }

    // vect1 / N -> vect1
    template <class Unit, typename Num1, typename Num2>
    vect1<Unit,decltype(Num1()/Num2())> operator /(vect1<Unit,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect1<Unit,decltype(Num1()/Num2())>(m.value() / n);
    }

    // vect1 / vect1 -> N
    template <class Unit, typename Num1, typename Num2>
    decltype(Num1()/Num2()) operator /(vect1<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return m1.value() / m2.value();
    }

	// squared_norm_value(vect1) -> N
	template <class Unit, typename Num>
    Num squared_norm_value(vect1<Unit,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return sqr(v.value());
    }

	// norm(vect1) -> vect1
    template <class Unit, typename Num>
    vect1<Unit,Num> norm(vect1<Unit,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return vect1<Unit,Num>(abs(v.value()));
    }
	
    ///////////////////////// STATIC-UNIT 2-DIMENSIONAL VECTORS AND POINTS /////////////////////////

    template <class Unit, typename Num> class signed_azimuth;

    template <class Unit, typename Num> class unsigned_azimuth;

    template <class Unit, typename Num = double>
    class vect2
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;

        // Construct without values.
        explicit vect2() { }

        // Construct using two numbers of the same number type.
        explicit vect2(Num x, Num y): x_(x), y_(y) { }

        // Construct using two vect1s of the same number type.
        explicit vect2(vect1<Unit,Num> x, vect1<Unit,Num> y):
            x_(x.value()), y_(y.value()) { }

        // Construct using another vect2 of the same unit
        // and the same number type.
        vect2(const vect2<Unit,Num>& o):
            x_(o.x_), y_(o.y_) { }

        // Construct a versor using a point1 representing an angle.
        template <class Unit2, typename Num2>
        explicit vect2(const point1<Unit2,Num2>& a):
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a)))
        {
			ASSERT_IS_NUMERIC(Num2)
            ASSERT_IS_ANGLE(Unit2)
        }

        // Construct a versor using a signed_azimuth.
        template <class Unit2, typename Num2>
        explicit vect2(const signed_azimuth<Unit2,Num2>& a):
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a)))
		{
			ASSERT_IS_NUMERIC(Num2)
		}

        // Construct a versor using an unsigned_azimuth.
        template <class Unit2, typename Num2>
        explicit vect2(const unsigned_azimuth<Unit2,Num2>& a):
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a)))
		{
			ASSERT_IS_NUMERIC(Num2)
		}

        // +vect2 -> vect2
        vect2<Unit,Num> operator +() const
        {
            return *this;
        }

        // -vect2 -> vect2
        vect2<Unit,Num> operator -() const
        {
            return vect2<Unit,Num>(-x_, -y_);
        }

        // vect2 += vect2 -> vect2
        vect2<Unit,Num> operator +=(vect2<Unit,Num> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            return *this;
        }

        // vect2 -= vect2 -> vect2
        vect2<Unit,Num> operator -=(vect2<Unit,Num> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            return *this;
        }

        // vect2 *= N -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator *=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ *= static_cast<Num>(n);
            y_ *= static_cast<Num>(n);
            return *this;
        }

        // vect2 /= N -> vect2
        template <typename Num2>
        vect2<Unit,Num> operator /=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ /= static_cast<Num>(n);
            y_ /= static_cast<Num>(n);
            return *this;
        }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component.
        vect1<Unit,Num> const x() const { return vect1<Unit,Num>(x_); }

		// Get mutable x component.
        vect1<Unit,Num>& x() { return reinterpret_cast<vect1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        vect1<Unit,Num> const y() const { return vect1<Unit,Num>(y_); }

		// Get mutable y component.
        vect1<Unit,Num>& y() { return reinterpret_cast<vect1<Unit,Num>&>(y_); }

    private:

        // Components.
        Num x_, y_;
    };

    // vect2 == vect2 -> bool
    template <class Unit, typename Num>
    bool operator ==(vect2<Unit,Num> m1, vect2<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // vect2 != vect2 -> bool
    template <class Unit, typename Num>
    bool operator !=(vect2<Unit,Num> m1, vect2<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }
	
    // is_equal(vect2, vect2, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(vect2<Unit,Num1> m1, vect2<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(squared_norm_value(m1 - m2))
			<= squared_norm_value(tolerance);
    }
	
    template <class Unit, typename Num = double>
    class point2
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;

        // Construct without values.
        explicit point2() { }

        // Construct using two numbers of the same number type.
        explicit point2(Num x, Num y): x_(x), y_(y) { }
		
        // Construct using two point1s of the same unit and number type.
        explicit point2(point1<Unit,Num> x, point1<Unit,Num> y):
            x_(x.value()), y_(y.value()) { }

        // Construct using another point2 of the same unit and number type.
        template <typename Num2>
        point2(const point2<Unit,Num2>& o):
            x_(o.x_), y_(o.y_)
		{
			ASSERT_IS_NUMERIC(Num2)
		}

        // point2 += vect2 -> point2
        point2<Unit,Num> operator +=(vect2<Unit,Num> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            return *this;
        }

        // point2 -= vect2 -> point2
        point2<Unit,Num> operator -=(vect2<Unit,Num> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            return *this;
        }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component.
        point1<Unit,Num> const x() const { return point1<Unit,Num>(x_); }

		// Get mutable x component.
        point1<Unit,Num>& x() { return reinterpret_cast<point1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        point1<Unit,Num> const y() const { return point1<Unit,Num>(y_); }

		// Get mutable y component.
        point1<Unit,Num>& y() { return reinterpret_cast<point1<Unit,Num>&>(y_); }

    private:

        // Components.
        Num x_, y_;
    };

	// midpoint(point2, point2, weight) -> point2
    template <class Unit, typename Num1, typename Num2, typename Num3>
	point2<Unit,decltype((Num1()+Num2())*Num3())> midpoint(
		point2<Unit,Num1> p1, point2<Unit,Num2> p2, Num3 weight = 0.5f)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		//return p1 + (p2 - p1) * weight;
		return point2<Unit,decltype((Num1()+Num2())*Num3())>(
			p1.x().value() * (1 - weight) + p2.x().value() * weight,
			p1.y().value() * (1 - weight) + p2.y().value() * weight);
	}

	// barycentric_combination(int, point2[], Num[]) -> point2
    template <class Unit, typename Num>
	point2<Unit,Num> barycentric_combination(
		int n, point2<Unit,Num> p[], Num weights[])
	{
		ASSERT_IS_NUMERIC(Num)
		Num result_x = 0;
		Num result_y = 0;
		for (int i = 0; i < n; ++i)
		{
			result_x += p[i].x().value() * weights[i];
			result_y += p[i].y().value() * weights[i];
		}
		return point2<Unit,Num>(result_x, result_y);
	}

	// point2 - point2 -> vect2
    template <class Unit, typename Num1, typename Num2>
	vect2<Unit,decltype(Num1()-Num2())> operator -(
		point2<Unit,Num1> m1, point2<Unit,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		return vect2<Unit,decltype(Num1()-Num2())>(
			m1.x().value() - m2.x().value(),
			m1.y().value() - m2.y().value());
	}

    // point2 == point2 -> bool
    template <class Unit, typename Num>
    bool operator ==(point2<Unit,Num> m1, point2<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // point2 != point2 -> bool
    template <class Unit, typename Num>
    bool operator !=(point2<Unit,Num> m1, point2<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }

    // is_equal(point2, point2, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(point2<Unit,Num1> m1, point2<Unit,Num2> m2,
		vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return static_cast<Num3>(squared_norm_value(m1 - m2))
			<= squared_norm_value(tolerance);
    }
	
    // point2 + vect2 -> point2
    template <class Unit, typename Num1, typename Num2>
    point2<Unit,decltype(Num1()+Num2())> operator +(
		point2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return point2<Unit,decltype(Num1()+Num2())>(
			m1.x().value() + m2.x().value(),
			m1.y().value() + m2.y().value());
    }

    // point2 - vect2 -> point2
    template <class Unit, typename Num1, typename Num2>
    point2<Unit,decltype(Num1()-Num2())> operator -(
		point2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return point2<Unit,decltype(Num1()-Num2())>(
			m1.x().value() - m2.x().value(),
			m1.y().value() - m2.y().value());
    }

    // vect2 + vect2 -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()+Num2())> operator +(vect2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect2<Unit,decltype(Num1()+Num2())>(
			m1.x().value() + m2.x().value(),
			m1.y().value() + m2.y().value());
    }

    // vect2 - vect2 -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()-Num2())> operator -(
		vect2<Unit,Num1> m1, vect2<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect2<Unit,decltype(Num1()-Num2())>(
			m1.x().value() - m2.x().value(),
			m1.y().value() - m2.y().value());
    }

    // N * vect2 -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()*Num2())> operator *(Num1 n, vect2<Unit,Num2> m)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect2<Unit,decltype(Num1()*Num2())>(
			n * m.x().value(),
			n * m.y().value());
    }

    // vect2 * N -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()*Num2())> operator *(vect2<Unit,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect2<Unit,decltype(Num1()*Num2())>(
			m.x().value() * n,
			m.y().value() * n);
    }

    // vect2 / N -> vect2
    template <class Unit, typename Num1, typename Num2>
    vect2<Unit,decltype(Num1()/Num2())> operator /(vect2<Unit,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect2<Unit,decltype(Num1()*Num2())>(
			m.x().value() / n,
			m.y().value() / n);
    }

    template <class Unit, typename Num>
    Num squared_norm_value(vect2<Unit,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
		return sqr(v.x().value()) + sqr(v.y().value());
    }

    template <class Unit, typename Num>
    vect1<Unit,Num> norm(vect2<Unit,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return vect1<Unit,Num>(
			static_cast<Num>(sqrt(squared_norm_value(v))));
    }

	
    ///////////////////////// STATIC 3-DIMENSIONAL POINTS AND VECTORS /////////////////////////

    template <class Unit, typename Num = double>
    class vect3
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;

        // Construct without values.
        explicit vect3() { }

        // Construct using three numbers of the same number type.
        explicit vect3(Num x, Num y, Num z): x_(x), y_(y), z_(z) { }

        // Construct using three vect1s of the same unit and number type.
        explicit vect3(vect1<Unit,Num> x, vect1<Unit,Num> y, vect1<Unit,Num> z):
            x_(x.value()), y_(y.value()), z_(z.value()) { }

        // Construct using another vect3 of the same unit and number type.
        vect3(const vect3<Unit,Num>& o):
            x_(o.x_), y_(o.y_), z_(o.z_) { }

        // +vect3 -> vect3
        vect3<Unit,Num> operator +() const
        {
            return *this;
        }

        // -vect3 -> vect3
        vect3<Unit,Num> operator -() const
        {
            return vect3<Unit,Num>(-x_, -y_, -z_);
        }

        // vect3 += vect3 -> vect3
        vect3<Unit,Num> operator +=(vect3<Unit,Num> m2)
        {
            x_ += m2.x().value();
            y_ += m2.y().value();
            z_ += m2.z().value();
            return *this;
        }

        // vect3 -= vect3 -> vect3
        vect3<Unit,Num> operator -=(vect3<Unit,Num> m2)
        {
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            z_ -= m2.z().value();
            return *this;
        }

        // vect3 *= N -> vect3
        template <typename Num2>
        vect3<Unit,Num> operator *=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ *= static_cast<Num>(n);
            y_ *= static_cast<Num>(n);
            z_ *= static_cast<Num>(n);
            return *this;
        }

        // vect3 /= N -> vect3
        template <typename Num2>
        vect3<Unit,Num> operator /=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ /= static_cast<Num>(n);
            y_ /= static_cast<Num>(n);
            z_ /= static_cast<Num>(n);
            return *this;
        }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component.
        vect1<Unit,Num> const x() const { return vect1<Unit,Num>(x_); }

		// Get mutable x component.
        vect1<Unit,Num>& x() { return reinterpret_cast<vect1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        vect1<Unit,Num> const y() const { return vect1<Unit,Num>(y_); }

		// Get mutable y component.
        vect1<Unit,Num>& y() { return reinterpret_cast<vect1<Unit,Num>&>(y_); }

        // Get unmutable z component.
        vect1<Unit,Num> const z() const { return vect1<Unit,Num>(z_); }

		// Get mutable z component.
        vect1<Unit,Num>& z() { return reinterpret_cast<vect1<Unit,Num>&>(z_); }

    private:

        // Components.
        Num x_, y_, z_;
    };

    // vect3 == vect3 -> bool
    template <class Unit, typename Num>
    bool operator ==(vect3<Unit,Num> m1, vect3<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // vect3 != vect3 -> bool
    template <class Unit, typename Num>
    bool operator !=(vect3<Unit,Num> m1, vect3<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value()
            || m1.z().value() != m2.z().value();
    }

    // is_equal(vect3, vect3, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(vect3<Unit,Num1> m1, vect3<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		return static_cast<Num3>(squared_norm_value(m1 - m2))
			<= squared_norm_value(tolerance);
    }
	
    template <class Unit, typename Num = double>
    class point3
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;

        // Construct without values.
        explicit point3() { }

        // Construct using two numbers of the same number type.
        explicit point3(Num x, Num y, Num z): x_(x), y_(y), z_(z) { }

        // Construct using three point1s of the same unit and number type.
        explicit point3(point1<Unit,Num> x, point1<Unit,Num> y, point1<Unit,Num> z):
            x_(x.value()), y_(y.value()), z_(z.value()) { }

        // Construct using another point3 of the same unit and number type.
        point3(const point3<Unit,Num>& o):
            x_(o.x_), y_(o.y_), z_(o.z_) { }

        // point3 += vect3 -> point3
        point3<Unit,Num> operator +=(vect3<Unit,Num> m2)
		{
            x_ += m2.x().value();
            y_ += m2.y().value();
            z_ += m2.z().value();
            return *this;
        }

        // point3 -= vect3 -> point3
        point3<Unit,Num> operator -=(vect3<Unit,Num> m2)
		{
            x_ -= m2.x().value();
            y_ -= m2.y().value();
            z_ -= m2.z().value();
            return *this;
        }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component.
        point1<Unit,Num> const x() const { return point1<Unit,Num>(x_); }

		// Get mutable x component.
        point1<Unit,Num>& x() { return reinterpret_cast<point1<Unit,Num>&>(x_); }

        // Get unmutable y component.
        point1<Unit,Num> const y() const { return point1<Unit,Num>(y_); }

		// Get mutable y component.
        point1<Unit,Num>& y() { return reinterpret_cast<point1<Unit,Num>&>(y_); }

        // Get unmutable z component.
        point1<Unit,Num> const z() const { return point1<Unit,Num>(z_); }

		// Get mutable z component.
        point1<Unit,Num>& z() { return reinterpret_cast<point1<Unit,Num>&>(z_); }

    private:

        // Components.
        Num x_, y_, z_;
    };

	// midpoint(point3, point3, weight) -> point3
    template <class Unit, typename Num1, typename Num2, typename Num3>
	point3<Unit,decltype((Num1()+Num2())*Num3())> midpoint(
		point3<Unit,Num1> p1, point3<Unit,Num2> p2, Num3 weight = 0.5f)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		//return p1 + (p2 - p1) * weight;
		return point3<Unit,decltype((Num1()+Num2())*Num3())>(
			p1.x().value() * (1 - weight) + p2.x().value() * weight,
			p1.y().value() * (1 - weight) + p2.y().value() * weight,
			p1.z().value() * (1 - weight) + p2.z().value() * weight);
	}

	// barycentric_combination(int, point3[], Num[]) -> point3
    template <class Unit, typename Num>
	point3<Unit,Num> barycentric_combination(
		int n, point3<Unit,Num> p[], Num weights[])
	{
		ASSERT_IS_NUMERIC(Num)
		Num result_x = 0;
		Num result_y = 0;
		Num result_z = 0;
		for (int i = 0; i < n; ++i)
		{
			result_x += p[i].x().value() * weights[i];
			result_y += p[i].y().value() * weights[i];
			result_z += p[i].z().value() * weights[i];
		}
		return point3<Unit,Num>(result_x, result_y, result_z);
	}

	// point3 - point3 -> vect3
    template <class Unit, typename Num1, typename Num2>
	vect3<Unit,decltype(Num1()-Num2())> operator -(
		point3<Unit,Num1> m1, point3<Unit,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		return vect3<Unit,decltype(Num1()-Num2())>(
			m1.x().value() - m2.x().value(),
			m1.y().value() - m2.y().value(),
			m1.z().value() - m2.z().value());
	}
	
    // point3 == point3 -> bool
    template <class Unit, typename Num>
    bool operator ==(point3<Unit,Num> m1, point3<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // point3 != point3 -> bool
    template <class Unit, typename Num>
    bool operator !=(point3<Unit,Num> m1, point3<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value()
            || m1.z().value() != m2.z().value();
    }

    // is_equal(point3, point3, tolerance) -> bool
    template <class Unit, typename Num1, typename Num2, typename Num3>
    bool is_equal(point3<Unit,Num1> m1, point3<Unit,Num2> m2, vect1<Unit,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		return static_cast<Num3>(squared_norm_value(m1 - m2))
			<= squared_norm_value(tolerance);
    }
	
    // point3 + vect3 -> point3
    template <class Unit, typename Num1, typename Num2>
    point3<Unit,decltype(Num1()+Num2())> operator +(point3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return point3<Unit,decltype(Num1()+Num2())>(
			m1.x().value() + m2.x().value(),
			m1.y().value() + m2.y().value(),
			m1.z().value() + m2.z().value());
    }

    // point3 - vect3 -> point3
    template <class Unit, typename Num1, typename Num2>
    point3<Unit,decltype(Num1()-Num2())> operator -(
		point3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return point3<Unit,decltype(Num1()-Num2())>(
			m1.x().value() - m2.x().value(),
			m1.y().value() - m2.y().value(),
			m1.z().value() - m2.z().value());
    }


    // vect3 + vect3 -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()+Num2())> operator +(vect3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect3<Unit,decltype(Num1()+Num2())>(
			m1.x().value() + m2.x().value(),
			m1.y().value() + m2.y().value(),
			m1.z().value() + m2.z().value());
    }

    // vect3 - vect3 -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()-Num2())> operator -(
		vect3<Unit,Num1> m1, vect3<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect3<Unit,decltype(Num1()-Num2())>(
			m1.x().value() - m2.x().value(),
			m1.y().value() - m2.y().value(),
			m1.z().value() - m2.z().value());
    }

    // N * vect3 -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()*Num2())> operator *(Num1 n, vect3<Unit,Num2> m)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect3<Unit,decltype(Num1()*Num2())>(
			n * m.x().value(),
			n * m.y().value(),
			n * m.z().value());
    }

    // vect3 * N -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()*Num2())> operator *(vect3<Unit,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect3<Unit,decltype(Num1()*Num2())>(
			m.x().value() * n,
			m.y().value() * n,
			m.z().value() * n);
    }

    // vect3 / N -> vect3
    template <class Unit, typename Num1, typename Num2>
    vect3<Unit,decltype(Num1()/Num2())> operator /(vect3<Unit,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return vect3<Unit,decltype(Num1()*Num2())>(
			m.x().value() / n,
			m.y().value() / n,
			m.z().value() / n);
    }

    template <class Unit, typename Num>
    Num squared_norm_value(vect3<Unit,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
		return sqr(v.x().value()) + sqr(v.y().value()) + sqr(v.z().value());
    }

    template <class Unit, typename Num>
    vect1<Unit,Num> norm(vect3<Unit,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return vect1<Unit,Num>(
			static_cast<Num>(sqrt(squared_norm_value(v))));
    }


    ///////////////////////// STATIC CONVERSIONS /////////////////////////

    // Scalar measures
    template <class ToUnit, class FromUnit, typename Num>
    vect1<ToUnit,Num> convert(vect1<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return vect1<ToUnit,Num>(static_cast<Num>(
			FromUnit::ratio() / ToUnit::ratio() * m.value()));
    }

    template <class ToUnit, class FromUnit, typename Num>
    point1<ToUnit,Num> convert(point1<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return point1<ToUnit,Num>(static_cast<Num>(
            (FromUnit::offset() - ToUnit::offset()
            + m.value() * FromUnit::ratio())
            / ToUnit::ratio()));
    }

    // Planar measures
    template <class ToUnit, class FromUnit, typename Num>
    vect2<ToUnit,Num> convert(vect2<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return vect2<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()));
    }

    template <class ToUnit, class FromUnit, typename Num>
    point2<ToUnit,Num> convert(point2<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return point2<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()));
    }

    // Spacial measures
    template <class ToUnit, class FromUnit, typename Num>
    vect3<ToUnit,Num> convert(vect3<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return vect3<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()),
            convert<ToUnit,FromUnit,Num>(m.z()));
    }

    template <class ToUnit, class FromUnit, typename Num>
    point3<ToUnit,Num> convert(point3<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return point3<ToUnit,Num>(
            convert<ToUnit,FromUnit,Num>(m.x()),
            convert<ToUnit,FromUnit,Num>(m.y()),
            convert<ToUnit,FromUnit,Num>(m.z()));
    }

	// Azimuths
    template <class ToUnit, class FromUnit, typename Num>
    signed_azimuth<ToUnit,Num> convert(signed_azimuth<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return signed_azimuth<ToUnit,Num>(convert<ToUnit>(point1<FromUnit>(m.value())));
    }

    template <class ToUnit, class FromUnit, typename Num>
    unsigned_azimuth<ToUnit,Num> convert(unsigned_azimuth<FromUnit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_HAVE_SAME_MAGNITUDE(ToUnit, FromUnit)
        return unsigned_azimuth<ToUnit,Num>(convert<ToUnit>(point1<FromUnit>(m.value())));
    }


    ///////////////////////// STATIC CASTS /////////////////////////

    // Scalar measures
    template <class Unit, typename ToNum, typename FromNum>
    vect1<Unit,ToNum> cast(vect1<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return vect1<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }

    template <class Unit, typename ToNum, typename FromNum>
    point1<Unit,ToNum> cast(point1<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return point1<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }

    // Planar measures
    template <class Unit, typename ToNum, typename FromNum>
    vect2<Unit,ToNum> cast(vect2<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return vect2<Unit,ToNum>(
            cast<Unit,ToNum,FromNum>(m.x()),
            cast<Unit,ToNum,FromNum>(m.y()));
    }

    template <class Unit, typename ToNum, typename FromNum>
    point2<Unit,ToNum> cast(point2<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return point2<Unit,ToNum>(
            cast<Unit,ToNum,FromNum>(m.x()),
            cast<Unit,ToNum,FromNum>(m.y()));
    }

    // Spacial measures
    template <class Unit, typename ToNum, typename FromNum>
    vect3<Unit,ToNum> cast(vect3<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return vect3<Unit,ToNum>(
            cast<Unit,ToNum,FromNum>(m.x()),
            cast<Unit,ToNum,FromNum>(m.y()),
            cast<Unit,ToNum,FromNum>(m.z()));
    }

    template <class Unit, typename ToNum, typename FromNum>
    point3<Unit,ToNum> cast(point3<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return point3<Unit,ToNum>(
            cast<Unit,ToNum,FromNum>(m.x()),
            cast<Unit,ToNum,FromNum>(m.y()),
            cast<Unit,ToNum,FromNum>(m.z()));
    }

	// Azimuths
    template <class Unit, typename ToNum, typename FromNum>
    signed_azimuth<Unit,ToNum> cast(signed_azimuth<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return signed_azimuth<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }

    template <class Unit, typename ToNum, typename FromNum>
    unsigned_azimuth<Unit,ToNum> cast(unsigned_azimuth<Unit,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return unsigned_azimuth<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }
	
    ///////////////////////// STATIC ANGLES UTILS /////////////////////////
	
	// Private.
	// For integral numbers.
	template <typename Num>
	Num normalize_signed_azimuth(Num x, Num one_turn,
		typename std::enable_if<std::is_integral<Num>::value >::type* = 0)
	{
		ASSERT_IS_NUMERIC(Num)
		const Num half_turn = one_turn / 2;
		if (abs(x) < half_turn) return x;
		Num x2 = (x + half_turn) % one_turn;
		return x2 >= 0 ? x2 - half_turn : x2 + half_turn;
	}

	// Private.
	// For floating-point numbers.
	template <typename Num>
	Num normalize_signed_azimuth(Num x, Num one_turn,
		typename std::enable_if<std::is_floating_point<Num>
		::value >::type* = 0)
	{
		ASSERT_IS_NUMERIC(Num)
		const Num half_turn = one_turn * 0.5f;
		if (abs(x) < half_turn) return x;
		Num x2 = fmod(x + half_turn, one_turn);
		return x2 >= 0 ? x2 - half_turn : x2 + half_turn;
	}

	// Private.
	// For integral numbers.
	template <typename Num>
	Num normalize_unsigned_azimuth(Num x, Num one_turn,
		typename std::enable_if<std::is_integral<Num>::value >::type* = 0)
	{
		ASSERT_IS_NUMERIC(Num)
		Num x2 = x % one_turn;
		return x2 >= 0 ? x2 : x2 + one_turn;
	}

	// Private.
	// For floating-point numbers.
	template <typename Num>
	Num normalize_unsigned_azimuth(Num x, Num one_turn,
		typename std::enable_if<std::is_floating_point<Num>
		::value >::type* = 0)
	{
		ASSERT_IS_NUMERIC(Num)
		Num x2 = fmod(x, one_turn);
		return x2 >= 0 ? x2 : x2 + one_turn;
	}
	
	//// TRIGONOMETRY ////
	
    // sin(vect1) -> N
    template <class Unit, typename Num>
    Num sin(vect1<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_IS_ANGLE(Unit)
        return std::sin(convert<radians>(m).value());
    }

    // cos(vect1) -> N
    template <class Unit, typename Num>
    Num cos(vect1<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_IS_ANGLE(Unit)
        return std::cos(convert<radians>(m).value());
    }

    // tan(vect1) -> N
    template <class Unit, typename Num>
    Num tan(vect1<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_IS_ANGLE(Unit)
        return std::tan(convert<radians>(m).value());
    }
	
    // sin(point1) -> N
    template <class Unit, typename Num>
    Num sin(point1<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_IS_ANGLE(Unit)
        return std::sin(convert<radians>(m).value());
    }

    // cos(point1) -> N
    template <class Unit, typename Num>
    Num cos(point1<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_IS_ANGLE(Unit)
        return std::cos(convert<radians>(m).value());
    }

    // tan(point1) -> N
    template <class Unit, typename Num>
    Num tan(point1<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        ASSERT_IS_ANGLE(Unit)
        return std::tan(convert<radians>(m).value());
    }

	
	//// SIGNED AZIMUTH ////
	
	// Azimuth are meaningful only if their numeric_type is floating point
	// or their turn fraction is integer.
	// In other words, azimuths with a non-integer turn fraction,
	// like radians, and integer numeric type must be forbidden.	
    template <class Unit = Angle::base_unit, typename Num = double>
    class signed_azimuth
    {
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;

        // Construct without values.
        explicit signed_azimuth() { }

        // Construct using one number of the same number type.
        explicit signed_azimuth(Num v): v_(normalize_(v)) { }

        // Construct using another signed_azimuth of the same unit and number type.
        signed_azimuth(const signed_azimuth<Unit,Num>& o):
            v_(o.v_) { }

        // Construct using a point1 representing an angle.
        explicit signed_azimuth(point1<Unit, Num> o):
            v_(normalize_(o.value()))
        {
            ASSERT_IS_ANGLE(Unit)
        }

        // Construct using an unsigned_azimuth.
        explicit signed_azimuth(unsigned_azimuth<Unit, Num> o):
            v_(normalize_(o.value())) { }

        // Construct using a vect2.
        template <class Unit2, typename Num2>
	    explicit signed_azimuth(vect2<Unit2,Num2> v):
            v_(convert<Unit>(signed_azimuth<radians,Num>(
                static_cast<Num>(atan2(v.y().value(),
				v.x().value())))).value())
		{
			ASSERT_IS_NUMERIC(Num2)
		}

		// Convert to a point1.
		operator point1<Unit,Num>() const
		{
			return point1<Unit,Num>(v_);
		}

        // Get unmutable value (mutable getter not available).
        Num value() const { return v_; }

        // signed_azimuth += vect1 -> signed_azimuth
        signed_azimuth<Unit,Num> operator +=(vect1<Unit,Num> m)
        {
			v_ = normalize_(v_ + m.value());
			return *this;
		}

        // signed_azimuth -= vect1 -> signed_azimuth
        signed_azimuth<Unit,Num> operator -=(vect1<Unit,Num> m)
        {
			v_ = normalize_(v_ - m.value());
			return *this;
		}

    private:
        static Num normalize_(Num x)
        {
			return normalize_signed_azimuth(x, static_cast<Num>(Unit::turn_fraction()));
        }

        Num v_;
    };

	// signed_azimuth - signed_azimuth -> vect1
	// As a difference between two point angles it is meant the shortest
	// vect angle that if added to the second point angle gets the first one.
    template <class Unit, typename Num1, typename Num2>
	vect1<Unit,decltype(Num1()-Num2())> operator -(
		signed_azimuth<Unit,Num1> m1, signed_azimuth<Unit,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		auto difference = m1.value() - m2.value();
		auto one_turn = Unit::turn_fraction();
		if (difference * 2 < -one_turn) difference += one_turn;
		else if (difference * 2 >= one_turn) difference -= one_turn;
		return vect1<Unit,decltype(Num1()-Num2())>(difference);
	}
	
    // signed_azimuth == signed_azimuth -> bool
    template <class Unit, typename Num>
    bool operator ==(signed_azimuth<Unit,Num> m1, signed_azimuth<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() == m2.value();
    }
	
    // signed_azimuth != signed_azimuth -> bool
    template <class Unit, typename Num>
    bool operator !=(signed_azimuth<Unit,Num> m1, signed_azimuth<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() != m2.value();
    }
	
    // angle_distance(signed_azimuth, signed_azimuth) -> vect1
    template <class Unit, typename Num>
    vect1<Unit,Num> angle_distance(signed_azimuth<Unit,Num> m1, signed_azimuth<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		Num value_distance(abs(m1.value() - m2.value()));
		if (value_distance * 2 > Unit::turn_fraction())
			value_distance = Unit::turn_fraction() - value_distance;
		return vect1<Unit,Num>(value_distance);
    }
	
    // is_equal(signed_azimuth, signed_azimuth, tolerance) -> bool
    template <class Unit, typename Num>
    bool is_equal(signed_azimuth<Unit,Num> m1,
		signed_azimuth<Unit,Num> m2, vect1<Unit,Num> tolerance)
    {
		ASSERT_IS_NUMERIC(Num)
        return angle_distance(m1, m2) <= tolerance;
    }
	
    // signed_azimuth + vect1 -> signed_azimuth
    template <class Unit, typename Num1, typename Num2>
    signed_azimuth<Unit,decltype(Num1()+Num2())> operator +(
		signed_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return signed_azimuth<Unit,decltype(Num1()+Num2())>(
			m1.value() + m2.value());
    }
	
    // signed_azimuth - vect1 -> signed_azimuth
    template <class Unit, typename Num1, typename Num2>
    signed_azimuth<Unit,decltype(Num1()-Num2())> operator -(
		signed_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return signed_azimuth<Unit,decltype(Num1()-Num2())>(
			m1.value() - m2.value());
    }
	
    // sin(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num sin(signed_azimuth<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return std::sin(convert<radians>(m).value());
    }

    // cos(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num cos(signed_azimuth<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return std::cos(convert<radians>(m).value());
    }

    // tan(signed_azimuth) -> N
    template <class Unit, typename Num>
    Num tan(signed_azimuth<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return std::tan(convert<radians>(m).value());
    }

    template <class Unit = Angle::base_unit, typename Num = double>
    class unsigned_azimuth
    {
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Unit unit;
		typedef Num numeric_type;

        // Construct without values.
        explicit unsigned_azimuth() { }

        // Construct using one number of the same number type.
        explicit unsigned_azimuth(Num v): v_(normalize_(v)) { }

        // Construct using another unsigned_azimuth of the same unit
		// and the same number type.
        unsigned_azimuth(const unsigned_azimuth<Unit,Num>& o):
            v_(o.v_) { }

        // Construct using a point1 representing an angle.
        explicit unsigned_azimuth(point1<Unit, Num> o):
            v_(normalize_(o.value()))
        {
            ASSERT_IS_ANGLE(Unit)
        }

        // Construct using a signed_azimuth.
        explicit unsigned_azimuth(signed_azimuth<Unit, Num> o):
            v_(normalize_(o.value())) { }

        // Construct using a vect2.
        template <class Unit2, typename Num2>
	    explicit unsigned_azimuth(vect2<Unit2,Num2> v):
            v_(convert<Unit>(unsigned_azimuth<radians,Num>(
                static_cast<Num>(atan2(v.y().value(),
				v.x().value())))).value())
		{
			ASSERT_IS_NUMERIC(Num2)
		}

		// Convert to a point1.
		operator point1<Unit,Num>() const
		{
			return point1<Unit,Num>(v_);
		}
		
        // Get unmutable value (mutable getter not available).
        Num value() const { return v_; }

        // unsigned_azimuth += vect1 -> unsigned_azimuth
        unsigned_azimuth<Unit,Num> operator +=(vect1<Unit,Num> m)
		{
			v_ = normalize_(v_ + m.value());
			return *this;
		}

        // unsigned_azimuth -= vect1 -> unsigned_azimuth
        unsigned_azimuth<Unit,Num> operator -=(vect1<Unit,Num> m)
		{
			v_ = normalize_(v_ - m.value());
			return *this;
		}

    private:
        static Num normalize_(Num x)
        {
			return normalize_unsigned_azimuth(x, static_cast<Num>(Unit::turn_fraction()));
        }

        Num v_;
    };
	
	// unsigned_azimuth - unsigned_azimuth -> vect1
	// As a difference between two point angles it is meant the shortest
	// vect angle that if added to the second point angle gets the first one.
    template <class Unit, typename Num1, typename Num2>
	vect1<Unit,decltype(Num1()-Num2())> operator -(
		unsigned_azimuth<Unit,Num1> m1, unsigned_azimuth<Unit,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		auto difference = m1.value() - m2.value();
		auto one_turn = Unit::turn_fraction();
		if (difference * 2 < -one_turn) difference += one_turn;
		else if (difference * 2 >= one_turn) difference -= one_turn;
		return vect1<Unit,decltype(Num1()-Num2())>(difference);
	}

    // unsigned_azimuth == unsigned_azimuth -> bool
    template <class Unit, typename Num>
    bool operator ==(unsigned_azimuth<Unit,Num> m1, unsigned_azimuth<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() == m2.value();
    }

    // unsigned_azimuth != unsigned_azimuth -> bool
    template <class Unit, typename Num>
    bool operator !=(unsigned_azimuth<Unit,Num> m1, unsigned_azimuth<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
        return m1.value() != m2.value();
    }

    // angle_distance(unsigned_azimuth, unsigned_azimuth) -> vect1
    template <class Unit, typename Num>
    vect1<Unit,Num> angle_distance(unsigned_azimuth<Unit,Num> m1, unsigned_azimuth<Unit,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		Num value_distance(abs(m1.value() - m2.value()));
		if (value_distance * 2 > Unit::turn_fraction())
			value_distance = Unit::turn_fraction() - value_distance;
		return vect1<Unit,Num>(value_distance);
    }
	
    // is_equal(unsigned_azimuth, unsigned_azimuth, tolerance) -> bool
    template <class Unit, typename Num>
    bool is_equal(unsigned_azimuth<Unit,Num> m1, unsigned_azimuth<Unit,Num> m2, vect1<Unit,Num> tolerance)
    {
		ASSERT_IS_NUMERIC(Num)
        return angle_distance(m1, m2) <= tolerance;
    }
	
    // unsigned_azimuth + vect1 -> unsigned_azimuth
    template <class Unit, typename Num1, typename Num2>
    unsigned_azimuth<Unit,decltype(Num1()+Num2())> operator +(
		unsigned_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return unsigned_azimuth<Unit,decltype(Num1()+Num2())>(
			m1.value() + m2.value());
    }

    // unsigned_azimuth - vect1 -> unsigned_azimuth
    template <class Unit, typename Num1, typename Num2>
    unsigned_azimuth<Unit,decltype(Num1()-Num2())> operator -(
		unsigned_azimuth<Unit,Num1> m1, vect1<Unit,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return unsigned_azimuth<Unit,decltype(Num1()-Num2())>(
			m1.value() - m2.value());
    }
	
    // sin(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num sin(unsigned_azimuth<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return std::sin(convert<radians>(m).value());
    }

    // cos(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num cos(unsigned_azimuth<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return std::cos(convert<radians>(m).value());
    }

    // tan(unsigned_azimuth) -> N
    template <class Unit, typename Num>
    Num tan(unsigned_azimuth<Unit,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return std::tan(convert<radians>(m).value());
    }

    ///////////////////////// STATIC NUMERIC CASTS /////////////////////////
	
    template <typename ToNum, typename FromNum, class Unit>
    vect1<Unit,ToNum> cast(vect1<Unit,FromNum> m)
    {
        return vect1<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    point1<Unit,ToNum> cast(point1<Unit,FromNum> m)
    {
        return point1<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    vect2<Unit,ToNum> cast(vect2<Unit,FromNum> m)
    {
        return vect2<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
			static_cast<ToNum>(m.y().value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    point2<Unit,ToNum> cast(point2<Unit,FromNum> m)
    {
        return point2<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
			static_cast<ToNum>(m.y().value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    vect3<Unit,ToNum> cast(vect3<Unit,FromNum> m)
    {
        return vect3<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
			static_cast<ToNum>(m.y().value()),
			static_cast<ToNum>(m.z().value()));
    }
	
    template <typename ToNum, typename FromNum, class Unit>
    point3<Unit,ToNum> cast(point3<Unit,FromNum> m)
    {
        return point3<Unit,ToNum>(static_cast<ToNum>(m.x().value()),
			static_cast<ToNum>(m.y().value()),
			static_cast<ToNum>(m.z().value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    signed_azimuth<Unit,ToNum> cast(signed_azimuth<Unit,FromNum> m)
    {
        return signed_azimuth<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }

    template <typename ToNum, typename FromNum, class Unit>
    unsigned_azimuth<Unit,ToNum> cast(unsigned_azimuth<Unit,FromNum> m)
    {
        return unsigned_azimuth<Unit,ToNum>(static_cast<ToNum>(m.value()));
    }
}


//////////////////////////////// DYN_MEASURE ////////////////////////////////
namespace measures
{
    ///////////////////////// DYNAMIC UNIT SCALAR AND 1-DIMENSIONAL POINTS AND VECTORS /////////////////////////

    template <class Magnitude, typename Num = double>
    class dyn_vect1
    {
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_vect1():
            unit_(Magnitude::base_unit::id()), x_(Num()) { }

        // Construct using one number of the same number type,
		// specifying a unit of the same magnitude.
        explicit dyn_vect1(Magnitude unit, Num x):
            unit_(unit), x_(x) { }

        // Construct using another dyn_vect1 of the same number type
		// and magnitude. The unit is copied.
        dyn_vect1(const dyn_vect1<Magnitude,Num>& o):
            unit_(o.unit()), x_(o.value()) { }

        // Construct using a vect1 of the same number type
		// and magnitude. The unit is copied.
        template <class Unit>
        explicit dyn_vect1(const vect1<Unit,Num>& o):
            unit_(Unit::id()), x_(o.value())
		{
			assert_same_type(Magnitude(0), Unit::magnitude(0));
		}

        // Convert to a vect1 of the same magnitude.
        template <class Unit>
        operator vect1<Unit,Num>() const
		{		
			assert_same_type(Magnitude(0), Unit::magnitude(0));
            return vect1<Unit,Num>(Unit::id() == unit_ ? x_ :
				convert(Unit::id(), *this).value());
		}
		
        // Get the unit.
		Magnitude unit() const { return unit_; }

        // Get unmutable value.
        Num value() const { return x_; }

        // Get mutable value.
        Num& value() { return x_; }

        // +dyn_vect1 -> dyn_vect1
        dyn_vect1<Magnitude,Num> operator +() const
        {
            return *this;
        }

        // -dyn_vect1 -> dyn_vect1
        dyn_vect1<Magnitude,Num> operator -() const
        {
            return dyn_vect1<Magnitude,Num>(unit_, -x_);
        }

        // dyn_vect1 += dyn_vect1 -> dyn_vect1
        dyn_vect1<Magnitude,Num> operator +=(dyn_vect1<Magnitude,Num> m)
        {
			x_ += (unit_ == m.unit() ? m : convert(unit_, m)).value();
            return *this;
        }

        // dyn_vect1 -= dyn_vect1 -> dyn_vect1
        dyn_vect1<Magnitude,Num> operator -=(dyn_vect1<Magnitude,Num> m)
        {
			x_ -= (unit_ == m.unit() ? m : convert(unit_, m)).value();
            return *this;
        }

        // dyn_vect1 *= N -> dyn_vect1
        template <typename Num2>
        dyn_vect1<Magnitude,Num> operator *=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ *= static_cast<Num>(n);
            return *this;
        }

        // dyn_vect1 /= N -> dyn_vect1
        template <typename Num2>
        dyn_vect1<Magnitude,Num> operator /=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ /= static_cast<Num>(n);
            return *this;
        }
		
    private:
		Magnitude unit_;
        Num x_;
    };
	
    // dyn_vect1 == dyn_vect1 -> bool
    template <class Magnitude, typename Num>
    bool operator ==(dyn_vect1<Magnitude,Num> m1, dyn_vect1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.value() == m2.value();
    }

    // dyn_vect1 != dyn_vect1 -> bool
    template <class Magnitude, typename Num>
    bool operator !=(dyn_vect1<Magnitude,Num> m1, dyn_vect1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.value() != m2.value();
    }
	
    // dyn_vect1 < dyn_vect1 -> bool
    template <class Magnitude, typename Num>
    bool operator <(dyn_vect1<Magnitude,Num> m1, dyn_vect1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() < m2.value();
        return m1.value() < convert(m1.unit(), m2).value();
    }
	
    // dyn_vect1 <= dyn_vect1 -> bool
    template <class Magnitude, typename Num>
    bool operator <=(dyn_vect1<Magnitude,Num> m1, dyn_vect1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() <= m2.value();
        return m1.value() <= convert(m1.unit(), m2).value();
    }
	
    // dyn_vect1 > dyn_vect1 -> bool
    template <class Magnitude, typename Num>
    bool operator >(dyn_vect1<Magnitude,Num> m1, dyn_vect1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() > m2.value();
        return m1.value() > convert(m1.unit(), m2).value();
    }
	
    // dyn_vect1 >= dyn_vect1 -> bool
    template <class Magnitude, typename Num>
    bool operator >=(dyn_vect1<Magnitude,Num> m1, dyn_vect1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() >= m2.value();
        return m1.value() >= convert(m1.unit(), m2).value();
    }

    // is_equal(dyn_vect1, dyn_vect1, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_vect1<Magnitude,Num1> m1,
		dyn_vect1<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto val1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1)).value();
		auto val2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2)).value();
        return abs(val1 - val2) <= tolerance.value();
    }

    // is_less(dyn_vect1, dyn_vect1, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_less(dyn_vect1<Magnitude,Num1> m1, dyn_vect1<Magnitude,Num2> m2,
		dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto val1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1)).value();
		auto val2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2)).value();
		return static_cast<Num3>(val1)
			< static_cast<Num3>(val2 - tolerance.value());
    }

    // is_less_or_equal(dyn_vect1, dyn_vect1, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(dyn_vect1<Magnitude,Num1> m1,
		dyn_vect1<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto val1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1)).value();
		auto val2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2)).value();
        return static_cast<Num3>(val1)
			<= static_cast<Num3>(val2 + tolerance.value());
    }
	
    template <class Magnitude, typename Num = double>
    class dyn_point1
    {
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_point1():
            unit_(Magnitude::base_unit::id()), x_(Num()) { }

        // Construct using one number of the same number type,
		// specifying a unit of the same magnitude.
        explicit dyn_point1(Magnitude unit, Num x):
            unit_(unit), x_(x) { }
			
        // Construct using another dyn_point1 of the same number type
		// and magnitude. The unit is copied.
        dyn_point1(const dyn_point1<Magnitude,Num>& o):
            unit_(o.unit()), x_(o.value()) { }

        // Construct using a point1 of the same number type,
		// and the same magnitude.
		// The unit is copied.
        template <class Unit>
        explicit dyn_point1(const point1<Unit,Num>& o):
            unit_(Unit::id()), x_(o.value())
		{
			assert_same_type(Magnitude(0), Unit::magnitude(0));
		}

        // Convert to a point1 of the same magnitude.
        template <class Unit>
        operator point1<Unit,Num>() const
		{
			assert_same_type(Magnitude(0), Unit::magnitude(0));
            return point1<Unit,Num>(Unit::id() == unit_ ? x_ :
				convert(Unit::id(), *this).value());
		}
		
        // Get the unit.
		Magnitude unit() const { return unit_; }

        // Get unmutable value.
        Num value() const { return x_; }

        // Get mutable value.
        Num& value() { return x_; }

        // dyn_point1 += dyn_vect1 -> dyn_point1
        dyn_point1<Magnitude,Num> operator +=(dyn_vect1<Magnitude,Num> m)
        {
            x_ += (unit_ == m.unit() ? m : convert(unit_, m)).value();
            return *this;
        }

        // dyn_point1 -= dyn_vect1 -> dyn_point1
        dyn_point1<Magnitude,Num> operator -=(dyn_vect1<Magnitude,Num> m)
        {
            x_ -= (unit_ == m.unit() ? m : convert(unit_, m)).value();
            return *this;
        }

    private:
        Magnitude unit_;
        Num x_;
    };

	// midpoint(dyn_point1, dyn_point1, weight) -> dyn_point1
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
	dyn_point1<Magnitude,decltype((Num1()+Num2())*Num3())> midpoint(
		dyn_point1<Magnitude,Num1> m1, dyn_point1<Magnitude,Num2> m2,
		Num3 weight = 0.5f)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		//return p1 + (p2 - p1) * weight;

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point1<Magnitude,decltype((Num1()+Num2())*Num3())>(
				m1.unit(),
				m1.value() * (1 - weight) + m2.value() * weight);
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_point1<Magnitude,decltype((Num1()+Num2())*Num3())>(
			unit_id,
			convert(unit_id, m1).value() * (1 - weight)
			+ convert(unit_id, m2).value() * weight);
	}

	// barycentric_combination(int, dyn_point1[], Num[]) -> dyn_point1
    template <class Magnitude, typename Num>
	dyn_point1<Magnitude,Num> barycentric_combination(
		int n, dyn_point1<Magnitude,Num> p[], Num weights[])
	{
		ASSERT_IS_NUMERIC(Num)
		assert(n > 0);

		// If all points have the same unit, use it.
		// Otherwise use the magnitude base unit.
		auto unit_id = p[0].unit();
		Num result = Num();
		for (int i = 1; i < n; ++i)
		{
			if (p[i].unit() != unit_id)
			{
				// As units differ, convert all to base unit.
				unit_id = Magnitude::base_unit::id();
				for (int i = 0; i < n; ++i)
				{
					result += convert(unit_id, p[i]).value() * weights[i];
				}
				return dyn_point1<Magnitude,Num>(unit_id, result);
			}
		}
		
		// Use common unit.
		for (int i = 0; i < n; ++i)
		{
			result += p[i].value() * weights[i];
		}
		return dyn_point1<Magnitude,Num>(unit_id, result);		
	}

	// dyn_point1 - dyn_point1 -> dyn_vect1
	template <class Magnitude, typename Num1, typename Num2>
	dyn_vect1<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_point1<Magnitude,Num1> m1, dyn_point1<Magnitude,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect1<Magnitude,decltype(Num1()-Num2())>(m1.unit(),
				m1.value() - m2.value());
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_vect1<Magnitude,decltype(Num1()-Num2())>(unit_id,
			convert(unit_id, m1).value()
			- convert(unit_id, m2).value());
	}

    // dyn_point1 == dyn_point1 -> bool
    template <class Magnitude, typename Num>
    bool operator ==(dyn_point1<Magnitude,Num> m1, dyn_point1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.value() == m2.value();
    }

    // dyn_point1 != dyn_point1 -> bool
    template <class Magnitude, typename Num>
    bool operator !=(dyn_point1<Magnitude,Num> m1, dyn_point1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.value() != m2.value();
    }

    // dyn_point1 < dyn_point1 -> bool
    template <class Magnitude, typename Num>
    bool operator <(dyn_point1<Magnitude,Num> m1, dyn_point1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() < m2.value();
        return m1.value() < convert(m1.unit(), m2).value();
    }

    // dyn_point1 <= dyn_point1 -> bool
    template <class Magnitude, typename Num>
    bool operator <=(dyn_point1<Magnitude,Num> m1, dyn_point1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() <= m2.value();
        return m1.value() <= convert(m1.unit(), m2).value();
    }

    // dyn_point1 > dyn_point1 -> bool
    template <class Magnitude, typename Num>
    bool operator >(dyn_point1<Magnitude,Num> m1, dyn_point1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() > m2.value();
        return m1.value() > convert(m1.unit(), m2).value();
    }

    // dyn_point1 >= dyn_point1 -> bool
    template <class Magnitude, typename Num>
    bool operator >=(dyn_point1<Magnitude,Num> m1, dyn_point1<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() == m2.unit()) return m1.value() >= m2.value();
        return m1.value() >= convert(m1.unit(), m2).value();
    }

    // is_equal(dyn_point1, dyn_point1, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_point1<Magnitude,Num1> m1,
		dyn_point1<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto val1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1)).value();
		auto val2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2)).value();
        return abs(val1 - val2) <= tolerance.value();
    }

    // is_less(dyn_point1, dyn_point1, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_less(dyn_point1<Magnitude,Num1> m1,
		dyn_point1<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto val1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1)).value();
		auto val2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2)).value();
        return static_cast<Num3>(val1)
			< static_cast<Num3>(val2 - tolerance.value());
    }

    // is_less_or_equal(dyn_point1, dyn_point1, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_less_or_equal(dyn_point1<Magnitude,Num1> m1,
		dyn_point1<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto val1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1)).value();
		auto val2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2)).value();
        return static_cast<Num3>(val1)
			<= static_cast<Num3>(val2 + tolerance.value());
    }
	
    // dyn_point1 + dyn_vect1 -> dyn_point1
    template <class Magnitude, typename Num1, typename Num2>
    dyn_point1<Magnitude,decltype(Num1()+Num2())> operator +(
		dyn_point1<Magnitude,Num1> m1, dyn_vect1<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point1<Magnitude,decltype(Num1()+Num2())>(
				m1.unit(), m1.value() + m2.value());
		}
		auto unit_id = Magnitude::base_unit::id();
        return dyn_point1<Magnitude,decltype(Num1()+Num2())>(
			unit_id,
			convert(unit_id, m1).value() + convert(unit_id, m2).value());
    }

    // dyn_point1 - dyn_vect1 -> dyn_point1
    template <class Magnitude, typename Num1, typename Num2>
    dyn_point1<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_point1<Magnitude,Num1> m1, dyn_vect1<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point1<Magnitude,decltype(Num1()-Num2())>(
				m1.unit(), m1.value() - m2.value());
		}
		auto unit_id = Magnitude::base_unit::id();
        return dyn_point1<Magnitude,decltype(Num1()-Num2())>(
			unit_id,
			convert(unit_id, m1).value() - convert(unit_id, m2).value());
    }

    // dyn_vect1 + dyn_vect1 -> dyn_vect1
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect1<Magnitude,decltype(Num1()+Num2())> operator +(
		dyn_vect1<Magnitude,Num1> m1, dyn_vect1<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect1<Magnitude,decltype(Num1()+Num2())>(
				m1.unit(), m1.value() + m2.value());
		}
		auto unit_id = Magnitude::base_unit::id();
        return dyn_vect1<Magnitude,decltype(Num1()+Num2())>(
			unit_id,
			convert(unit_id, m1).value() + convert(unit_id, m2).value());
    }

    // dyn_vect1 - dyn_vect1 -> dyn_vect1
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect1<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_vect1<Magnitude,Num1> m1, dyn_vect1<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect1<Magnitude,decltype(Num1()-Num2())>(
				m1.unit(), m1.value() - m2.value());
		}
		auto unit_id = Magnitude::base_unit::id();
        return dyn_vect1<Magnitude,decltype(Num1()-Num2())>(
			unit_id, convert(unit_id, m1).value() - convert(unit_id, m2).value());
    }

    // N * dyn_vect1 -> dyn_vect1
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect1<Magnitude,decltype(Num1()*Num2())> operator *(
		Num1 n, dyn_vect1<Magnitude,Num2> m)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return dyn_vect1<Magnitude,decltype(Num1()*Num2())>(
			m.unit(), n * m.value());
    }

    // dyn_vect1 * N -> dyn_vect1
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect1<Magnitude,decltype(Num1()*Num2())> operator *(
		dyn_vect1<Magnitude,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return dyn_vect1<Magnitude,decltype(Num1()*Num2())>(
			m.unit(), m.value() * n);
    }

    // dyn_vect1 / N -> dyn_vect1
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect1<Magnitude,decltype(Num1()/Num2())> operator /(
		dyn_vect1<Magnitude,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return dyn_vect1<Magnitude,decltype(Num1()/Num2())>(
			m.unit(), m.value() / n);
    }
	
	// dyn_vect1 / dyn_vect1 -> N
    template <class Magnitude, typename Num1, typename Num2>
    decltype(Num1()/Num2()) operator /(dyn_vect1<Magnitude,Num1> m1, dyn_vect1<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return m1.value() / m2.value();
    }

    template <class Magnitude, typename Num>
    Num squared_norm_value(dyn_vect1<Magnitude,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return sqr(v.value());
    }

    template <class Magnitude, typename Num>
    dyn_vect1<Magnitude,Num> norm(dyn_vect1<Magnitude,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_vect1<Magnitude,Num>(v.unit(), abs(v.value()));
    }

    ///////////////////////// DYNAMIC 2-DIMENSIONAL POINTS AND VECTORS /////////////////////////

    template <class Magnitude, typename Num = double>
    class dyn_vect2
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_vect2():
            unit_(Magnitude::base_unit::id()), x_(Num()), y_(Num()) { }

        // Construct using one number of the same number type,
		// specifying a unit of the same magnitude.
        explicit dyn_vect2(Magnitude unit, Num x, Num y):
            unit_(unit), x_(x), y_(y) { }
		
        // Construct using two dyn_vect1s of the same number types.
		// The unit is copied from the first argument.
        explicit dyn_vect2(const dyn_vect1<Magnitude,Num> x,
			const dyn_vect1<Magnitude,Num> y):
            unit_(x.unit()),
            x_(x.value()),
            y_(convert(x.unit(), y).value()) { }

        // Construct using another dyn_vect2 of the same number type
		// and magnitude. The unit is copied.
        dyn_vect2(const dyn_vect2<Magnitude,Num>& o):
            unit_(o.unit()), x_(o.x().value()), y_(o.y().value()) { }
		
        // Construct using a vect2,
        // possibly of a different number type,
		// but of the same magnitude.
		// The unit is copied.
        template <class Unit, typename Num2>
        explicit dyn_vect2(const vect2<Unit,Num2>& o):
            unit_(Unit::id()),
            x_(static_cast<Num>(o.x().value())),
            y_(static_cast<Num>(o.y().value()))
		{
			ASSERT_IS_NUMERIC(Num2)
			assert_same_type(Magnitude(0), Unit::magnitude(0));
		}
		
        // Construct a versor using a point1 representing an angle.
        template <class Unit2, typename Num2>
        explicit dyn_vect2(Magnitude unit,
			const point1<Unit2,Num2>& a):
			unit_(unit),
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a)))
        {
			ASSERT_IS_NUMERIC(Num2)
            ASSERT_IS_ANGLE(Unit2)
        }
		
        // Construct a versor using a dyn_signed_azimuth.
        template <class Unit2, typename Num2>
        explicit dyn_vect2(Magnitude unit,
			const signed_azimuth<Unit2,Num2>& a):
			unit_(unit),
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a)))
		{
			ASSERT_IS_NUMERIC(Num2)
		}

        // Construct a versor using a dyn_unsigned_azimuth.
        template <class Unit2, typename Num2>
        explicit dyn_vect2(Magnitude unit,
			const unsigned_azimuth<Unit2,Num2>& a):
			unit_(unit),
            x_(static_cast<Num>(cos(a))),
            y_(static_cast<Num>(sin(a)))
		{
			ASSERT_IS_NUMERIC(Num2)
		}

        // Convert to a vect2 of the same magnitude.
        template <class Unit>
        operator vect2<Unit,Num>() const
		{
			assert_same_type(Magnitude(0), Unit::magnitude(0));
			if (Unit::id() == unit_) return vect2<Unit,Num>(x_, y_);
			auto m = convert(Unit::id(), *this);
            return vect2<Unit,Num>(m.x().value(), m.y().value());
		}

        // +dyn_vect2 -> dyn_vect2
        dyn_vect2<Magnitude,Num> operator +() const
        {
            return *this;
        }

        // -dyn_vect2 -> dyn_vect2
        dyn_vect2<Magnitude,Num> operator -() const
        {
            return dyn_vect2<Magnitude,Num>(unit_, -x_, -y_);
        }
		
        // dyn_vect2 += dyn_vect2 -> dyn_vect2
        dyn_vect2<Magnitude,Num> operator +=(dyn_vect2<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ += mm.x().value();
            y_ += mm.y().value();
            return *this;
        }

        // dyn_vect2 -= dyn_vect2 -> dyn_vect2
        dyn_vect2<Magnitude,Num> operator -=(dyn_vect2<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ -= mm.x().value();
            y_ -= mm.y().value();
            return *this;
        }

        // dyn_vect2 *= N -> dyn_vect2
        template <typename Num2>
        dyn_vect2<Magnitude,Num> operator *=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ *= static_cast<Num>(n);
            y_ *= static_cast<Num>(n);
            return *this;
        }

        // dyn_vect2 /= N -> dyn_vect2
        template <typename Num2>
        dyn_vect2<Magnitude,Num> operator /=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ /= static_cast<Num>(n);
            y_ /= static_cast<Num>(n);
            return *this;
        }
		
        // Get the unit.
		Magnitude unit() const { return unit_; }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component (mutable getter not available).
        dyn_vect1<Magnitude,Num> const x() const
		{
			return dyn_vect1<Magnitude,Num>(unit_, x_);
		}

        // Get unmutable y component (mutable getter not available).
        dyn_vect1<Magnitude,Num> const y() const
		{
			return dyn_vect1<Magnitude,Num>(unit_, y_);
		}

        // Set x component.
        template <typename Num2>
        void x(const dyn_vect1<Magnitude,Num2> new_x)
		{
			ASSERT_IS_NUMERIC(Num2)
			x_ = (unit_ == new_x.unit() ? new_x :
				convert(unit_, new_x)).value();
        }

        // Set y component.
        template <typename Num2>
        void y(const dyn_vect1<Magnitude,Num2> new_y)
        {
			ASSERT_IS_NUMERIC(Num2)
			y_ = (unit_ == new_y.unit() ? new_y :
				convert(unit_, new_y)).value();
        }
		
    private:
		Magnitude unit_;

        // Components.
        Num x_, y_;
    };
	
    // dyn_vect2 == dyn_vect2 -> bool
    template <class Magnitude, typename Num>
    bool operator ==(dyn_vect2<Magnitude,Num> m1, dyn_vect2<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // dyn_vect2 != dyn_vect2 -> bool
    template <class Magnitude, typename Num>
    bool operator !=(dyn_vect2<Magnitude,Num> m1, dyn_vect2<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }

    // is_equal(dyn_vect2, dyn_vect2, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_vect2<Magnitude,Num1> m1,
		dyn_vect2<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto mm1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1));
		auto mm2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2));
        return squared_norm_value(mm1 - mm2) <= squared_norm_value(tolerance);
    }
	
    template <class Magnitude, typename Num = double>
    class dyn_point2
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_point2():
            unit_(Magnitude::base_unit::id()), x_(Num()), y_(Num()) { }

        // Construct using one number of the same number type,
		// specifying a unit of the same magnitude.
        explicit dyn_point2(Magnitude unit, Num x, Num y):
            unit_(unit), x_(x), y_(y) { }

        // Construct using two dyn_point1s of the same number types.
		// The unit is copied from the first argument.
        explicit dyn_point2(const dyn_point1<Magnitude,Num> x,
			const dyn_point1<Magnitude,Num> y):
            unit_(x.unit()),
            x_(x.value()),
            y_(convert(x.unit(), y).value()) { }

        // Construct using another dyn_point2 of the same number type
		// and magnitude. The unit is copied.
        dyn_point2(const dyn_point2<Magnitude,Num>& o):
            unit_(o.unit()), x_(o.x().value()), y_(o.y().value()) { }

        // Construct using a point2,
        // possibly of a different number type,
		// but of the same magnitude.
		// The unit is copied.
        template <class Unit, typename Num2>
        explicit dyn_point2(const point2<Unit,Num2>& o):
            unit_(Unit::id()),
            x_(static_cast<Num>(o.x().value())),
            y_(static_cast<Num>(o.y().value()))
		{
			ASSERT_IS_NUMERIC(Num2)
			assert_same_type(Magnitude(0), Unit::magnitude(0));
		}

        // Convert to a point2.
        template <class Unit>
        operator point2<Unit,Num>() const
		{		
			assert_same_type(Magnitude(0), Unit::magnitude(0));
			if (Unit::id() == unit_) return point2<Unit,Num>(x_, y_);
			auto m = convert(Unit::id(), *this);
            return point2<Unit,Num>(m.x().value(), m.y().value());
		}
		
        // dyn_point2 += dyn_vect2 -> dyn_point2
        dyn_point2<Magnitude,Num> operator +=(dyn_vect2<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ += mm.x().value();
            y_ += mm.y().value();
            return *this;
        }

        // dyn_point2 -= dyn_vect2 -> dyn_point2
        dyn_point2<Magnitude,Num> operator -=(dyn_vect2<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ -= mm.x().value();
            y_ -= mm.y().value();
            return *this;
        }

        // Get the unit.
		Magnitude unit() const { return unit_; }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component (mutable getter not available).
        dyn_point1<Magnitude,Num> const x() const
		{
			return dyn_point1<Magnitude,Num>(unit_, x_);
		}

        // Get unmutable y component (mutable getter not available).
        dyn_point1<Magnitude,Num> const y() const
		{
			return dyn_point1<Magnitude,Num>(unit_, y_);
		}

        // Set x component.
        template <typename Num2>
        void x(const dyn_point1<Magnitude,Num2> new_x)
		{
			ASSERT_IS_NUMERIC(Num2)
			x_ = (unit_ == new_x.unit() ? new_x :
				convert(unit_, new_x)).value();
        }

        // Set y component.
        template <typename Num2>
        void y(const dyn_point1<Magnitude,Num2> new_y)
        {
			ASSERT_IS_NUMERIC(Num2)
			y_ = (unit_ == new_y.unit() ? new_y :
				convert(unit_, new_y)).value();
        }

    private:
		Magnitude unit_;

        // Components.
        Num x_, y_;
    };

	// midpoint(dyn_point2, dyn_point2, weight) -> dyn_point2
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
	dyn_point2<Magnitude,decltype((Num1()+Num2())*Num3())> midpoint(
		dyn_point2<Magnitude,Num1> m1, dyn_point2<Magnitude,Num2> m2,
		Num3 weight = 0.5f)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		//return p1 + (p2 - p1) * weight;

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point2<Magnitude,decltype((Num1()+Num2())*Num3())>(
				m1.unit(),
				m1.x().value() * (1 - weight) + m2.x().value() * weight,
				m1.y().value() * (1 - weight) + m2.y().value() * weight);
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_point2<Magnitude,decltype((Num1()+Num2())*Num3())>(
			unit_id,
			convert(unit_id, m1.x()).value() * (1 - weight)
				+ convert(unit_id, m2.x()).value() * weight,
			convert(unit_id, m1.y()).value() * (1 - weight)
				+ convert(unit_id, m2.y()).value() * weight);
	}

	// barycentric_combination(int, dyn_point2[], Num[]) -> dyn_point2
    template <class Magnitude, typename Num>
	dyn_point2<Magnitude,Num> barycentric_combination(
		int n, dyn_point2<Magnitude,Num> p[], Num weights[])
	{
		ASSERT_IS_NUMERIC(Num)
		assert(n > 0);

		// If all points have the same unit, use it.
		// Otherwise use the magnitude base unit.
		auto unit_id = p[0].unit();
		Num resultx = Num();
		Num resulty = Num();
		for (int i = 1; i < n; ++i)
		{
			if (p[i].unit() != unit_id)
			{
				// As units differ, convert all to base unit.
				unit_id = Magnitude::base_unit::id();
				for (int i = 0; i < n; ++i)
				{
					resultx += convert(unit_id, p[i].x()).value() * weights[i];
					resulty += convert(unit_id, p[i].y()).value() * weights[i];
				}
				return dyn_point2<Magnitude,Num>(unit_id, resultx, resulty);
			}
		}
		
		// Use common unit.
		for (int i = 0; i < n; ++i)
		{
			resultx += p[i].x().value() * weights[i];
			resulty += p[i].y().value() * weights[i];
		}
		return dyn_point2<Magnitude,Num>(unit_id, resultx, resulty);		
	}

	// dyn_point2 - dyn_point2 -> dyn_vect2
	template <class Magnitude, typename Num1, typename Num2>
	dyn_vect2<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_point2<Magnitude,Num1> m1, dyn_point2<Magnitude,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		
		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect2<Magnitude,decltype(Num1()-Num2())>(m1.unit(),
				m1.x().value() - m2.x().value(),
				m1.y().value() - m2.y().value());
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_vect2<Magnitude,decltype(Num1()-Num2())>(unit_id,
			convert(unit_id, m1.x()).value() - convert(unit_id, m2.x()).value(),
			convert(unit_id, m1.y()).value() - convert(unit_id, m2.y()).value());
	}

    // dyn_point2 == dyn_point2 -> bool
    template <class Magnitude, typename Num>
    bool operator ==(dyn_point2<Magnitude,Num> m1, dyn_point2<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value();
    }

    // dyn_point2 != dyn_point2 -> bool
    template <class Magnitude, typename Num>
    bool operator !=(dyn_point2<Magnitude,Num> m1, dyn_point2<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value();
    }

    // is_equal(dyn_point2, dyn_point2, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_point2<Magnitude,Num1> m1,
		dyn_point2<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto mm1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1));
		auto mm2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2));
        return squared_norm_value(mm1 - mm2) <= squared_norm_value(tolerance);
    }
	
    // dyn_point2 + dyn_vect2 -> dyn_point2
    template <class Magnitude, typename Num1, typename Num2>
    dyn_point2<Magnitude,decltype(Num1()+Num2())> operator +(
		dyn_point2<Magnitude,Num1> m1, dyn_vect2<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point2<Magnitude,decltype(Num1()+Num2())>(
				m1.unit(),
				m1.x().value() + m2.x().value(),
				m1.y().value() + m2.y().value());
		}
		Magnitude unit_id = Magnitude::base_unit::id();
        return dyn_point2<Magnitude,decltype(Num1()+Num2())>(
			unit_id,
			convert(unit_id, m1).x().value() + convert(unit_id, m2).x().value(),
			convert(unit_id, m1).y().value() + convert(unit_id, m2).y().value());
    }

    // dyn_point2 - dyn_vect2 -> dyn_point2
    template <class Magnitude, typename Num1, typename Num2>
    dyn_point2<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_point2<Magnitude,Num1> m1, dyn_vect2<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point2<Magnitude,decltype(Num1()-Num2())>(m1.unit(),
				m1.x().value() - m2.x().value(),
				m1.y().value() - m2.y().value());
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_point2<Magnitude,decltype(Num1()-Num2())>(unit_id,
			convert(unit_id, m1.x()).value() - convert(unit_id, m2.x()).value(),
			convert(unit_id, m1.y()).value() - convert(unit_id, m2.y()).value());
    }

    // dyn_vect2 + dyn_vect2 -> dyn_vect2
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect2<Magnitude,decltype(Num1()+Num2())> operator +(
		dyn_vect2<Magnitude,Num1> m1, dyn_vect2<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect2<Magnitude,decltype(Num1()+Num2())>(
				m1.unit(),
				m1.x().value() + m2.x().value(),
				m1.y().value() + m2.y().value());
		}
		Magnitude unit_id = Magnitude::base_unit::id();
        return dyn_vect2<Magnitude,decltype(Num1()+Num2())>(
			unit_id,
			convert(unit_id, m1).x().value() + convert(unit_id, m2).x().value(),
			convert(unit_id, m1).y().value() + convert(unit_id, m2).y().value());
    }

    // dyn_vect2 - dyn_vect2 -> dyn_vect2
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect2<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_vect2<Magnitude,Num1> m1, dyn_vect2<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect2<Magnitude,decltype(Num1()-Num2())>(
				m1.unit(),
				m1.x().value() - m2.x().value(),
				m1.y().value() - m2.y().value());
		}
		auto unit_id = Magnitude::base_unit::id();
        return dyn_vect2<Magnitude,decltype(Num1()-Num2())>(
			unit_id,
			convert(unit_id, m1).x().value() - convert(unit_id, m2).x().value(),
			convert(unit_id, m1).y().value() - convert(unit_id, m2).y().value());
    }

    // N * dyn_vect2 -> dyn_vect2
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect2<Magnitude,decltype(Num1()*Num2())> operator *(
		Num1 n, dyn_vect2<Magnitude,Num2> m)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

        return dyn_vect2<Magnitude,decltype(Num1()*Num2())>(m.unit(),
			n * m.x().value(),
			n * m.y().value());
    }

    // dyn_vect2 * N -> dyn_vect2
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect2<Magnitude,decltype(Num1()*Num2())> operator *(
		dyn_vect2<Magnitude,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

        return dyn_vect2<Magnitude,decltype(Num1()*Num2())>(m.unit(),
			m.x().value() * n,
			m.y().value() * n);
    }

    // dyn_vect2 / N -> dyn_vect2
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect2<Magnitude,decltype(Num1()/Num2())> operator /(
		dyn_vect2<Magnitude,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return dyn_vect2<Magnitude,decltype(Num1()/Num2())>(m.unit(),
			m.x().value() / n,
			m.y().value() / n);
    }

	
    template <class Magnitude, typename Num>
    Num squared_norm_value(dyn_vect2<Magnitude,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return sqr(v.x().value()) + sqr(v.y().value());
    }

    template <class Magnitude, typename Num>
    dyn_vect1<Magnitude,Num> norm(dyn_vect2<Magnitude,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_vect1<Magnitude,Num>(v.unit(),
			static_cast<Num>(sqrt(squared_norm_value(v))));
    }

	
    ///////////////////////// DYNAMIC 3-DIMENSIONAL POINTS AND VECTORS /////////////////////////

    template <class Magnitude, typename Num = double>
    class dyn_vect3
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_vect3():
            unit_(Magnitude::base_unit::id()),
				x_(Num()), y_(Num()), z_(Num()) { }

        // Construct using one number of the same number type,
		// specifying a unit of the same magnitude.
        explicit dyn_vect3(Magnitude unit, Num x, Num y, Num z):
            unit_(unit), x_(x), y_(y), z_(z) { }
			
        // Construct using three dyn_vect1s of the same magnitude
		// and number types.
		// The unit is copied from the first argument.
        explicit dyn_vect3(const dyn_vect1<Magnitude,Num> x,
			const dyn_vect1<Magnitude,Num> y,
			const dyn_vect1<Magnitude,Num> z):
            unit_(x.unit()),
            x_(x.value()),
            y_(convert(x.unit(), y).value()),
            z_(convert(x.unit(), z).value()) { }

        // Construct using another dyn_vect3 of the same number type
		// and magnitude. The unit is copied.
        dyn_vect3(const dyn_vect3<Magnitude,Num>& o):
            unit_(o.unit()), x_(o.x().value()), y_(o.y().value()),
			z_(o.z().value()) { }

        // Construct using a vect3,
        // possibly of a different number type,
		// but of the same magnitude.
		// The unit is copied.
        template <class Unit, typename Num2>
        explicit dyn_vect3(const vect3<Unit,Num2>& o):
            unit_(Unit::id()),
            x_(static_cast<Num>(o.x().value())),
            y_(static_cast<Num>(o.y().value())),
            z_(static_cast<Num>(o.z().value()))
		{
			ASSERT_IS_NUMERIC(Num2)
			assert_same_type(Magnitude(0), Unit::magnitude(0));
		}

        // Convert to a vect3.
        template <class Unit>
        operator vect3<Unit,Num>() const
		{
			assert_same_type(Magnitude(0), Unit::magnitude(0));
			if (Unit::id() == unit_) return vect2<Unit,Num>(x_, y_, z_);
			auto m = convert(Unit::id(), *this);
            return vect2<Unit,Num>(m.x().value(), m.y().value(),
				m.z().value());
		}
		
        // +dyn_vect3 -> dyn_vect3
        dyn_vect3<Magnitude,Num> operator +() const
        {
            return *this;
        }

        // -dyn_vect3 -> dyn_vect3
        dyn_vect3<Magnitude,Num> operator -() const
        {
            return dyn_vect3<Magnitude,Num>(unit_, -x_, -y_, -z_);
        }
		
        // dyn_vect3 += dyn_vect3 -> dyn_vect3
        dyn_vect3<Magnitude,Num> operator +=(dyn_vect3<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ += mm.x().value();
            y_ += mm.y().value();
            z_ += mm.z().value();
            return *this;
        }

        // dyn_vect3 -= dyn_vect3 -> dyn_vect3
        dyn_vect3<Magnitude,Num> operator -=(dyn_vect3<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ -= mm.x().value();
            y_ -= mm.y().value();
            z_ -= mm.z().value();
            return *this;
        }

        // dyn_vect3 *= N -> dyn_vect3
        template <typename Num2>
        dyn_vect3<Magnitude,Num> operator *=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ *= static_cast<Num>(n);
            y_ *= static_cast<Num>(n);
            z_ *= static_cast<Num>(n);
            return *this;
        }

        // dyn_vect3 /= N -> dyn_vect3
        template <typename Num2>
        dyn_vect3<Magnitude,Num> operator /=(Num2 n)
        {
			ASSERT_IS_NUMERIC(Num2)
            x_ /= static_cast<Num>(n);
            y_ /= static_cast<Num>(n);
            z_ /= static_cast<Num>(n);
            return *this;
        }

        // Get the unit.
		Magnitude unit() const { return unit_; }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component (mutable getter not available).
        dyn_vect1<Magnitude,Num> const x() const
		{
			return dyn_vect1<Magnitude,Num>(unit_, x_);
		}

        // Get unmutable y component (mutable getter not available).
        dyn_vect1<Magnitude,Num> const y() const
		{
			return dyn_vect1<Magnitude,Num>(unit_, y_);
		}

        // Get unmutable z component (mutable getter not available).
        dyn_vect1<Magnitude,Num> const z() const
		{
			return dyn_vect1<Magnitude,Num>(unit_, z_);
		}

        // Set x component.
        template <typename Num2>
        void x(const dyn_vect1<Magnitude,Num2> new_x)
		{
			ASSERT_IS_NUMERIC(Num2)
			x_ = (unit_ == new_x.unit() ? new_x :
				convert(unit_, new_x)).value();
        }

        // Set y component.
        template <typename Num2>
        void y(const dyn_vect1<Magnitude,Num2> new_y)
        {
			ASSERT_IS_NUMERIC(Num2)
			y_ = (unit_ == new_y.unit() ? new_y :
				convert(unit_, new_y)).value();
        }

        // Set z component.
        template <typename Num2>
        void z(const dyn_vect1<Magnitude,Num2> new_z)
        {
			ASSERT_IS_NUMERIC(Num2)
			z_ = (unit_ == new_z.unit() ? new_z :
				convert(unit_, new_z)).value();
        }

    private:
		Magnitude unit_;

        // Components.
        Num x_, y_, z_;
    };

    // dyn_vect3 == dyn_vect3 -> bool
    template <class Magnitude, typename Num>
    bool operator ==(dyn_vect3<Magnitude,Num> m1, dyn_vect3<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // dyn_vect3 != dyn_vect3 -> bool
    template <class Magnitude, typename Num>
    bool operator !=(dyn_vect3<Magnitude,Num> m1, dyn_vect3<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value()
            || m1.z().value() != m2.z().value();
    }

    // is_equal(dyn_vect3, dyn_vect3, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_vect3<Magnitude,Num1> m1,
		dyn_vect3<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto mm1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1));
		auto mm2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2));
        return squared_norm_value(mm1 - mm2) <= squared_norm_value(tolerance);
    }
	
    template <class Magnitude, typename Num = double>
    class dyn_point3
	{
	ASSERT_IS_NUMERIC(Num)
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_point3():
            unit_(Magnitude::base_unit::id()),
				x_(Num()), y_(Num()), z_(Num()) { }

        // Construct using one number of the same number type,
		// specifying a unit of the same magnitude.
        explicit dyn_point3(Magnitude unit, Num x, Num y, Num z):
            unit_(unit), x_(x), y_(y), z_(z) { }
		
        // Construct using three dyn_point1s of the same magnitude
		// and number types.
		// The unit is copied from the first argument.
        explicit dyn_point3(const dyn_point1<Magnitude,Num> x,
			const dyn_point1<Magnitude,Num> y,
			const dyn_point1<Magnitude,Num> z):
            unit_(x.unit()),
            x_(x.value()),
            y_(convert(x.unit(), y).value()),
            z_(convert(x.unit(), z).value()) { }

        // Construct using another dyn_point3 of the same number type
		// and magnitude. The unit is copied.
        dyn_point3(const dyn_point3<Magnitude,Num>& o):
            unit_(o.unit()), x_(o.x().value()), y_(o.y().value()),
			z_(o.z().value()) { }

        // Construct using a point3,
        // possibly of a different number type,
		// but of the same magnitude.
		// The unit is copied.
        template <class Unit, typename Num2>
        explicit dyn_point3(const point3<Unit,Num2>& o):
            unit_(Unit::id()),
            x_(static_cast<Num>(o.x().value())),
            y_(static_cast<Num>(o.y().value())),
            z_(static_cast<Num>(o.z().value()))
		{
			ASSERT_IS_NUMERIC(Num2)
			assert_same_type(Magnitude(0), Unit::magnitude(0));
		}
		
        // Convert to a point3.
        template <class Unit>
        operator point3<Unit,Num>() const
		{		
			assert_same_type(Magnitude(0), Unit::magnitude(0));
			if (Unit::id() == unit_) return point2<Unit,Num>(x_, y_, z_);
			auto m = convert(Unit::id(), *this);
            return point2<Unit,Num>(m.x().value(), m.y().value(),
				m.z().value());
		}
			
        // dyn_point3 += dyn_vect3 -> dyn_point3
        dyn_point3<Magnitude,Num> operator +=(dyn_vect3<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ += mm.x().value();
            y_ += mm.y().value();
            z_ += mm.z().value();
            return *this;
        }

        // dyn_point3 -= dyn_vect3 -> dyn_point3
        dyn_point3<Magnitude,Num> operator -=(dyn_vect3<Magnitude,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
            x_ -= mm.x().value();
            y_ -= mm.y().value();
            z_ -= mm.z().value();
            return *this;
        }

        // Get the unit.
		Magnitude unit() const { return unit_; }

        // Get unmutable component array.
        Num const* data() const { return &x_; }

        // Get mutable component array.
        Num* data() { return &x_; }
		
        // Get unmutable x component (mutable getter not available).
        dyn_point1<Magnitude,Num> const x() const
		{
			return dyn_point1<Magnitude,Num>(unit_, x_);
		}

        // Get unmutable y component (mutable getter not available).
        dyn_point1<Magnitude,Num> const y() const
		{
			return dyn_point1<Magnitude,Num>(unit_, y_);
		}

        // Get unmutable z component (mutable getter not available).
        dyn_point1<Magnitude,Num> const z() const
		{
			return dyn_point1<Magnitude,Num>(unit_, z_);
		}

        // Set x component.
        template <typename Num2>
        void x(const dyn_point1<Magnitude,Num2> new_x)
		{
			ASSERT_IS_NUMERIC(Num2)
			x_ = (unit_ == new_x.unit() ? new_x :
				convert(unit_, new_x)).value();
        }

        // Set y component.
        template <typename Num2>
        void y(const dyn_point1<Magnitude,Num2> new_y)
        {
			ASSERT_IS_NUMERIC(Num2)
			y_ = (unit_ == new_y.unit() ? new_y :
				convert(unit_, new_y)).value();
        }

        // Set z component.
        template <typename Num2>
        void z(const dyn_point1<Magnitude,Num2> new_z)
        {
			ASSERT_IS_NUMERIC(Num2)
			z_ = (unit_ == new_z.unit() ? new_z :
				convert(unit_, new_z)).value();
        }

    private:
		Magnitude unit_;

        // Components.
        Num x_, y_, z_;
    };

	// midpoint(dyn_point3, dyn_point3, weight) -> dyn_point3
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
	dyn_point3<Magnitude,decltype((Num1()+Num2())*Num3())> midpoint(
		dyn_point3<Magnitude,Num1> m1, dyn_point3<Magnitude,Num2> m2,
		Num3 weight = 0.5f)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		//return p1 + (p2 - p1) * weight;

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point3<Magnitude,decltype((Num1()+Num2())*Num3())>(
				m1.unit(),
				m1.x().value() * (1 - weight) + m2.x().value() * weight,
				m1.y().value() * (1 - weight) + m2.y().value() * weight,
				m1.z().value() * (1 - weight) + m2.z().value() * weight);
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_point3<Magnitude,decltype((Num1()+Num2())*Num3())>(
			unit_id,
			convert(unit_id, m1.x()).value() * (1 - weight)
				+ convert(unit_id, m2.x()).value() * weight,
			convert(unit_id, m1.y()).value() * (1 - weight)
				+ convert(unit_id, m2.y()).value() * weight,
			convert(unit_id, m1.z()).value() * (1 - weight)
				+ convert(unit_id, m2.z()).value() * weight);
	}

	// barycentric_combination(int, dyn_point3[], Num[]) -> dyn_point3
    template <class Magnitude, typename Num>
	dyn_point3<Magnitude,Num> barycentric_combination(
		int n, dyn_point3<Magnitude,Num> p[], Num weights[])
	{
		ASSERT_IS_NUMERIC(Num)
		assert(n > 0);

		// If all points have the same unit, use it.
		// Otherwise use the magnitude base unit.
		auto unit_id = p[0].unit();
		Num resultx = Num();
		Num resulty = Num();
		Num resultz = Num();
		for (int i = 1; i < n; ++i)
		{
			if (p[i].unit() != unit_id)
			{
				// As units differ, convert all to base unit.
				unit_id = Magnitude::base_unit::id();
				for (int i = 0; i < n; ++i)
				{
					resultx += convert(unit_id, p[i].x()).value() * weights[i];
					resulty += convert(unit_id, p[i].y()).value() * weights[i];
					resultz += convert(unit_id, p[i].z()).value() * weights[i];
				}
				return dyn_point3<Magnitude,Num>(unit_id,
					resultx, resulty, resultz);
			}
		}
		
		// Use common unit.
		for (int i = 0; i < n; ++i)
		{
			resultx += p[i].x().value() * weights[i];
			resulty += p[i].y().value() * weights[i];
			resultz += p[i].z().value() * weights[i];
		}
		return dyn_point3<Magnitude,Num>(unit_id, resultx, resulty, resultz);		
	}

	// dyn_point3 - dyn_point3 -> dyn_vect3
	template <class Magnitude, typename Num1, typename Num2>
	dyn_vect3<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_point3<Magnitude,Num1> m1, dyn_point3<Magnitude,Num2> m2)
	{
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect3<Magnitude,decltype(Num1()-Num2())>(m1.unit(),
				m1.x().value() - m2.x().value(),
				m1.y().value() - m2.y().value(),
				m1.z().value() - m2.z().value());
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_vect3<Magnitude,decltype(Num1()-Num2())>(unit_id,
			convert(unit_id, m1.x()).value() - convert(unit_id, m2.x()).value(),
			convert(unit_id, m1.y()).value() - convert(unit_id, m2.y()).value(),
			convert(unit_id, m1.z()).value() - convert(unit_id, m2.z()).value());
	}

	
    // dyn_point3 == dyn_point3 -> bool
    template <class Magnitude, typename Num>
    bool operator ==(dyn_point3<Magnitude,Num> m1, dyn_point3<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.x().value() == m2.x().value()
            && m1.y().value() == m2.y().value()
            && m1.z().value() == m2.z().value();
    }

    // dyn_point3 != dyn_point3 -> bool
    template <class Magnitude, typename Num>
    bool operator !=(dyn_point3<Magnitude,Num> m1, dyn_point3<Magnitude,Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.x().value() != m2.x().value()
            || m1.y().value() != m2.y().value()
            || m1.z().value() != m2.z().value();
    }

    // is_equal(dyn_point3, dyn_point3, tolerance) -> bool
    template <class Magnitude, typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_point3<Magnitude,Num1> m1,
		dyn_point3<Magnitude,Num2> m2, dyn_vect1<Magnitude,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
		auto mm1 = (m1.unit() == tolerance.unit() ? m1 :
			convert(tolerance.unit(), m1));
		auto mm2 = (m2.unit() == tolerance.unit() ? m2 :
			convert(tolerance.unit(), m2));
        return squared_norm_value(mm1 - mm2) <= squared_norm_value(tolerance);
    }
	
    // dyn_point3 + dyn_vect3 -> dyn_point3
    template <class Magnitude, typename Num1, typename Num2>
    dyn_point3<Magnitude,decltype(Num1()+Num2())> operator +(
		dyn_point3<Magnitude,Num1> m1, dyn_vect3<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point3<Magnitude,decltype(Num1()+Num2())>(
				m1.unit(),
				m1.x().value() + m2.x().value(),
				m1.y().value() + m2.y().value(),
				m1.z().value() + m2.z().value());
		}
		Magnitude unit_id = Magnitude::base_unit::id();
        return dyn_point3<Magnitude,decltype(Num1()+Num2())>(
			unit_id,
			convert(unit_id, m1).x().value() + convert(unit_id, m2).x().value(),
			convert(unit_id, m1).y().value() + convert(unit_id, m2).y().value(),
			convert(unit_id, m1).z().value() + convert(unit_id, m2).z().value());
    }

    // dyn_point3 - dyn_vect3 -> dyn_point3
    template <class Magnitude, typename Num1, typename Num2>
    dyn_point3<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_point3<Magnitude,Num1> m1, dyn_vect3<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_point3<Magnitude,decltype(Num1()-Num2())>(m1.unit(),
				m1.x().value() - m2.x().value(),
				m1.y().value() - m2.y().value(),
				m1.z().value() - m2.z().value());
		}
		auto unit_id = Magnitude::base_unit::id();
		return dyn_point3<Magnitude,decltype(Num1()-Num2())>(unit_id,
			convert(unit_id, m1.x()).value() - convert(unit_id, m2.x()).value(),
			convert(unit_id, m1.y()).value() - convert(unit_id, m2.y()).value(),
			convert(unit_id, m1.z()).value() - convert(unit_id, m2.z()).value());
    }

    // dyn_vect3 + dyn_vect3 -> dyn_vect3
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect3<Magnitude,decltype(Num1()+Num2())> operator +(
		dyn_vect3<Magnitude,Num1> m1, dyn_vect3<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect3<Magnitude,decltype(Num1()+Num2())>(
				m1.unit(),
				m1.x().value() + m2.x().value(),
				m1.y().value() + m2.y().value(),
				m1.z().value() + m2.z().value());
		}
		Magnitude unit_id = Magnitude::base_unit::id();
        return dyn_vect3<Magnitude,decltype(Num1()+Num2())>(
			unit_id,
			convert(unit_id, m1).x().value() + convert(unit_id, m2).x().value(),
			convert(unit_id, m1).y().value() + convert(unit_id, m2).y().value(),
			convert(unit_id, m1).z().value() + convert(unit_id, m2).z().value());
    }

    // dyn_vect3 - dyn_vect3 -> dyn_vect3
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect3<Magnitude,decltype(Num1()-Num2())> operator -(
		dyn_vect3<Magnitude,Num1> m1, dyn_vect3<Magnitude,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the magnitude base unit.
		if (m1.unit() == m2.unit())
		{
			return dyn_vect3<Magnitude,decltype(Num1()-Num2())>(
				m1.unit(),
				m1.x().value() - m2.x().value(),
				m1.y().value() - m2.y().value(),
				m1.z().value() - m2.z().value());
		}
		auto unit_id = Magnitude::base_unit::id();
        return dyn_vect3<Magnitude,decltype(Num1()-Num2())>(
			unit_id,
			convert(unit_id, m1).x().value() - convert(unit_id, m2).x().value(),
			convert(unit_id, m1).y().value() - convert(unit_id, m2).y().value(),
			convert(unit_id, m1).z().value() - convert(unit_id, m2).z().value());
    }

    // N * dyn_vect3 -> dyn_vect3
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect3<Magnitude,decltype(Num1()*Num2())> operator *(
		Num1 n, dyn_vect3<Magnitude,Num2> m)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return dyn_vect3<Magnitude,decltype(Num1()*Num2())>(m.unit(),
			n * m.x().value(),
			n * m.y().value(),
			n * m.z().value());
    }

    // dyn_vect3 * N -> dyn_vect3
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect3<Magnitude,decltype(Num1()*Num2())> operator *(
		dyn_vect3<Magnitude,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return dyn_vect3<Magnitude,decltype(Num1()*Num2())>(m.unit(),
			m.x().value() * n,
			m.y().value() * n,
			m.z().value() * n);
    }

    // dyn_vect3 / N -> dyn_vect3
    template <class Magnitude, typename Num1, typename Num2>
    dyn_vect3<Magnitude,decltype(Num1()/Num2())> operator /(
		dyn_vect3<Magnitude,Num1> m, Num2 n)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
        return dyn_vect3<Magnitude,decltype(Num1()/Num2())>(m.unit(),
			m.x().value() / n,
			m.y().value() / n,
			m.z().value() / n);
    }

    template <class Magnitude, typename Num>
    Num squared_norm_value(dyn_vect3<Magnitude,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return sqr(v.x().value()) + sqr(v.y().value()) + sqr(v.z().value());
    }

    template <class Magnitude, typename Num>
    dyn_vect1<Magnitude,Num> norm(dyn_vect3<Magnitude,Num> v)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_vect1<Magnitude,Num>(v.unit(),
			static_cast<Num>(sqrt(squared_norm_value(v))));
    }
}


    // DYNAMIC ANGLES

// TODO: conversioni tra dyn_point1, dyn_signed_azimuth, e dyn_unsigned_azimuth.
// le differenze tra point1<radians> e angle_point<radians> sono:
// * sin(point1<U>) e' illegale, mentre sin(angle_point<U>) e' legale, e analogo per cos e tan
// * vect2<U>(point1<U2>) e' illegale, mentre vect2<U>(angle_point<radians>) e' legale
// * point_180<radians>(point1<U>) e' illegale, mentre point_180<radians>(angle_point<radians>) e' legale,
//   e analogo per point_360 e per degrees o turns invece di radians.
// Soluzione:
// definire le operazioni apparentemente illegali, cioe'
// sin(point1<U1>), vect2<U2>(point1<U1>), point_180<U2>(point1<U1>), e simili,
// e all'interno di tali funzioni inserire la funzione
// is_angle<U1>()
// definita nel seguente modo:
// template <typename T>
// same_type(Angle(), T())
namespace measures
{	
    template <typename Num = double>
    class dyn_signed_azimuth
    {
	ASSERT_IS_NUMERIC(Num)
        template <typename> class dyn_unsigned_azimuth;
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_signed_azimuth():
            unit_(Angle::base_unit::id()), x_(Num()) { }

        // Construct using one number of the same number type,
		// specifying an angle unit.
        explicit dyn_signed_azimuth(Angle unit, Num x):
            unit_(unit), x_(normalize_(unit, x)) { }

        // Construct using another dyn_signed_azimuth of the same number type.
		// The unit is copied.
        dyn_signed_azimuth(const dyn_signed_azimuth<Num>& o):
            unit_(o.unit()), x_(o.value()) { }
			
        // Construct using a signed_azimuth of the same number type.
		// The unit is copied.
        template <class Unit>
        explicit dyn_signed_azimuth(const signed_azimuth<Unit,Num>& o):
            unit_(Unit::id()), x_(o.value()) { }
		
        // Construct using a dyn_unsigned_azimuth of the same number type.
		// The unit is copied.
        explicit dyn_signed_azimuth(const dyn_unsigned_azimuth<Num>& o):
            unit_(o.unit()), x_(normalize_(o.unit(), o.value())) { }
		
        // Construct using a unsigned_azimuth of the same number type.
		// The unit is copied.
        template <class Unit>
        explicit dyn_signed_azimuth(const unsigned_azimuth<Unit,Num>& o):
            unit_(Unit::id()), x_(normalize_(Unit::id(), o.value())) { }
			
        // Construct using a dyn_point1 representing an angle,
        // of the same number type.
		// The unit is copied.
        explicit dyn_signed_azimuth(const dyn_point1<Angle,Num>& o):
            unit_(o.unit()), x_(normalize_(o.unit(), o.value())) { }

        // Construct using a point1 representing an angle,
        // of the same number type.
		// The unit is copied.
        template <class Unit>
        explicit dyn_signed_azimuth(const point1<Unit,Num>& o):
            unit_(Unit::id()), x_(normalize_(Unit::id(), o.value()))
		{
            ASSERT_IS_ANGLE(Unit)
		}
		
        // Construct with specified unit using any dyn_vect2,
		// possibly of a different number type.
        template <class Magnitude2, typename Num2>
        explicit dyn_signed_azimuth(Angle unit, const dyn_vect2<Magnitude2,Num2>& o):
            unit_(unit),
            x_(convert(unit, dyn_signed_azimuth<Num>(radians::id(),
                static_cast<Num>(atan2(o.y().value(), o.x().value())))).value())
		{
			ASSERT_IS_NUMERIC(Num2)
		}

        // Construct with specified unit using any vect2,
		// possibly of a different number type.
        template <class Unit, typename Num2>
        explicit dyn_signed_azimuth(Angle unit, const vect2<Unit,Num2>& o):
            unit_(unit),
            x_(convert(unit, dyn_signed_azimuth<Num>(radians::id(),
                static_cast<Num>(atan2(o.y().value(), o.x().value())))).value())
		{
			ASSERT_IS_NUMERIC(Num2)
		}

		// Convert to a dyn_point1.
		operator dyn_point1<Angle,Num>() const
		{
			return dyn_point1<Angle,Num>(unit_, x_);
		}
				
        // Get the unit.
		Angle unit() const { return unit_; }
		
        // Get unmutable value (mutable getter not available).
        Num value() const { return x_; }

        // dyn_signed_azimuth += dyn_vect1 -> dyn_signed_azimuth
        dyn_signed_azimuth<Num> operator +=(dyn_vect1<Angle,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
			x_ = normalize_(unit_, x_ + mm.value());
            return *this;
        }
		
        // dyn_signed_azimuth -= dyn_vect1 -> dyn_signed_azimuth
        dyn_signed_azimuth<Num> operator -=(dyn_vect1<Angle,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
			x_ = normalize_(unit_, x_ - mm.value());
            return *this;
        }
		
    private:
        static Num normalize_(Angle unit, Num x)
        {
			return normalize_signed_azimuth(x, static_cast<Num>(unit.turn_fraction()));
        }
		
		Angle unit_;
        Num x_;
    };

    // dyn_signed_azimuth - dyn_signed_azimuth -> dyn_vect1
	// As a difference between two point angles it is meant the shortest
	// vect angle that if added to the second point angle gets the first one.
    template <typename Num1, typename Num2>
    dyn_vect1<Angle,decltype(Num1()-Num2())> operator -(
		dyn_signed_azimuth<Num1> m1, dyn_signed_azimuth<Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		
		// If m1 and m2 have the same unit, use it.
		// Otherwise use the angle base unit.
		Angle unit_id = m1.unit() == m2.unit() ?
			m1.unit() : Angle::base_unit::id();
		auto difference = m1.unit() == m2.unit() ?
			m1.value() - m2.value() :
			convert(unit_id, m1).value()
			- convert(unit_id, m2).value();
		auto one_turn = unit_id.turn_fraction();
		if (difference * 2 < -one_turn) difference += one_turn;
		else if (difference * 2 > one_turn) difference -= one_turn;
		return dyn_vect1<Angle,decltype(Num1()-Num2())>(unit_id, difference);
	}
	
    // dyn_signed_azimuth == dyn_signed_azimuth -> bool
    template <typename Num>
    bool operator ==(dyn_signed_azimuth<Num> m1, dyn_signed_azimuth<Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.value() == m2.value();
    }
	
    // dyn_signed_azimuth != dyn_signed_azimuth -> bool
    template <typename Num>
    bool operator !=(dyn_signed_azimuth<Num> m1, dyn_signed_azimuth<Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.value() != m2.value();
    }

    // angle_distance(dyn_signed_azimuth, dyn_signed_azimuth) -> dyn_vect1
    template <typename Num>
    dyn_vect1<Angle,Num> angle_distance(
		dyn_signed_azimuth<Num> m1, dyn_signed_azimuth<Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the angle base unit.
		Angle unit_id(m1.unit() == m2.unit() ?
			m1.unit() : Angle::base_unit::id());
		Num value_distance(abs(m1.unit() == m2.unit() ?
			m1.value() - m2.value() :
			convert(unit_id, m1).value() - convert(unit_id, m2).value()));
		if (value_distance * 2 > unit_id.turn_fraction())
		{
			value_distance = unit_id.turn_fraction() - value_distance;
		}
		return dyn_vect1<Angle,Num>(unit_id, value_distance);
    }

    // is_equal(dyn_signed_azimuth, dyn_signed_azimuth, tolerance) -> bool
    template <typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_signed_azimuth<Num1> m1,
		dyn_signed_azimuth<Num2> m2, dyn_vect1<Angle,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return angle_distance(m1, m2) <= tolerance;
    }

    // dyn_signed_azimuth + dyn_vect1 -> dyn_signed_azimuth
    template <typename Num1, typename Num2>
    dyn_signed_azimuth<decltype(Num1()+Num2())> operator +(
		dyn_signed_azimuth<Num1> m1, dyn_vect1<Angle,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		//TODO 
        return dyn_signed_azimuth<decltype(Num1()+Num2())>(m1.unit(),
			m1.value() + m2.value());
    }

    // dyn_signed_azimuth - dyn_vect1 -> dyn_signed_azimuth
    template <typename Num1, typename Num2>
    dyn_signed_azimuth<decltype(Num1()-Num2())> operator -(
		dyn_signed_azimuth<Num1> m1, dyn_vect1<Angle,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		//TODO 
        return dyn_signed_azimuth<decltype(Num1()-Num2())>(m1.unit(),
			m1.value() - m2.value());
    }

    template <typename Num = double>
    class dyn_unsigned_azimuth
    {
	ASSERT_IS_NUMERIC(Num)
        template <typename> class dyn_signed_azimuth;
    public:
		typedef Num numeric_type;

        // Construct without values.
        explicit dyn_unsigned_azimuth():
            unit_(Angle::base_unit::id()), x_(Num()) { }

        // Construct using one number of the same number type,
		// specifying an angle unit.
        explicit dyn_unsigned_azimuth(Angle unit, Num x):
            unit_(unit), x_(normalize_(unit, x)) { }

        // Construct using another dyn_unsigned_azimuth of the same number type.
		// The unit is copied.
        dyn_unsigned_azimuth(const dyn_unsigned_azimuth<Num>& o):
            unit_(o.unit()), x_(o.value()) { }
			
        // Construct using an unsigned_azimuth of the same number type.
		// The unit is copied.
        template <class Unit>
        explicit dyn_unsigned_azimuth(const unsigned_azimuth<Unit,Num>& o):
            unit_(Unit::id()), x_(o.value()) { }
		
        // Construct using a dyn_signed_azimuth of the same number type.
		// The unit is copied.
        explicit dyn_unsigned_azimuth(const dyn_signed_azimuth<Num>& o):
            unit_(o.unit()), x_(normalize_(o.unit(), o.value())) { }

        // Construct using a signed_azimuth of the same number type.
		// The unit is copied.
        template <class Unit>
        explicit dyn_unsigned_azimuth(const signed_azimuth<Unit,Num>& o):
            unit_(Unit::id()), x_(normalize_(Unit::id(), o.value())) { }
			
        // Construct using a dyn_point1 representing an angle,
        // of the same number type.
		// The unit is copied.
        explicit dyn_unsigned_azimuth(const dyn_point1<Angle,Num>& o):
            unit_(o.unit()), x_(normalize_(o.unit(), o.value())) { }

        // Construct using a point1 representing an angle,
        // of the same number type.
		// The unit is copied.
        template <class Unit>
        explicit dyn_unsigned_azimuth(const point1<Unit,Num>& o):
            unit_(Unit::id()), x_(normalize_(Unit::id(), o.value()))
		{
            ASSERT_IS_ANGLE(Unit)
		}
		
        // Construct with specified unit using any dyn_vect2,
		// possibly of a different number type.
        template <class Magnitude2, typename Num2>
        explicit dyn_unsigned_azimuth(Angle unit, const dyn_vect2<Magnitude2,Num2>& o):
            unit_(unit),
            x_(convert(unit, dyn_unsigned_azimuth<Num>(radians::id(),
                static_cast<Num>(atan2(o.y().value(), o.x().value())))).value())
		{
			ASSERT_IS_NUMERIC(Num2)
		}

        // Construct with specified unit using any vect2,
		// possibly of a different number type.
        template <class Unit, typename Num2>
        explicit dyn_unsigned_azimuth(Angle unit, const vect2<Unit,Num2>& o):
            unit_(unit),
            x_(convert(unit, dyn_unsigned_azimuth<Num>(radians::id(),
                static_cast<Num>(atan2(o.y().value(), o.x().value())))).value())
		{
			ASSERT_IS_NUMERIC(Num2)
		}
		
		// Convert to a dyn_point1.
		operator dyn_point1<Angle,Num>() const
		{
			return dyn_point1<Angle,Num>(unit_, x_);
		}

        // Get the unit.
		Angle unit() const { return unit_; }
		
        // Get unmutable value (mutable getter not available).
        Num value() const { return x_; }
		
        // dyn_unsigned_azimuth += dyn_vect1 -> dyn_unsigned_azimuth
        dyn_unsigned_azimuth<Num> operator +=(dyn_vect1<Angle,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
			x_ = normalize_(unit_, x_ + mm.value());
            return *this;
        }
		
        // dyn_unsigned_azimuth -= dyn_vect1 -> dyn_unsigned_azimuth
        dyn_unsigned_azimuth<Num> operator -=(dyn_vect1<Angle,Num> m)
        {
            auto mm = unit_ == m.unit() ? m : convert(unit_, m);
			x_ = normalize_(unit_, x_ - mm.value());
            return *this;
        }
		
    private:
        static Num normalize_(Angle unit, Num x)
        {
			return normalize_unsigned_azimuth(x, static_cast<Num>(unit.turn_fraction()));
        }
		
		Angle unit_;
        Num x_;
    };

    // dyn_unsigned_azimuth - dyn_unsigned_azimuth -> dyn_vect1
	// As a difference between two point angles it is meant the shortest
	// vect angle that if added to the second point angle gets the first one.
    template <typename Num1, typename Num2>
    dyn_vect1<Angle,decltype(Num1()-Num2())> operator -(
		dyn_unsigned_azimuth<Num1> m1, dyn_unsigned_azimuth<Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the angle base unit.
		Angle unit_id(m1.unit() == m2.unit() ?
			m1.unit() : Angle::base_unit::id());
		decltype(Num1()-Num2()) value_distance(m1.unit() == m2.unit() ?
			m1.value() - m2.value() :
			convert(unit_id, m1).value() - convert(unit_id, m2).value());
		auto one_turn = unit_id.turn_fraction();
		if (value_distance * 2 < -one_turn) value_distance += one_turn;
		else if (value_distance * 2 > one_turn) value_distance -= one_turn;
		return dyn_vect1<Angle,decltype(Num1()-Num2())>(unit_id, value_distance);
	}

    // dyn_unsigned_azimuth == dyn_unsigned_azimuth -> bool
    template <typename Num>
    bool operator ==(dyn_unsigned_azimuth<Num> m1, dyn_unsigned_azimuth<Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return false;
        return m1.value() == m2.value();
    }

    // dyn_unsigned_azimuth != dyn_unsigned_azimuth -> bool
    template <typename Num>
    bool operator !=(dyn_unsigned_azimuth<Num> m1, dyn_unsigned_azimuth<Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)
		if (m1.unit() != m2.unit()) return true;
        return m1.value() != m2.value();
    }

    // angle_distance(dyn_unsigned_azimuth, dyn_unsigned_azimuth) -> dyn_vect1
    template <typename Num>
    dyn_vect1<Angle,Num> angle_distance(
		dyn_unsigned_azimuth<Num> m1, dyn_unsigned_azimuth<Num> m2)
    {
		ASSERT_IS_NUMERIC(Num)

		// If m1 and m2 have the same unit, use it.
		// Otherwise use the angle base unit.
		Angle unit_id(m1.unit() == m2.unit() ?
			m1.unit() : Angle::base_unit::id());
		Num value_distance(abs(m1.unit() == m2.unit() ?
			m1.value() - m2.value() :
			convert(unit_id, m1).value() - convert(unit_id, m2).value()));
		if (value_distance * 2 > unit_id.turn_fraction())
			value_distance = unit_id.turn_fraction() - value_distance;
		return dyn_vect1<Angle,Num>(unit_id, value_distance);
    }

    // is_equal(dyn_unsigned_azimuth, dyn_unsigned_azimuth, tolerance) -> bool
    template <typename Num1, typename Num2, typename Num3>
    bool is_equal(dyn_unsigned_azimuth<Num1> m1,
		dyn_unsigned_azimuth<Num2> m2, dyn_vect1<Angle,Num3> tolerance)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		ASSERT_IS_NUMERIC(Num3)
        return angle_distance(m1, m2) <= tolerance;
    }
	
    // dyn_unsigned_azimuth + dyn_vect1 -> dyn_unsigned_azimuth
    template <typename Num1, typename Num2>
    dyn_unsigned_azimuth<decltype(Num1()+Num2())> operator +(
		dyn_unsigned_azimuth<Num1> m1, dyn_vect1<Angle,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		//TODO
        return dyn_unsigned_azimuth<decltype(Num1()+Num2())>(m1.unit(),
			m1.value() + m2.value());
    }

    // dyn_unsigned_azimuth - dyn_vect1 -> dyn_unsigned_azimuth
    template <typename Num1, typename Num2>
    dyn_unsigned_azimuth<decltype(Num1()-Num2())> operator -(
		dyn_unsigned_azimuth<Num1> m1, dyn_vect1<Angle,Num2> m2)
    {
		ASSERT_IS_NUMERIC(Num1)
		ASSERT_IS_NUMERIC(Num2)
		//TODO
        return dyn_unsigned_azimuth<decltype(Num1()-Num2())>(m1.unit(),
			m1.value() - m2.value());
    }

    ///////////////////////// DYNAMIC CONVERSIONS /////////////////////////

    template <class Magnitude, typename Num>
    dyn_vect1<Magnitude,Num> convert(Magnitude dest_unit, dyn_vect1<Magnitude,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_vect1<Magnitude,Num>(dest_unit, static_cast<Num>(m.value()
            * (m.unit().ratio()
			/ dest_unit.ratio())));
    }

    template <class Magnitude, typename Num>
    dyn_point1<Magnitude,Num> convert(Magnitude dest_unit, dyn_point1<Magnitude,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_point1<Magnitude,Num>(dest_unit, static_cast<Num>(m.value()
            * (m.unit().ratio() / dest_unit.ratio())
            + (m.unit().offset() - dest_unit.offset())
            / dest_unit.ratio()));
    }

    template <class Magnitude, typename Num>
    dyn_vect2<Magnitude,Num> convert(Magnitude dest_unit, dyn_vect2<Magnitude,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_vect2<Magnitude,Num>(dest_unit,
            convert<>(dest_unit, m.x()).value(),
            convert<>(dest_unit, m.y()).value());
    }

    template <class Magnitude, typename Num>
    dyn_point2<Magnitude,Num> convert(Magnitude dest_unit, dyn_point2<Magnitude,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_point2<Magnitude,Num>(dest_unit,
            convert<>(dest_unit, m.x()).value(),
            convert<>(dest_unit, m.y()).value());
    }

    template <class Magnitude, typename Num>
    dyn_vect3<Magnitude,Num> convert(Magnitude dest_unit, dyn_vect3<Magnitude,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_vect3<Magnitude,Num>(dest_unit,
            convert<>(dest_unit, m.x()).value(),
            convert<>(dest_unit, m.y()).value(),
            convert<>(dest_unit, m.z()).value());
    }

    template <class Magnitude, typename Num>
    dyn_point3<Magnitude,Num> convert(Magnitude dest_unit, dyn_point3<Magnitude,Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_point3<Magnitude,Num>(dest_unit,
            convert<>(dest_unit, m.x()).value(),
            convert<>(dest_unit, m.y()).value(),
            convert<>(dest_unit, m.z()).value());
    }

    template <typename Num>
    dyn_signed_azimuth<Num> convert(Angle dest_unit, dyn_signed_azimuth<Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_signed_azimuth<Num>(dest_unit, static_cast<Num>(m.value()
            * (m.unit().ratio() / dest_unit.ratio())
            + (m.unit().offset() - dest_unit.offset())
            / dest_unit.ratio()));
    }

    template <typename Num>
    dyn_unsigned_azimuth<Num> convert(Angle dest_unit, dyn_unsigned_azimuth<Num> m)
    {
		ASSERT_IS_NUMERIC(Num)
        return dyn_unsigned_azimuth<Num>(dest_unit, static_cast<Num>(m.value()
            * (m.unit().ratio() / dest_unit.ratio())
            + (m.unit().offset() - dest_unit.offset())
            / dest_unit.ratio()));
    }
	
	
    ///////////////////////// DYNAMIC CASTS /////////////////////////

    // Scalar measures
    template <class Magnitude, typename ToNum, typename FromNum>
    dyn_vect1<Magnitude,ToNum> cast(dyn_vect1<Magnitude,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_vect1<Magnitude,ToNum>(m.unit(), static_cast<ToNum>(m.value()));
    }

    template <class Magnitude, typename ToNum, typename FromNum>
    dyn_point1<Magnitude,ToNum> cast(dyn_point1<Magnitude,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_point1<Magnitude,ToNum>(m.unit(), static_cast<ToNum>(m.value()));
    }

    // Planar measures
    template <class Magnitude, typename ToNum, typename FromNum>
    dyn_vect2<Magnitude,ToNum> cast(dyn_vect2<Magnitude,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_vect2<Magnitude,ToNum>(m.unit(),
            cast<Magnitude,ToNum,FromNum>(m.x()),
            cast<Magnitude,ToNum,FromNum>(m.y()));
    }

    template <class Magnitude, typename ToNum, typename FromNum>
    dyn_point2<Magnitude,ToNum> cast(dyn_point2<Magnitude,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_point2<Magnitude,ToNum>(m.unit(),
            cast<Magnitude,ToNum,FromNum>(m.x()),
            cast<Magnitude,ToNum,FromNum>(m.y()));
    }

    // Spacial measures
    template <class Magnitude, typename ToNum, typename FromNum>
    dyn_vect3<Magnitude,ToNum> cast(dyn_vect3<Magnitude,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_vect3<Magnitude,ToNum>(m.unit(),
            cast<Magnitude,ToNum,FromNum>(m.x()),
            cast<Magnitude,ToNum,FromNum>(m.y()),
            cast<Magnitude,ToNum,FromNum>(m.z()));
    }

    template <class Magnitude, typename ToNum, typename FromNum>
    dyn_point3<Magnitude,ToNum> cast(dyn_point3<Magnitude,FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_point3<Magnitude,ToNum>(m.unit(),
            cast<Magnitude,ToNum,FromNum>(m.x()),
            cast<Magnitude,ToNum,FromNum>(m.y()),
            cast<Magnitude,ToNum,FromNum>(m.z()));
    }

	// Azimuths
    template <typename ToNum, typename FromNum>
    dyn_signed_azimuth<ToNum> cast(dyn_signed_azimuth<FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_signed_azimuth<ToNum>(m.unit(), static_cast<ToNum>(m.value()));
    }

    template <typename ToNum, typename FromNum>
    dyn_unsigned_azimuth<ToNum> cast(dyn_unsigned_azimuth<FromNum> m)
    {
		ASSERT_IS_NUMERIC(ToNum);
		ASSERT_IS_NUMERIC(FromNum);
        return dyn_unsigned_azimuth<ToNum>(m.unit(), static_cast<ToNum>(m.value()));
    }
}
#endif
