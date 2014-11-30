#ifndef _A6_PARENT_H__
#define _A6_PARENT_H__

#include "a6bank.h"

_Cormonitor Printer;

_Task Parent {
    void main();

    Printer& printer;
    Bank& bank;
    const unsigned int numberOfStudents;
    const unsigned int parentalDelay;

    enum PrintStates {
        Start = 'S',
        Deposit = 'D',
        Finish = 'F'
    };

public:
    Parent( Printer &prt, Bank &bank, unsigned int numberOfStudents, unsigned int parentalDelay );
    ~Parent();
};

#endif
