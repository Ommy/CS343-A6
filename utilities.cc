#include "utilities.h"

#include <limits>

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

unsigned int sum( const std::vector<unsigned int>& vector ) {
    return sum( vector.begin(), vector.end());
}

unsigned int sum( const std::deque<unsigned int>& deque ) {
    return sum( deque.begin(), deque.end());
}

unsigned int sum( const unsigned int* array, const unsigned int size) {
    unsigned int total = 0;
    for (unsigned int i = 0; i < size; ++i) {
        total += array[size];
    }
    return total;
}