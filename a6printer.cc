#include "a6printer.h"

#include <iostream>
#include <sstream>
#include <string>

Printer::Printer(unsigned int numberOfStudents, unsigned int numberOfVendingMachines, unsigned int numberOfCouriers)
: numberOfStudents(numberOfStudents), numberOfVendingMachines(numberOfVendingMachines), numberOfCouriers(numberOfCouriers), numberOfColumns(0) {

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
    printEverythingIfCollided(kind, lid, state);
    infoState[lid] = new Info(kind, state);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    printEverythingIfCollided(kind, lid, state);
    infoState[lid] = new ValueInfo(kind, state, value1);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    printEverythingIfCollided(kind, lid, state);
    infoState[lid] = new ValuesInfo(kind, state, value1, value2);
}

void Printer::printEverythingIfCollided( Kind kind, unsigned int lid, char state ) {
    lid += kindIndex[kind];
    if (infoState.find(lid) != infoState.end()) {
        if (state == 'F') {
            printEverything("...");
        } else {
            printEverything("");
        }
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

    flushInfo();
}

void Printer::flushInfo(){
    kindIndex.clear();
    infoState.clear();
}

Printer::Info::Info( Kind kind, char state ) : 
kind(kind), state(state) {
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