#ifndef _A6_VENDING_MACHINE
#define _A6_VENDING_MACHINE

#include "a6watcard.h"

#include <array>

_Cormonitor Printer;
_Task NameServer;

_Task VendingMachine {
  public:
    enum Flavours { 
        BLUE_BLACK_CHERRY = 0,
        CLASSICAL_CREAM_SODA,
        ROCK_ROOT_BEER,
        JAZZ_LIME,
        NUMBER_OF_FLAVOURS
    };

    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, 
                    NameServer &nameServer, 
                    unsigned int id, 
                    unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
private:
    enum State {
        InsufficientFunds,
        InsufficientStock,
        Restocking,
        Success
    };

    void main();

    Printer& printer;
    NameServer& nameServer;
    const unsigned int id;
    const unsigned int sodaCost;
    const unsigned int maxStockPerFlavour;

    std::array<unsigned int, NUMBER_OF_FLAVOURS> stock;
    State state;
};

#endif