#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H

#include "printer.h"
#include "nameserver.h"
#include "truck.h"

_Task BottlingPlant {
	public:
		enum Flavours { Blues, BlackCherry, CreamSoda, RootBeer, Lime, NUM_OF_FLAVOURS };	// flavours of soda (YOU DEFINE)
	private:
		void main();
		Printer prt;
		NameServer ns;
		unsigned int nvm;
		unsigned int mshippf;
		unsigned int mstockpf;
		unsigned int time;

		bool terminated = false;

		unsigned int prod_run[BottlingPlant::NUM_OF_FLAVOURS] = {};

  public:
	
	_Exception Shutdown {};					// shutdown plant
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );
};

#endif