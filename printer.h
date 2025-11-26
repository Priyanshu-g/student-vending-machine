#ifndef PRINTER_H
#define PRINTER_H 

#include <vector>
#include <iostream>

using namespace std;

_Monitor Printer {
  public:
  enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
  private:
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
  void load( Slot slot, Kind kind);
  void load( Slot slot, Kind kind, unsigned int i);

 public:
  ~Printer();
  Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
  void print( Kind kind, char state );
  void print( Kind kind, char state, unsigned int value1 );
  void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
  void print( Kind kind, unsigned int lid, char state );
  void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
  void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};

#endif