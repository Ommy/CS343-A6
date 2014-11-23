#include "a6nameserver.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) {
    printer = &prt;
    numberOfStudents = numStudents;
    registeredMachine = 0;
    numberOfVendingMachines = numVendingMachines;
    machines = new VendingMachine*[numVendingMachines];
    for (unsigned int i = 0; i < numberOfStudents; i++) {
        machineAssignment[i] = machines[(i % numberOfVendingMachines)];
        studentsCurrentMachine[i] = i % numberOfVendingMachines;
    }
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    machines[registeredMachine++] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    unsigned int currentMachine = studentsCurrentMachine[id];
    unsigned int nextMachine = (currentMachine+1) % numberOfVendingMachines;
    studentsCurrentMachine[id] = nextMachine;
    return machineAssignment[nextMachine];
}

VendingMachine ** NameServer::getMachineList() {
    return machines;
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
    delete[] machines;
}