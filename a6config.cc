#include "a6config.h"
#include "utilities.h"

#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

void Config::processConfigFile( const char *configFile, ConfigParms &cparms ) {
    const unsigned int Parmnum = 9;
    struct {
        const char *name;               // configuration name
        bool used;                  // already supplied ?
        unsigned int &value;                // location to put configuration value
    } static parms[Parmnum] = {
        { "SodaCost", false, cparms.sodaCost },
        { "NumStudents", false, cparms.numStudents },
        { "MaxPurchases", false, cparms.maxPurchases },
        { "NumVendingMachines", false, cparms.numVendingMachines },
        { "MaxStockPerFlavour", false, cparms.maxStockPerFlavour },
        { "MaxShippedPerFlavour", false, cparms.maxShippedPerFlavour },
        { "TimeBetweenShipments", false, cparms.timeBetweenShipments },
        { "ParentalDelay", false, cparms.parentalDelay },
        { "NumCouriers", false, cparms.numCouriers },
    };
    string name;
    unsigned int value, cnt, posn;
    unsigned int numOfParm = 0;

    try {
        ifstream in( configFile );          // open the configuration file for input

    for ( cnt = 0 ; cnt < Parmnum; cnt += 1 ) { // parameter names can appear in any order
      if ( comments( in, name ) ) break;        // eof ?
        for ( posn = 0; posn < Parmnum && name != parms[posn].name; posn += 1 ); // linear search
      if ( posn == Parmnum ) break;         // configuration not found ?
      if ( parms[posn].used ) break;        // duplicate configuration ?
        in >> value;
      if ( in.fail() ) break;
        in.ignore( numeric_limits<int>::max(), '\n' ); // ignore remainder of line
        numOfParm += 1;
        parms[posn].used = true;
        parms[posn].value = value;
    }

    if ( numOfParm != Parmnum ) {
        cerr << "Error: file \"" << configFile << "\" is corrupt." << endl;
        exit( EXIT_FAILURE );
    }
    if ( ! comments( in, name ) ) {         // ! eof ?
        cerr << "Error: file \"" << configFile << "\" has extraneous data." << endl;
        exit( EXIT_FAILURE );
    }

    } catch( uFile::Failure ) {
        cerr << "Error: could not open input file \"" << configFile << "\"" << endl;
        exit( EXIT_FAILURE );    
    }
}
