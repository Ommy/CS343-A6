#include "a6student.h"
#include "a6main.h"
#include "a6nameserver.h"
#include "a6printer.h"

#include <memory>
#include <iostream>

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

Student::~Student() {
}

void Student::main() {
    unsigned int numberOfSodasToPurchase = A6::mprng(1, maxPurchases);    
    VendingMachine::Flavours favouriteFlavour = static_cast<VendingMachine::Flavours>(A6::mprng(0, 3));
    
    printer.print(Printer::Student, studentId, (char)Start, favouriteFlavour, numberOfSodasToPurchase);

    VendingMachine* machine = nameServer.getMachine(studentId);
    printer.print(Printer::Student, studentId, (char)Selected, machine->getId());

    WATCard::FWATCard fwatcard;
    fwatcard.cancel();
    

    WATCard* watcard;
    unsigned int numberOfSodasPurchased = 0;
    while (numberOfSodasPurchased < numberOfSodasToPurchase) {

        bool hasLostCard = false;
        while (true) {
            if (fwatcard.cancelled() || hasLostCard == true) {
                fwatcard.cancel();
                fwatcard = cardOffice.create(studentId, 5);
            }

            try {
                if (hasLostCard == false) {
                    yield(A6::mprng(1,10));
                } else {
                    hasLostCard = false;
                }

                watcard = fwatcard();
                break;
            } catch (WATCardOffice::Lost& lost) {
                hasLostCard = true;
                printer.print(Printer::Student, studentId, (char)Lost);
            }
        }

        try {
            machine->buy(favouriteFlavour, *watcard);
            numberOfSodasPurchased++;
            printer.print(Printer::Student, studentId, (char)Bought, watcard->getBalance());
        } catch (VendingMachine::Funds& funds) {
            fwatcard = cardOffice.transfer(studentId, 5 + machine->cost(), watcard);
        } catch (VendingMachine::Stock& stock) {
            machine = nameServer.getMachine(studentId);
            printer.print(Printer::Student, studentId, (char)Selected, machine->getId());
        } 
    }

    if (watcard != NULL) {
        delete watcard;
    }
    printer.print(Printer::Student, studentId, (char)Finish);
}
