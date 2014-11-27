#ifndef _UTILITIES_H__
#define _UTILITIES_H__

#include "a6vendingmachine.h"

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
unsigned int sum( std::vector<unsigned int>& vector );
unsigned int sum( std::deque<unsigned int>& deque );
unsigned int sum( std::array<unsigned int, VendingMachine::NUMBER_OF_FLAVOURS>& array );

#endif