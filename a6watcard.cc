#include "a6watcard.h"

WATCard::WATCard() {
    watcardBalance = 0;
}

void WATCard::deposit( unsigned int amount ) {
    watcardBalance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
    // Since the machine will be withdrawing money, it can first
    // call getBalance() to determine if card has sufficient funds
    // so we don't have to ever go negative
    watcardBalance -= amount;
}

unsigned int WATCard::getBalance() {
    return watcardBalance;
}