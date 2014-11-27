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

WATCard::FWATCard WATCardOffice::createJob(Type type, unsigned int sid, unsigned int amount, WATCard *card) {
    WATCardOffice::Args arg(type, sid, amount, card);
    WATCardOffice::Job * job = new WATCardOffice::Job(arg);
    jobQueue.push_back(job);
    return job->result;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    WATCard::FWATCard job = createJob(CreateJob, sid, amount, NULL);
    printer.print(Printer::WATCardOffice, (char)Create, sid, amount);
    return job;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    WATCard::FWATCard job = createJob(TransferJob, sid, amount, card);
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

    for (unsigned int i = 0; i < couriers.size(); ++i) {
        createJob(Exit, 0, 0, 0);
    }

    while (!jobQueue.empty()) {
        _Accept(requestWork);
    }

    for (unsigned int i = 0; i < couriers.size(); ++i) {
        delete couriers[i];
    }
}

WATCardOffice::Args::Args(  Type type, 
                            unsigned int sid, 
                            unsigned amount, 
                            WATCard * card) :   type(type),
                                                sid(sid), 
                                                amount(amount), 
                                                card(card) {

}

WATCardOffice::Courier::Courier(Printer &prt, Bank &bank, WATCardOffice& office, unsigned int id)
: printer(prt), bank(bank), office(office), id(id) {

}

void WATCardOffice::Courier::main() {
    printer.print(Printer::Courier, id, (char)Start);

    while (true) {
        WATCardOffice::Job* job = office.requestWork();

        Args jobArgs = job->args;
        WATCard* card = NULL;
        switch (jobArgs.type) {
            case CreateJob:
                card = new WATCard();
                break;
            case TransferJob:
                card = jobArgs.card;
                break;
            default:
                printer.print(Printer::Courier, id, (char)Finish);
                delete job;
                return;
        }

        printer.print(Printer::Courier, id, (char)StartTransfer);
        bank.withdraw(jobArgs.sid, jobArgs.amount);
        card->deposit(jobArgs.amount);
        printer.print(Printer::Courier, id, (char)CompleteTransfer);
        if (A6::mprng(5) == 0) {
            delete card;
            job->result.exception(new WATCardOffice::Lost());
        } else {
            job->result.delivery(card);
        }
        delete job;
    }
}
