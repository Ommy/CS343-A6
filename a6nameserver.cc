#include "a6nameserver.h"
#include "a6printer.h"

#include <iostream>

NameServer::NameServer( Printer &prt, 
                        unsigned int numVendingMachines, 
                        unsigned int numStudents ) :    printer(prt),
                                                        numberOfStudents(numStudents),
                                                        numberOfVendingMachines(numVendingMachines),
                                                        idToMachine(numberOfVendingMachines)
                                                        {
}

NameServer::~NameServer(){
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    printer.print(Printer::NameServer, (char)Register, vendingmachine->getId());

    idToMachine[vendingmachine->getId()] = vendingmachine;
    machineIds.push_back(vendingmachine->getId());

    if (machineIds.size() == numberOfVendingMachines) {
        for (unsigned int sid = 0; sid < numberOfStudents; sid++) {
            studentsMachine[sid] = machineIds[(sid % numberOfVendingMachines)];
        }
    }
}

VendingMachine * NameServer::getMachine( unsigned int sid ) {
    unsigned int current = studentsMachine[sid];
    printer.print(Printer::NameServer, (char)New, sid, idToMachine[current]->getId());
    studentsMachine[sid] = (current + 1) % numberOfVendingMachines;
    return idToMachine[current];
}

VendingMachine ** NameServer::getMachineList() {
    return idToMachine.data();
}

void NameServer::main() {
    printer.print(Printer::NameServer, (char)Start);

    while (true) {
        _Accept (~NameServer) {
            break;
        } or _When(machineIds.size() < numberOfVendingMachines) _Accept ( VMregister ) {

        } or _When(machineIds.size() == numberOfVendingMachines) _Accept ( getMachine, getMachineList ) {

        }
    }
    
    printer.print(Printer::NameServer, (char)Finish);
}