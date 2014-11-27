#include "a6truck.h"
#include "a6main.h"
#include "a6nameserver.h"
#include "a6bottlingplant.h"
#include "a6printer.h"

#include <algorithm>
#include <iostream>

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
    printer.print(Printer::Truck, (char)Start);

    VendingMachine** machines = server.getMachineList();
    while (true) {
        yield(A6::mprng(1,10));

        try {
            bottlingPlant.getShipment(cargo.data());
        } catch (BottlingPlant::Shutdown &shutdown) {
            break;
        }

        printer.print(Printer::Truck, (char)PickUp, std::accumulate(cargo.begin(), cargo.end(), 0));

        for (unsigned int i = nextMachine(lastMachineStocked); i != lastMachineStocked; i = nextMachine(i)) {
            printer.print(Printer::Truck, (char)Delivery, i, std::accumulate(cargo.begin(), cargo.end(), 0));

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
            }

            unsigned int amountInInventory = 0;
            for (unsigned int flavour = 0; flavour < VendingMachine::NUMBER_OF_FLAVOURS; ++flavour) {
                amountInInventory += inventory[flavour];
            }
            
            if (amountInInventory < numberOfVendingMachines * maxStockPerFlavour) {
                printer.print(Printer::Truck, (char)Unsuccessful, i, numberOfVendingMachines * maxStockPerFlavour - amountInInventory);
            }

            machines[i]->restocked();

            printer.print(Printer::Truck, (char)End, i, std::accumulate(cargo.begin(), cargo.end(), 0));

            if (hasNoCargo()) {
                lastMachineStocked = i;
                break;
            }
        }
    }

    printer.print(Printer::Truck, (char)Finish);
}

bool Truck::hasNoCargo() {
    return (unsigned int)std::count(cargo.begin(), cargo.end(), 0) == cargo.size();
}

unsigned int Truck::nextMachine(unsigned int machine) {
    return ((machine + 1) % numberOfVendingMachines);
}