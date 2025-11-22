#include "printer.h"
#include <bits/charconv.h>
#include <cinttypes>
#include <uPRNG.h>

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ):prt(prt), bnk(bank), students(numStudents), delay(parentalDelay){}

void Parent::main(){
    for(;;){
        // first select random student
        unsigned int selected_student = prng() % students // aasume 0 indexes

        int added = 0;
        // get 10?
        if (prng() % 20 == 1){
            added = 10;
        } else {
            // get 1-3
            added = (prng() % 3) + 1
        }

        // yield
        yield( delay );

        // send to bank
        bnk.deposit( id, added );

        // check for dtor
        _Accept( ~Parent ){
            break;
        } _Else {}
    }
}