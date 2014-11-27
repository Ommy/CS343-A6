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

Student::~Student() {
    delete fwatcard();
}

void Student::main() {
    unsigned int numberOfSodasToPurchase = A6::mprng(1, maxPurchases);
    VendingMachine::Flavours favouriteFlavour = static_cast<VendingMachine::Flavours>(A6::mprng(0, 3));
    VendingMachine* machine = nameServer.getMachine(studentId);
    
    fwatcard = cardOffice.create(studentId, 5);

    printer.print(Printer::Student, studentId, (char)Start, favouriteFlavour, numberOfSodasToPurchase);

    unsigned int numberOfSodasPurchased = 0;
    while (numberOfSodasPurchased < numberOfSodasToPurchase) {
        try {
            yield(A6::mprng(1,10));
            machine->buy(favouriteFlavour, *fwatcard());
            numberOfSodasPurchased++;
            printer.print(Printer::Student, studentId, (char)Bought, (*fwatcard()).getBalance());
        } catch (VendingMachine::Funds& funds) {
            fwatcard = cardOffice.transfer(studentId, 5 + machine->cost(), fwatcard());
        } catch (VendingMachine::Stock& stock) {
            machine = nameServer.getMachine(studentId);
            printer.print(Printer::Student, studentId, (char)Selected, machine->getId());
        } catch (WATCardOffice::Lost& lost) {
            fwatcard.cancel();
            fwatcard = cardOffice.create(studentId, 5);
            printer.print(Printer::Student, studentId, (char)Lost);
        }
    }

    printer.print(Printer::Student, studentId, (char)Finish);
}