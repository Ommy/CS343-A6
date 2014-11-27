#include "a6vendingmachine.h"
#include "a6printer.h"
#include "a6nameserver.h"

#include <iostream>

VendingMachine::VendingMachine( Printer &prt, 
                                NameServer &nameServer, 
                                unsigned int id, 
                                unsigned int sodaCost, 
                                unsigned int maxStockPerFlavour ) : printer(prt), 
                                                                    nameServer(nameServer), 
                                                                    id(id), 
                                                                    sodaCost(sodaCost), 
                                                                    maxStockPerFlavour(maxStockPerFlavour) {
    
    nameServer.VMregister(this);
    stock.fill(0);
}

void VendingMachine::buy(Flavours flavour, WATCard &card ) {
    buyFlavour = flavour;
    buyCard = &card;
    waitCondition.wait();

    if (state == InsufficientFunds) {
        uRendezvousAcceptor();
        throw Funds();
    } else if (state == InsufficientStock) {
        uRendezvousAcceptor();
        throw Stock();
    }
    
    state = Success;
}

unsigned int * VendingMachine::inventory() {
    return stock.data();
}

void VendingMachine::restocked() {
}

unsigned int VendingMachine::cost() {
    return sodaCost;
}

unsigned int VendingMachine::getId() {
    return id;
}

void VendingMachine::main() {
    printer.print(Printer::Vending, id, (char)Start, sodaCost);

    state = Success;

    while (true) {
        _Accept(~VendingMachine) {
            break;
        } or _Accept( inventory ) {
            printer.print(Printer::Vending, id, (char)StartReload);
            _Accept(restocked) {
                printer.print(Printer::Vending, id, (char)CompleteReload);
            }
        } or _Accept ( buy ) {
            if (buyCard->getBalance() < sodaCost) {
                state = InsufficientFunds;
            } else if (stock[buyFlavour] == 0) {
                state = InsufficientStock;
            } else {
                buyCard->withdraw(sodaCost);
                stock[static_cast<int>(buyFlavour)] -= 1;
                printer.print(Printer::Vending, id, (char)Bought, buyFlavour, stock[static_cast<int>(buyFlavour)]);
                state = Success;
            }
            waitCondition.signalBlock();
        }
    }

    printer.print(Printer::Vending, id, (char)Finish);
}