#include "a6watcardoffice.h"
#include "a6main.h"
#include "a6printer.h"
#include "utilities.h"

#include <iostream>

WATCardOffice::WATCardOffice(   Printer &prt, 
                                Bank &bank, 
                                unsigned int numCouriers) : printer(prt), 
                                                            bank(bank), 
                                                            numberOfCouriers(numCouriers) {
    for (unsigned int i = 0; i < numberOfCouriers; i++) {
        couriers.push_back(new Courier(prt, bank, *this, i));
    }
}

WATCardOffice::~WATCardOffice() {
}

WATCard::FWATCard WATCardOffice::createJob(unsigned int sid, unsigned int amount, WATCard *card) {
    WATCardOffice::Args arg(sid, amount, card);
    WATCardOffice::Job * job = new WATCardOffice::Job(arg);
    jobQueue.push_back(job);
    return job->result;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    WATCard::FWATCard job = createJob(sid, amount, NULL);
    printer.print(Printer::WATCardOffice, (char)Create, sid, amount);
    return job;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    WATCard::FWATCard job = createJob(sid, amount, card);
    printer.print(Printer::WATCardOffice, (char)Transfer, sid, amount);
    return job;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    return poll_front(jobQueue);
}

void WATCardOffice::main() {
    printer.print(Printer::WATCardOffice, (char)Start);

    while (true) {
        _Accept(~WATCardOffice) {
            break;
        } or _Accept(create, transfer) {
        } or _When(!jobQueue.empty()) _Accept(requestWork) {
            printer.print(Printer::WATCardOffice, (char)Work);
        }
    }

    printer.print(Printer::WATCardOffice, (char)Finish);

    for (unsigned int i = 0; i < couriers.size(); i++) {
        delete couriers[i];
    }
}

WATCardOffice::Courier::Courier(Printer &prt, Bank &bank, WATCardOffice& office, unsigned int id)
: printer(prt), bank(bank), office(office), id(id) {

}

void WATCardOffice::Courier::main() {
    printer.print(Printer::Courier, id, (char)Start);

    while (true) {
        _Accept( ~Courier ) {
            break;
        } _Else {
            WATCardOffice::Job* job = office.requestWork();
            Args jobArgs = job->args;

            WATCard* card = jobArgs.card;
            if (jobArgs.card == NULL) {
                card = new WATCard();
            }

            printer.print(Printer::Courier, id, (char)StartTransfer);
            bank.withdraw(jobArgs.sid, jobArgs.amount);
            card->deposit(jobArgs.amount);
            printer.print(Printer::Courier, id, (char)CompleteTransfer);
            // if (A6::mprng(5) == 0) {
                // No need to delete, assumption that Future_ISM handles that for us
                // delete card;
                // job->result.exception(new WATCardOffice::Lost());
            // } else {
                job->result.delivery(card);
            // }
        }
    }

    printer.print(Printer::Courier, id, (char)Finish);
}
