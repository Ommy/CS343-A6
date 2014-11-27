#include "utilities.h"

#include <algorithm>
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

unsigned int sum( std::vector<unsigned int>& vector ) {
    return std::accumulate( vector.begin(), vector.end(), 0);
}

unsigned int sum( std::deque<unsigned int>& deque ) {
    return std::accumulate( deque.begin(), deque.end(), 0);
}

unsigned int sum( std::array<unsigned int, VendingMachine::NUMBER_OF_FLAVOURS>& array ) {
    return std::accumulate( array.begin(), array.end(), 0);
}