#include "a6printer.h"

#include <iostream>
#include <sstream>
#include <string>

Printer::Printer(   unsigned int numberOfStudents, 
                    unsigned int numberOfVendingMachines, 
                    unsigned int numberOfCouriers) :    numberOfStudents(numberOfStudents), 
                                                        numberOfVendingMachines(numberOfVendingMachines), 
                                                        numberOfCouriers(numberOfCouriers), 
                                                        numberOfColumns(0) {

    kindIndex[Parent] = numberOfColumns;
    std::cout << "Parent" << "\t";
    numberOfColumns++;

    kindIndex[WATCardOffice] = numberOfColumns;
    std::cout << "WATOff" << "\t";
    numberOfColumns++;

    kindIndex[NameServer] = numberOfColumns;
    std::cout << "Names" << "\t";
    numberOfColumns++;

    kindIndex[Truck] = numberOfColumns;
    std::cout << "Truck" << "\t";
    numberOfColumns++;

    kindIndex[BottlingPlant] = numberOfColumns;
    std::cout << "Plant" << "\t";
    numberOfColumns++;

    kindIndex[Student] = numberOfColumns;
    for (unsigned int i = 0; i < numberOfStudents; i++, numberOfColumns++) {
        std::cout << "Stud" << i << "\t";
    }
    kindIndex[Vending] = numberOfColumns;
    for (unsigned int i = 0; i < numberOfVendingMachines; i++, numberOfColumns++) {
        std::cout << "Mach" << i << "\t";
    }
    kindIndex[Courier] = numberOfColumns;
    for (unsigned int i = 0; i < numberOfCouriers; i++, numberOfColumns++) {
        std::cout << "Cour" << i << "\t";
    }
    std::cout << std::endl;
    
    for (unsigned int i = 0; i < numberOfColumns; i++) {
        std::cout << "*******" << "\t";
    }
    std::cout << std::endl;
}

Printer::~Printer() {
    std::cout << "***********************" << std::endl;
}

void Printer::print( Kind kind, char state ) {
    print(kind, 0, state);
}

void Printer::print( Kind kind, char state, int value1 ) {
    print(kind, 0, state, value1);
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
    print(kind, 0, state, value1, value2);
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    printInfo(kind, lid, state, new Info(kind, state));
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    printInfo(kind, lid, state, new ValueInfo(kind, state, value1));
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    printInfo(kind, lid, state, new ValuesInfo(kind, state, value1, value2));
}

void Printer::printInfo( Kind kind, unsigned int lid, char state, Info* info ) {
    lid += kindIndex[kind];
    if (state == 'F') {
        printEverything("");
        infoState[lid] = info;
        printEverything("...");
    } else if (infoState.find(lid) != infoState.end()) {
        printEverything("");
        infoState[lid] = info;   
    } else {
        infoState[lid] = info;
    }
}

void Printer::printEverything(std::string placeholder){
    for (unsigned int i = 0; i < numberOfColumns; i++) {
        if (infoState.find(i) != infoState.end()) {
            infoState[i]->print();
        } else {
            std::cout << placeholder;
        }
        std::cout << "\t";
    }
    std::cout << std::endl;

    infoState.clear();
}

Printer::Info::Info( Kind kind, char state ) : kind(kind), state(state) {
}

void Printer::Info::print() {
    std::cout << state;
}

Printer::ValueInfo::ValueInfo( Kind kind, char state, int value )
: Info(kind, state), value(value) {
}

void Printer::ValueInfo::print() {
    std::cout << state << value;
}

Printer::ValuesInfo::ValuesInfo( Kind kind, char state, int value1, int value2 )
: Info(kind, state), value1(value1), value2(value2) {
}

void Printer::ValuesInfo::print() {
    std::cout << state << value1 << "," << value2;
}

void Printer::main() {
    
}