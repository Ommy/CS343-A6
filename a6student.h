#ifndef _A6_STUDENT_H__
#define _A6_STUDENT_H__

#include "a6watcardoffice.h"
#include "a6vendingmachine.h"

_Task NameServer;
_Cormonitor Printer;

_Task Student {
    void main();
  public:
    Student(    Printer &prt, 
                NameServer &nameServer, 
                WATCardOffice &cardOffice, 
                unsigned int id, 
                unsigned int maxPurchases );
  private:
    Printer & printer;
    NameServer & nameServer;
    WATCardOffice & cardOffice;
    const unsigned int studentId;
    const unsigned int maxPurchases;
    VendingMachine * machine;
    enum States {
        Started = 'S',
        SelectedVendingMachine = 'V',
        Bought = 'B',
        Lost = 'L',
        Finished = 'F'
    };
};

#endif