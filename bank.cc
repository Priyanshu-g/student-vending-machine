#include "bank.h"

Bank::Bank( unsigned int numStudents ){
    accounts = new unsigned int[numStudents]();
    accountWithdraws = new uCondition[numStudents]();
}
// id is 0 indexed!
void Bank::deposit( unsigned int id, unsigned int amount ){
    accounts[id] += amount;

    // if there is a courier waiting and now the account has enough, wake them up
    if (!accountWithdraws[id].empty() && accounts[id] >= accountWithdraws[id].front()){
        accountWithdraws[id].signal();
    }
}
void Bank::withdraw( unsigned int id, unsigned int amount ){
    // if not enough
    if (accounts[id] < amount){
        // wait till my amount is ready
        accountWithdraws[id].wait( amount );
    }

    // assume my amount is present, debit the card
    accounts[id] -= amount;
}

Bank::~Bank(){
    delete accounts;
    delete accountWithdraws;
}