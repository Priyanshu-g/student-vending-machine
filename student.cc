#include "student.h"
#include "bottlingplant.h"
#include "vendingmachine.h"
#include <uPRNG.h>

#include <iostream>

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases ) : prt(prt), nameServer(nameServer), cardOffice(cardOffice) {
    this->id = id; // Set ID here because we skipped groupoff in the init list
    this->maxPurchases = maxPurchases; // Ditto
    numPurchases = prng( 1, maxPurchases ); // Set to random between [1, maxPurchases]
    favouriteFlavour = ( BottlingPlant::Flavours ) prng( BottlingPlant::NUM_OF_FLAVOURS ); // Set to random between [0, 3]
    prt.print( Printer::Student, id, 'S', (unsigned int) favouriteFlavour, numPurchases );
    watCard = cardOffice.create( id, 5 );
    giftCard = groupoff.giftCard( id );
}

// ISSUES:
    // - Student blocks permanently after one full round robin of vms
    // - Second student is permanently blocked on creating the watCard -> Create is never entered, request is never made
    // - Student also blocks if card is lost, Seems create doesn't work - Courier is looping back around

void Student::main( ) {
    
    vm = nameServer.getMachine( id );
    prt.print( Printer::Student, id, 'V', vm->getId() );

    bool delay = true;

    for ( ;; ) {
        if ( numDrank >= numPurchases ) { prt.print( Printer::Student, id, 'F', numDrank, numFree ); break; }

        if ( delay ) { yield( prng( 1, 10 ) ); }
        // if ( delay ) { yield( 1000000 ); }
        else { delay = true; }
        // Attempt to purchase soda, looking for Funds, Stock, or Free
            try {
                #ifndef CLUMSYCOURIER
                    _Select ( giftCard ) { // Prioritize using giftCard.
                        WATCard *gc = giftCard();
                        vm->buy( favouriteFlavour, *(giftCard()) );
                        prt.print( Printer::Student, id, 'G', (unsigned int) favouriteFlavour, giftCard()->getBalance() );
                        delete gc;
                        giftCard.reset(); // Always reset giftCard once used
                    } 
                #endif
                #ifndef TESTGCONLY
                    #if !defined(CLUMSYCOURIER) && !defined(TESTGCONLY)
                        or
                    #endif
                    _Select ( watCard ) {
                            vm->buy( favouriteFlavour, *(watCard()) );
                            prt.print( Printer::Student, id, 'B', (unsigned int) favouriteFlavour, watCard()->getBalance() );
                    }
                #endif
                numDrank++;
            } _Catch ( VendingMachine::Funds & ) {
                watCard = cardOffice.transfer( id, vm->cost() + 5, watCard() );
            } _Catch ( VendingMachine::Stock & ) {
                vm = nameServer.getMachine( id );
                prt.print( Printer::Student, id, 'V', vm->getId() );
            } _Catch ( VendingMachine::Free & ) {
                prt.print( Printer::Student, id, 'A', (unsigned int) favouriteFlavour );
                numFree++;
                if ( prng( 2 ) == 0 ) { prt.print( Printer::Student, id, 'X' ); yield( 4 ); } // Watch ad, 50% chance
                delay = false; // Attempt another purchase WITHOUT YIELDING
            } _Catch ( WATCardOffice::Lost & ) {
                prt.print( Printer::Student, id, 'L' );
                watCard = cardOffice.create( id, 5 );
                delay = false; // Attempt another purchase WITHOUT YIELDING
            } // try
        
    }

    try{
        delete watCard();
    } _Catch ( WATCardOffice::Lost & ) {
        prt.print( Printer::Student, id, 'L' );
    }
}