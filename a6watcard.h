#ifndef _A6_WATCARD_H__
#define _A6_WATCARD_H__

#include <uFuture.h>

class WATCard {
    WATCard( const WATCard& );             // prevent copying
    WATCard& operator=( const WATCard& );

    unsigned int balance;

    public:
    WATCard();
    typedef Future_ISM<WATCard*> FWATCard;  // future watcard pointer
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

#endif