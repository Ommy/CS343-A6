#include "a6parent.h"
#include "a6printer.h"
#include "a6main.h"
#include "a6bank.h"

#include <iostream>

Parent::Parent( Printer& prt,
                Bank& bank,
                unsigned int numberOfStudents,
                unsigned int parentalDelay ) :  printer( prt ),
                         bank( bank ),
                         numberOfStudents( numberOfStudents ),
parentalDelay( parentalDelay ) {

}

Parent::~Parent() {
}

void Parent::main() {
    printer.print( Printer::Parent, ( char )Start );

    while ( true ) {
        _Accept ( ~Parent )  {
            break;
        }
        _Else {
            unsigned int amount = A6::mprng( 1,3 );
            unsigned int student = A6::mprng( numberOfStudents - 1 );
            yield( parentalDelay );

            printer.print( Printer::Parent, ( char )Deposit, student, amount );
            bank.deposit( student, amount );
        }
    }

    printer.print( Printer::Parent, ( char )Finish );
}