#ifndef _A6_WATCARD_OFFICE_H__
#define _A6_WATCARD_OFFICE_H__

#include "a6bank.h"
#include "a6watcard.h"
#include <vector>
#include <deque>
#include <map>
#include <memory>

_Cormonitor Printer;

_Task WATCardOffice {
    enum JobType {
        KillJob,
        CreateJob,
        TransferJob
    };

    struct Args {
        Args( JobType type, unsigned int sid, unsigned amount, WATCard * card );
        const JobType type;
        const unsigned int sid;
        const unsigned int amount;
        WATCard* card;
    };

    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };

    _Task Courier {
    public:
        Courier( Printer &prt, Bank &bank, WATCardOffice &office, unsigned int id );
    private:
        void main();

        Printer& printer;
        Bank& bank;
        WATCardOffice& office;
        const unsigned int id;

        enum PrintStates {
            Start = 'S',
            StartTransfer = 't',
            CompleteTransfer = 'T',
            Finish = 'F'
        };
    };                 // communicates with bank

    void main();
    WATCard::FWATCard createJob( JobType type, unsigned int sid, unsigned int amount, WATCard* card );

    Printer& printer;
    Bank& bank;
    const unsigned int numberOfCouriers;
    std::vector<std::shared_ptr<Courier>> couriers;
    std::deque<WATCardOffice::Job*> jobQueue;

    enum PrintStates {
        Start = 'S',
        Work = 'W',
        Create = 'C',
        Transfer = 'T',
        Finish = 'F'
    };

public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numberOfCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
