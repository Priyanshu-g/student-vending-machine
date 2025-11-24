#include "watcardoffice.h"

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) 
    : prt(prt), bank(bank) {
        for ( ; numCouriers > 0; --numCouriers) {
            couriers.emplace_back( Courier() );
        }
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Job * newJob = new Job( Job::Args( Job::JobKind::CREATE, sid, amount, nullptr ) );
    jobs.emplace_back( newJob );
    return newJob->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    Job * newJob = new Job( Job::Args( Job::JobKind::TRANSFER, sid, amount, card ) );
    jobs.emplace_back( newJob );
    return newJob->result;
}

Job * WATCardOffice::requestWork( ) { // Barging is irrelevant because it doesn't matter which courier takes a task
    // if jobs are empty, block but clear mutex
    if ( jobs.empty() ) { courierBench.wait(); }
    if ( !jobs.empty() ) { courierBench.signal(); }
    return jobs.pop_front();
}

void WATCardOffice::Courier::main( ) {

}

void WATCardOffice::main( ) {
    for ( ;; ) {
        _Accept( requestWork ) {

        } or _Accept ( create ) {

        } or _Accept ( transfer ) {

        }
    }
}