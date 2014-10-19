#include <cstdlib>  // for getenv, system, exit, EXIT_FAILURE, EXIT_SUCCESS
#include <fstream>  // for ifstream, ofstream
#include <iostream> // for cout, endl
#include <iomanip>  // for setw
#include <sstream>  // for stringstream
#include <string>   // for string
#include <vector>   // for vector
using namespace std;

// tags: @PERFECT, @ALLOWED, @ILLEGAL, @END

// Command-line suffix to discard output.
string redirection_command;

// Compiler command-line to output compiler version information.
string compiler_version_command;

// Compiler command-line prefix to check for compilation errors.
string compiler_base_errors_command;

// Compiler command-line prefix to check for compilation warnings.
string compiler_base_warnings_command;

int n_tot_hits = 0;  

string getenv_string(const char *env_var_name, bool required = true)
{
    const char *env_var_value = getenv(env_var_name);
    if (! env_var_value && required)
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

bool CheckErrors(const string &filename, int n_block, int n_tot_blocks)
{
    const string compiler_errors_command = compiler_base_errors_command + " "
        + filename + " " + redirection_command;
    cerr << setw(5) << n_block << "/" << n_tot_blocks << "\r";
    return system(compiler_errors_command.c_str()) != 0;
}

bool CheckWarnings(const string &filename, int n_block, int n_tot_blocks)
{
    const string compiler_warnings_command = compiler_base_warnings_command
        + " " + filename + " " + redirection_command;
    cerr << setw(5) << n_block << "/" << n_tot_blocks << "\r";
    return system(compiler_warnings_command.c_str()) != 0;
}

void test_file(string dir, string filename)
{
    if (system((compiler_version_command + " "
        + redirection_command).c_str()) > 0)
    {
        cout << "Wrong test: the command '"
            << compiler_version_command << "' cannot be executed." << endl;
        exit(EXIT_FAILURE);
    }
    int n_hits = 0;
    int n_current_block = 0;
    string line;
    int n_tot_blocks;
    do
    {
        ++n_current_block;
        ifstream is(dir + filename);
        if (! is)
        {
            cout << "Impossible to read file '" << dir + filename
                << "'." << endl;
            exit(EXIT_FAILURE);
        }
        stringstream test_filename;
        test_filename << dir << "_test.cpp";
        ofstream os(test_filename.str().c_str());
        if (! os)
        {
            cout << "Impossible to write temporary file '"
                << test_filename.str() << "'." << endl;
            exit(EXIT_FAILURE);
        }
        enum e_asserted { undefined, perfect, allowed, illegal }
            asserted = undefined;
        int n_line = 0;
        int n_block = 0;
        bool in_block = false;
        while (is)
        {
            ++n_line;
            getline(is, line);
            if (line.find("@PERFECT") != string::npos)
            {
                if (in_block)
                {
                    cout << "Wrong test: @PERFECT marker"
                        " found inside another block, at line " << n_line
                        << "." << endl;
                    exit(EXIT_FAILURE);
                }
                ++n_block;
                in_block = true;
                if (n_block == n_current_block) asserted = perfect;
            }
            else if (line.find("@ALLOWED") != string::npos)
            {
                if (in_block)
                {
                    cout << "Wrong test: @ALLOWED marker"
                        " found inside another block, at line " << n_line
                        << "." << endl;
                    exit(EXIT_FAILURE);
                }
                ++n_block;
                in_block = true;
                if (n_block == n_current_block) asserted = allowed;
            }
            else if (line.find("@ILLEGAL") != string::npos)
            {
                if (in_block)
                {
                    cout << "Wrong test: @ILLEGAL marker"
                        " found inside another block, at line " << n_line
                        << "." << endl;
                    exit(EXIT_FAILURE);
                }
                ++n_block;
                in_block = true;
                if (n_block == n_current_block) asserted = illegal;
            }
            else if (line.find("@END") != string::npos)
            {
                if (! in_block)
                {
                    cout << "Wrong test: @END marker"
                        " found outside any block, at line " << n_line
                        << "." << endl;
                    exit(EXIT_FAILURE);
                }
                in_block = false;
            }
            else
            {
                if ((! in_block || n_block == n_current_block)
                    && line.size() > 0)
                { os << line << endl; }
            }
        }
        if (in_block)
        {
            cout << "Wrong test: missing @END marker." << endl;
            exit(EXIT_FAILURE);
        }
        n_tot_blocks = n_block;
        is.close();
        os.close();

        switch (asserted)
        {
            case perfect:
                if (CheckWarnings(test_filename.str(), n_current_block, n_tot_blocks))
                {
                    cout << endl << compiler_base_warnings_command + " " + test_filename.str() << endl;
                    cout << "****** Compilation with some errors or warnings, while a compilation with"
                        " no errors nor warnings was expected." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case allowed:
                if (CheckErrors(test_filename.str(), n_current_block, n_tot_blocks))
                {
                    cout << endl << compiler_base_errors_command + " " + test_filename.str() << endl;
                    cout << "****** Compilation with some errors,"
                        " while a compilation with no errors was expected." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case illegal:
                if (! CheckErrors(test_filename.str(), n_current_block, n_tot_blocks))
                {
                    cout << endl << compiler_base_errors_command + " " + test_filename.str() << endl;
                    cout << "****** ERROR: Compilation with no errors,"
                        " while a compilation with errors was expected." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                cout << "****** ERROR: No static test tag defined among PERFECT, ALLOWED, ILLEGAL." << endl;
                exit(EXIT_FAILURE);
        }
        ++n_hits;
        remove((dir + "_test.cpp").c_str());
    } while (n_current_block < n_tot_blocks);
    n_tot_hits += n_hits;
}

int main(int argc, char *argv[])
{
    redirection_command = getenv_string("REDIRECTION_COMMAND");
    compiler_version_command = getenv_string("COMPILER_VERSION_COMMAND");
    compiler_base_errors_command
        = getenv_string("COMPILER_BASE_ERRORS_COMMAND");
    compiler_base_warnings_command
        = getenv_string("COMPILER_BASE_WARNINGS_COMMAND", false);

    string tmp_dir = "";
    vector<string> input_files;
    for (int arg = 1; arg < argc; ++arg)
    {
        switch (argv[arg][0])
        {
            case '-':
            case '/':
                switch (argv[arg][1])
                {
                    case 't':
                        tmp_dir = argv[arg] + 2;
                        break;
                }
                break;
            default:
                input_files.push_back(argv[arg]);
        }
    }
    if (input_files.size() == 0)
    {
        cout << "Usage: static_tester [/t<temporary directory>] first_input_file ..." << endl;
        exit(EXIT_FAILURE);
    }
    for (auto file_name: input_files)
    {
        cout << "Processing test file '" << tmp_dir + file_name << "'" << endl;
        test_file(tmp_dir, file_name);
    }
    if (input_files.size() == 1)
    {
       cout << "One test file processed." << endl;
    }
    else
    {
       cout << input_files.size() << " test files processed." << endl;
    }
    cout << "No errors, total successes: " << n_tot_hits << "." << endl;
    return EXIT_SUCCESS;
}
