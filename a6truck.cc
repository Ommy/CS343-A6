#include "a6truck.h"
#include "MPRNG.h"
#include "a6nameserver.h"
#include "a6bottlingplant.h"

extern MPRNG mprng;

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
    while (true) {
        _Accept (~Truck) {
            break;
        } _Else {
            VendingMachine ** machines = server->getMachineList();
            yield(mprng(1,10));
            try {
                bottlingPlant->getShipment(cargo);
            } _CatchResume(BottlingPlant::Shutdown &shutdown) {
                return;
            }
            unsigned int i = nextMachine(lastMachineStocked);
            while (true) {
                unsigned int * inventory = machines[i]->inventory();
                for (unsigned int flavour = 0; flavour < VendingMachine::NUMBER_OF_FLAVOURS; flavour++) {
                    unsigned int currentStock = inventory[flavour];
                    if (currentStock + cargo[flavour] > maxStockOfEachFlavour) {
                        inventory[flavour] = maxStockOfEachFlavour;
                        cargo[flavour] -= (maxStockOfEachFlavour - currentStock);
                    } else {
                        inventory[flavour] = cargo[flavour];
                        cargo[flavour] = 0;
                    }
                }
                i = nextMachine(i);
                if (i == lastMachineStocked) {
                    break;
                }
            }
        }
    }
}

unsigned int Truck::nextMachine(unsigned int machine) {
    return ((machine + 1) % VendingMachine::NUMBER_OF_FLAVOURS);
}

Truck::~Truck() {
    delete[] cargo;
}