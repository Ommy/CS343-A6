#include "a6vendingmachine.h"
#include "a6printer.h"
#include "a6nameserver.h"

#include <iostream>

VendingMachine::VendingMachine( Printer& prt,
                                NameServer& nameServer,
                                unsigned int id,
                                unsigned int sodaCost,
                                unsigned int maxStockPerFlavour ) : printer( prt ), 
                                                                    nameServer( nameServer ), 
                                                                    id( id ), 
                                                                    sodaCost( sodaCost ), 
                                                                    maxStockPerFlavour( maxStockPerFlavour ) {
    nameServer.VMregister( this );
    stock.fill( 0 );
}

VendingMachine::BuyOrder::BuyOrder(   Flavours flavour,
                                      WATCard& watcard ) :    flavour( flavour ),
watcard( watcard ),
result( Success ) {
}

void VendingMachine::buy( Flavours flavour, WATCard& watcard ) {
    order = std::shared_ptr<BuyOrder>( new BuyOrder( flavour, watcard ) );
    processOrderCondition.wait();

    if ( order->result == InsufficientFunds ) {
        uRendezvousAcceptor();
        throw Funds();
    } else if ( order->result == InsufficientStock ) {
        uRendezvousAcceptor();
        throw Stock();
    }
}

unsigned int* VendingMachine::inventory() {
    return stock.data();
}

void VendingMachine::restocked() {
}

unsigned int VendingMachine::cost() {
    return sodaCost;
}

unsigned int VendingMachine::getId() {
    return id;
}

void VendingMachine::main() {
    printer.print( Printer::Vending, id, ( char )Start, sodaCost );

    while ( true ) {
        _Accept( ~VendingMachine ) {
            break;
        }
        or _Accept( inventory ) {
            printer.print( Printer::Vending, id, ( char )StartReload );
            _Accept( restocked ) {
                printer.print( Printer::Vending, id, ( char )CompleteReload );
            }
        }
        or _Accept ( buy ) {
            WATCard& watcard = order->watcard;
            unsigned int flavour = static_cast<int>( order->flavour );
            if ( watcard.getBalance() < sodaCost ) {
                order->result = InsufficientFunds;
            } else if ( stock[flavour] == 0 ) {
                order->result = InsufficientStock;
            } else {
                watcard.withdraw( sodaCost );
                stock[flavour] -= 1;
                order->result = Success;

                printer.print( Printer::Vending, id, ( char )Bought, flavour, stock[flavour] );
            }

            processOrderCondition.signalBlock();
        }
    }

    printer.print( Printer::Vending, id, ( char )Finish );
}
