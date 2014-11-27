#include <iostream>
#include <sstream>
#include <deque>

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
        Config::processConfigFile(configFilename.c_str(), configParams);
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
    Printer * printer = new Printer(configParams.numStudents, configParams.numVendingMachines, configParams.numCouriers);
    Bank * bank = new Bank(configParams.numStudents);
    Parent * parent = new Parent(*printer, *bank, configParams.numStudents, configParams.parentalDelay);
    WATCardOffice * office = new WATCardOffice(*printer, *bank, configParams.numCouriers);
    NameServer * nameServer = new NameServer(*printer, configParams.numVendingMachines, configParams.numStudents);
    BottlingPlant * plant = new BottlingPlant(*printer, *nameServer, configParams.numVendingMachines, configParams.maxShippedPerFlavour, configParams.maxStockPerFlavour, configParams.timeBetweenShipments );

    std::deque<VendingMachine*> vendingMachines;
    for (unsigned int i = 0; i < configParams.numVendingMachines; ++i) {
        vendingMachines.push_back(new VendingMachine(*printer, *nameServer, i, configParams.sodaCost, configParams.maxStockPerFlavour));
    }

    std::deque<Student*> students;
    for (unsigned int i = 0; i < configParams.numStudents; ++i) {
        students.push_back(new Student(*printer, *nameServer, *office, i, configParams.maxPurchases));
    }

    for (unsigned int i = 0; i < configParams.numStudents; ++i) {
        delete students[i];
    }

    delete plant;

    for (unsigned int i = 0; i < configParams.numVendingMachines; ++i) {
        delete vendingMachines[i];
    }
    
    delete nameServer;
    delete office;
    delete parent;
    delete bank;
    delete printer;
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