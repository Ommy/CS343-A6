#include "a6bottlingplant.h"
#include "a6main.h"
#include "a6truck.h"
#include "a6printer.h"
#include "utilities.h"

BottlingPlant::BottlingPlant(   Printer &prt, 
                                NameServer &nameServer, 
                                unsigned int numberOfVendingMachines, 
                                unsigned int maxShippedPerFlavour, 
                                unsigned int maxStockPerFlavour, 
                                unsigned int timeBetweenShipments ) :   printer(prt), 
                                                                        nameServer(nameServer), 
                                                                        numberOfVendingMachines(numberOfVendingMachines), 
                                                                        maxShippedPerFlavour(maxShippedPerFlavour), 
                                                                        maxStockPerFlavour(maxStockPerFlavour), 
                                                                        timeBetweenShipments(timeBetweenShipments),
                                                                        shuttingDown(false) {
    shipment.fill(0);
}

BottlingPlant::~BottlingPlant() {
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
    printer.print(Printer::BottlingPlant, (char)Start);

    Truck truck(printer, nameServer, *this, numberOfVendingMachines, maxStockPerFlavour);
    
    while (true) {
        yield(timeBetweenShipments);

        for (unsigned int i = 0; i < VendingMachine::NUMBER_OF_FLAVOURS; ++i) {
            shipment[i] = A6::mprng(maxShippedPerFlavour);
        }
        
        printer.print(Printer::BottlingPlant, (char)Generate, sum(std::begin(shipment), std::end(shipment)));
        
        _Accept( ~BottlingPlant ) {
            shuttingDown = true;
            _Accept( getShipment );
            break;
        } or _Accept( getShipment ) {
            printer.print(Printer::BottlingPlant, (char)PickUp);
        }
    }

    printer.print(Printer::BottlingPlant, (char)Finish);
}
