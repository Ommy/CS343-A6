#ifndef _A6_VENDING_MACHINE
#define _A6_VENDING_MACHINE

#include "a6watcard.h"

#include <array>
#include <memory>

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
    void buy( Flavours flavour, WATCard &watcard );
    unsigned int* inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
    private:
    enum State {
        InsufficientFunds,
        InsufficientStock,
        Success
    };

    struct BuyOrder {
        Flavours flavour;
        WATCard& watcard;
        State result;
        BuyOrder( Flavours flavour, WATCard& watcard );
    };

    void main();

    Printer& printer;
    NameServer& nameServer;
    const unsigned int id;
    const unsigned int sodaCost;
    const unsigned int maxStockPerFlavour;

    std::array<unsigned int, NUMBER_OF_FLAVOURS> stock;
    std::shared_ptr<BuyOrder> order;
    uCondition processOrderCondition;

    enum PrintStates {
        Start = 'S',
        StartReload = 'r',
        CompleteReload = 'R',
        Bought = 'B',
        Finish = 'F',
    };
};

#endif