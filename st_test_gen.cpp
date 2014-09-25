#include <string>
#include <iostream>
#include <map>
using namespace std;

map<string,string> magnitudes;

string measure_type(bool dynamic, bool absolute, int dim,
	const string& unit, const string& num_type)
{
    string result = dynamic ? "dyn_": "";
    result += absolute ? "point" : "vect";
    result += char('0' + dim);
    result += "<";
    if (dynamic)
	{
        result += magnitudes[unit];
	}
	else
    {
        result += unit;
    }
	if (num_type.size() > 0) result += ",";
    result += num_type;
    result += ">";
    return result;
}

string measure_init_list(bool dynamic, const string& unit, int n_values, const string& num_type)
{
    if (n_values == 0) return "";
    string result = "(";
    if (dynamic && unit.size() > 0)
	{
		result += unit;
		result += "::id()";
		if (n_values > 0) result += ", ";
    }
	for (int d = 1; d <= n_values ; ++d)
	{
		if (d > 1) result += ", ";
		if (num_type.size() > 0)
		{
			result += num_type;
			result += "(";
		}
		result += char('0' + d);
		result += char('0' + d);
		result += '.';
		if (num_type.size() > 0)
		{
			result += ")";
		}
	}
    result += ")";
    return result;
}

string var_decl(bool dynamic, bool absolute, int dim, const string& unit,
	const string& num_type, const string& var_name, int n_values)
{
    string result = measure_type(dynamic, absolute, dim, unit, num_type);
    result += " ";
    result += var_name;
    result += measure_init_list(dynamic, unit, n_values, num_type);
    result += ";";
    return result;
}

string temp_decl(bool dynamic, bool absolute, int dim, const string& unit,
	const string& num_type, int n_values)
{
    string result = measure_type(dynamic, absolute, dim, unit, num_type);
    result += " ";
	int len = result.size();
    result += measure_init_list(dynamic, unit, n_values, num_type);
	if (len == result.size()) result += "()";
    return result;
}

string var_decl_copy(bool dynamic, bool absolute, int dim, const string& unit,
	const string& num_type, const string& var_name, const string& arguments)
{
    string result = measure_type(dynamic, absolute, dim, unit, num_type);
    result += " ";
    result += var_name;
    result += "(";
    result += arguments;
    result += ");";
    return result;
}

void generate_type_with_no_unit(ostream& os,
	bool dynamic, bool absolute, int dim)
{
	// Any measure must have a unit.
	os << "@ILLEGAL" << endl;
	os << "// generate_type_with_no_unit" << endl;
	os << var_decl(dynamic, absolute, dim, "", "", "m1", 0) << endl;
	os << "@END" << endl;
}

void generate_construction_from_numbers(ostream& os,
	bool dynamic, bool absolute, int dim)
{
	// Any measure using a defined unit can be constructed from zero numbers,
	// or as many numbers as the measure dimensions.
    for (int d = 0; d <= dim + 1; ++d)
    {
		os << (d == 0 || d == dim ? "@LEGAL" : "@ILLEGAL") << endl;
		os << "// generate_construction_from_numbers " << d << endl;
        os << var_decl(dynamic, absolute, dim, "meters", "",
			string("m1_") + char('0' + d), d) << endl;
        os << var_decl(dynamic, absolute, dim, "inches", "",
			string("m2_") + char('0' + d), d) << endl;
        os << var_decl(dynamic, absolute, dim, "degrees", "",
			string("m3_") + char('0' + d), d) << endl;
        os << var_decl(dynamic, absolute, dim, "nounit", "",
			string("m4_") + char('0' + d), d) << endl;
		os << "@END" << endl;
    }
}

void generate_construction_from_another_same_unit_measure(ostream& os,
	bool dynamic, bool absolute, int dim)
{
    os << "@LEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 1" << endl;

	// Any measure can be constructed by copying another measure
	// having the same dynamicity, the same absoluteness,
	// the same dimensions, the same unit, and the same number type.
    os << var_decl(dynamic, absolute, dim, "meters",
		"", "m1", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"", "m1_1", "m1") << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"double", "m1_3", "m1") << endl;

    os << var_decl(dynamic, absolute, dim, "meters",
		"float", "m2", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"float", "m2_2", "m2") << endl;

    os << var_decl(dynamic, absolute, dim, "nounit",
		"", "m3", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"", "m3_1", "m3") << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"double", "m3_3", "m3") << endl;

    os << var_decl(dynamic, absolute, dim, "nounit",
		"float", "m4", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"float", "m4_2", "m4") << endl;

    os << "@END" << endl;
	
	// No measure using a float number type can be constructed
	// by copying another measure using an unspecified number type.
    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 2" << endl;
    os << var_decl(dynamic, absolute, dim, "meters",
		"", "m1", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"float", "m1_2", "m1") << endl;
    os << "@END" << endl;

	// No measure using a long double number type can be constructed
	// by copying another measure using an unspecified number type.
    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 3" << endl;
    os << var_decl(dynamic, absolute, dim, "meters",
		"", "m1", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"long double", "m1_4", "m1") << endl;
    os << "@END" << endl;

	// No measure using an unspecified number type can be constructed
	// by copying another measure using a float number type.
    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 4" << endl;
    os << var_decl(dynamic, absolute, dim, "meters",
		"float", "m2", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"", "m2_1", "m2") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 5" << endl;
    os << var_decl(dynamic, absolute, dim, "meters",
		"float", "m2", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"double", "m2_3", "m2") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 6" << endl;
    os << var_decl(dynamic, absolute, dim, "meters",
		"float", "m2", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "meters",
		"long double", "m2_4", "m2") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 7" << endl;
    os << var_decl(dynamic, absolute, dim, "nounit",
		"", "m3", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"float", "m3_2", "m3") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 8" << endl;
    os << var_decl(dynamic, absolute, dim, "nounit",
		"", "m3", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"long double", "m3_4", "m3") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 9" << endl;
    os << var_decl(dynamic, absolute, dim, "nounit",
		"float", "m4", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"", "m4_1", "m4") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 10" << endl;
    os << var_decl(dynamic, absolute, dim, "nounit",
		"float", "m4", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"double", "m4_3", "m4") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_construction_from_another_same_unit_measure 11" << endl;
    os << var_decl(dynamic, absolute, dim, "nounit",
		"float", "m4", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "nounit",
		"long double", "m4_4", "m4") << endl;
    os << "@END" << endl;
}

void generate_construction_from_another_different_unit_measure(ostream& os, bool dynamic, bool absolute, int dim)
{
    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_construction_from_another_different_unit_measure 1" << endl;
    os << var_decl(dynamic, absolute, dim, "meters", "", "m1", 0) << endl;
    os << var_decl_copy(dynamic, absolute, dim, "inches", "", "m1_1", "m1") << endl;
    os << "@END" << endl;

	if (dynamic)
	{
		os << "@ILLEGAL" << endl;
		os << "// generate_construction_from_another_different_unit_measure 2" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", "", "m1", 0) << endl;
		os << var_decl_copy(dynamic, absolute, dim, "seconds", "", "m1_1", "m1") << endl;
		os << "@END" << endl;
	}
}

void generate_construction_of_vect_from_point(ostream& os, bool dynamic, int dim)
{
    os << "@ILLEGAL" << endl;
	os << "// generate_construction_of_vect_from_point" << endl;
    os << var_decl(dynamic, true, dim, "meters", "", "m1", 0) << endl;
    os << var_decl_copy(dynamic, false, dim, "meters", "", "m1_1", "m1") << endl;
    os << "@END" << endl;
}

void generate_construction_of_point_from_vect(ostream& os, bool dynamic, int dim)
{
    os << "@ILLEGAL" << endl;
	os << "// generate_construction_of_point_from_vect" << endl;
    os << var_decl(dynamic, false, dim, "meters", "", "m1", 0) << endl;
    os << var_decl_copy(dynamic, true, dim, "meters", "", "m1_1", "m1") << endl;
    os << "@END" << endl;
}

void generate_member_operations_with_same_unit(ostream& os, bool dynamic, bool absolute, int dim)
{
	os << "@LEGAL" << endl;
	os << "// generate_member_operations_with_same_unit 1" << endl;
	os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
	os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
	if (absolute)
	{
		os << var_decl(dynamic, true, dim, "meters", "float", "pf", 0) << endl;
		os << var_decl(dynamic, true, dim, "meters", "double", "pd", 0) << endl;
		os << "pf += vf;" << endl;
		os << "pd += vd;" << endl;
		os << "pf -= vf;" << endl;
		os << "pd -= vd;" << endl;
	}
	else
	{
		os << "vf += vf;" << endl;
		os << "vd += vd;" << endl;
		os << "vf -= vf;" << endl;
		os << "vd -= vd;" << endl;
		os << "vf *= 2.1f;" << endl;
		os << "vd *= 2.1;" << endl;
		os << "vf /= 2.1f;" << endl;
		os << "vd /= 2.1;" << endl;
		os << "vf *= 2.1;" << endl;
		os << "vd *= 2.1f;" << endl;
		os << "vf /= 2.1;" << endl;
		os << "vd /= 2.1f;" << endl;
	}
	os << "@END" << endl;
	
	if (absolute)
	{
		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 2" << endl;
		os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
		os << var_decl(dynamic, true, dim, "meters", "float", "pf", 0) << endl;
		os << "pf += vd;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 3" << endl;
		os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
		os << var_decl(dynamic, true, dim, "meters", "double", "pd", 0) << endl;
		os << "pd += vf;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 4" << endl;
		os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
		os << var_decl(dynamic, true, dim, "meters", "float", "pf", 0) << endl;
		os << "pf -= vd;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 5" << endl;
		os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
		os << var_decl(dynamic, true, dim, "meters", "double", "pd", 0) << endl;
		os << "pd -= vf;" << endl;
		os << "@END" << endl;
	}
	else
	{
		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 6" << endl;
		os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
		os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
		os << "vf += vd;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 7" << endl;
		os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
		os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
		os << "vd += vf;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 8" << endl;
		os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
		os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
		os << "vf -= vd;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_member_operations_with_same_unit 9" << endl;
		os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
		os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
		os << "vd -= vf;" << endl;
		os << "@END" << endl;
	}
}

void generate_member_operations_with_different_units(ostream& os, bool dynamic, bool absolute, int dim)
{
	os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_member_operations_with_different_units 1" << endl;
	os << var_decl(dynamic, false, dim, "inches", "", "v", 0) << endl;
	os << var_decl(dynamic, absolute, dim, "meters", "", "m", 0) << endl;
	os << "m += v;" << endl;
	os << "@END" << endl;

	os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_member_operations_with_different_units 2" << endl;
	os << var_decl(dynamic, false, dim, "inches", "", "v", 0) << endl;
	os << var_decl(dynamic, absolute, dim, "meters", "", "m", 0) << endl;
	os << "m -= v;" << endl;
	os << "@END" << endl;
}

void generate_non_member_operations(ostream& os, bool dynamic, bool absolute, int dim)
{
	string num_types[] = { "int", "float", "double", "long double" };
	const int sz_num_types = sizeof num_types / sizeof num_types[0];

	// operator ==
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (t1 == t2 ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations == " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "bool b = m1 == m2;" << endl;
			os << "if (b) return 1;" << endl;
			os << "@END" << endl;
		}
	}

	// operator !=
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (t1 == t2 ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations != " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "bool b = m1 != m2;" << endl;
			os << "if (b) return 1;" << endl;
			os << "@END" << endl;
		}
	}
	
	// is_equal
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			for (int t3 = 0; t3 < sz_num_types; ++t3)
			{
				for (int toler_abs = 0; toler_abs < 2; ++toler_abs)
				{
					bool toler_absolute = toler_abs == 1;
					for (int toler_dim = 1; toler_dim <= dim; ++toler_dim)
					{
						os << (! toler_absolute && toler_dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
						os << "// generate_non_member_operations is_equal " << t1 << " " << t2 << endl;
						os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
						os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
						os << var_decl(dynamic, toler_absolute, toler_dim, "meters", num_types[t3], "toler", 1) << endl;
						os << "bool b = is_equal(m1, m2, toler);" << endl;
						os << "if (b) return 1;" << endl;
						os << "@END" << endl;
					}
				}
			}
		}
	}

	// operator <
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (t1 == t2 && dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations < " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "bool b = m1 < m2;" << endl;
			os << "if (b) return 1;" << endl;
			os << "@END" << endl;
		}
	}

	// operator <=
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (t1 == t2 && dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations <= " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "bool b = m1 <= m2;" << endl;
			os << "if (b) return 1;" << endl;
			os << "@END" << endl;
		}
	}
	
	// operator >
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (t1 == t2 && dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations > " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "bool b = m1 > m2;" << endl;
			os << "if (b) return 1;" << endl;
			os << "@END" << endl;
		}
	}

	// operator >=
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (t1 == t2 && dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations >= " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "bool b = m1 >= m2;" << endl;
			os << "if (b) return 1;" << endl;
			os << "@END" << endl;
		}
	}
	
	// is_less
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			for (int t3 = 0; t3 < sz_num_types; ++t3)
			{
				for (int toler_abs = 0; toler_abs < 2; ++toler_abs)
				{
					bool toler_absolute = toler_abs == 1;
					for (int toler_dim = 1; toler_dim <= dim; ++toler_dim)
					{
						os << (dim == 1 && ! toler_absolute && toler_dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
						os << "// generate_non_member_operations is_less " << t1 << " " << t2 << endl;
						os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
						os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
						os << var_decl(dynamic, toler_absolute, toler_dim, "meters", num_types[t3], "toler", 1) << endl;
						os << "bool b = is_less(m1, m2, toler);" << endl;
						os << "if (b) return 1;" << endl;
						os << "@END" << endl;
					}
				}
			}
		}
	}

	// is_less_or_equal
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			for (int t3 = 0; t3 < sz_num_types; ++t3)
			{
				for (int toler_abs = 0; toler_abs < 2; ++toler_abs)
				{
					bool toler_absolute = toler_abs == 1;
					for (int toler_dim = 1; toler_dim <= dim; ++toler_dim)
					{
						os << (dim == 1 && ! toler_absolute && toler_dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
						os << "// generate_non_member_operations is_less_or_equal " << t1 << " " << t2 << endl;
						os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
						os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
						os << var_decl(dynamic, toler_absolute, toler_dim, "meters", num_types[t3], "toler", 1) << endl;
						os << "bool b = is_less_or_equal(m1, m2, toler);" << endl;
						os << "if (b) return 1;" << endl;
						os << "@END" << endl;
					}
				}
			}
		}
	}
	
	// midpoint
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			for (int t3 = 0; t3 < sz_num_types; ++t3)
			{
				os << (absolute ? "@LEGAL" : "@ILLEGAL") << endl;
				os << "// generate_non_member_operations midpoint " << t1 << " " << t2 << endl;
				os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
				os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
				os << num_types[t3] << " w = 3;" << endl;
				os << "midpoint(m1, m2, w);" << endl;
				os << "@END" << endl;
			}
		}
	}

	// barycentric_combination
	/*
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			for (int t3 = 0; t3 < sz_num_types; ++t3)
			{
				os << (absolute ? "@LEGAL" : "@ILLEGAL") << endl;
				os << "// generate_non_member_operations barycentric_combination " << t1 << " " << t2 << endl;
				os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
				os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
				temp_decl(dynamic, absolute, dim, "meters", num_types[t1], dim)
				os << num_types[t3] << " w = 3;" << endl;
				os << measure_type(dynamic, absolute, dim, unit, num_type) << " m[] = { ";
				point1<meters>(2), point1<meters>(22), point1<meters>(222) };

				
				os << measure_type(dynamic, absolute, dim, unit, num_type) << " bc = barycentric_combination(3, m, w);" << endl;
				os << "@END" << endl;
			}
		}
	}
	*/
	
	// operator + 1
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (! absolute ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations + 1 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "m1 + m2;" << endl;
			os << "@END" << endl;
		}
	}

	// operator + 2
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (absolute ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations + 2 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, ! absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "m1 + m2;" << endl;
			os << "@END" << endl;
		}
	}

	// operator - 1
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << "@LEGAL" << endl;
			os << "// generate_non_member_operations - 1 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "m1 - m2;" << endl;
			os << "@END" << endl;
		}
	}

	// operator - 2
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (absolute ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations - 2 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, ! absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "m1 - m2;" << endl;
			os << "@END" << endl;
		}
	}

	// operator * 1
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (! absolute ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations * 1 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << num_types[t2] << " n = 3;" << endl;
			os << "m1 * n;" << endl;
			os << "@END" << endl;
		}
	}

	// operator * 2
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (! absolute ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations * 2 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << num_types[t2] << " n = 3;" << endl;
			os << "n * m1;" << endl;
			os << "@END" << endl;
		}
	}

	// operator / 1
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (! absolute ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations / 1 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << num_types[t2] << " n = 3;" << endl;
			os << "m1 / n;" << endl;
			os << "@END" << endl;
		}
	}

	// operator / 2
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		for (int t2 = 0; t2 < sz_num_types; ++t2)
		{
			os << (! absolute && dim == 1 ? "@LEGAL" : "@ILLEGAL") << endl;
			os << "// generate_non_member_operations / 2 " << t1 << " " << t2 << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_types[t2], "m2", 0) << endl;
			os << "m1 / m2;" << endl;
			os << "@END" << endl;
		}
	}

    // squared_norm_value
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		os << (! absolute ? "@LEGAL" : "@ILLEGAL") << endl;
		os << "// generate_non_member_operations squared_norm_value " << t1 << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
		os << num_types[t1] << " snv = squared_norm_value(m1);" << endl;
		os << "if (snv < 0) return 1;" << endl;
		os << "@END" << endl;
	}

    // norm
	for (int t1 = 0; t1 < sz_num_types; ++t1)
	{
		os << (! absolute ? "@LEGAL" : "@ILLEGAL") << endl;
		os << "// generate_non_member_operations norm " << t1 << " " << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_types[t1], "m1", 0) << endl;
		os << measure_type(dynamic, absolute, 1, "meters", num_types[t1]) << " n = norm(m1);" << endl;
		os << "@END" << endl;
	}

/*
v1
	    // vect1 == vect1 -> bool
	    // vect1 != vect1 -> bool
	    // vect1 < vect1 -> bool
	    // vect1 <= vect1 -> bool
	    // vect1 > vect1 -> bool
	    // vect1 >= vect1 -> bool
	    // is_equal(vect1, vect1, tolerance) -> bool
	    // is_less(vect1, vect1, tolerance) -> bool
	    // is_less_or_equal(vect1, vect1, tolerance) -> bool
p1
	    // midpoint(point1, point1) -> point1
    // barycentric_combination(int, point1[], Num[]) -> point1
		// point1 - point1 -> vect1
	    // point1 == point1 -> bool
	    // point1 != point1 -> bool
	    // point1 < point1 -> bool
	    // point1 <= point1 -> bool
	    // point1 > point1 -> bool
	    // point1 >= point1 -> bool
	    // is_equal(point1, point1, tolerance) -> bool
	    // is_less(point1, point1, tolerance) -> bool
	    // is_less_or_equal(point1, point1, tolerance) -> bool
		// point1 + vect1 -> point1
		// point1 - vect1 -> point1
		// vect1 + vect1 -> vect1
		// vect1 - vect1 -> vect1
		// N * vect1 -> vect1
		// vect1 * N -> vect1
		// vect1 / N -> vect1
		// vect1 / vect1 -> N
    // squared_norm_value
    // norm
v2
	    // vect2 == vect2 -> bool
	    // vect2 != vect2 -> bool
	    // is_equal(vect2, vect2, tolerance) -> bool
p2
	    // midpoint(point2, point2) -> point2
    // barycentric_combination(int, point2[], Num[]) -> point2
		// point2 - point2 -> vect2
	    // point2 == point2 -> bool
	    // point2 != point2 -> bool
	    // is_equal(point2, point2, tolerance) -> bool
		// point2 + vect2 -> point2
		// point2 - vect2 -> point2
		// vect2 + vect2 -> vect2
		// vect2 - vect2 -> vect2
		// N * vect2 -> vect2
		// vect2 * N -> vect2
		// vect2 / N -> vect2
    // squared_norm_value
    // norm
v3
	    // vect3 == vect3 -> bool
	    // vect3 != vect3 -> bool
	    // is_equal(vect3, vect3, tolerance) -> bool
p3
	    // midpoint(point3, point3) -> point3
    // barycentric_combination(int, point3[], Num[]) -> point3
		// point3 - point3 -> vect3
	    // point3 == point3 -> bool
	    // point3 != point3 -> bool
	    // is_equal(point3, point3, tolerance) -> bool
		// point3 + vect3 -> point3
		// point3 - vect3 -> point3
		// vect3 + vect3 -> vect3
		// vect3 - vect3 -> vect3
		// N * vect3 -> vect3
		// vect3 * N -> vect3
		// vect3 / N -> vect3
    // squared_norm_value
    // norm

DV1
	    // dyn_vect1 == dyn_vect1 -> bool
	    // dyn_vect1 != dyn_vect1 -> bool
	    // dyn_vect1 < dyn_vect1 -> bool
	    // dyn_vect1 <= dyn_vect1 -> bool
	    // dyn_vect1 > dyn_vect1 -> bool
	    // dyn_vect1 >= dyn_vect1 -> bool
	    // is_equal(dyn_vect1, dyn_vect1, tolerance) -> bool
	    // is_less(dyn_vect1, dyn_vect1, tolerance) -> bool
	    // is_less_or_equal(dyn_vect1, dyn_vect1, tolerance) -> bool
DP1
	    // midpoint(dyn_point1, dyn_point1) -> dyn_point1
    // barycentric_combination(int, dyn_point1[], Num[]) -> dyn_point1
		// dyn_point1 - dyn_point1 -> dyn_vect1
	    // dyn_point1 == dyn_point1 -> bool
	    // dyn_point1 != dyn_point1 -> bool
	    // dyn_point1 < dyn_point1 -> bool
	    // dyn_point1 <= dyn_point1 -> bool
	    // dyn_point1 > dyn_point1 -> bool
	    // dyn_point1 >= dyn_point1 -> bool
	    // is_equal(dyn_point1, dyn_point1, tolerance) -> bool
	    // is_less(dyn_point1, dyn_point1, tolerance) -> bool
	    // is_less_or_equal(dyn_point1, dyn_point1, tolerance) -> bool
		// dyn_point1 + dyn_vect1 -> dyn_point1
		// dyn_point1 - dyn_vect1 -> dyn_point1
		// dyn_vect1 + dyn_vect1 -> dyn_vect1
		// dyn_vect1 - dyn_vect1 -> dyn_vect1
		// N * dyn_vect1 -> dyn_vect1
		// dyn_vect1 * N -> dyn_vect1
		// dyn_vect1 / N -> dyn_vect1
		// dyn_vect1 / dyn_vect1 -> N
    // squared_norm_value
    // norm
DV2
	    // dyn_vect2 == dyn_vect2 -> bool
	    // dyn_vect2 != dyn_vect2 -> bool
	    // is_equal(dyn_vect2, dyn_vect2, tolerance) -> bool
DP2
	    // midpoint(dyn_point2, dyn_point2) -> dyn_point2
    // barycentric_combination(int, dyn_point2[], Num[]) -> dyn_point2
		// dyn_point2 - dyn_point2 -> dyn_vect2
	    // dyn_point2 == dyn_point2 -> bool
	    // dyn_point2 != dyn_point2 -> bool
	    // is_equal(dyn_point2, dyn_point2, tolerance) -> bool
		// dyn_point2 + dyn_vect2 -> dyn_point2
		// dyn_point2 - dyn_vect2 -> dyn_point2
		// dyn_vect2 + dyn_vect2 -> dyn_vect2
		// dyn_vect2 - dyn_vect2 -> dyn_vect2
		// N * dyn_vect2 -> dyn_vect2
		// dyn_vect2 * N -> dyn_vect2
		// dyn_vect2 / N -> dyn_vect2
    // squared_norm_value
    // norm
DV3
	    // dyn_vect3 == dyn_vect3 -> bool
	    // dyn_vect3 != dyn_vect3 -> bool
	    // is_equal(dyn_vect3, dyn_vect3, tolerance) -> bool
DP3
	    // midpoint(dyn_point3, dyn_point3) -> dyn_point3
    // barycentric_combination(int, dyn_point3[], Num[]) -> dyn_point3
		// dyn_point3 - dyn_point3 -> dyn_vect3
	    // dyn_point3 == dyn_point3 -> bool
	    // dyn_point3 != dyn_point3 -> bool
	    // is_equal(dyn_point3, dyn_point3, tolerance) -> bool
		// dyn_point3 + dyn_vect3 -> dyn_point3
		// dyn_point3 - dyn_vect3 -> dyn_point3
		// dyn_vect3 + dyn_vect3 -> dyn_vect3
		// dyn_vect3 - dyn_vect3 -> dyn_vect3
		// N * dyn_vect3 -> dyn_vect3
		// dyn_vect3 * N -> dyn_vect3
		// dyn_vect3 / N -> dyn_vect3
    // squared_norm_value
    // norm
*/	
}

void generate_vect_non_member_operations_with_different_units(ostream& os, bool dynamic, int dim)
{
    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_vect_non_member_operations_with_different_units 1" << endl;
    os << var_decl(dynamic, false, dim, "meters", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, dim, "inches", "", "m2", 0) << endl;
    os << "m1 == m2;" << endl;
    os << "@END" << endl;

    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_vect_non_member_operations_with_different_units 2" << endl;
    os << var_decl(dynamic, false, dim, "meters", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, dim, "inches", "", "m2", 0) << endl;
    os << "m1 != m2;" << endl;
    os << "@END" << endl;

    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_vect_non_member_operations_with_different_units 3" << endl;
    os << var_decl(dynamic, false, dim, "meters", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, dim, "inches", "", "m2", 0) << endl;
    os << "m1 + m2;" << endl;
    os << "@END" << endl;

    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_vect_non_member_operations_with_different_units 4" << endl;
    os << var_decl(dynamic, false, dim, "meters", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, dim, "inches", "", "m2", 0) << endl;
    os << "m1 - m2;" << endl;
    os << "@END" << endl;
}

void generate_vect_non_member_operations_with_different_number_types(ostream& os, bool dynamic, int dim)
{
    os << "@ILLEGAL" << endl;
	os << "// generate_vect_non_member_operations_with_different_number_types 1" << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "mf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "md", 0) << endl;
    os << "mf == md;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_vect_non_member_operations_with_different_number_types 2" << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "mf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "md", 0) << endl;
    os << "mf != md;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_vect_non_member_operations_with_different_number_types 3" << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "mf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "md", 0) << endl;
    os << "mf != md;" << endl;
    os << "@END" << endl;
	
    os << "@ILLEGAL" << endl;
	os << "// generate_vect_non_member_operations_with_different_number_types 4" << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "mf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "md", 0) << endl;
    os << "md1 == mf1;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_vect_non_member_operations_with_different_number_types 5" << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "mf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "md", 0) << endl;
    os << "md1 != mf1;" << endl;
    os << "@END" << endl;

    os << "@LEGAL" << endl;
	os << "// generate_vect_non_member_operations_with_different_number_types 6" << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "mf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "md", 0) << endl;
    os << "mf + md;" << endl;
    os << "md + mf;" << endl;
    os << "mf - md;" << endl;
    os << "md - mf;" << endl;
    os << "2.1 * mf;" << endl;
    os << "2.1f * md;" << endl;
    os << "mf * 2.1;" << endl;
    os << "md * 2.1f;" << endl;
    os << "mf / 2.1;" << endl;
    os << "md / 2.1f;" << endl;
    os << "@END" << endl;
}

void generate_point_non_member_operations_with_same_unit_and_number_type(ostream& os, bool dynamic, int dim)
{
    os << "@LEGAL" << endl;
	os << "// generate_point_non_member_operations_with_same_unit_and_number_type" << endl;
    os << var_decl(dynamic, true, dim, "meters", "float", "pf", 0) << endl;
    os << var_decl(dynamic, true, dim, "meters", "double", "pd", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
    os << "bool bf = pf == pf;" << endl;
    os << "bool bd = pd == pd;" << endl;
    os << "if (bf || bd) return 1;" << endl;
    os << "pf = pf + vf;" << endl;
    os << "pd = pd + vd;" << endl;
    os << "pf = pf - vf;" << endl;
    os << "pd = pd - vd;" << endl;
    os << "@END" << endl;
}

void generate_point_non_member_operations_with_different_units(ostream& os, bool dynamic, int dim)
{
    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_point_non_member_operations_with_different_units 1" << endl;
    os << var_decl(dynamic, true, dim, "meters", "", "p1", 0) << endl;
    os << var_decl(dynamic, true, dim, "inches", "", "p2", 0) << endl;
    os << "p1 == p2;" << endl;
    os << "@END" << endl;

    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_point_non_member_operations_with_different_units 2" << endl;
    os << var_decl(dynamic, true, dim, "meters", "", "p", 0) << endl;
    os << var_decl(dynamic, false, dim, "inches", "", "v", 0) << endl;
    os << "p + v;" << endl;
    os << "@END" << endl;

    os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_point_non_member_operations_with_different_units 3" << endl;
    os << var_decl(dynamic, true, dim, "meters", "", "p", 0) << endl;
    os << var_decl(dynamic, false, dim, "inches", "", "v", 0) << endl;
    os << "p - v;" << endl;
    os << "@END" << endl;
}

void generate_point_non_member_operations_with_different_number_types(ostream& os, bool dynamic, int dim)
{
    os << "@ILLEGAL" << endl;
	os << "// generate_point_non_member_operations_with_different_number_types 1" << endl;
    os << var_decl(dynamic, true, dim, "meters", "float", "p1", 0) << endl;
    os << var_decl(dynamic, true, dim, "meters", "double", "p2", 0) << endl;
    os << "p1 == p2;" << endl;
    os << "@END" << endl;

    os << "@LEGAL" << endl;
	os << "// generate_point_non_member_operations_with_different_number_types 2" << endl;
    os << var_decl(dynamic, true, dim, "meters", "float", "pf", 0) << endl;
    os << var_decl(dynamic, true, dim, "meters", "double", "pd", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "float", "vf", 0) << endl;
    os << var_decl(dynamic, false, dim, "meters", "double", "vd", 0) << endl;
    os << "pf + vd;" << endl;
    os << "pd + vf;" << endl;
    os << "pf - vd;" << endl;
    os << "pd - vf;" << endl;
    os << "@END" << endl;
}

void generate_conversion(ostream& os, bool dynamic,
    const string& var_dest, const string& var_source, const string& unit_dest, const string& unit_source = "")
{
    os << var_dest << " = convert";
    if (! dynamic)
    {
		os << "<";
        os << unit_dest;
        if (unit_source.size() > 0) os << "," << unit_source;
		os << ">";
    }
    os << "(";
    if (dynamic) os << unit_dest << "::id(), ";
    os << var_source << ");" << endl;
}

void generate_conversion_set(ostream& os, bool dynamic,
    const string& var1, const string& var2, const string& var3,
    const string& unit1, const string& unit2, const string& unit3)
{
    generate_conversion(os, dynamic, var1, var2, unit1);
    generate_conversion(os, dynamic, var1, var2, unit1, unit2);
    generate_conversion(os, dynamic, var1, var3, unit1);
    generate_conversion(os, dynamic, var1, var3, unit1, unit3);
}

void generate_conversions_with_same_magnitude_and_same_number_type(ostream& os, bool dynamic, bool absolute, int dim, const string& num_type)
{
    os << "@LEGAL" << endl;
	os << "// generate_conversions_with_same_magnitude_and_same_number_type" << endl;
    os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "inches", num_type, "m2", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "mm", num_type, "m3", 0) << endl;
    generate_conversion_set(os, dynamic, "m1", "m2", "m3", "meters", "inches", "mm");
    generate_conversion_set(os, dynamic, "m2", "m3", "m1", "inches", "mm", "meters");
    generate_conversion_set(os, dynamic, "m3", "m1", "m2", "mm", "meters", "inches");
    os << "@END" << endl;
}

void generate_conversions_with_different_number_types(
    ostream& os, bool dynamic, bool absolute, int dim,
    const string& num_type1, const string& num_type2)
{
    os << "@ILLEGAL" << endl;
	os << "// generate_conversions_with_different_number_types 1" << endl;
    os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "inches", num_type2, "m2", 0) << endl;
    generate_conversion(os, dynamic, "m1", "m2", "meters");
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_conversions_with_different_number_types 2" << endl;
    os << var_decl(dynamic, absolute, dim, "meters", num_type2, "m3", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "inches", num_type1, "m4", 0) << endl;
    generate_conversion(os, dynamic, "m3", "m4", "meters");
    os << "@END" << endl;
}

void generate_conversions_with_different_magnitudes(ostream& os, bool dynamic, bool absolute, int dim, const string& num_type)
{
    os << "@ILLEGAL" << endl;
	os << "// generate_conversions_with_different_magnitudes 1" << endl;
    os << var_decl(false, absolute, dim, "meters", num_type, "m1", 0) << endl;
    os << var_decl(false, absolute, dim, "seconds", num_type, "m2", 0) << endl;
    generate_conversion(os, false, "m1", "m2", "meters");
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_conversions_with_different_magnitudes 2" << endl;
    os << var_decl(false, absolute, dim, "meters", num_type, "m1", 0) << endl;
    os << var_decl(false, absolute, dim, "seconds", num_type, "m2", 0) << endl;
    generate_conversion(os, false, "m1", "m2", "meters", "seconds");
    os << "@END" << endl;
}

void generate_defined_derived_units_with_same_number_type(ostream& os, bool dynamic, bool absolute, int dim, const string& num_type)
{
	if (absolute)
	{
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 1" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << "m3 = m1 / m2;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 2" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << "m2 = m1 / m3;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 3" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << "m1 = m3 * m2;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 4" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << "m1 = m2 * m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 5" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << "m4 = m3 / m2;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 6" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << "m2 = m3 / m4;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 7" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << "m3 = m4 * m2;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 8" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << "m3 = m2 * m4;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 9" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
		os << "m4 = m1 / m5;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 10" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
		os << "m5 = m1 / m4;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 11" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
		os << "m1 = m4 * m5;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 12" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
		os << "m1 = m5 * m4;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 13" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
		os << "m2 = m5 / m2;" << endl;
		os << "@END" << endl;
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 14" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
		os << "m5 = m2 * m2;" << endl;
		os << "@END" << endl;
	}
	else
	{
		os << "@LEGAL" << endl;
		os << "// generate_defined_derived_units_with_same_number_type 15" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
		os << "m3 = m1 / m2;" << endl;
		if (dim == 1) os << "m2 = m1 / m3;" << endl;
		os << "m1 = m3 * m2;" << endl;
		os << "m1 = m2 * m3;" << endl;

		os << "m4 = m3 / m2;" << endl;
		if (dim == 1) os << "m2 = m3 / m4;" << endl;
		os << "m3 = m4 * m2;" << endl;
		os << "m3 = m2 * m4;" << endl;

		os << "m4 = m1 / m5;" << endl;
		if (dim == 1) os << "m5 = m1 / m4;" << endl;
		os << "m1 = m4 * m5;" << endl;
		os << "m1 = m5 * m4;" << endl;

		os << "m2 = m5 / m2;" << endl;
		os << "m5 = m2 * m2;" << endl;

		os << "@END" << endl;

		if (dim > 1)
		{
			os << "@ILLEGAL" << endl;
			os << "// generate_defined_derived_units_with_same_number_type 16" << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
			os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
			os << "m2 = m1 / m3;" << endl;
			os << "@END" << endl;
		}
		
		if (dim > 1)
		{
			os << "@ILLEGAL" << endl;
			os << "// generate_defined_derived_units_with_same_number_type 17" << endl;
			os << var_decl(dynamic, absolute, 1, "seconds", num_type, "m2", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type, "m3", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
			os << "m2 = m3 / m4;" << endl;
			os << "@END" << endl;
		}

		if (dim > 1)
		{
			os << "@ILLEGAL" << endl;
			os << "// generate_defined_derived_units_with_same_number_type 18" << endl;
			os << var_decl(dynamic, absolute, dim, "meters", num_type, "m1", 0) << endl;
			os << var_decl(dynamic, absolute, dim, "meters_per_squared_second", num_type, "m4", 0) << endl;
			os << var_decl(dynamic, absolute, 1, "squared_seconds", num_type, "m5", 0) << endl;
			os << "m5 = m1 / m4;" << endl;
			os << "@END" << endl;
		}
	}
}

void generate_undefined_derived_units(ostream& os, bool dynamic, bool absolute, int dim)
{
    os << "@ILLEGAL" << endl;
	os << "// generate_undefined_derived_units 1" << endl;
    os << var_decl(dynamic, absolute, dim, "km", "", "m1", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "hours", "", "m2", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "km_per_hours", "", "m3", 0) << endl;
    os << "m3 = m1 / m2;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_undefined_derived_units 2" << endl;
    os << var_decl(dynamic, absolute, dim, "km", "", "m1", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "hours", "", "m2", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "km_per_hours", "", "m3", 0) << endl;
    os << "m2 = m1 / m3;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_undefined_derived_units 3" << endl;
    os << var_decl(dynamic, absolute, dim, "km", "", "m1", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "hours", "", "m2", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "km_per_hours", "", "m3", 0) << endl;
    os << "m1 = m3 * m2;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_undefined_derived_units 4" << endl;
    os << var_decl(dynamic, absolute, dim, "km", "", "m1", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "hours", "", "m2", 0) << endl;
    os << var_decl(dynamic, absolute, dim, "km_per_hours", "", "m3", 0) << endl;
    os << "m1 = m2 * m3;" << endl;
    os << "@END" << endl;
}

void generate_defined_derived_units_with_different_number_types(ostream& os, bool dynamic, bool absolute, int dim, const string& num_type1, const string& num_type2)
{
	if (dim == 1)
	{
		os << (absolute ? "@ILLEGAL" : "@LEGAL") << endl;
		os << "// generate_defined_derived_units_with_different_number_types 1" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << "m1 / m2;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 2" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m3 = m1 / m2;" << endl;
		os << "@END" << endl;

		os << (absolute ? "@ILLEGAL" : "@LEGAL") << endl;
		os << "// generate_defined_derived_units_with_different_number_types 3" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 / m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 4" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type1, "m3", 0) << endl;
		os << "m2 = m1 / m3;" << endl;
		os << "@END" << endl;

		os << (absolute ? "@ILLEGAL" : "@LEGAL") << endl;
		os << "// generate_defined_derived_units_with_different_number_types 5" << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m3 * m2;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 6" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 = m3 * m2;" << endl;
		os << "@END" << endl;

		os << (absolute ? "@ILLEGAL" : "@LEGAL") << endl;
		os << "// generate_defined_derived_units_with_different_number_types 7" << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m2 * m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 8" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 = m2 * m3;" << endl;
		os << "@END" << endl;
	}
	else
	{
		os << (absolute ? "@ILLEGAL" : "@LEGAL") << endl;
		os << "// generate_defined_derived_units_with_different_number_types 9" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type2, "m2", 0) << endl;
		os << "m1 / m2;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 10" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << "m1 / m2;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 11" << endl;
		os << var_decl(dynamic, absolute, 1, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << "m1 / m2;" << endl;
		os << "@END" << endl;
		
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 12" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m3 = m1 / m2;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 13" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m3 = m1 / m2;" << endl;
		os << "@END" << endl;

		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 14" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 / m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 15" << endl;
		os << var_decl(dynamic, absolute, 1, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 / m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 16" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 / m3;" << endl;
		os << "@END" << endl;


		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 17" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type1, "m3", 0) << endl;
		os << "m2 = m1 / m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 18" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type1, "m3", 0) << endl;
		os << "m2 = m1 / m3;" << endl;
		os << "@END" << endl;
		
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 19" << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m3 * m2;" << endl;
		os << "@END" << endl;

		os << (absolute ? "@ILLEGAL" : "@LEGAL") << endl;
		os << "// generate_defined_derived_units_with_different_number_types 20" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m3 * m2;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 21" << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m3 * m2;" << endl;
		os << "@END" << endl;
		
		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 22" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 = m3 * m2;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 23" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 = m3 * m2;" << endl;
		os << "@END" << endl;

		
		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 24" << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m2 * m3;" << endl;
		os << "@END" << endl;

		os << (absolute ? "@ILLEGAL" : "@LEGAL") << endl;
		os << "// generate_defined_derived_units_with_different_number_types 25" << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m2 * m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 26" << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type1, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m2 * m3;" << endl;
		os << "@END" << endl;


		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 27" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 = m2 * m3;" << endl;
		os << "@END" << endl;

		os << "@ILLEGAL" << endl;
		os << "// generate_defined_derived_units_with_different_number_types 28" << endl;
		os << var_decl(dynamic, absolute, dim, "meters", num_type1, "m1", 0) << endl;
		os << var_decl(dynamic, absolute, 1, "seconds", num_type2, "m2", 0) << endl;
		os << var_decl(dynamic, absolute, dim, "meters_per_second", num_type2, "m3", 0) << endl;
		os << "m1 = m2 * m3;" << endl;
		os << "@END" << endl;
	}
}

// ANGLES

string angle_type(bool dynamic, bool signed_angle, const string& unit, const string& num_type)
{
    string result = dynamic ? "dyn_": "";
    result += signed_angle ? "signed_azimuth" : "unsigned_azimuth";
    result += "<";
    if (! dynamic)
    {
        result += unit;
        if (num_type.size() > 0) result += ",";
    }
    result += num_type;
    result += ">";
    return result;
}

string var_az_decl(bool dynamic, bool signed_angle, const string& unit, const string& num_type, const string& var_name, int n_values)
{
    string result = angle_type(dynamic, signed_angle, unit, num_type);
    result += " ";
    result += var_name;
    result += measure_init_list(dynamic, unit, n_values, num_type);
    result += ";";
    return result;
}

string var_az_decl_copy(bool dynamic, bool signed_angle, const string& unit, const string& num_type, const string& var_name, const string& arguments)
{
    string result = angle_type(dynamic, signed_angle, unit, num_type);
    result += " ";
    result += var_name;
    result += "(";
    result += arguments;
    result += ");";
    return result;
}

void generate_az_type_with_no_unit(ostream& os,
	bool dynamic, bool signed_angle)
{
	os << "@LEGAL" << endl;
	os << "// generate_az_type_with_no_unit" << endl;
	os << var_az_decl(dynamic, signed_angle, "", "", "m1", 0) << endl;
	os << "@END" << endl;
}

void generate_az_construction_from_numbers(ostream& os, bool dynamic, bool signed_angle)
{
    for (int d = 0; d <= 1; ++d)
    {
        os << "@LEGAL" << endl;
		os << "// generate_az_construction_from_numbers 1" << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", d) << endl;
        os << var_az_decl(dynamic, signed_angle, "degrees", "", "m2", d) << endl;
        os << var_az_decl(dynamic, signed_angle, "turns", "", "m3", d) << endl;
        os << var_az_decl(dynamic, signed_angle, "gradians", "", "m4", d) << endl;
        os << "@END" << endl;
    }

    os << "@ILLEGAL" << endl;
	os << "// generate_az_construction_from_numbers 2" << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 2) << endl;
    os << "@END" << endl;
}

void generate_az_construction_from_another_same_unit_angle(ostream& os, bool dynamic, bool signed_angle)
{
    os << "@LEGAL" << endl;
	os << "// generate_az_construction_from_another_same_unit_angle 1" << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 0) << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "", "m1_1", "m1") << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "double", "m1_2", "m1") << endl;

    os << var_az_decl(dynamic, signed_angle, "radians", "float", "m2", 0) << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "float", "m2_1", "m2") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_az_construction_from_another_same_unit_angle 2" << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 0) << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "float", "m1_1", "m1") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_az_construction_from_another_same_unit_angle 3" << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", "double", "m1", 0) << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "float", "m1_1", "m1") << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
	os << "// generate_az_construction_from_another_same_unit_angle 4" << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", "float", "m1", 0) << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "double", "m1_1", "m1") << endl;
    os << "@END" << endl;
}

void generate_az_construction_from_another_different_unit_angle(ostream& os, bool dynamic, bool signed_angle)
{
	os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_az_construction_from_another_different_unit_angle" << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 0) << endl;
	os << var_az_decl_copy(dynamic, signed_angle, "degrees", "", "m1_1", "m1") << endl;
	os << "@END" << endl;
}

void generate_az_construction_from_another_different_kind_angle(ostream& os, bool dynamic, bool signed_angle)
{
	// An azimuth cannot be created from a non-angle.
    os << "@ILLEGAL" << endl;
	os << "// generate_az_construction_from_another_different_kind_angle 1" << endl;
    os << var_decl(dynamic, true, 1, "meters", "", "m1", 0) << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "", "m1_1", "m1") << endl;
    os << "@END" << endl;

	// An azimuth cannot be created from a relative angle.
    os << "@ILLEGAL" << endl;
	os << "// generate_az_construction_from_another_different_kind_angle 2" << endl;
    os << var_decl(dynamic, false, 1, "radians", "", "m1", 0) << endl;
    os << var_az_decl_copy(dynamic, signed_angle, "radians", "", "m1_1", "m1") << endl;
    os << "@END" << endl;

	// A relative angle cannot be created from an azimuth.
    os << "@ILLEGAL" << endl;
	os << "// generate_az_construction_from_another_different_kind_angle 3" << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 0) << endl;
    os << var_decl_copy(dynamic, false, 1, "radians", "", "m1_1", "m1") << endl;
    os << "@END" << endl;

	os << "@LEGAL" << endl;
	os << "// generate_az_construction_from_another_different_kind_angle 4" << endl;
    os << var_decl(dynamic, true, 1, "radians", "", "m1", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "", "m2", 0) << endl;

	// An azimuth can be created from an absolute angle.
	os << var_az_decl_copy(dynamic, signed_angle, "radians", "", "m1_1", "m1") << endl;

	// An azimuth can be created even from an azimuth of different kind.
	os << var_az_decl_copy(dynamic, ! signed_angle, "radians", "", "m2_2", "m2") << endl;

	// An absolute angle can be created from an azimuth.
	os << var_decl_copy(dynamic, true, 1, "radians", "", "m2_1", "m2") << endl;
	os << "@END" << endl;
}

void generate_az_member_operations_with_same_unit(ostream& os, bool dynamic, bool signed_angle)
{
	// To any azimuth, a measure can be added or subtracted, as long as
	// the measure is a 1-dimension vector that has the same unit
	// and the same numer type of the azimuth.
	os << "@LEGAL" << endl;
	os << "// generate_az_member_operations_with_same_unit 1" << endl;
	os << var_decl(dynamic, false, 1, "radians", "float", "vf", 0) << endl;
	os << var_decl(dynamic, false, 1, "radians", "double", "vd", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "float", "af", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "double", "ad", 0) << endl;
	os << "af += vf;" << endl;
	os << "ad += vd;" << endl;
	os << "af -= vf;" << endl;
	os << "ad -= vd;" << endl;
	os << "@END" << endl;

	// To any azimuth, a 2-dimension measure cannot be added.
	os << "@ILLEGAL" << endl;
	os << "// generate_az_member_operations_with_same_unit 2" << endl;
	os << var_decl(dynamic, false, 2, "radians", "double", "vd", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "double", "ad", 0) << endl;
	os << "ad += vd;" << endl;
	os << "@END" << endl;

	// To any azimuth, a 2-dimension measure cannot be subtracted.
	os << "@ILLEGAL" << endl;
	os << "// generate_az_member_operations_with_same_unit 2" << endl;
	os << var_decl(dynamic, false, 2, "radians", "double", "vd", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "double", "ad", 0) << endl;
	os << "ad -= vd;" << endl;
	os << "@END" << endl;
	
	// To any azimuth having a float number type,
	// a measure with a double number type cannot be added.
	os << "@ILLEGAL" << endl;
	os << "// generate_az_member_operations_with_same_unit 2" << endl;
	os << var_decl(dynamic, false, 1, "radians", "double", "vd", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "float", "af", 0) << endl;
	os << "af += vd;" << endl;
	os << "@END" << endl;

	// To any azimuth having a double number type,
	// a measure with a float number type cannot be added.
	os << "@ILLEGAL" << endl;
	os << "// generate_az_member_operations_with_same_unit 3" << endl;
	os << var_decl(dynamic, false, 1, "radians", "float", "vf", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "double", "ad", 0) << endl;
	os << "ad += vf;" << endl;
	os << "@END" << endl;

	// To any azimuth having a float number type,
	// a measure with a double number type cannot be subtracted.
	os << "@ILLEGAL" << endl;
	os << "// generate_az_member_operations_with_same_unit 4" << endl;
	os << var_decl(dynamic, false, 1, "radians", "double", "vd", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "float", "af", 0) << endl;
	os << "af -= vd;" << endl;
	os << "@END" << endl;

	// To any azimuth having a double number type,
	// a measure with a float number type cannot be subtracted.
	os << "@ILLEGAL" << endl;
	os << "// generate_az_member_operations_with_same_unit 5" << endl;
	os << var_decl(dynamic, false, 1, "radians", "float", "vf", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "radians", "double", "ad", 0) << endl;
	os << "ad -= vf;" << endl;
	os << "@END" << endl;
}

void generate_az_member_operations_with_different_units(ostream& os, bool dynamic, bool signed_angle)
{
	// To any dynamic azimuth, a dynamic measure can be added
	// or subtracted, as long as the measure is a 1-dimension vector
	// that has the same magnitude and the same numer type.
	os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
	os << "// generate_az_member_operations_with_different_units 1" << endl;
	os << var_decl(dynamic, false, 1, "turns", "float", "vf", 0) << endl;
	os << var_decl(dynamic, false, 1, "turns", "double", "vd", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "degrees", "float", "af", 0) << endl;
	os << var_az_decl(dynamic, signed_angle, "degrees", "double", "ad", 0) << endl;
	os << "af += vf;" << endl;
	os << "ad += vd;" << endl;
	os << "af -= vf;" << endl;
	os << "ad -= vd;" << endl;
	os << "@END" << endl;	
}

void generate_az_non_member_operations_with_same_unit_and_number_type(ostream& os, bool dynamic, bool signed_angle)
{
    if (signed_angle)
    {
        os << "@LEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "float", "mf1", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "float", "mf2", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "double", "md1", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "double", "md2", 0) << endl;
        os << "bool bf = mf1 == mf2;" << endl;
        os << "bool bd = md1 == md2;" << endl;
        os << "if (bf || bd) return 1;" << endl;
        os << "mf1 = mf1 + mf2;" << endl;
        os << "md1 = md1 + md2;" << endl;
        os << "mf1 = mf1 - mf2;" << endl;
        os << "md1 = md1 - md2;" << endl;
        os << "mf2 = 2.1f * mf1;" << endl;
        os << "md2 = 2.1 * md1;" << endl;
        os << "mf2 = mf1 * 2.1f;" << endl;
        os << "md2 = md1 * 2.1;" << endl;
        os << "mf2 = mf1 / 2.1f;" << endl;
        os << "md2 = md1 / 2.1;" << endl;
        os << "@END" << endl;
    }
    else
    {
        os << "@LEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "float", "pf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "double", "pd", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "float", "vf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "double", "vd", 0) << endl;
        os << "bool bf = pf == pf;" << endl;
        os << "bool bd = pd == pd;" << endl;
        os << "if (bf || bd) return 1;" << endl;
        os << "pf = pf + vf;" << endl;
        os << "pd = pd + vd;" << endl;
        os << "pf = pf - vf;" << endl;
        os << "pd = pd - vd;" << endl;
        os << "@END" << endl;
    }
}

void generate_az_non_member_operations_with_different_units(ostream& os, bool dynamic, bool signed_angle)
{
    if (signed_angle)
    {
        os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "degrees", "", "m2", 0) << endl;
        os << "m1 == m2;" << endl;
        os << "@END" << endl;

        os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "degrees", "", "m2", 0) << endl;
        os << "m1 + m2;" << endl;
        os << "@END" << endl;

        os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "", "m1", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "degrees", "", "m2", 0) << endl;
        os << "m1 - m2;" << endl;
        os << "@END" << endl;
    }
    else
    {
        os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "", "p1", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "degrees", "", "p2", 0) << endl;
        os << "p1 == p2;" << endl;
        os << "@END" << endl;

        os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "", "p", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "degrees", "", "v", 0) << endl;
        os << "p + v;" << endl;
        os << "@END" << endl;

        os << (dynamic ? "@LEGAL" : "@ILLEGAL") << endl;
        os << var_az_decl(dynamic, signed_angle, "radians", "", "p", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "degrees", "", "v", 0) << endl;
        os << "p - v;" << endl;
        os << "@END" << endl;
    }
}

void generate_az_non_member_operations_with_different_number_types(ostream& os, bool dynamic, bool signed_angle)
{
    if (signed_angle)
    {
        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "mf == md;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "md1 == mf1;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "mf + md;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "md + mf;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "mf - md;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "md - mf;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << "2.1 * mf;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "2.1f * md;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << "mf * 2.1;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "md * 2.1f;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "mf", 0) << endl;
        os << "mf / 2.1;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "md", 0) << endl;
        os << "md / 2.1f;" << endl;
        os << "@END" << endl;
    }
    else
    {
        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "p1", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "p2", 0) << endl;
        os << "p1 == p2;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "p", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "v", 0) << endl;
        os << "p + v;" << endl;
        os << "@END" << endl;

        os << "@ILLEGAL" << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "float", "p", 0) << endl;
        os << var_az_decl(dynamic, signed_angle, "meters", "double", "v", 0) << endl;
        os << "p - v;" << endl;
        os << "@END" << endl;
    }
}

void generate_az_conversions_with_same_magnitude_and_same_number_type(
    ostream& os, bool dynamic, bool signed_angle, const string& num_type)
{
    os << "@LEGAL" << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", num_type, "m1", 0) << endl;
    os << var_az_decl(dynamic, signed_angle, "degrees", num_type, "m2", 0) << endl;
    os << var_az_decl(dynamic, signed_angle, "turns", num_type, "m3", 0) << endl;
    generate_conversion_set(os, dynamic, "m1", "m2", "m3", "radians", "degrees", "turns");
    generate_conversion_set(os, dynamic, "m2", "m3", "m1", "degrees", "turns", "radians");
    generate_conversion_set(os, dynamic, "m3", "m1", "m2", "turns", "radians", "degrees");
    os << "@END" << endl;
}

void generate_az_conversions_with_different_number_types(
    ostream& os, bool dynamic, bool signed_angle,
    const string& num_type1, const string& num_type2)
{
    os << "@LEGAL" << endl;

    os << var_az_decl(dynamic, signed_angle, "radians", num_type1, "m1", 0) << endl;
    os << var_az_decl(dynamic, signed_angle, "degrees", num_type2, "m2", 0) << endl;
    generate_conversion(os, dynamic, "m1", "m2", "radians");

    os << var_az_decl(dynamic, signed_angle, "radians", num_type2, "m3", 0) << endl;
    os << var_az_decl(dynamic, signed_angle, "degrees", num_type1, "m4", 0) << endl;
    generate_conversion(os, dynamic, "m3", "m4", "radians");

    os << "@END" << endl;
}

void generate_az_conversions_with_different_magnitudes(
    ostream& os, bool dynamic, bool signed_angle, const string& num_type)
{
    os << "@ILLEGAL" << endl;
    os << var_az_decl(false, signed_angle, "radians", num_type, "m1", 0) << endl;
    os << var_az_decl(false, signed_angle, "seconds", num_type, "m2", 0) << endl;
    generate_conversion(os, false, "m1", "m2", "radians");
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
    os << var_az_decl(false, signed_angle, "radians", num_type, "m1", 0) << endl;
    os << var_az_decl(false, signed_angle, "seconds", num_type, "m2", 0) << endl;
    generate_conversion(os, false, "m1", "m2", "radians", "seconds");
    os << "@END" << endl;
}

void generate_az_defined_derived_units_with_same_number_type(
    ostream& os, bool dynamic, bool signed_angle, const string& num_type)
{
    os << (signed_angle ? "@ILLEGAL" : "@LEGAL") << endl;
    os << var_az_decl(dynamic, signed_angle, "radians", num_type, "m1", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "seconds", num_type, "m2", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "radians_per_second", num_type, "m3", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "radians_per_squared_second", num_type, "m4", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "squared_seconds", num_type, "m5", 0) << endl;
    os << "m3 = m1 / m2;" << endl;
    os << "m2 = m1 / m3;" << endl;
    os << "m1 = m3 * m2;" << endl;
    os << "m1 = m2 * m3;" << endl;

    os << "m4 = m3 / m2;" << endl;
    os << "m2 = m3 / m4;" << endl;
    os << "m3 = m4 * m2;" << endl;
    os << "m3 = m2 * m4;" << endl;

    os << "m4 = m1 / m5;" << endl;
    os << "m5 = m1 / m4;" << endl;
    os << "m1 = m4 * m5;" << endl;
    os << "m1 = m5 * m4;" << endl;

    os << "m2 = m5 / m2;" << endl;
    os << "m5 = m2 * m2;" << endl;

    os << "@END" << endl;
}

void generate_az_undefined_derived_units(ostream& os, bool dynamic, bool signed_angle)
{
    os << "@ILLEGAL" << endl;
    os << var_az_decl(dynamic, signed_angle, "turns", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "minutes", "", "m2", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "turns_per_minute", "", "m3", 0) << endl;
    os << "m3 = m1 / m2;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
    os << var_az_decl(dynamic, signed_angle, "turns", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "minutes", "", "m2", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "turns_per_minute", "", "m3", 0) << endl;
    os << "m2 = m1 / m3;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
    os << var_az_decl(dynamic, signed_angle, "turns", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "minutes", "", "m2", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "turns_per_minute", "", "m3", 0) << endl;
    os << "m1 = m3 * m2;" << endl;
    os << "@END" << endl;

    os << "@ILLEGAL" << endl;
    os << var_az_decl(dynamic, signed_angle, "turns", "", "m1", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "minutes", "", "m2", 0) << endl;
    os << var_decl(dynamic, false, signed_angle, "turns_per_minute", "", "m3", 0) << endl;
    os << "m1 = m2 * m3;" << endl;
    os << "@END" << endl;
}

void generate_program(ostream& os)
{
	// Add the associations between pre-defined unit names and their magnitude names.
	magnitudes["radians"] = "Angle";
//	magnitudes["degrees"] = "Angle";
//	magnitudes["gradians"] = "Angle";
	magnitudes["turns"] = "Angle";
	
//	magnitudes["steradians"] = "SolidAngle";
//	magnitudes["allrounds"] = "SolidAngle";
//	magnitudes["square_degrees"] = "SolidAngle";
	
//	magnitudes["nounit"] = "Unitless";
	
	// Begin generated file with header inclusion.
    os << "#include \"measures.hpp\"" << endl;
    os << "using namespace measures;" << endl;
    os << endl;
	
	// Define alternative angle units.
	os << "DEFINE_UNIT(degrees, Angle, \"°\", 3.1415926535897932384626433832795/180, 0)" << endl; magnitudes["degrees"] = "Angle";
	os << "DEFINE_UNIT(gradians, Angle, \" gon\", 3.1415926535897932384626433832795/200, 0)" << endl; magnitudes["gradians"] = "Angle";
	
	// Define solid angles with three units.
	os << "DEFINE_MAGNITUDE(SolidAngle, steradians, \" sr\")" << endl; magnitudes["steradians"] = "SolidAngle";
	os << "DEFINE_UNIT(allrounds, SolidAngle, \" all-round\", 4*3.1415926535897932384626433832795, 0)" << endl; magnitudes["allrounds"] = "SolidAngle";
	os << "DEFINE_UNIT(square_degrees, SolidAngle, \"°°\", 3.1415926535897932384626433832795/180*3.1415926535897932384626433832795/180, 0)" << endl; magnitudes["square_degrees"] = "SolidAngle";
	
	// Define unitless measures.
	os << "DEFINE_MAGNITUDE(Unitless, nounit, \"\")" << endl; magnitudes["nounit"] = "Unitless";
	
	// Define length and time units and some of their derived units.
    os << "DEFINE_MAGNITUDE(Length, meters, \" m\")" << endl; magnitudes["meters"] = "Length";
    os << "DEFINE_UNIT(inches, Length, \"\\\"\", 0.0254, 0)" << endl; magnitudes["inches"] = "Length";
    os << "DEFINE_UNIT(mm, Length, \" mm\", 0.001, 0)" << endl; magnitudes["mm"] = "Length";
    os << "DEFINE_UNIT(km, Length, \" Km\", 1000, 0)" << endl; magnitudes["km"] = "Length";

    os << "DEFINE_MAGNITUDE(Time, seconds, \" s\")" << endl; magnitudes["seconds"] = "Time";
    os << "DEFINE_UNIT(minutes, Time, \" min\", 60, 0)" << endl; magnitudes["minutes"] = "Time";
    os << "DEFINE_UNIT(hours, Time, \" h\", 3600, 0)" << endl; magnitudes["hours"] = "Time";

    os << "DEFINE_MAGNITUDE(Speed, meters_per_second, \" m/s\")" << endl; magnitudes["meters_per_second"] = "Speed";
    os << "DEFINE_UNIT(km_per_hour, Speed, \" Km/h\", 1/3.6, 0)" << endl; magnitudes["km_per_hour"] = "Speed";

    os << "DEFINE_MAGNITUDE(Acceleration, meters_per_squared_second, \" m/s2\")" << endl; magnitudes["meters_per_squared_second"] = "Acceleration";
    os << "DEFINE_UNIT(km_per_hours_per_second, Acceleration, \" Km/h/s\", 1/3.6, 0)" << endl; magnitudes["km_per_hours_per_second"] = "Acceleration";

    os << "DEFINE_MAGNITUDE(SquaredTime, squared_seconds, \" s2\")" << endl; magnitudes["squared_seconds"] = "SquaredTime";

	// Define the combined operations among the defined units.
    os << "DEFINE_DERIVED_MAGNITUDE_SQUARED_SCALAR(Time, SquaredTime)" << endl;
    os << "DEFINE_DERIVED_MAGNITUDE_SCALAR_VECTOR(Time, Speed, Length)" << endl;
    os << "DEFINE_DERIVED_MAGNITUDE_SCALAR_VECTOR(Time, Acceleration, Speed)" << endl;
    os << "DEFINE_DERIVED_MAGNITUDE_SCALAR_VECTOR(SquaredTime, Acceleration, Length)" << endl;

	// Define the units derived from time and angle units.
    os << "DEFINE_MAGNITUDE(AngularSpeed, radians_per_second, \" rad/s\")" << endl;
    os << "DEFINE_UNIT(turns_per_minute, AngularSpeed, \" rpm\", 60 / (2 * 3.14159265358979323846), 0)" << endl;
    os << "DEFINE_MAGNITUDE(AngularAcceleration, radians_per_squared_second, \" rad/s2\")" << endl;

	// Define the combined operations among angle and derived units.
    os << "DEFINE_DERIVED_MAGNITUDE_SCALAR_SCALAR(Time, AngularSpeed, Angle)" << endl;
    os << "DEFINE_DERIVED_MAGNITUDE_SCALAR_SCALAR(Time, AngularAcceleration, AngularSpeed)" << endl;
    os << "DEFINE_DERIVED_MAGNITUDE_SCALAR_SCALAR(SquaredTime, AngularAcceleration, Angle)" << endl;

	// Define unitless combined operations.
    os << "DEFINE_DERIVED_MAGNITUDE_SQUARED_VECTOR(Unitless, Unitless)" << endl;

	//TODO
    //os << "DEFINE_DERIVED_MAGNITUDE_SQUARED_SCALAR(Angle, SolidAngle)" << endl;
	
	// Begin "main" function.
    os << endl;
    os << "int main()" << endl;
    os << "{" << endl;
	
//#if 0
	// Consider statically-typed and dynamically-typed measures.
//    int i_dynamic = 1;
    for (int i_dynamic = 0; i_dynamic <= 1; ++i_dynamic)
    {
		bool dyn = i_dynamic == 1;
		
		// Consider 1-dimension (i.e. scalar), 2-dimensions, and 3-dimensions measures.
        for (int dim = 1; dim <= 3; ++dim)
        {
			// Consider absolute measures (i.e. points) and relative measures (i.e. mathematical vectors).
            for (int i_absolute = 0; i_absolute <= 1; ++i_absolute)
            {
				bool absolute = i_absolute == 1;
				
				// Emit descriptive comment.
				os << "// " << (dyn ? "dyn_" : "")
					<< (! absolute ? "vect" : "point") << dim << endl;
				
				generate_type_with_no_unit(os, dyn, absolute, dim);
				
				// Test construction of measures.
                generate_construction_from_numbers(os, dyn, absolute, dim);
                generate_construction_from_another_same_unit_measure(os, dyn, absolute, dim);
                generate_construction_from_another_different_unit_measure(os, dyn, absolute, dim);
                generate_construction_of_vect_from_point(os, dyn, dim);
                generate_construction_of_point_from_vect(os, dyn, dim);
				
				// Test measure member operations.
                generate_member_operations_with_same_unit(os, dyn, absolute, dim);
                generate_member_operations_with_different_units(os, dyn, absolute, dim);

				// Test measure non-member operations.
                generate_non_member_operations(os, dyn, absolute, dim);

				// Numeric casts
				
				// Test measure unit conversions.
                generate_conversions_with_same_magnitude_and_same_number_type(os, dyn, absolute, dim, "float");
                generate_conversions_with_same_magnitude_and_same_number_type(os, dyn, absolute, dim, "double");
                generate_conversions_with_different_magnitudes(os, dyn, absolute, dim, "float");
                generate_conversions_with_different_magnitudes(os, dyn, absolute, dim, "double");
                generate_conversions_with_different_number_types(os, dyn, absolute, dim, "float", "double");
                generate_conversions_with_different_number_types(os, dyn, absolute, dim, "double", "float");

				// Test derived units operations.
                generate_defined_derived_units_with_same_number_type(os, dyn, absolute, dim, "float");
                generate_defined_derived_units_with_same_number_type(os, dyn, absolute, dim, "double");
                generate_undefined_derived_units(os, dyn, absolute, dim);
                generate_defined_derived_units_with_different_number_types(os, dyn, absolute, dim, "float", "double");
                generate_defined_derived_units_with_different_number_types(os, dyn, absolute, dim, "double", "float");
// */
            }
        }
    }
//#endif

	// Plane directions, i.e. signed_azimuth and unsigned_azimuth.
	// Plane directions are only scalar and only absolute.
	// Consider both statically-typed and dynamically-typed plane directions.
    for (int i_dynamic = 0; i_dynamic <= 1; ++i_dynamic)
    {
		bool dyn = i_dynamic == 1;

		// Consider both signed and unsigned azimuths.
		for (int i_signed_angle = 0; i_signed_angle <= 1; ++i_signed_angle)
		{
			bool signed_angle = i_signed_angle == 1;

			// Emit descriptive comment.
			os << "// " << (dyn ? "dyn_" : "")
				<< (signed_angle ? "signed" : "unsigned") << "_azimuth" << endl;

			generate_az_type_with_no_unit(os, dyn, signed_angle);
			
			// Test construction of azimuths.
			generate_az_construction_from_numbers(os, dyn, signed_angle);
			generate_az_construction_from_another_same_unit_angle(os, dyn, signed_angle);
            generate_az_construction_from_another_different_unit_angle(os, dyn, signed_angle);
            generate_az_construction_from_another_different_kind_angle(os, dyn, signed_angle);

			// Test azimuth member operations.
			generate_az_member_operations_with_same_unit(os, dyn, signed_angle);
			generate_az_member_operations_with_different_units(os, dyn, signed_angle);
			/*
			TODO
			generate_az_non_member_operations_with_same_unit_and_number_type(os, dyn, signed_angle);
			generate_az_non_member_operations_with_different_units(os, dyn, signed_angle);
			generate_az_non_member_operations_with_different_number_types(os, dyn, signed_angle);
			generate_az_conversions_with_same_magnitude_and_same_number_type(os, dyn, signed_angle, "float");
			generate_az_conversions_with_same_magnitude_and_same_number_type(os, dyn, signed_angle, "double");
			generate_az_conversions_with_different_magnitudes(os, dyn, signed_angle, "float");
			generate_az_conversions_with_different_magnitudes(os, dyn, signed_angle, "double");
			generate_az_conversions_with_different_number_types(os, dyn, signed_angle, "float", "double");
			generate_az_conversions_with_different_number_types(os, dyn, signed_angle, "double", "float");

			generate_az_defined_derived_units_with_same_number_type(os, dyn, signed_angle, "float");
			generate_az_defined_derived_units_with_same_number_type(os, dyn, signed_angle, "double");
	/*
			generate_az_undefined_derived_units(os, dyn, signed_angle);
			generate_az_defined_derived_units_with_different_number_types(os, dyn, signed_angle, "float", "double");
			generate_az_defined_derived_units_with_different_number_types(os, dyn, signed_angle, "double", "float");
	*/
        }
    }
    os << "}" << endl;
}

int main()
{	
    generate_program(cout);
}
