#include "a6bottlingplant.h"
#include "a6printer.h"
#include "a6main.h"
#include "a6truck.h"

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numberOfVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments )
: printer(prt), nameServer(nameServer), numberOfVendingMachines(numberOfVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments) {
    shuttingDown = false;
    for (unsigned int i = 0; i < VendingMachine::NUMBER_OF_FLAVOURS; ++i) {
        shipment[i] = 0;
    }
}

BottlingPlant::~BottlingPlant() {
    delete truck;
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    if (shuttingDown == true) {
        uRendezvousAcceptor();
        throw Shutdown();
    } else {
        for (unsigned int i = 0; i < VendingMachine::NUMBER_OF_FLAVOURS; ++i) {
            cargo[i] = shipment[i];
            shipment[i] = 0;
        }
    }
}

void BottlingPlant::main() {
    truck = new Truck(printer, nameServer, *this, numberOfVendingMachines, maxStockPerFlavour); 
    
    while (true) {
        yield(timeBetweenShipments);
        for (unsigned int i = 0; i < VendingMachine::NUMBER_OF_FLAVOURS; ++i) {
            shipment[i] = mprng(maxShippedPerFlavour);
        }

        _Accept(~BottlingPlant) {
            shuttingDown = true;
            break;
        } or _Accept( getShipment ) {
        }
    }
}
