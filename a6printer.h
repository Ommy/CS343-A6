#ifndef _A6_PRINTER_H__
#define _A6_PRINTER_H__

#include <map>

_Cormonitor Printer {
    // void main();
public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
private:
    void printEverything(std::string);
    void flushInfo();
    unsigned int numOfStudents;
    unsigned int numOfVendingMachines;
    unsigned int numOfCouriers;
    int totalColumns;

    std::map<Kind, unsigned int> kindIndex;
    std::map<unsigned int, char> singleState;
    std::map<unsigned int, std::pair<char, int> > stateWithValue;
    std::map<unsigned int, std::pair<char, std::pair<int, int> > > stateWithValues;
};

#endif