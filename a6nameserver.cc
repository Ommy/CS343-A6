#include "a6nameserver.h"
#include "a6printer.h"

NameServer::NameServer( Printer &prt, 
                        unsigned int numVendingMachines, 
                        unsigned int numStudents ) :    printer(prt),
                                                        numberOfStudents(numStudents),
                                                        numberOfVendingMachines(numVendingMachines),
                                                        numberOfMachinesRegistered(0),
                                                        machines(numberOfVendingMachines),
                                                        machineAssignments(numberOfStudents) {
    for (unsigned int i = 0; i < numberOfStudents; i++) {
        machineAssignments[i] = i % numberOfVendingMachines;
    }
}

NameServer::~NameServer(){
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    printer.print(Printer::NameServer, (char)Register, vendingmachine->getId());
    machines[numberOfMachinesRegistered] = vendingmachine;
    numberOfMachinesRegistered += 1;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    unsigned int current = machineAssignments[id];
    unsigned int next = (current + 1) % numberOfVendingMachines;
    machineAssignments[id] = next;

    printer.print(Printer::NameServer, (char)New, id, next);
    return machines[machineAssignments[id]];
}

VendingMachine ** NameServer::getMachineList() {
    return machines.data();
}

void NameServer::main() {
    printer.print(Printer::NameServer, (char)Start);

    while (true) {
        _Accept (~NameServer) {
            break;
        } or _When(numberOfMachinesRegistered < numberOfVendingMachines) _Accept ( VMregister ) {

        } or _When(numberOfMachinesRegistered == numberOfVendingMachines) _Accept ( getMachine, getMachineList ) {

        }
    }
    
    printer.print(Printer::NameServer, (char)Finish);
}