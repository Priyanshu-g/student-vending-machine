#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H

#include "printer.h"
#include "bank.h"

#include <deque>

_Task WATCardOffice {
	Printer & prt;
	Bank & bank;
	uCondition courierBench;

	struct Job {							// marshalled arguments and return future
		enum JobKind { CREATE, TRANSFER };
		struct Args {
			const Job::JobKind kind;
			const unsigned int sid, amount;
			WATCard * card;
		};
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
	};
	_Task Courier { 						// communicates with bank (YOU DEFINE "Courier")
		unsigned int id;
		Bank & bank;
		void main();
	  public:
		Courier( unsigned int id, Bank & bank );
	};					

	std::deque<Courier> couriers {};
	std::deque<Job *> jobs {};
	Job::Args argsToPass;
	Job * jobToPass;

	void main();
  public:
	_Exception Lost {};						// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
	Job * requestWork() __attribute__(( warn_unused_result ));
};

#endif