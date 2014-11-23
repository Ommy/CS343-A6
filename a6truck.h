#ifndef _A6_TRUCK_H__
#define _A6_TRUCK_H__

#include "a6bottlingplant.h"

_Cormonitor Printer;
_Task NameServer;

_Task Truck {
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
  private:
    void main();
    bool hasCargo();
    unsigned int nextMachine(unsigned int);
    unsigned int numberOfVendingMachines;
    unsigned int maxStockOfEachFlavour;
    unsigned int lastMachineStocked;
    Printer * printer;
    NameServer * server;
    BottlingPlant * bottlingPlant;

    unsigned int * cargo;
};

#endif