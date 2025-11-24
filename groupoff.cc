#include "groupoff.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
    : prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
    for ( int sid = 0; sid < numStudents; sid++ ) { 
        giftCards.emplace_back( WATCard::FWATCard() ); 
        listPermutation.emplace_back( sid );
    }
    for (int sid = 0; sid < numStudents; sid++ ) { // Shuffle permutation
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

    --targetGiftCard; // Re-adjust indicies for readability

    for ( ; targetGiftCard >= 0; ) {
        yield( groupoffDelay );

        giftCards.at( listPermutation.at( targetGiftCard ) ).delivery( (new WATCard())->deposit( sodaCost ) );
        --targetGiftCard;
        prt.print( Printer::Groupoff, 'D', listPermutation.at( targetGiftCard ), sodaCost );

        _Accept ( ~Groupoff ) {
            prt.print( Printer::Groupoff, 'F' );
            break;
        } _Else { }
    }
}