#include "a6nameserver.h"

NameServer::NameServer( Printer &prt, 
                        unsigned int numVendingMachines, 
                        unsigned int numStudents ) :    printer(prt),
                                                        numberOfStudents(numStudents),
                                                        numberOfVendingMachines(numVendingMachines),
                                                        numberOfMachinesRegistered(0),
                                                        machines(numberOfVendingMachines) {
    for (unsigned int i = 0; i < numberOfStudents; i++) {
        machineAssignment[i] = machines[(i % numberOfVendingMachines)];
        studentsCurrentMachine[i] = i % numberOfVendingMachines;
    }
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    machines[numberOfMachinesRegistered++] = vendingmachine;
    if (numberOfMachinesRegistered == numberOfVendingMachines) {
        while (!vendingMachineLock.empty()) {
            vendingMachineLock.signal();
        }
    } 
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    while (numberOfMachinesRegistered < numberOfVendingMachines) {
        vendingMachineLock.wait();
    }
    unsigned int currentMachine = studentsCurrentMachine[id];
    unsigned int nextMachine = (currentMachine+1) % numberOfVendingMachines;
    studentsCurrentMachine[id] = nextMachine;
    return machineAssignment[nextMachine];
}

VendingMachine ** NameServer::getMachineList() {
    while (numberOfMachinesRegistered < numberOfVendingMachines) {
        vendingMachineLock.wait();
    }
    return machines.data();
}

void NameServer::main() {
    while (true) {
        _Accept (~NameServer) {
            break;
        } or _Accept ( VMregister ) {
        } or _Accept ( getMachine ){
        } or _Accept ( getMachineList ){
        }
    }
}

NameServer::~NameServer(){
    for (unsigned int i = 0; i < machines.size(); ++i) {
        delete machines[i];
    }
}