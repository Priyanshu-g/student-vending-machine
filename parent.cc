#include "printer.h"
#include <bits/charconv.h>
#include <uPRNG.h>

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ):prt(prt), bnk(bank), students(numStudents), delay(parentalDelay){}

void Parent::main(){
    prt.print(Printer::Parent, 'S');
    unsigned int gifts = 0;
    for(;;){
        // first select random student
        unsigned int selected_student = prng( students ) // aasume 0 indexes

        int added = 0;
        // get 10?
        if (prng( 20 ) == 0){
            added = 10;
        } else {
            // get 1-3
            added = prng( 3 ) + 1
        }

        // yield
        yield( delay );

        prt.print(Printer::Parent, (added == 10 ? 'E' : 'D'), selected_student, added); // if fixed 10 then E, else D since random amount
        // must print here after yield

        // send to bank
        bnk.deposit( id, added );

        gifts += added;

        // check for dtor
        _Accept( ~Parent ){
            prt.print(Printer::Parent, 'F', gifts);
            break;
        } _Else {}
    }
}