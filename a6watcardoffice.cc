#include "a6watcardoffice.h"
#include "a6main.h"
#include "a6printer.h"
#include "utilities.h"

#include <iostream>

WATCardOffice::WATCardOffice(   Printer& prt,
                                Bank& bank,
                                unsigned int numberOfCouriers ) : printer( prt ),
                                         bank( bank ),
                                         numberOfCouriers( numberOfCouriers ),
couriers( numberOfCouriers ) {
}

WATCardOffice::~WATCardOffice() {
}

WATCard::FWATCard WATCardOffice::createJob( JobType type, unsigned int sid, unsigned int amount,
        WATCard* card ) {
    WATCardOffice::Args arg( type, sid, amount, card );
    WATCardOffice::Job* job = new WATCardOffice::Job( arg );
    jobQueue.push_back( job );
    return job->result;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    WATCard::FWATCard job = createJob( CreateJob, sid, amount, NULL );
    printer.print( Printer::WATCardOffice, ( char )Create, sid, amount );
    return job;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard* card ) {
    WATCard::FWATCard job = createJob( TransferJob, sid, amount, card );
    printer.print( Printer::WATCardOffice, ( char )Transfer, sid, amount );
    return job;
}

WATCardOffice::Job* WATCardOffice::requestWork() {
    return poll_front( jobQueue );
}

void WATCardOffice::main() {
    printer.print( Printer::WATCardOffice, ( char )Start );

    for ( unsigned int i = 0; i < numberOfCouriers; i++ ) {
        couriers[i] = std::shared_ptr<Courier>( new Courier( printer, bank, *this, i ) );
    }

    while ( true ) {
        _Accept( ~WATCardOffice ) {
            break;
        }
        or _Accept( create, transfer ) {

        } or _When( !jobQueue.empty() ) _Accept( requestWork ) {
            printer.print( Printer::WATCardOffice, ( char )Work );
        }
    }

    for ( unsigned int i = 0; i < couriers.size(); ++i ) {
        createJob( KillJob, 0, 0, NULL );
    }

    while ( !jobQueue.empty() ) {
        _Accept( requestWork );
    }

    for ( unsigned int i = 0; i < couriers.size(); ++i ) {
        couriers[i].reset();
    }

    printer.print( Printer::WATCardOffice, ( char )Finish );
}

WATCardOffice::Args::Args(  JobType type,
                            unsigned int sid,
                            unsigned amount,
                            WATCard* card ) :   type( type ),
sid( sid ),
amount( amount ),
card( card ) {

}

WATCardOffice::Courier::Courier( Printer& prt,
                                 Bank& bank,
                                 WATCardOffice& office,
                                 unsigned int id ) : printer( prt ),
bank( bank ),
office( office ),
id( id ) {

}

void WATCardOffice::Courier::main() {
    printer.print( Printer::Courier, id, ( char )Start );

    while ( true ) {
        std::unique_ptr<WATCardOffice::Job> job( office.requestWork() );

        WATCard* card = NULL;
        Args args = job->args;
        if ( args.type == KillJob ) {
            break;
        }

        if ( args.type == CreateJob ) {
            card = new WATCard();
        } else if ( args.type == TransferJob ) {
            card = args.card;
        }

        printer.print( Printer::Courier, id, ( char )StartTransfer, args.sid, args.amount );

        bank.withdraw( args.sid, args.amount );
        card->deposit( args.amount );

        printer.print( Printer::Courier, id, ( char )CompleteTransfer, args.sid, args.amount );

        if ( A6::mprng( 1, 6 ) == 1 ) {
            delete card;
            job->result.exception( new WATCardOffice::Lost() );
        } else {
            job->result.delivery( card );
        }
    }

    printer.print( Printer::Courier, id, ( char )Finish );
}
