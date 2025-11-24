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

Student::~Student( ) { prt.print( Printer::Student, id, 'F', numDrank, numFree ); }

void Student::main( ) {
    prt.print( Printer::Student, id, 'S', (unsigned int) favouriteFlavour, numPurchases );
    
    vm = nameServer.getMachine( id );
    prt.print( Printer::Student, id, 'V', vm->getId() );

    bool delay = true;

    for ( ;; ) {
        if ( delay ) { delay = true; yield( prng( 1, 10 ) ); }
        // Attempt to purchase soda, looking for Funds, Stock, or Free
        _Enable {
            try {
                _Select ( giftCard ) { // Prioritize using giftCard.
                    vm->buy( favouriteFlavour, giftCard );
                    prt.print( Printer::Student, id, 'G', (unsigned int) favouriteFlavour, giftCard()->getBalance() );
                    giftCard.reset(); // Always reset giftCard once used
                } or _Select ( watCard ) {
                    vm->buy( favouriteFlavour, watCard );
                    prt.print( Printer::Student, id, 'B', (unsigned int) favouriteFlavour, watCard()->getBalance() );
                }
                numDrank++;
            } _Catch ( VendingMachine::Funds & ) {
                cardOffice.transfer( id, vm->cost() + 5, watCard() );
            } _Catch ( WATCardOffice::Lost & ) {
                prt.print( Printer::Student, id, 'L' );
                cardOffice.create( id, 5 );
                delay = false; // Attempt another purchase WITHOUT YIELDING
            } _Catch ( VendingMachine::Stock & ) {
                vm = nameServer.getMachine( id );
                prt.print( Printer::Student, id, 'V', vm->getId() );
            } _Catch ( VendingMachine::Free & ) {
                prt.print( Printer::Student, id, 'A', (unsigned int) favouriteFlavour );
                numDrank++; numFree++;
                if ( prng( 2 ) == 0 ) { prt.printer( Printer::Student, id, 'X' ); yield( 4 ); } // Watch ad, 50% chance
                delay = false; // Attempt another purchase WITHOUT YIELDING
            } // try
        }
    }
}