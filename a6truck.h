#ifndef _A6_TRUCK_H__
#define _A6_TRUCK_H__

#include "a6bottlingplant.h"

#include <vector>

_Cormonitor Printer;
_Task NameServer;

_Task Truck {
  public:
    Truck(  Printer &prt, 
            NameServer &nameServer, 
            BottlingPlant &plant,
            unsigned int numVendingMachines, 
            unsigned int maxStockPerFlavour );
    ~Truck();
  private:
    void main();
    bool hasNoCargo();
    unsigned int nextMachine(unsigned int);

    Printer& printer;
    NameServer& server;
    BottlingPlant& bottlingPlant;
    const unsigned int numberOfVendingMachines;
    const unsigned int maxStockPerFlavour;
    unsigned int lastMachineStocked;

    std::vector<unsigned int> cargo;

    enum States {
        Start = 'S',
        PickUp = 'P',
        Delivery = 'd',
        Unsuccessful = 'U',
        End = 'D',
        Finish = 'F'
    };
};

#endif