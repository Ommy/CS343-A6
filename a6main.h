#ifndef _A6_MAIN_H__
#define _A6_MAIN_H__

#include "MPRNG.h"
#include "a6config.h"

#include <string>
#include <stdexcept>

class A6 {
    std::string filename;
    unsigned int randomSeed;
    std::string configFilename;

    ConfigParams configParams;

    std::string getCorrectUsage();
    void throwInvalidUsage( std::string usageString = "" );

public:
    A6( int argc, char** argv );
    ~A6();
    void run();

    static MPRNG mprng;
};

class CommandLineArgsException : public std::runtime_error {
public:
    CommandLineArgsException( std::string what ) : std::runtime_error( what ) {}
};

#endif
