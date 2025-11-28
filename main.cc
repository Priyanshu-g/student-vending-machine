#include <iostream>
#include <uPRNG.h>

#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "groupoff.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"

int main ( int argc, char * argv[] ) {

    const char * in = "soda.config";
    int64_t seed = 0;
    int64_t processors = 1;
    
	struct cmd_error {};

	try {
		switch ( argc ) {
			case 4:
				if ( strcmp( argv[3], "d" ) != 0 ) {
                    processors = convert( argv[3] );
					if ( processors <= 0 ) { throw cmd_error(); }
				}
			// FALL THROUGH
			case 3:
				if ( strcmp( argv[2], "d" ) != 0 ) {
					seed = convert( argv[2] );
					if ( seed <= 0 ) { throw cmd_error(); }
                    set_seed( seed );
				}
			// FALL THROUGH
			case 2:
				if ( strcmp( argv[1], "d" ) != 0 ) {
                    in = argv[1];
				}
			// FALL THROUGH
			case 1:
				break;
			// FALL THROUGH
		} // switch
	} catch ( ... ) {
		cout << "Usage: " << argv[0] << " [ config-file | ’d’ [ seed (> 0) | ’d’ [ processors (> 0) | ’d’ ] ] ]";
		exit( EXIT_FAILURE );
	} // try

    ConfigParms config {};
    processConfigFile(in, config);

    uProcessor p[processors - 1]; // number of kernel threads
    VendingMachine * vendingMachines[config.numVendingMachines];
    Student * students[config.numStudents];
    {
        Printer prt ( config.numStudents, config.numVendingMachines, config.numCouriers );
        Bank bank ( config.numStudents );
        WATCardOffice cardOffice ( prt, bank, config.numCouriers );
        Groupoff groupoff ( prt, config.numStudents, config.sodaCost, config.groupoffDelay );
        NameServer nameServer ( prt, config.numVendingMachines, config.numStudents );
        for ( unsigned int vm = 0; vm < config.numVendingMachines; vm++ ) {
            vendingMachines[vm] = new VendingMachine( prt, nameServer, vm, config.sodaCost );
        }
        BottlingPlant * bottlingPlant  = new BottlingPlant( prt, nameServer, config.numVendingMachines, config.maxShippedPerFlavour, config.maxStockPerFlavour, config.timeBetweenShipments );
        

        {
            Parent * parent = new Parent( prt, bank, config.numStudents, config.parentalDelay );
        
            for ( unsigned int student = 0; student < config.numStudents; student++ ) {
                students[student] = new Student( prt, nameServer, cardOffice, groupoff, student, config.maxPurchases );
            }

            for ( unsigned int student = 0; student < config.numStudents; student++ ) { delete students[student]; }
            delete parent;
        }
        delete bottlingPlant; // Per the suggestion of the assignment description
        for ( unsigned int vm = 0; vm < config.numVendingMachines; vm++ ) { delete vendingMachines[vm]; }
    }
}

// TODO: There is a weird issue where parent and students all finish but bottling plant fails to fail.