#include "a6truck.h"
#include "a6main.h"
#include "a6nameserver.h"
#include "a6bottlingplant.h"

#include <algorithm>

Truck::Truck(   Printer &prt, 
                NameServer &nameServer, 
                BottlingPlant &plant,
                unsigned int numVendingMachines, 
                unsigned int maxStockPerFlavour) :  printer(prt),
                                                    server(nameServer),
                                                    bottlingPlant(plant),
                                                    numberOfVendingMachines(numVendingMachines),
                                                    maxStockPerFlavour(maxStockPerFlavour),
                                                    lastMachineStocked(0),
                                                    cargo(VendingMachine::NUMBER_OF_FLAVOURS) {
}

Truck::~Truck() {
}

void Truck::main() {
    VendingMachine** machines = server.getMachineList();

    while (true) {
        _Accept (~Truck) {
            break;
        } _Else {
            yield(A6::mprng(1,10));
            try {
                bottlingPlant.getShipment(cargo.data());
            } _CatchResume(BottlingPlant::Shutdown &shutdown) {
                return;
            }

            for (unsigned int i = nextMachine(lastMachineStocked); i != lastMachineStocked; i = nextMachine(i)) {
                unsigned int * inventory = machines[i]->inventory();
                for (unsigned int flavour = 0; flavour < VendingMachine::NUMBER_OF_FLAVOURS; ++flavour) {
                    unsigned int currentStock = inventory[flavour];
                    
                    if (currentStock + cargo[flavour] > maxStockPerFlavour) {
                        inventory[flavour] = maxStockPerFlavour;
                        cargo[flavour] -= (maxStockPerFlavour - currentStock);
                    } else {
                        inventory[flavour] += cargo[flavour];
                        cargo[flavour] = 0;
                    }

                    machines[i]->restocked();
                }

                if (hasNoCargo()) {
                    lastMachineStocked = i;
                    break;
                }
            }
        }
    }
}

unsigned int Truck::nextMachine(unsigned int machine) {
    return ((machine + 1) % VendingMachine::NUMBER_OF_FLAVOURS);
}

bool Truck::hasNoCargo() {
    return (unsigned int)std::count(cargo.begin(), cargo.end(), 0) == cargo.size();
}