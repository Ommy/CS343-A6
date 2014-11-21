#include "a6bank.h"

Bank::Bank( unsigned int numStudents ) {
    numberOfStudents = numStudents;
    accountCheck = new uCondition[numberOfStudents];
    bankAccount = new unsigned int [numberOfStudents];
    for (unsigned int i = 0; i < numberOfStudents; i++) {
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