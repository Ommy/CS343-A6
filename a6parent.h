#ifndef _A6_PARENT_H__
#define _A6_PARENT_H__

#include "a6bank.h"

_Cormonitor Printer;

_Task Parent {
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif