#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H

#include "printer.h"
#include "bank.h"
#include "watcard.h"

#include <deque>

_Task WATCardOffice {
	Printer & prt;
	Bank & bank;
	uCondition courierBench;
	
	_Exception End{};

	struct Job {							// marshalled arguments and return future
		struct Args {
			unsigned int sid, amount;
			WATCard * card;
			Args( unsigned int sid, unsigned int amount, WATCard * card );
		};
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
	};
	_Task Courier { 						// communicates with bank (YOU DEFINE "Courier")
		Printer & prt;
		WATCardOffice & cardOffice;
		unsigned int id;
		Bank & bank;
		void main();
	  public:
		Courier( Printer & prt, WATCardOffice & cardOffice, unsigned int id, Bank & bank );
		~Courier();
	};		

	std::deque<WATCardOffice::Courier *> couriers {};
	std::deque<WATCardOffice::Job *> jobs {};
	WATCardOffice::Job * jobToPass;

	void main();
  public:
	_Exception Lost {};						// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	~WATCardOffice();
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
	Job * requestWork() __attribute__(( warn_unused_result ));
};

#endif