#include "a6truck.h"
#include "a6main.h"
#include "a6nameserver.h"
#include "a6bottlingplant.h"

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
                unsigned int numVendingMachines, unsigned int maxStockPerFlavour) {
    printer = &prt;
    server = &nameServer;
    bottlingPlant = &plant;

    numberOfVendingMachines = numVendingMachines;
    maxStockOfEachFlavour = maxStockPerFlavour;
    lastMachineStocked = 0;

    cargo = new unsigned int [VendingMachine::NUMBER_OF_FLAVOURS];
}


void Truck::main() {
    VendingMachine ** machines = server->getMachineList();
    while (true) {
        _Accept (~Truck) {
            break;
        } _Else {
            yield(mprng(1,10));
            try {
                bottlingPlant->getShipment(cargo);
            } _CatchResume(BottlingPlant::Shutdown &shutdown) {
                return;
            }
    Restock:for (unsigned int i = nextMachine(lastMachineStocked); i != lastMachineStocked; i = nextMachine(i)) {
                unsigned int * inventory = machines[i]->inventory();
                for (unsigned int flavour = 0; flavour < VendingMachine::NUMBER_OF_FLAVOURS; flavour++) {
                    unsigned int currentStock = inventory[flavour];
                    if (currentStock + cargo[flavour] > maxStockOfEachFlavour) {
                        inventory[flavour] = maxStockOfEachFlavour;
                        cargo[flavour] -= (maxStockOfEachFlavour - currentStock);
                    } else {
                        inventory[flavour] += cargo[flavour];
                        cargo[flavour] = 0;
                    }
                    if (hasNoCargo()) {
                        lastMachineStocked = i;
                        break Restock;
                    }
                }
            }
        }
    }
}

unsigned int Truck::nextMachine(unsigned int machine) {
    return ((machine + 1) % VendingMachine::NUMBER_OF_FLAVOURS);
}

bool Truck::hasNoCargo() {
    for (int i = 0; i < VendingMachine::NUMBER_OF_FLAVOURS; i++) {
        if (cargo[i] != 0) {
            return false;
        }
    }
    return true;
}

Truck::~Truck() {
    delete[] cargo;
}