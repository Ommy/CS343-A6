#ifndef _A6_STUDENT_H__
#define _A6_STUDENT_H__

#include "a6watcardoffice.h"

_Task NameServer;
_Cormonitor Printer;

_Task Student {
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
};

#endif