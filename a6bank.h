#ifndef _A6_BANK_H__
#define _A6_BANK_H__

_Monitor Bank {
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
  private:
    unsigned int numberOfStudents;
    unsigned int *bankAccount;
    uCondition * accountCheck;
};

#endif