#ifndef _A6_NAME_SERVER_H__
#define _A6_NAME_SERVER_H__

#include "a6vendingmachine.h"

#include <map>
#include <vector>

_Cormonitor Printer;

_Task NameServer {
  public:
    NameServer( Printer &prt, 
                unsigned int numVendingMachines, 
                unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
  private:
    void main();
    
    Printer& printer;
    const unsigned int numberOfStudents;
    const unsigned int numberOfVendingMachines;

    unsigned int numberOfMachinesRegistered;
    std::vector<VendingMachine*> machines;
    std::map<unsigned int, VendingMachine*> machineAssignment;
    std::map<unsigned int, unsigned int> studentsCurrentMachine;
    uCondition vendingMachineLock;

};

#endif