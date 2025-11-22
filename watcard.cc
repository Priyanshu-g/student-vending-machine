#include "watcard.h"

_Exception InsufficentFuncds{};

WATCard::WATCard(){}

void WATCard::deposit( unsigned int amount ){
    balance += amount;
}
void WATCard::withdraw( unsigned int amount ){

    // SHOULD NEVER HAPPEN, PURELY FOR DEBUGGING TO CATCH OVERFLOW
    if (balance < amount){ 
        _Throw InsufficentFuncds();
    }

    balance -= amount;
}
unsigned int WATCard::getBalance(){
    return balance;
}