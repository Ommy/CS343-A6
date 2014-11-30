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
    unsigned int numberOfVendingMachines,
    unsigned int numberOfStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *    getMachine( unsigned int id );
    VendingMachine**    getMachineList();

    private:
    void main();

    Printer& printer;
    const unsigned int numberOfStudents;
    const unsigned int numberOfVendingMachines;

    std::deque<unsigned int> machinesRegistered;
    std::vector<VendingMachine*> machines;
    std::map<unsigned int, unsigned int> machineUsedByStudent;

    enum PrintStates {
        Start = 'S',
        Register = 'R',
        New = 'N',
        Finish = 'F'
    };
};

#endif