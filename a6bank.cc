#include "a6bank.h"

Bank::Bank( unsigned int numberOfStudents ) :   numberOfStudents(numberOfStudents), 
                                                bankAccount(numberOfStudents, 0), 
                                                accountCheck(numberOfStudents) {
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    bankAccount[id] += amount;
    accountCheck[id].signal();    
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while (bankAccount[id] < amount) {
        accountCheck[id].wait();
    }
    bankAccount[id] -= amount;
}

Bank::~Bank(){
}