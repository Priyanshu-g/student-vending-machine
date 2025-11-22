#include "truck.h"
#include "uPRNG.h"

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour ):prt(prt), ns(nameServer), plant(plant), nvm(nvm), mspf(maxStockPerFlavour){}


void Truck::load_cargo(){
    prt.print(Printer::Truck, 'd', currentVM, bottles); // begin delivery to currentVM, bottles left

    // inventory array to fill
    unsigned int * inven = vms[currentVM]->inventory();

    unsigned int missed = 0;
    for(int i = 0; i < BottlingPlant::NUM_OF_FLAVOURS; ++i){
        // add up till mspf
        unsigned int to_be_added = min(inven[i] + cargo[i], mspf) - inven[i];

        // remove from cargo and total
        cargo[i] -= to_be_added;
        bottles -= to_be_added;

        // add to vending machine
        inven[i] += to_be_added;
        missed += mspf - inven[i]; // we missed whatever more we could have added
    }

    if (missed != 0) {
        // if failed to fill any flavour
        prt.print(Printer::Truck, 'U', currentVM, missed);
    }

    prt.print(Printer::Truck, 'D', currentVM, bottles); // end delivery, this is bottles remaining

    vms[currentVM]->restocked();

    currentVM = (currentVM + 1) % nvm;
}

        
void Truck::main(){
    prt.print(Printer::Truck, 'S');
    // get vending machines
    vms = ns.getMachineList();

    for(;;){
        // yield between 1 and 10
        yield( prng(10) + 1);
        
        // load in cargo
        try{
            plant.getShipment(cargo);
        } catch (BottlingPlant::Shutdown& e){
            break;
        }

        bottles = 0; // reset since new batch
        for(int i = 0; i < BottlingPlant::NUM_OF_FLAVOURS; ++i){
            bottles += cargo[i];
        }

        prt.print(Printer::Truck, 'P', bottles);

        // now serve the machine AFTER iterator, until we run out, OR iterator loops fully
        for(int i = 0; i < nvm; ++i){
            // loop vending machine times, but will use iterator (managed by load_cargo)
            if (bottles == 0) {
                // we just loaded that drained us (now empty)
                break;
            }

            load_cargo();

            // successful delivery, still some stuff left
            // check for robbery
            if (prng( 10 ) == 0) {
                prt.print(Printer::Truck, 'R', bottles);
                break; // robbed, cargo dead
            }
        }
    }

    prt.print(Printer::Truck, 'F'); // finished when break out and end
}

