#ifndef _A6_WATCARD_OFFICE_H__
#define _A6_WATCARD_OFFICE_H__

#include "a6bank.h"
#include "a6watcard.h"

_Cormonitor Printer;

_Task WATCardOffice {
    struct Args {

    };

    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };
    
    _Task Courier { 

    };                 // communicates with bank

    void main();
  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif