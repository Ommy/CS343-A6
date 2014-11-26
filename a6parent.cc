#include "a6parent.h"
#include "a6printer.h"
#include "a6main.h"
#include "a6bank.h"

Parent::Parent( Printer &prt, 
                Bank &bank, 
                unsigned int numberOfStudents, 
                unsigned int parentalDelay ) :  printer(prt), 
                                                bank(bank), 
                                                numberOfStudents(numberOfStudents), 
                                                parentalDelay(parentalDelay) {

}

void Parent::main() {
    while (true) {
        _Accept (~Parent)  {
            break;
        } _Else {
            unsigned int money = A6::mprng(1,3);
            unsigned int student = A6::mprng(numberOfStudents - 1);
            yield(parentalDelay);
            bank.deposit(student, money);
        }
    }
}