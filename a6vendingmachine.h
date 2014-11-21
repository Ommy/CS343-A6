#ifndef _A6_VENDING_MACHINE
#define _A6_VENDING_MACHINE

#include "a6watcard.h"


_Cormonitor Printer;
_Task NameServer;

_Task VendingMachine {
    void main();
  public:
    enum Flavours { 
        RED
    };
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif