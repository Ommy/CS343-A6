#ifndef _A6_NAME_SERVER
#define _A6_NAME_SERVER

#include "a6vendingmachine.h"
#include <map>

_Cormonitor Printer;

_Task NameServer {
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
  private:
    void main();
    unsigned int numberOfStudents;
    unsigned int numberOfVendingMachines;
    unsigned int registeredMachine;
    Printer * printer;
    VendingMachine ** machines;
    std::map<unsigned int, VendingMachine*> machineAssignment;
    std::map<unsigned int, unsigned int> studentsCurrentMachine;
    uCondition vendingMachineLock;

};

#endif