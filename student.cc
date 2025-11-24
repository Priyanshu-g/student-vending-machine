#include "student.h"
#include "bottlingplant.h"
#include "vendingmachine.h"
#include <uPRNG.h>

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases ) : prt(prt), nameServer(nameServer), cardOffice(cardOffice) {
                this->id = id; // Set ID here because we skipped groupoff in the init list
                numPurchases = prng( 1, maxPurchases ); // Set to random between [1, maxPurchases]
                favouriteFlavour = ( BottlingPlant::Flavours ) prng( BottlingPlant::NUM_OF_FLAVOURS ); // Set to random between [0, 3]
                watCard = cardOffice.create( id, 5 );
                giftCard = groupoff.giftCard( id );
             }

void Student::main( ) {
    prt.print( Printer::Student, id, 'S', (unsigned int) favouriteFlavour, numPurchases );
    
    prt.print( Printer::Student, id, 'V', vm->getId() );
    vm = nameServer.getMachine( id );

    bool delay = true;

    for ( ;; ) {
        if ( delay ) { delay = true; yield( prng( 1, 10 ) ); }
        // Attempt to purchase soda, looking for Funds, Stock, or Free
        _Enable {
            try {
                _Select ( giftCard ) { // Prioritize using giftCard.
                    vm->buy( favouriteFlavour, giftCard );
                    prt.print( Printer::Student, id, 'G', favouriteFlavour, giftCard()->getBalance() );
                    giftCard.reset(); // Always reset giftCard once used
                } or _Select ( watCard ) {
                    vm->buy( favouriteFlavour, watCard );
                    prt.print( Printer::Student, id, 'B', favouriteFlavour, watCard()->getBalance() );
                }
            } _Catch ( VendingMachine::Funds & ) {
                // PENDING PIAZZA ANSWER -> should giftCard reset if funds is thrown by a call using it?
                cardOffice.transfer( id, vm->cost() + 5, watCard() );
            } _Catch ( WATCardOffice::Lost & ) {
                cardOffice.create( id, 5 );
                delay = false; // Attempt another purchase WITHOUT YIELDING
            } _Catch ( VendingMachine::Stock & ) {
                vm = nameServer.getMachine( id );
            } _Catch ( VendingMachine::Free & ) {
                if ( prng( 2 ) == 0 ) { yield( 4 ); } // Watch ad, 50% chance
                delay = false; // Attempt another purchase WITHOUT YIELDING
            } // try
        }
        
        
    }
}