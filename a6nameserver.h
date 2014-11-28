#ifndef _A6_NAME_SERVER_H__
#define _A6_NAME_SERVER_H__

#include "a6vendingmachine.h"

#include <map>
#include <deque>
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

    enum PrintStates {
        Start = 'S',
        Register = 'R',
        New = 'N',
        Finish = 'F'
    };
  private:
    void main();
    
    Printer& printer;
    const unsigned int numberOfStudents;
    const unsigned int numberOfVendingMachines;

    std::deque<unsigned int> machineIds;
    std::map<unsigned int, unsigned int> studentsMachine;
    std::vector<VendingMachine*> idToMachine;
    uCondition vendingMachineLock;

};

#endif