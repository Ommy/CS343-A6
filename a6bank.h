#ifndef _A6_BANK_H__
#define _A6_BANK_H__

#include <vector>

_Monitor Bank {
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
  private:
    unsigned int numberOfStudents;
    std::vector<unsigned int> bankAccount;
    std::vector<uCondition> accountCheck;
};

#endif