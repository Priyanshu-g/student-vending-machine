#ifndef TRUCK_H
#define TRUCK_H

#include "printer.h"
#include "bottlingplant.h"

_Task Truck {
	void main();

	Printer prt;
	NameServer ns;
	BottlingPlant plant;

	unsigned int nvm;
	unsigned int mspf;

	VendingMachine** vms;

	unsigned int bottles = 0;

	unsigned int currentVM = 0;

	unsigned int cargo[BottlingPlant::NUM_OF_FLAVOURS] = {};

	void load_cargo();

  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif