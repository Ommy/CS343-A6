#include "a6watcardoffice.h"
#include "a6main.h"
#include "utilities.h"

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers) : printer(prt), bank(bank), numberOfCouriers(numCouriers) {
    for (unsigned int i = 0; i < numberOfCouriers; i++) {
        couriers.push_back(new Courier(prt, bank, *this));
    }
}

WATCardOffice::~WATCardOffice() {
    for (unsigned int i = 0; i < couriers.size(); i++) {
        delete couriers[i];
    }

}

WATCard::FWATCard WATCardOffice::createJob(unsigned int sid, unsigned int amount, WATCard *card) {
    WATCardOffice::Args arg(sid, amount, card);    

    WATCardOffice::Job * job = new WATCardOffice::Job(arg);
    jobQueue.push_back(job);
    jobLock.signal();
    return job->result;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    return createJob(sid, amount, NULL);
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    return createJob(sid, amount, card);
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    while (jobQueue.empty()){
        jobLock.wait();
    }
    return poll_front(jobQueue);
}

void WATCardOffice::main() {
    while (true) {
        _Accept(~WATCardOffice) {
            break;
        } or _Accept(create) {

        } or _Accept(transfer) {

        } or _Accept(requestWork) {

        }
    }
}

void WATCardOffice::Courier::main() {
    while (true) {
        _Accept( ~Courier ) {
            break;
        } _Else {
            WATCardOffice::Job * job = office.requestWork();
            Args jobArgs = job->args;

            WATCard* card = jobArgs.card;
            if (jobArgs.card == NULL) {
                card = new WATCard();
            }

            bank.withdraw(jobArgs.sid, jobArgs.amount);
            card->deposit(jobArgs.amount);
            if (mprng(5) == 0) {
                delete card;
                job->result.exception(new WATCardOffice::Lost());
            } else {
                job->result.delivery(card);
            }
        }
    }
}

WATCardOffice::Courier::Courier(Printer &prt, Bank &bank, WATCardOffice& office)
: printer(prt), bank(bank), office(office) {

}
