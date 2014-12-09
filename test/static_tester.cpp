#include <cstdlib>  // for getenv, system, exit, EXIT_FAILURE, EXIT_SUCCESS
#include <fstream>  // for ifstream, ofstream
#include <iostream> // for cout, endl
#include <iomanip>  // for setw
#include <sstream>  // for stringstream
#include <string>   // for string
#include <vector>   // for vector
#include <algorithm>// for count
using namespace std;

// sample test for legality: [@[allowed snippet]@[illegal snippet]@]

char const tag1[] = "[@[";
char const tag2[] = "]@[";
char const tag3[] = "]@]";
size_t const tag1size = sizeof tag1 - 1;
size_t const tag2size = sizeof tag2 - 1;
size_t const tag3size = sizeof tag3 - 1;

string getenv_string(const char *env_var_name)
{
    const char *env_var_value = getenv(env_var_name);
    if (! env_var_value)
    {
        cout << "Environment variabile '" << env_var_name
            << "' not defined." << endl;
        exit(EXIT_FAILURE);
    }
    string value(env_var_value ? env_var_value : "");
    if (value[0] == '\"' && value[value.size() - 1] == '\"')
    {
        value = value.substr(1, value.size() - 2);
    }
    return value;
}

void test_allowed_file(string const& file_name,
    string const& compiler_base_command, string const& redirection_command)
{
    string const compiler_command = compiler_base_command + " "
        + file_name + " " + redirection_command;
    if (system(compiler_command.c_str()) != 0)
    {
        cout << "Error: some unexpected errors when running" << endl
            << compiler_base_command + " " + file_name << endl;
        exit(EXIT_FAILURE);
    }
}

bool fail_snippet(bool first_snippet, string const& contents,
    int i_curr_element, string const& temp_path,
    string const& compiler_command)
{
    // Save file without every element, but for current element
    // insert only a snippet.
    ofstream os(temp_path.c_str());
    if (! os)
    {
        cout << "Impossible to write temporary file '"
            << temp_path << "'." << endl;
        exit(EXIT_FAILURE);
    }

    int i_element = 0;
    size_t tag1_pos;
    size_t tag3_end_pos = 0;
    while ((tag1_pos = contents.find(tag1, tag3_end_pos))
        != string::npos)
    {
        // Find middle tag of the element.
        size_t tag2_pos = contents.find(tag2, tag1_pos + tag1size);
        
        // Find end tag of the element.
        size_t tag3_pos = contents.find(tag3, tag2_pos + tag2size);
        
        os << contents.substr(tag3_end_pos, tag1_pos - tag3_end_pos);
        if (i_element == i_curr_element)
        {
            if (first_snippet)
            {
                os << contents.substr(tag1_pos + tag1size,
                    tag2_pos - tag1_pos - tag1size);
            }
            else
            {
                os << contents.substr(tag2_pos + tag2size,
                    tag3_pos - tag2_pos - tag2size);
            }
        }
        
        tag3_end_pos = tag3_pos + tag3size;
        ++i_element;
    }
    os << contents.substr(tag3_end_pos);
    os.close();
    return system(compiler_command.c_str()) != 0;
}

string tell_position(string const& contents, int offset)
{
    stringstream sstr;
    sstr << "row " << count(contents.begin(),
        contents.begin() + offset, '\n') + 1
        << " column " << offset - contents.rfind('\n', offset);
    return sstr.str();
}

void test_illegal_file(string const& temp_dir, string const& file_name,
    string const& compiler_base_command, string const& redirection_command)
{
    stringstream sstr;
    sstr << ifstream(file_name).rdbuf();
    string contents = sstr.str();
    
    // Check and count all elements in the file.
    int n_elements = 0;
    size_t tag1_pos;
    size_t tag3_end_pos = 0;
    while ((tag1_pos = contents.find(tag1, tag3_end_pos))
        != string::npos)
    {
        // Find middle tag of the element.
        size_t tag2_pos = contents.find(tag2, tag1_pos + tag1size);
        if (tag2_pos == string::npos)
        {
            cout << "Error: missing tag '" << tag2 << "' after "
                << tell_position(contents, tag1_pos + tag1size)
                << " in file '" << file_name + "'." << endl;
            exit(EXIT_FAILURE);
        }

        // Find end tag of the element.
        size_t tag3_pos = contents.find(tag3, tag2_pos + tag2size);
        if (tag3_pos == string::npos)
        {
            cout << "Error: missing tag '" << tag3 << "' after "
                << tell_position(contents, tag2_pos + tag1size)
                << " in file '" << file_name + "'." << endl;
            exit(EXIT_FAILURE);
        }
        
        tag3_end_pos = tag3_pos + tag3size;
        ++n_elements;
    }
    if (tag3_end_pos == 0)
    {
        cout << "Error: no tag '" << tag1 << "' in file '"
            << file_name + "'." << endl;
        exit(EXIT_FAILURE);
    }

    string const temp_path = temp_dir + "_TEST.cpp";
    string const compiler_command = compiler_base_command + " "
        + temp_path + " " + redirection_command;

    // Loop for every test element in the file.
    for (int i_element = 0; i_element < n_elements; ++i_element)
    {
        // Make two tests for the current element, one with the first part
        // as allowed, and the other with the second part as illegal.
        if (fail_snippet(true, contents, i_element, temp_path,
            compiler_command))
        {
            cout << "Error: unexpected error for snippet n."
                << (i_element + 1) << " asserted allowed in file '"
                << file_name + "', when running" << endl
                << compiler_base_command << " " << temp_path << endl;
            exit(EXIT_FAILURE);
        }

        if (! fail_snippet(false, contents, i_element, temp_path,
            compiler_command))
        {
            cout << "Error: no errors for snippet n."
                << (i_element + 1) << " asserted illegal in file '"
                << file_name + "', when running" << endl
                << compiler_base_command << " " << temp_path << endl;
            exit(EXIT_FAILURE);
        }
    }    
}

void print_usage(string const& msg)
{
    cout << msg << endl;
    cout << "Usage: static_tester [/t<temporary directory>]"
        " {/p<perfect file>} {/a<allowed file>} {/i<illegal file>}" << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    string redirection_command
        = getenv_string("REDIRECTION_COMMAND");
    string compiler_version_command
        = getenv_string("COMPILER_VERSION_COMMAND");
    string compiler_base_errors_command
        = getenv_string("COMPILER_BASE_ERRORS_COMMAND");
    string compiler_base_warnings_command
        = getenv_string("COMPILER_BASE_WARNINGS_COMMAND");

    string temp_dir = "";
    vector<string> perfect_files, allowed_files, illegal_files;
    for (int arg = 1; arg < argc; ++arg)
    {
        switch (argv[arg][0])
        {
            case '-':
            case '/':
                switch (argv[arg][1])
                {
                    case 't': temp_dir = argv[arg] + 2; break;
                    case 'p': perfect_files.push_back(argv[arg] + 2); break;
                    case 'a': allowed_files.push_back(argv[arg] + 2); break;
                    case 'i': illegal_files.push_back(argv[arg] + 2); break;
                    default:
                        print_usage(string(
                            "Error: unrecognized command line option '")
                            + argv[arg][1] + "'");
                }
                break;
            default:
                print_usage(string(
                    "Error: unrecognized command line argument '")
                    + argv[arg] + "'");
        }
    }
    if (perfect_files.size() + allowed_files.size()
        + illegal_files.size() == 0)
    {
        print_usage("Error: No input files");
    }
    
    if (system((compiler_version_command + " "
        + redirection_command).c_str()) > 0)
    {
        cout << "Error: the command '" << compiler_version_command
            << "' cannot be executed." << endl;
        exit(EXIT_FAILURE);
    }

    for (auto file_name: perfect_files)
    {
        cout << "Processing file '" << file_name
            << "', asserted PERFECT." << endl;
        test_allowed_file(file_name,
            compiler_base_warnings_command, redirection_command);
    }
    for (auto file_name: allowed_files)
    {
        cout << "Processing test file '" << file_name
            << "', asserted ALLOWED." << endl;
        test_allowed_file(file_name,
            compiler_base_errors_command, redirection_command);
    }
    for (auto file_name: illegal_files)
    {
        cout << "Processing test file '" << file_name
            << "', asserted ILLEGAL." << endl;
        test_illegal_file(temp_dir, file_name,
            compiler_base_errors_command, redirection_command);
    }
    cout << "OK" << endl;
    return EXIT_SUCCESS;
}
