#ifndef _A6_BOTTLING_PLANT_H__
#define _A6_BOTTLING_PLANT_H__

#include "a6nameserver.h"
#include "a6vendingmachine.h"

#include <array>
#include <memory>

_Cormonitor Printer;
_Task Truck;

_Task BottlingPlant {
    void main();

    Printer& printer;
    NameServer& nameServer;
    const unsigned int numberOfVendingMachines;
    const unsigned int maxShippedPerFlavour;
    const unsigned int maxStockPerFlavour;
    const unsigned int timeBetweenShipments;

    bool shuttingDown;
    std::array<unsigned int, VendingMachine::NUMBER_OF_FLAVOURS> shipment;

    enum PrintStates {
        Start = 'S',
        Generate = 'G',
        PickUp = 'P',
        Finish = 'F',
    };

public:
    _Event Shutdown {};
    BottlingPlant(  Printer &prt,
                    NameServer &nameServer,
                    unsigned int numberOfVendingMachines,
                    unsigned int maxShippedPerFlavour,
                    unsigned int maxStockPerFlavour,
                    unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};

#endif