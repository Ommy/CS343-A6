#include "a6bank.h"

Bank::Bank( unsigned int numberOfStudents ) : numberOfStudents( numberOfStudents ), 
                                              bankAccounts( numberOfStudents, 0 ), 
                                              notEnoughMoneyConditions( numberOfStudents ) {
}

Bank::~Bank() {
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    bankAccounts[id] += amount;
    while ( !notEnoughMoneyConditions[id].empty() ) {
        notEnoughMoneyConditions[id].signal();
    }
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while ( bankAccounts[id] < amount ) {
        notEnoughMoneyConditions[id].wait();
    }
    bankAccounts[id] -= amount;
}
