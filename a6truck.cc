#include "a6truck.h"
#include "a6main.h"
#include "a6nameserver.h"
#include "a6bottlingplant.h"
#include "a6printer.h"
#include "utilities.h"

#include <algorithm>
#include <iostream>

Truck::Truck(   Printer &prt, 
                NameServer &nameServer, 
                BottlingPlant &plant,
                unsigned int numberOfVendingMachines, 
                unsigned int maxStockPerFlavour) :  printer(prt),
                                                    server(nameServer),
                                                    bottlingPlant(plant),
                                                    numberOfVendingMachines(numberOfVendingMachines),
                                                    maxStockPerFlavour(maxStockPerFlavour),
                                                    cargo(VendingMachine::NUMBER_OF_FLAVOURS) 
{
}

Truck::~Truck() {
}

void Truck::main() {
    VendingMachine** machines = server.getMachineList();
    printer.print(Printer::Truck, (char)Start);

    // set to numberOfVendingMachines - 1 to start with 0th machine
    unsigned int currentMachine = numberOfVendingMachines - 1;
    while (true) {
        yield(A6::mprng(1,10));

        try {
            bottlingPlant.getShipment(cargo.data());
        } catch (BottlingPlant::Shutdown &shutdown) {
            break;
        }

        printer.print(Printer::Truck, (char)PickUp, sum(cargo));

        unsigned int finalMachine = currentMachine;
        while (true) { 

            // only deliver if there is cargo
            if (sum(cargo) == 0) {
                break;
            }

            currentMachine = (currentMachine + 1) % numberOfVendingMachines;

            printer.print(Printer::Truck, (char)Delivery, currentMachine, sum(cargo));

            {   // check inventory of vending machine
                unsigned int * inventory = machines[currentMachine]->inventory();
                for (unsigned int flavour = 0; flavour < VendingMachine::NUMBER_OF_FLAVOURS; flavour++) {
                    unsigned int currentStock = inventory[flavour];
                    if (currentStock + cargo[flavour] > maxStockPerFlavour) {
                        inventory[flavour] = maxStockPerFlavour;
                        cargo[flavour] -= (maxStockPerFlavour - currentStock);
                    } else {
                        inventory[flavour] += cargo[flavour];
                        cargo[flavour] = 0;
                    }
                }

                unsigned int amountInInventory = sum(inventory, (unsigned int)VendingMachine::NUMBER_OF_FLAVOURS);
                unsigned int maxStockPossible = numberOfVendingMachines * maxStockPerFlavour;
                if (amountInInventory < maxStockPossible) {
                    unsigned int availableSpace = maxStockPossible - amountInInventory;
                    printer.print(Printer::Truck, (char)Unsuccessful, currentMachine, availableSpace);
                }
            }

            machines[currentMachine]->restocked();

            printer.print(Printer::Truck, (char)End, currentMachine, sum(cargo));

            if ( currentMachine == finalMachine ) {
                break;
            }
        }
    }

    printer.print(Printer::Truck, (char)Finish);
}