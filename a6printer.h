#ifndef _A6_PRINTER_H__
#define _A6_PRINTER_H__

#include <map>

_Cormonitor Printer {
public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numberOfStudents, unsigned int numberOfVendingMachines, unsigned int numberOfCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

private:
    class Info {
    protected:
        const Kind kind;
        const char state;
    public:
        Info( Kind kind, char state );
        virtual void print();
    };

    class ValueInfo : public Info {
        const int value;
    public:
        ValueInfo( Kind kind, char state, int value );
        virtual void print();
    };

    class ValuesInfo : public Info {
        const int value1;
        const int value2;
    public:
        ValuesInfo( Kind kind, char state, int value1, int value2);
        virtual void print();
    };

    void printEverythingIfCollided( Kind kind, unsigned int lid, char state );
    void printEverything(std::string);
    void flushInfo();

    const unsigned int numberOfStudents;
    const unsigned int numberOfVendingMachines;
    const unsigned int numberOfCouriers;
    unsigned int numberOfColumns;

    std::map<Kind, unsigned int> kindIndex;
    std::map<unsigned int, Info*> infoState;
};

#endif