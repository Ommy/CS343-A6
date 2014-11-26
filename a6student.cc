#include "a6student.h"
#include "a6main.h"
#include "a6nameserver.h"
#include "a6printer.h"

Student::Student(   Printer &prt, 
                    NameServer &nameServer, 
                    WATCardOffice &cardOffice, 
                    unsigned int id, 
                    unsigned int maxPurchases ) :   printer(prt), 
                                                    nameServer(nameServer), 
                                                    cardOffice(cardOffice), 
                                                    studentId(id), 
                                                    maxPurchases(maxPurchases) {
}

void Student::main() {
    unsigned int numberOfSodasToPurchase = A6::mprng(1, maxPurchases);
    VendingMachine::Flavours favouriteFlavour = static_cast<VendingMachine::Flavours>(A6::mprng(0, 3));
    VendingMachine* machine = nameServer.getMachine(studentId);

    printer.print(Printer::Student, (char)Started, favouriteFlavour, numberOfSodasToPurchase);

    WATCard::FWATCard fwatcard;
    WATCard* watcard = NULL;

    bool hasLostCard = false;
    unsigned int numberOfSodasPurchased = 0;

    while (numberOfSodasPurchased < numberOfSodasToPurchase) {
        try {
            if (numberOfSodasPurchased == 0 || hasLostCard) {
                fwatcard = cardOffice.create(studentId, 5);
            }
            if (!hasLostCard) {
                yield(A6::mprng(1,10));
            } else {
                hasLostCard = false;
            }
            watcard = fwatcard();
            machine->buy(favouriteFlavour, *watcard);
            numberOfSodasPurchased++;

        } catch (WATCardOffice::Lost& lost) {
            hasLostCard = true;
        } catch (VendingMachine::Funds& funds) {
            fwatcard = cardOffice.transfer(studentId, 5 + machine->cost(), watcard);
        } catch (VendingMachine::Stock& stock) {
            machine = nameServer.getMachine(studentId);
            printer.print(Printer::Student, (char)SelectedVendingMachine, machine->getId());
        }
    }
}