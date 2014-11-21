#ifndef _A6_TRUCK_H__
#define _A6_TRUCK_H__

#include "a6bottlingplant.h"

_Cormonitor Printer;
_Task NameServer;

_Task Truck {
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif