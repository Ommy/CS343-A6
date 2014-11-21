#ifndef _A6_BOTTLING_PLANT_H__
#define _A6_BOTTLING_PLANT_H__

#include "a6nameserver.h"

_Cormonitor Printer;

_Task BottlingPlant {
    void main();
  public:
    _Event Shutdown {};
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif