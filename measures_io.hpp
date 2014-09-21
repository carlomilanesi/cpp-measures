// measures_io.hpp
// measures.hpp
// Released under Mozilla Public Licence 2.0
// by Carlo Milanesi (carlo.milanesi@tiscali.it)
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
// dyn_vect1		D 1 mm
// dyn_point1		D [1] mm
// dyn_vect2		D 1 2 mm
// dyn_point2		D [1 2] mm
// dyn_vect3		D 1 2 3 mm
// dyn_point3		D [1 2 3] mm
// signed_azimuth	D S1^
// unsigned_azimuth	D U1^

namespace measures
{
	///////////////////////// STATIC UNIT SCALAR AND 1-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const vect1<Unit,Num>& o)
	{
		os << o.value() << Unit::symbol();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		vect1<Unit,Num>& o)
	{
		Num v;
		is >> v;
		is.ignore(strlen(Unit::symbol()));
		o = vect1<Unit,Num>(v);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const point1<Unit,Num>& o)
	{
		os << "[" << o.value() << "]" << Unit::symbol();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		point1<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(1 + strlen(Unit::symbol()));
		o = point1<Unit,Num>(v);
		return is;
	}

	///////////////////////// STATIC UNIT 2-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const vect2<Unit,Num>& o)
	{
		os << o.x().value() << " " << o.y().value() << Unit::symbol();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, vect2<Unit,Num>& o)
	{
		Num v1, v2;
		is >> v1 >> v2;
		is.ignore(strlen(Unit::symbol()));
		o = vect2<Unit,Num>(v1, v2);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const point2<Unit,Num>& o)
	{
		os << "[" << o.x().value() << " " << o.y().value() << "]" << Unit::symbol();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, point2<Unit,Num>& o)
	{
		Num v1, v2;
		is.ignore();
		is >> v1 >> v2;
		is.ignore(1 + strlen(Unit::symbol()));
		o = point2<Unit,Num>(v1, v2);
		return is;
	}

	///////////////////////// STATIC UNIT 3-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const vect3<Unit,Num>& o)
	{
		os << o.x().value() << " " << o.y().value() << " " << o.z().value() << Unit::symbol();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, vect3<Unit,Num>& o)
	{
		Num v1, v2, v3;
		is >> v1 >> v2 >> v3;
		is.ignore(strlen(Unit::symbol()));
		o = vect3<Unit,Num>(v1, v2, v3);
		return is;
	}

	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os, const point3<Unit,Num>& o)
	{
		os << "[" << o.x().value() << " " << o.y().value() << " " << o.z().value() << "]" << Unit::symbol();
		return os;
	}

	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is, point3<Unit,Num>& o)
	{
		Num v1, v2, v3;
		is.ignore();
		is >> v1 >> v2 >> v3;
		is.ignore(1 + strlen(Unit::symbol()));
		o = point3<Unit,Num>(v1, v2, v3);
		return is;
	}

	///////////////////////// STATIC UNIT AZIMUTHS /////////////////////////
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const signed_azimuth<Unit,Num>& o)
	{
		os << "S" << o.value() << Unit::symbol();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		signed_azimuth<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(strlen(Unit::symbol()));
		o = signed_azimuth<Unit,Num>(v);
		return is;
	}
	
	template <class Unit, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const unsigned_azimuth<Unit,Num>& o)
	{
		os << "U" << o.value() << Unit::symbol();
		return os;
	}
	
	template <class Unit, typename Num>
	std::istream& operator >>(std::istream& is,
		unsigned_azimuth<Unit,Num>& o)
	{
		Num v;
		is.ignore();
		is >> v;
		is.ignore(strlen(Unit::symbol()));
		o = unsigned_azimuth<Unit,Num>(v);
		return is;
	}

	///////////////////////// DYNAMIC UNIT SCALAR AND 1-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Magnitude, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_vect1<Magnitude,Num>& o)
	{
		os << "D " << o.value() << o.unit().symbol();
		return os;
	}

	template <class Magnitude, typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_vect1<Magnitude,Num>& o)
	{
		Num v;
		is.ignore(2);
		is >> v;
		is.ignore(strlen(o.unit().symbol()));
		o = dyn_vect1<Magnitude,Num>(o.unit(), v);
		return is;
	}

	template <class Magnitude, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_point1<Magnitude,Num>& o)
	{
		os << "D [" << o.value() << "]" << o.unit().symbol();
		return os;
	}

	template <class Magnitude, typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_point1<Magnitude,Num>& o)
	{
		Num v;
		is.ignore(3);
		is >> v;
		is.ignore(1 + strlen(o.unit().symbol()));
		o = dyn_point1<Magnitude,Num>(o.unit(), v);
		return is;
	}

	///////////////////////// DYNAMIC UNIT 2-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Magnitude, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_vect2<Magnitude,Num>& o)
	{
		os << "D " << o.x().value() << " " << o.y().value() << o.unit().symbol();
		return os;
	}

	template <class Magnitude, typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_vect2<Magnitude,Num>& o)
	{
		Num v1, v2;
		is.ignore(2);
		is >> v1 >> v2;
		is.ignore(strlen(o.unit().symbol()));
		o = dyn_vect2<Magnitude,Num>(o.unit(), v1, v2);
		return is;
	}

	template <class Magnitude, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_point2<Magnitude,Num>& o)
	{
		os << "D [" << o.x().value() << " " << o.y().value() << "]" << o.unit().symbol();
		return os;
	}

	template <class Magnitude, typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_point2<Magnitude,Num>& o)
	{
		Num v1, v2;
		is.ignore(3);
		is >> v1 >> v2;
		is.ignore(1 + strlen(o.unit().symbol()));
		o = dyn_point2<Magnitude,Num>(o.unit(), v1, v2);
		return is;
	}

	///////////////////////// DYNAMIC UNIT 3-DIMENSIONAL VECTORS AND POINTS /////////////////////////

	template <class Magnitude, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_vect3<Magnitude,Num>& o)
	{
		os << "D " << o.x().value() << " " << o.y().value() << " "
			<< o.z().value() << o.unit().symbol();
		return os;
	}

	template <class Magnitude, typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_vect3<Magnitude,Num>& o)
	{
		Num v1, v2, v3;
		is.ignore(2);
		is >> v1 >> v2 >> v3;
		is.ignore(strlen(o.unit().symbol()));
		o = dyn_vect3<Magnitude,Num>(o.unit(), v1, v2, v3);
		return is;
	}

	template <class Magnitude, typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_point3<Magnitude,Num>& o)
	{
		os << "D [" << o.x().value() << " " << o.y().value()
			<< " " << o.z().value() << "]" << o.unit().symbol();
		return os;
	}

	template <class Magnitude, typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_point3<Magnitude,Num>& o)
	{
		Num v1, v2, v3;
		is.ignore(3);
		is >> v1 >> v2 >> v3;
		is.ignore(1 + strlen(o.unit().symbol()));
		o = dyn_point3<Magnitude,Num>(o.unit(), v1, v2, v3);
		return is;
	}
	
	///////////////////////// DYNAMIC UNIT AZIMUTHS /////////////////////////
	
	template <typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_signed_azimuth<Num>& o)
	{
		os << "D S" << o.value() << o.unit().symbol();
		return os;
	}
	
	template <typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_signed_azimuth<Num>& o)
	{
		Num v;
		is.ignore(3);
		is >> v;
		is.ignore(strlen(o.unit().symbol()));
		o = dyn_signed_azimuth<Num>(o.unit(), v);
		return is;
	}
	
	template <typename Num>
	std::ostream& operator <<(std::ostream& os,
		const dyn_unsigned_azimuth<Num>& o)
	{
		os << "D U" << o.value() << o.unit().symbol();
		return os;
	}
	
	template <typename Num>
	std::istream& operator >>(std::istream& is,
		dyn_unsigned_azimuth<Num>& o)
	{
		Num v;
		is.ignore(3);
		is >> v;
		is.ignore(strlen(o.unit().symbol()));
		o = dyn_unsigned_azimuth<Num>(o.unit(), v);
		return is;
	}
};
#endif
