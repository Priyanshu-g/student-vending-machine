#include "groupoff.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
    : prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
    for ( unsigned int sid = 0; sid < numStudents; sid++ ) { 
        giftCards.emplace_back( WATCard::FWATCard() ); 
        listPermutation.emplace_back( sid );
    }
    for ( unsigned int sid = 0; sid < numStudents; sid++ ) { // Shuffle permutation
        unsigned int temp = listPermutation.at( sid );
        unsigned int newIndex = prng( numStudents );
        listPermutation.at( sid ) = listPermutation.at( newIndex );
        listPermutation.at( newIndex ) = temp; 
    }
}

WATCard::FWATCard Groupoff::giftCard( unsigned int id ) {
    return giftCards.at( id );
}

void Groupoff::main( ) {
    prt.print( Printer::Groupoff, 'S' );

    for ( ; targetGiftCard < numStudents; ) { // Wait until all future gift cards requested
        _Accept ( giftCard ) {
            targetGiftCard++;
        }
    }

    for ( ; targetGiftCard > 0; ) {
        yield( groupoffDelay );

        WATCard * newCard = new WATCard();
        newCard->deposit( sodaCost );
        giftCards.at( listPermutation.at( targetGiftCard - 1 ) ).delivery( newCard );
        prt.print( Printer::Groupoff, 'D', listPermutation.at( targetGiftCard - 1 ), sodaCost );
        --targetGiftCard;

        _Accept ( ~Groupoff ) {
            break;
        } _Else { }
    }
    prt.print( Printer::Groupoff, 'F' );

}