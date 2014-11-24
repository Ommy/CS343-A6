#ifndef _A6_WATCARD_OFFICE_H__
#define _A6_WATCARD_OFFICE_H__

#include "a6bank.h"
#include "a6watcard.h"
#include <vector>
#include <deque>
#include <map>

_Cormonitor Printer;

_Task WATCardOffice {
    struct Args {
        Args(unsigned int sid, unsigned amount, WATCard * card) : sid(sid), amount(amount), card(card){}
        unsigned int sid;
        unsigned int amount;
        WATCard * card;
    };

    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };
    
    _Task Courier { 
      public:
        Courier(Printer &prt, Bank &bank, WATCardOffice &office);
      private:
        void main();

        Printer & printer;
        Bank & bank;
        WATCardOffice & office;
    };                 // communicates with bank

    void main();
  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
  private:
    WATCard::FWATCard createJob(unsigned int sid, unsigned int amount, WATCard* card);

    Printer & printer;
    Bank & bank;
    unsigned int numberOfCouriers;
    std::vector<Courier*> couriers;
    std::deque<WATCardOffice::Job*> jobQueue;
    uCondition jobLock;
    std::map<unsigned int, WATCard*> studentsWatcard;

};

#endif