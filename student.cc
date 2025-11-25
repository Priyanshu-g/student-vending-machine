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
        if ( delay ) { delay = true; yield( prng( 1, 10 ) ); }
        // Attempt to purchase soda, looking for Funds, Stock, or Free
        _Enable {
            try {
                _Select ( giftCard ) { // Prioritize using giftCard.
                    vm->buy( favouriteFlavour, *(giftCard()) );
                    if ( !failed ) {
                        failed = false;
                        prt.print( Printer::Student, id, 'G', (unsigned int) favouriteFlavour, giftCard()->getBalance() );
                        giftCard.reset(); // Always reset giftCard once used
                    }
                } or _Select ( watCard ) {
                    vm->buy( favouriteFlavour, *(watCard()) );
                    if ( !failed ) {
                        failed = false;
                        prt.print( Printer::Student, id, 'B', (unsigned int) favouriteFlavour, watCard()->getBalance() );
                    }
                }
                numDrank++;
            } _CatchResume ( VendingMachine::Funds & ) {
                watCard = cardOffice.transfer( id, vm->cost() + 5, watCard() );
                failed = true;
            } _CatchResume ( VendingMachine::Stock & ) {
                vm = nameServer.getMachine( id );
                prt.print( Printer::Student, id, 'V', vm->getId() );
                failed = true;
            } _CatchResume ( VendingMachine::Free & ) {
                prt.print( Printer::Student, id, 'A', (unsigned int) favouriteFlavour );
                numFree++;
                if ( prng( 2 ) == 0 ) { prt.print( Printer::Student, id, 'X' ); yield( 4 ); } // Watch ad, 50% chance
                delay = false; // Attempt another purchase WITHOUT YIELDING
                failed = true;
            } _Catch ( WATCardOffice::Lost & ) {
                prt.print( Printer::Student, id, 'L' );
                watCard = cardOffice.create( id, 5 );
                delay = false; // Attempt another purchase WITHOUT YIELDING
                failed = true;
            } // try
        }
    }
    // exit(0);
}