#include "a6printer.h"

#include <iostream>
#include <sstream>
#include <string>

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) {
    numOfStudents = numStudents;
    numOfVendingMachines = numVendingMachines;
    numOfCouriers = numCouriers;
    int totalColumnCount = 0;

    kindIndex[Parent] = totalColumnCount;
    std::cout << "Parent\t";
    totalColumnCount++;

    kindIndex[WATCardOffice] = totalColumnCount;
    std::cout << "WATOff\t";
    totalColumnCount++;

    kindIndex[NameServer] = totalColumnCount;
    std::cout << "Names\t";
    totalColumnCount++;

    kindIndex[Truck] = totalColumnCount;
    std::cout << "Truck\t";
    totalColumnCount++;

    kindIndex[BottlingPlant] = totalColumnCount;
    std::cout << "Plant\t";
    totalColumnCount++;

    kindIndex[Student] = totalColumnCount;
    for (unsigned int i = 0; i < numOfStudents; i++) {
        std::cout << "Stud" << i << "\t";
        totalColumnCount++;
    }

    kindIndex[Vending] = totalColumnCount;
    for (unsigned int i = 0; i < numOfVendingMachines; i++) {
        std::cout << "Mach" << i << "\t";
        totalColumnCount++;
    }

    kindIndex[Courier] = totalColumnCount;
    for (unsigned int i = 0; i < numOfCouriers; i++) {
        std::cout << "Cour" << i << "\t";
        totalColumnCount++;
    }

    totalColumns = totalColumnCount;
    std::cout << std::endl;

    for (int i = 0; i < totalColumnCount; i++) {
        std::cout << "*******" << "\t";
    }


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
    lid += kindIndex[kind];
    if (singleState.find(lid) != singleState.end()){
        if (state == 'F') {
            printEverything("...");
        } else {
            printEverything("");
        }
    }

    singleState[lid] = state;
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    lid += kindIndex[kind];
    if (stateWithValue.find(lid) != stateWithValue.end()){
        if (state == 'F') {
            printEverything("...");
        } else {
            printEverything("");
        }
    }

    stateWithValue[lid] = std::make_pair(state, value1);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    lid += kindIndex[kind];
    if (stateWithValues.find(lid) != stateWithValues.end()) {
        if (state == 'F') {
            printEverything("...");
        } else {
            printEverything("");
        }
    }

    stateWithValues[lid] = std::make_pair(state, std::make_pair(value1, value2));
}

void Printer::printEverything(std::string placeholder){
    for (int i = 0; i < totalColumns; i++) {
        if (singleState.find(i) != singleState.end()) {
            std::cout << singleState[i] << "\t";
        } else if (stateWithValue.find(i) != stateWithValue.end()) {
            std::cout << stateWithValue[i].first << stateWithValue[i].second << "\t";
        } else if (stateWithValues.find(i) != stateWithValues.end()) {
            std::cout << stateWithValues[i].first << stateWithValues[i].second.first << stateWithValues[i].second.second << "\t";
        } else {
            std::cout << placeholder << "\t";
        }
    }
    std::cout << std::endl;
    flushInfo();
}

void Printer::flushInfo(){
    kindIndex.clear();
    singleState.clear();
    stateWithValue.clear();
    stateWithValues.clear();
}

Printer::~Printer(){

}

