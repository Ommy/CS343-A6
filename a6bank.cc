#include "a6bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
    accountCheck = new uCondition[numStudents];
    bankAccount = new unsigned int [numStudents];
    for (unsigned int i = 0; i < numStudents; i++) {
        bankAccount[i] = 0;
    }
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
    delete[] bankAccount;
    delete[] accountCheck;
}