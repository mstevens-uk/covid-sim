#ifndef COVIDSIM_CLI_HPP_INCLUDED_
#define COVIDSIM_CLI_HPP_INCLUDED_

#include <functional>
#include <map>
#include <string>

// only a forward-declartion, no need to pull in all of Param.h in this header
struct Param;

// The parse_* functions are kept outside of the CmdLineArgs class because they
// can be used to parse any string input and are not specific to CLI arguments.
// e.g. they may eventually be useful for parsing from the (pre)param files

/**
 * Parses and checks if the input string is a readable file on-disk.
 */
void parse_read_file(std::string const& input, std::string& output);

/**
 * Parses and checks if the input string is a 32-bit integer.
 *
 * Will error if N > (2^31 - 1) || N < -(2^31 - 1)
 */
void parse_integer(std::string const& input, int& output);

/**
 * Parses and checks if the input string is a 32-bit long.
 *
 * Will error if N > (2^31 - 1) || N < -(2^31 - 1)
 *
 * @note: Windows uses the LLP64 data model with MinGW and Visual C++, meaning
 * that int and long have 32-bits even on 64-bit. So this function has the same
 * effect as parse_integer(). UNIX and Cygwin on the other hand use a LP64 data
 * model which means that int is 32-bits and long is 64-bits. Therefore long
 * cannot be interpreted as a 32-bit integer on all platforms. For consistency
 * across platforms, long should eventually be replaced with one of the fixed
 * width integer types from <cstdint> such as int32_t or int64_t.
 *
 * @see: https://en.wikipedia.org/wiki/64-bit_computing#64-bit_data_models
 */
void parse_long(std::string const& input, long& output);


class CmdLineArgs {
public:
    // Function prototype for a generic parser function
    using ParserFn = std::function<void(std::string const&)>;

    /**
     * Use this function when adding a new option to the CLI.
     *
     * This will bind the output variable specified to a parser function which
     * then gets inserted into a map of other options. This provides a strong
     * cohesion between an option name (i.e. 'P') with a C++ variable (i.e. 'ParamFile')
     */
    template<class T>
    void add_option(std::string const&& option, std::function<void(std::string const&, T&)> func, T& output);

    /**
     * Call this function once all add_option() calls have been made to process
     * the arguments passed in from the command-line.
     */
    int parse(int argc, char* argv[], Param& P);

private:
    std::map<std::string, ParserFn> m_option_map;
};

void PrintHelpAndExit();
void PrintDetailedHelpAndExit();

#endif
