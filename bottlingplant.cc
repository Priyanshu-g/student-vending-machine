#include "bottlingplant.h"
#include <uPRNG.h>
#include "truck.h"

#include <iostream>

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments ):prt(prt), ns(nameServer), nvm(numVendingMachines), mshippf(maxShippedPerFlavour), mstockpf(maxStockPerFlavour), time(timeBetweenShipments){
        }

void BottlingPlant::main(){
    prt.print(Printer::BottlingPlant, 'S');

    Truck t ( prt, ns, *this, nvm, mstockpf );

    for(;;){
        yield( time );

        // track total bottles
        unsigned int total = 0;

        for (int i = 0; i < NUM_OF_FLAVOURS; ++i) {

            prod_run[i] = prng( mshippf + 1 ); // 0 to maxShipPerFlavour inclusive 

            total += prod_run[i];

        }

        prt.print(Printer::BottlingPlant, 'G', total); // print production done with number of bottles made 
        
        _Accept( getShipment ){

            prt.print(Printer::BottlingPlant, 'P'); // picked up by truck

        } or _Accept( ~BottlingPlant ){ 
            std::cout << "FINISH BP" << std::endl;

            prt.print(Printer::BottlingPlant, 'F'); // all done!

            _Resume Shutdown() _At t;

            break; 

        }
    }
    std::cout << "TERMINATE BP" << std::endl;

}

void BottlingPlant::getShipment( unsigned int cargo[] ){ // refernce to the trucks cargo
    
    for (int i = 0; i < NUM_OF_FLAVOURS; ++i) cargo[i] = prod_run[i]; // overwrite since old drinks are expired
}