#ifndef _UTILITIES_H__
#define _UTILITIES_H__

#include <deque>
#include <fstream>
#include <sstream>
#include <string>

template <typename T>
T poll_front( std::deque<T>& deque ) {
    T value = deque.front();
    deque.pop_front();
    return value;
}

bool convertToSignedInteger( int& val, char* buffer );
bool convertToUnsignedInteger( unsigned int& val, char* buffer );
bool comments( std::ifstream &in, std::string &name );

#endif