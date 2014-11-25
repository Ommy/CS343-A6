#include "a6student.h"
#include "a6main.h"
#include "a6nameserver.h"

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ) 
: printer(prt), nameServer(nameServer), cardOffice(cardOffice), studentID(id), maxPurchases(maxPurchases) {
}

void Student::main() {
    numberOfSodasToPurchase = mprng(1,maxPurchases);
    favouriteFlavour = static_cast<VendingMachine::Flavours>(mprng(0, 3));
    machine = nameServer.getMachine(studentID);
    unsigned int numberOfSodasPurchased = 0;
    WATCard::FWATCard fwatcard;
    bool hasLostCard = false;
    WATCard * watcard = NULL;

    while (numberOfSodasPurchased < numberOfSodasToPurchase) {
        try {
            if (numberOfSodasPurchased == 0 || hasLostCard) {
                fwatcard = cardOffice.create(studentID, 5);
            }
            if (!hasLostCard) {
                yield(mprng(1,10));
            } else {
                hasLostCard = false;
            }
            watcard = fwatcard();
            machine->buy(favouriteFlavour, *watcard);
            numberOfSodasPurchased++;

        } catch (WATCardOffice::Lost& lost) {
            hasLostCard = true;
        } catch (VendingMachine::Funds& funds) {
            fwatcard = cardOffice.transfer(studentID, 5 + machine->cost(), watcard);
        } catch (VendingMachine::Stock& stock) {
            machine = nameServer.getMachine(studentID);
        }

    }
}