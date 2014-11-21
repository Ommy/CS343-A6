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

bool comments( std::ifstream &in, std::string &name ) {
    for ( ;; ) {
    in >> name;
      if ( in.fail() ) return true;
      if ( name.substr(0,1) != "#" ) break;
    in.ignore( std::numeric_limits<int>::max(), '\n' ); // ignore remainder of line
    }
    return false;
}