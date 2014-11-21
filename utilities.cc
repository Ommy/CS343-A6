#include <limits>

#include "utilities.h"

bool convertToSignedInteger( int& val, char* buffer ) {
    std::stringstream ss( buffer );
    ss >> std::dec >> val;
    return ! ss.fail()
           && std::string( buffer ).find_first_not_of( " ", ss.tellg() ) == std::string::npos;
}

bool convertToUnsignedInteger( unsigned int& val, char* buffer ) {
    if (std::string( buffer ).find("-") != std::string::npos) return false;
    std::stringstream ss( buffer );
    ss >> std::dec >> val;
    return ! ss.fail()
           && std::string( buffer ).find_first_not_of( " ", ss.tellg() ) == std::string::npos;
}