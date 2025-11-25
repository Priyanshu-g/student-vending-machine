#include "nameserver.h"
#include "vendingmachine.h"

void NameServer::main(){
    prt.print(Printer::NameServer, 'S');

    // wait till all Virtual Machines regiserted
    for(int needed = nvm; needed > 0; needed -= 1) {
        _Accept (VMregister) {}
    }

    // once all registered, we can turn into a monitor, our thread is not needed
}
NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):prt(prt),nvm(numVendingMachines), ns(numStudents){
    vmAssignment = new unsigned int[ns]();
    unsigned int vmit = 0; // assign VM 0 first
    for( unsigned int i = 0; i < ns; ++i ){ // for each student
        vmAssignment[i] = vmit; // assign
        vmit = (vmit + 1) % nvm; // then iterate over all VMs
    }
    vms = new VendingMachine*[nvm]();
}

void NameServer::VMregister( VendingMachine * vendingmachine ){
    unsigned int vmId = vendingmachine->getId();

    vms[vmId] = vendingmachine; // array will use the id location of the vending machine
    
    prt.print(Printer::NameServer, 'R', vmId); // registered VM
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    // remember, id is the student id

    // get ptr to the vending machine current assigned to the student
    VendingMachine* assigned_VM = vms[vmAssignment[id]];

    prt.print(Printer::NameServer, 'N', id, vmAssignment[id]); // give student id thier vm

    // move up assignment
    vmAssignment[id] = (vmAssignment[id] + 1) % nvm; // in round robin fashion

    // return assigned VM
    return assigned_VM;
}


VendingMachine ** NameServer::getMachineList() {
    return vms;
}

NameServer::~NameServer(){
    prt.print(Printer::NameServer, 'F'); // print finished when destoryed (not a thread anymore!)
}