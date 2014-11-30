#ifndef _A6_BANK_H__
#define _A6_BANK_H__

#include <vector>

_Monitor Bank {
    const unsigned int numberOfStudents;
    std::vector<unsigned int> bankAccounts;
    std::vector<uCondition> notEnoughMoneyConditions;
    public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif