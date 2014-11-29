#ifndef _A6_STUDENT_H__
#define _A6_STUDENT_H__

#include "a6watcardoffice.h"
#include "a6vendingmachine.h"

_Task NameServer;
_Cormonitor Printer;

_Task Student {
    void main();
    
    Printer & printer;
    NameServer & nameServer;
    WATCardOffice & cardOffice;
    const unsigned int studentId;
    const unsigned int maxPurchases;
    static const unsigned int INITIAL_BALANCE = 5;

    enum PrintStates {
        Start = 'S',
        Selected = 'V',
        Bought = 'B',
        Lost = 'L',
        Finish = 'F'
    };
    
  public:
    Student(    Printer &prt, 
                NameServer &nameServer, 
                WATCardOffice &cardOffice, 
                unsigned int id, 
                unsigned int maxPurchases );
    ~Student();
};

#endif