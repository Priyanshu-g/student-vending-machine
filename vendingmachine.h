#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "watcard.h"

_Task VendingMachine {
	Printer& prt;
	NameServer& ns;
	unsigned int id;
	unsigned int sc;

	unsigned int stock[BottlingPlant::NUM_OF_FLAVOURS] = {};

	void main();
  public:
	_Exception Funds {};					// insufficient funds
	_Exception Stock {};					// flavour out of stock
	_Exception Free {};						// free, advertisement
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	void buy( BottlingPlant::Flavours flavour, WATCard & card );
	unsigned int * inventory() __attribute__(( warn_unused_result ));
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};

#endif