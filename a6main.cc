#include "a6main.h"
#include "a6printer.h"
#include "a6bank.h"
#include "a6parent.h"
#include "a6watcardoffice.h"
#include "a6nameserver.h"
#include "a6vendingmachine.h"
#include "a6bottlingplant.h"
#include "a6student.h"
#include "a6config.h"
#include "MPRNG.h"
#include "utilities.h"

#include <iostream>
#include <deque>
#include <memory>
#include <sstream>

MPRNG A6::mprng;

A6::A6( int argc, char** argv ) {
    filename = std::string( argv[0] );
    configFilename = "soda.config";
    randomSeed = getpid();

    switch ( argc ) {
    case 3:
        if ( !convertToUnsignedInteger( randomSeed, argv[2] ) ) {
            throwInvalidUsage();
        }
    case 2:
        configFilename = argv[1];
    case 1:
        Config::processConfigFile( configFilename.c_str(), configParams );
        break;
    default:
        throwInvalidUsage();
        break;
    }

    mprng.seed( randomSeed );
}

A6::~A6() {
}

void A6::run() {
    Printer printer( configParams.numStudents,
                     configParams.numVendingMachines,
                     configParams.numCouriers );

    Bank bank( configParams.numStudents );
    std::unique_ptr<Parent> parent( new Parent( printer, 
                                                bank, 
                                                configParams.numStudents, 
                                                configParams.parentalDelay ) );

    std::unique_ptr<WATCardOffice> office( new WATCardOffice( printer,
                                                              bank, 
                                                              configParams.numCouriers ) );

    std::unique_ptr<NameServer> nameServer( new NameServer( printer, 
                                                            configParams.numVendingMachines, 
                                                            configParams.numStudents ) );

    std::deque<std::shared_ptr<VendingMachine>> vendingMachines( configParams.numVendingMachines );

    for ( unsigned int mid = 0; mid < configParams.numVendingMachines; ++mid ) {
        vendingMachines[mid] = std::shared_ptr<VendingMachine>( new VendingMachine( printer, 
                                                                                  *nameServer, 
                                                                                  mid, 
                                                                                  configParams.sodaCost, 
                                                                                  configParams.maxStockPerFlavour ) );
    }

    std::unique_ptr<BottlingPlant> plant( new BottlingPlant( printer, 
                                                             *nameServer, 
                                                             configParams.numVendingMachines, 
                                                             configParams.maxShippedPerFlavour, 
                                                             configParams.maxStockPerFlavour, 
                                                             configParams.timeBetweenShipments ) );

    std::deque<std::shared_ptr<Student>> students( configParams.numStudents );
    for ( unsigned int sid = 0; sid < configParams.numStudents; ++sid ) {
        students[sid] = std::shared_ptr<Student>( new Student( printer, 
                                                             *nameServer, 
                                                             *office, 
                                                             sid, 
                                                             configParams.maxPurchases ) );
    }

    for ( unsigned int sid = 0; sid < configParams.numStudents; ++sid ) {
        students[sid].reset();
    }

    plant.reset();

    for ( unsigned int mid = 0; mid < configParams.numVendingMachines; ++mid ) {
        vendingMachines[mid].reset();
    }

    nameServer.reset();
    office.reset();
    parent.reset();
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
        A6 a6( argc, argv );
        a6.run();
    } catch ( CommandLineArgsException& e ) {
        std::cerr << e.what() << std::endl;
        exit( EXIT_FAILURE );
    } catch ( std::exception& e ) {
        std::cerr << "FIX ERROR: " << e.what() << std::endl;
    }
}
