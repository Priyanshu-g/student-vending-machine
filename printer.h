#ifndef PRINTER_H
#define PRINTER_H 

#include <vector>

using namespace std;

_Monitor Printer {
  struct Slot {
    bool occupied = false;

    char state;

    int numbers = 0; // 0, 1, or 2

    unsigned int one;

    unsigned int two;
  };

  _Exception InvalidFunctionCall{};

  Slot p; // parent
  Slot go; // Groupoff
  Slot wco; // watcardoffice
  Slot ns; // Name Server
  Slot t; // truck
  Slot bp; // BottlingPlant

  vector<Slot> ss; // students
  vector<Slot> vms; // vending machines
  vector<Slot> cs; // coursies

  void dump();

 public:
  enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
  Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
  void print( Kind kind, char state );
  void print( Kind kind, char state, unsigned int value1 );
  void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
  void print( Kind kind, unsigned int lid, char state );
  void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
  void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};

#endif