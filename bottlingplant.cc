#include "bottlingplant.h"
#include "uPRNG.h"
#include "truck.h"


BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments ):prt(prt), ns(nameServer), nvm(numVendingMachines), mshippf(maxShippedPerFlavour), mstockpf(maxStockPerFlavour), time(timeBetweenShipments){
        }

void BottlingPlant::main(){
    prt.print(Printer::BottlingPlant, 'S');


    // TODO: check if stack works
    Truck( prt, ns, *this, nvm, mstockpf); // First create a truck

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

            prt.print(Printer::BottlingPlant, 'F'); // all done!

            terminated = true; // track finished to kill truck later too

            break; 

        }
    }
}

void BottlingPlant::getShipment( unsigned int cargo[] ){ // refernce to the trucks cargo
    if (terminated) _Throw Shutdown();
    
    for (int i = 0; i < NUM_OF_FLAVOURS; ++i) cargo[i] = prod_run[i]; // overwrite since old drinks are expired
}