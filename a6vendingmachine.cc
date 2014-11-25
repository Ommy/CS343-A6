#include "a6vendingmachine.h"
#include "a6printer.h"
#include "a6nameserver.h"

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour )
: printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour)  {
    
    nameServer.VMregister(this);
    for (unsigned int i = 0; i < NUMBER_OF_FLAVOURS; ++i) {
        currentStock[i] = 0;
    }
    state = Success;
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
    if (card.getBalance() < sodaCost) {
        state = InsufficientFunds;
    } else if (currentStock[flavour] == 0) {
        state = InsufficientStock;
    } else {
        card.withdraw(sodaCost);
        currentStock[static_cast<int>(flavour)] -= 1;
        state = Success;
    }
}

unsigned int * VendingMachine::inventory() {
    return currentStock;
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
    while (true) {
        _Accept(~VendingMachine) {
            break;
        } or _Accept( inventory ) {
            state = Restocking;
        } or _When (state == Restocking) _Accept ( restocked ) {
            state = Success;
        } or _When (state == Success) _Accept ( buy ) {
            if (state == InsufficientFunds) {
                throw Funds();
            } else if (state == InsufficientStock) {
                throw Stock();
            }
            state = Success;
        }
    }
}