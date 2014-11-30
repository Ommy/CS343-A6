#ifndef _UTILITIES_H__
#define _UTILITIES_H__

#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
T poll_front( std::deque<T>& deque ) {
    T value = deque.front();
    deque.pop_front();
    return value;
}

bool convertToSignedInteger( int& val, char* buffer );
bool convertToUnsignedInteger( unsigned int& val, char* buffer );

template<class IT>
    unsigned int sum( IT start, IT end ) {
    return std::accumulate( start, end, 0 );
}
unsigned int sum( const std::vector<unsigned int>& vector );
unsigned int sum( const std::deque<unsigned int>& deque );
unsigned int sum( const unsigned int* array, const unsigned int size );

#endif