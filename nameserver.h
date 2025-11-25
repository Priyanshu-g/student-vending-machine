#ifndef NAMESERVER_H
#define NAMESERVER_H

#include "printer.h"
#include <vector>
_Task VendingMachine;

_Task NameServer {
	void main();

	Printer& prt;
	unsigned int nvm;
	unsigned int ns;

	VendingMachine ** vms;

	std::vector<unsigned int> vmAssignment;
  public:
	~NameServer();
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	void VMregister( VendingMachine * vendingmachine );
	// 0 indexed student id!
	VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
	VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};

#endif