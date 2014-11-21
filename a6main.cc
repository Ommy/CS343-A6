#include <iostream>
#include <sstream>

#include "a6main.h"
#include "MPRNG.h"

MPRNG mprng;

A6::DefaultConfigFilename = "soda.config";

A6::A6( int argc, char** argv ) {
    filename = std::string( argv[0] );
    configFilename = DefaultConfigFilename;
    randomSeed = getpid();

    switch ( argc ) {
    case 3:
        if ( !convertToUnsignedInteger( randomSeed, argv[2] ) ) {
            throwInvalidUsage();
        }
    case 2:
        configFilename = argv[1];
    case 1:
        Config.processConfigFile(configFilename, configParms);
        break;
    default:
        throwInvalidUsage();
        break;
    }

    mprng.seed( seed );
}

A6::~A6() {
}

void A6::run() {
}

std::string A6::getCorrectUsage() {
    std::stringstream ss;
    ss << "Usage: " << this->filename << " ";
    ss << "[ config-file [ random-seed ] ]";
    return ss.str();
}

void A6::throwInvalidUsage( std::string usageString ) {
    std::stringstream ss;
    ss << usageString;
    ss << getCorrectUsage();

    throw CommandLineArgsException( ss.str() );
}

void uMain::main() {
    try {
        A6 A6( argc, argv );
        A6.run();
    } catch ( CommandLineArgsException& e ) {
        std::cerr << e.what() << std::endl;
        exit( EXIT_FAILURE );
    } catch ( std::exception& e ) {
        std::cerr << "FIX ERROR: " << e.what() << std::endl;
    }
}