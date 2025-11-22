#include "printer.h"

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ){

    cout << "Parent" << '\t';
    cout << "Gropoff" << '\t';
    cout << "WATOff" << '\t';
    cout << "Names" << '\t';
    cout << "Truck" << '\t';
    cout << "Plant" << '\t';
    Stud0 Stud1 Mach0 Mach1 Mach2 Cour0

    for (unsigned int i = 0; i < numStudents; ++i) {
        cout << "Stud" << i;
        if(i != numStudents - 1 || numVendingMachines != 0) cout << '\t';
        ss.push_back(Slot{})
    }

    for (unsigned int i = 0; i < numVendingMachines; ++i) {
        cout << "Mach" << i;
        if(i != numVendingMachines - 1 || numCouriers != 0) cout << '\t';
        vms.push_back(Slot{})
    }

    for (unsigned int i = 0; i < numCouriers; ++i) {
        cout << "Cour" << i;
        if(i != numCouriers - 1) cout << '\t';
        cs.push_back(Slot{})
    }
    
    cout << endl;

    // -1 due to print at the end without \t
    for (unsigned int i = 0; i < numStudents + numVendingMachines + numCouriers + 6 - 1; ++i) {
        cout << "*******" << '\t';
    }
    cout << "*******";
    cout << endl;
}

Printer::~Printer(){
    // release last items
    dump();
}

void Printer::dump() {
    
    unsigned int totalCols = 6 + ss.size() + vms.size() + cs.size();

    // map a linear index to the corresponding Slot
    auto getSlot = [this](unsigned int idx) -> Slot & {
        if (idx == 0) return p;              // Parent
        if (idx == 1) return go;             // Groupoff
        if (idx == 2) return wco;            // WATCardOffice
        if (idx == 3) return ns;             // NameServer
        if (idx == 4) return t;              // Truck
        if (idx == 5) return bp;             // BottlingPlant

        idx -= 6;
        if (idx < ss.size()) {
            return ss[idx];                  // Students
        }
        idx -= ss.size();
        if (idx < vms.size()) {
            return vms[idx];                 // Vending Machines
        }
        idx -= vms.size();
        return cs[idx];                      // Couriers
    };

    // find last occupied column so we avoid trailing tabs
    int last = -1;
    for (unsigned int i = 0; i < totalCols; ++i) {
        Slot &s = getSlot(i);
        if (s.occupied) last = i;
    }

    // if nothing to print, still output a newline - should never happen ideally
    if (last == -1) {
        cout << endl;
        return;
    }

    // print up to 'last' - thus no trailing \t
    for (int i = 0; i <= last; ++i) {
        if (i > 0) cout << '\t';

        Slot &s = getSlot(i);
        if (s.occupied) {
            cout << s.state;
            if (s.numbers >= 1) {
                cout << s.one;
            }
            if (s.numbers == 2) {
                cout << ',' << s.two;
            }

            // clear the slot
            s.occupied = false;
        } // else do nothing
    }

    cout << endl;
}

//   struct Slot {
//     bool occupied = false;

//     char state;

//     int numbers = 0; // 0, 1, or 2

//     unsigned int one;

//     unsigned int two;
//   };

//   enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };

// the following function load the slot struct into the correct column, throwing if
// providing lid when shouldn't and not providing lid when should
// will also print out if slot being loaded is occupied

void Printer::load( Slot slot, Kind kind){
    switch (kind) {
        case Parent:
            if (p.occupied) dump();
            p = slot;
            break;
        case Groupoff:
            if (go.occupied) dump();
            go = slot;
            break;
        case WATCardOffice:
            if (wco.occupied) dump();
            wco = slot;
            break;
        case NameServer:
            if (ns.occupied) dump();
            ns = slot;
            break;
        case Truck:
            if (t.occupied) dump();
            t = slot;
            break;
        case BottlingPlant:
            if (bp.occupied) dump();
            bp = slot;
            break;
        default:
            _Throw InvalidFunctionCall();
    }
}

void Pinter::load( Slot slot, Kind kind, unsigned int i){
    switch (kind) {
        case Student:
            if (ss[i].occupied) dump();
            ss[i] = slot;
            break;
        case Vending:
            if (vms[i].occupied) dump();
            vms[i] = slot;
            break;
        case Courier:
            if (wco[i].occupied) dump();
            wco[i] = slot;
            break;
        default:
            _Throw InvalidFunctionCall();
    }
}

// Each of the functions below make the custom slot 
// then load in via the correct load function

void Printer::print( Kind kind, char state ){
    Slot new_print = Slot{ occupied = true, state = state, numbers = 0 }

    load(new_print, kind)
}

void Printer::print( Kind kind, char state, unsigned int value1 ){
    Slot new_print = Slot{ occupied = true, state = state, numbers = 1, one = value1 }

    load(new_print, kind)
}

void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ){
    Slot new_print = Slot{ occupied = true, state = state, numbers = 2, one = value1, two = value2 }

    load(new_print, kind)
}



void Printer::print( Kind kind, unsigned int lid, char state ){
    Slot new_print = Slot{ occupied = true, state = state, numbers = 0 }

    load(new_print, kind, lid)
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ){
    Slot new_print = Slot{ occupied = true, state = state, numbers = 1, one = value1 }

    load(new_print, kind, lid)
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ){
    Slot new_print = Slot{ occupied = true, state = state, numbers = 2, one = value1, two = value2 }

    load(new_print, kind, lid)
}