// measures_io.hpp
// Released under Mozilla Public Licence 2.0
// by Carlo Milanesi (carlo.milanesi@tiscali.it) in october 2014
#ifndef MEASURES_IO_HPP
#define MEASURES_IO_HPP
#include "measures.hpp"
#include <iostream>

// vect1			1 mm
// point1			[1] mm
// vect2			1 2 mm
// point2			[1 2] mm
// vect3			1 2 3 mm
// point3			[1 2 3] mm
// signed_azimuth	S1^
// unsigned_azimuth	U1^

namespace measures
{
	///////////////////////// STATIC UNIT SCALAR AND 1-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const vect1<Unit,Num>& o)
	{
		os << o.value() << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		vect1<Unit,Num>& o)
	{
		Num v;
		is >> v;
		is.ignore(strlen(Unit::suffix()));
		o = vect1<Unit,Num>(v);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const point1<Unit,Num>& o)
	{
		os << "[" << o.value() << "]" << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		point1<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(1 + strlen(Unit::suffix()));
		o = point1<Unit,Num>(v);
		return is;
	}

	///////////////////////// STATIC UNIT 2-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const vect2<Unit,Num>& o)
	{
		os << o.x().value() << " " << o.y().value() << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, vect2<Unit,Num>& o)
	{
		Num v1, v2;
		is >> v1 >> v2;
		is.ignore(strlen(Unit::suffix()));
		o = vect2<Unit,Num>(v1, v2);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const point2<Unit,Num>& o)
	{
		os << "[" << o.x().value() << " " << o.y().value() << "]" << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, point2<Unit,Num>& o)
	{
		Num v1, v2;
		is.ignore();
		is >> v1 >> v2;
		is.ignore(1 + strlen(Unit::suffix()));
		o = point2<Unit,Num>(v1, v2);
		return is;
	}

	///////////////////////// STATIC UNIT 3-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const vect3<Unit,Num>& o)
	{
		os << o.x().value() << " " << o.y().value() << " " << o.z().value() << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, vect3<Unit,Num>& o)
	{
		Num v1, v2, v3;
		is >> v1 >> v2 >> v3;
		is.ignore(strlen(Unit::suffix()));
		o = vect3<Unit,Num>(v1, v2, v3);
		return is;
	}

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const point3<Unit,Num>& o)
	{
		os << "[" << o.x().value() << " " << o.y().value() << " " << o.z().value() << "]" << Unit::suffix();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, point3<Unit,Num>& o)
	{
		Num v1, v2, v3;
		is.ignore();
		is >> v1 >> v2 >> v3;
		is.ignore(1 + strlen(Unit::suffix()));
		o = point3<Unit,Num>(v1, v2, v3);
		return is;
	}

	///////////////////////// STATIC UNIT AZIMUTHS /////////////////////////
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const signed_azimuth<Unit,Num>& o)
	{
		os << "S" << o.value() << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		signed_azimuth<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(strlen(Unit::suffix()));
		o = signed_azimuth<Unit,Num>(v);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const unsigned_azimuth<Unit,Num>& o)
	{
		os << "U" << o.value() << Unit::suffix();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		unsigned_azimuth<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(strlen(Unit::suffix()));
		o = unsigned_azimuth<Unit,Num>(v);
		return is;
	}
}
#endif
