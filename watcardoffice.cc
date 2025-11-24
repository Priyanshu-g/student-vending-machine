#include "watcardoffice.h"

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) 
    : prt(prt), bank(bank) {
        for ( int id = 0 ; id < numCouriers; id++ ) {
            couriers.emplace_back( Courier( id, bank ) );
        }
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    jobToPass = new Job( Job::Args( Job::JobKind::CREATE, sid, amount, new WATCard() ) );
    return jobToPass->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    jobToPass = new Job( Job::Args( Job::JobKind::TRANSFER, sid, amount, card ) );
    return jobToPass->result;
}

Job * WATCardOffice::requestWork( ) { // Barging is irrelevant because it doesn't matter which courier takes a task
    courierBench.wait();
    if ( !jobs.empty() ) { courierBench.signal(); }
    return jobToPass;
}

WATCardOffice::Courier::Courier( unsigned int id, Bank & bank ) : id(id), bank(bank) { }

void WATCardOffice::Courier::main( ) {
    for ( ;; ) {
        Job * currentJob = WATCardOffice::requestWork();
        bank.withdraw( currentJob->args.sid, currentJob->args.amount );
        currentJob->args.card.deposit( currentJob->args.amount );
        if ( prng( 6 ) == 0 ) { // 1/6 chance to lose WATCard
            delete currentJob->args.card;
            currentJob->result.delivery( new WATCardOffice::Lost );
        } else {
            currentJob->result.delivery( currentJob->args.card );
        }
    }
}

void WATCardOffice::main( ) {
    prt.print( Printer::WATCardOffice, 'S' );

    for ( ;; ) {
        _When ( !jobs.empty ) _Accept( requestWork ) {
            jobToPass = jobs.pop_front();
            prt.print( Printer::WATCardOffice, 'W' ); // NOTE: This is not technically when the call completes
        } _Accept ( create ) {
            jobs.emplace_back( jobToPass );
            prt.print( Printer::WATCardOffice, 'C', jobToPass->args.sid, jobToPass->args.amount );
        } _Accept ( transfer ) {
            jobs.emplace_back( jobToPass );
            prt.print( Printer::WATCardOffice, 'T', jobToPass->args.sid, jobToPass->args.amount );
        } _Accept ( ~WATCardOffice ) {
            prt.print( Printer::WATCardOffice, 'F' );
            break;
        }
        courierBench.signalBlock();
    }
}