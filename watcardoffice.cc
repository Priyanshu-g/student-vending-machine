#include "watcardoffice.h"

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) 
    : prt(prt), bank(bank) {
        for ( unsigned int id = 0 ; id < numCouriers; id++ ) {
            couriers.emplace_back( new WATCardOffice::Courier( prt, *this, id, bank ) );
        }
}

WATCardOffice::~WATCardOffice( ) {
    for ( auto & it : couriers ) {
        _Resume End() _At *it;
    }

    while( !courierBench.empty() ) { 
        courierBench.signalBlock(); 
    }

    for ( auto & it : couriers ) {
        delete it;
    }

    for ( auto & it : jobs ) {
        delete it;
    }
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    jobToPass = new Job( Job::Args( sid, amount, new WATCard() ) );
    return jobToPass->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    jobToPass = new Job( Job::Args( sid, amount, card ) );
    return jobToPass->result;
}

WATCardOffice::Job * WATCardOffice::requestWork( ) { // Barging is irrelevant because it doesn't matter which courier takes a task
    if ( jobs.empty() ) { courierBench.wait(); }

    Job * jobToTake = jobs.front();
    jobs.pop_front();
    if ( !jobs.empty() ) { courierBench.signal(); } // daisy chain
    prt.print( Printer::WATCardOffice, 'W' );

    return jobToTake;
}

WATCardOffice::Job::Args::Args( unsigned int sid, unsigned int amount, WATCard * card )
    : sid(sid), amount(amount), card(card) {}

WATCardOffice::Courier::Courier( Printer & prt, WATCardOffice & cardOffice, unsigned int id, Bank & bank ) 
    : prt(prt), cardOffice(cardOffice), id(id), bank(bank) { }

WATCardOffice::Courier::~Courier( ) { prt.print( Printer::Courier, id, 'F' ); }

void WATCardOffice::Courier::main( ) {
    prt.print( Printer::Courier, id, 'S' );

    try{
        _Enable{
            for ( ;; ) {
                WATCardOffice::Job * currentJob = cardOffice.requestWork();

                prt.print( Printer::Courier, id, 't', currentJob->args.sid, currentJob->args.amount );
                bank.withdraw( currentJob->args.sid, currentJob->args.amount );
                currentJob->args.card->deposit( currentJob->args.amount );
                if ( 
                    #ifndef CLUMSYCOURIER
                        prng( 6 ) == 0 
                    #else
                        !(prng( 6 ) == 0)
                    #endif
                ) { // 1/6 chance to lose WATCard
                    delete currentJob->args.card;
                    currentJob->result.delivery( new WATCardOffice::Lost );
                    prt.print( Printer::Courier, id, 'L', currentJob->args.sid );
                } else {
                    currentJob->result.delivery( currentJob->args.card );
                    prt.print( Printer::Courier, id, 'T', currentJob->args.sid, currentJob->args.amount );
                }

                delete currentJob;
            }
        }
    } _Catch(WATCardOffice::End&){}
}

void WATCardOffice::main( ) {
    prt.print( Printer::WATCardOffice, 'S' );

    for ( ;; ) {
        // _When ( !jobs.empty() ) 
        _Accept( requestWork ) {
            // jobToPass = jobs.front();
            // jobs.pop_front();
            // courierBench.signalBlock();
        } or _Accept ( create ) {
            jobs.emplace_back( jobToPass );
            prt.print( Printer::WATCardOffice, 'C', jobToPass->args.sid, jobToPass->args.amount );
            if ( !courierBench.empty() ) { courierBench.signalBlock(); }
        } or _Accept ( transfer ) {
            jobs.emplace_back( jobToPass );
            prt.print( Printer::WATCardOffice, 'T', jobToPass->args.sid, jobToPass->args.amount );
            if ( !courierBench.empty() ) { courierBench.signalBlock(); }
        } or _Accept ( ~WATCardOffice ) {
            prt.print( Printer::WATCardOffice, 'F' );
            break;
        }
    }
}