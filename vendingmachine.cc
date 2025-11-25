#include "vendingmachine.h"
#include "uPRNG.h"
#include <unistd.h>

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost )
    : prt(prt), ns(nameServer), id(id), sc(sodaCost) {
    ns.VMregister( this );
}

#include <iostream>
void VendingMachine::main(){
    prt.print(Printer::Vending, id, 'S', sc);
    for(;;){
        try{
            // either someone buys, or we get restock attempt
            _Accept (buy){
            } or _Accept (inventory) {
                // if restock attempt, we must block (preventing any buying) until restocked is triggered
                _Accept (restocked){}
            } or _Accept (~VendingMachine) {
                prt.print(Printer::Vending, id, 'F');
                break;
            }
        } catch ( uMutexFailure::RendezvousFailure& ){}
    }
}

void VendingMachine::buy( BottlingPlant::Flavours flavour, WATCard & card ){
    if(card.getBalance() < sc){
        _Throw Funds(); // not enough to buy
    }

    if(stock[flavour] == 0){
        _Throw Stock();
    }

    // remove the drink since we must provide at this point
    stock[flavour] -= 1;

    // check if free
    if (prng( 5 ) == 0){
        prt.print(Printer::Vending, id, 'A');
        _Throw Free();
    }

    // only print soda bought if not free (following example in assignment doc)
    prt.print(Printer::Vending, id, 'B', flavour, stock[flavour]);

    card.withdraw(sc); // debit the card if not free    
}

unsigned int * VendingMachine::inventory() {
    prt.print(Printer::Vending, id, 'r');
    // just return my stock, they can edit it, I will block (in main) until restocked fires
    return stock;
}

void VendingMachine::restocked(){
    prt.print(Printer::Vending, id, 'R');
    // does nothing, just unblocks main()
}

_Nomutex unsigned int VendingMachine::cost() const{
    return sc;
}
_Nomutex unsigned int VendingMachine::getId() const{
    return id;
}