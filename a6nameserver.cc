#include "a6nameserver.h"
#include "a6printer.h"

#include <iostream>

NameServer::NameServer( Printer& prt, 
                        unsigned int numberOfVendingMachines,
                        unsigned int numberOfStudents ) : printer( prt ), 
                                                          numberOfStudents( numberOfStudents ), 
                                                          numberOfVendingMachines( numberOfVendingMachines ), 
                                                          machines( numberOfVendingMachines ) {
}

NameServer::~NameServer() {
}

void NameServer::VMregister( VendingMachine* vendingmachine ) {
    machines[vendingmachine->getId()] = vendingmachine;
    machinesRegistered.push_back( vendingmachine->getId() );

    printer.print( Printer::NameServer, ( char )Register, vendingmachine->getId() );
}

VendingMachine* NameServer::getMachine( unsigned int sid ) {
    unsigned int currentMachineId = machineUsedByStudent[sid];

    // cycle through next machines to retrieve for this student
    machineUsedByStudent[sid] = ( currentMachineId + 1 ) % numberOfVendingMachines;

    VendingMachine* currentMachine = machines[currentMachineId];
    printer.print( Printer::NameServer, ( char )New, sid, currentMachine->getId() );
    return currentMachine;
}

VendingMachine** NameServer::getMachineList() {
    return machines.data();
}

void NameServer::main() {
    printer.print( Printer::NameServer, ( char )Start );

    while ( true ) {
        _Accept ( ~NameServer ) {
            break;
        }
        or _When( machinesRegistered.size() < numberOfVendingMachines ) _Accept ( VMregister ) {
            if ( machinesRegistered.size() == numberOfVendingMachines ) {
                for ( unsigned int sid = 0; sid < numberOfStudents; ++sid ) {
                    machineUsedByStudent[sid] = machinesRegistered[( sid % numberOfVendingMachines )];
                }
            }
        }
        or _When( machinesRegistered.size() == numberOfVendingMachines ) _Accept ( getMachine,
                getMachineList ) {

        }
    }

    printer.print( Printer::NameServer, ( char )Finish );
}
