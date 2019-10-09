//
//  AUChordMachine.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 29/07/2019.
//

#ifndef AUChordMachine_hpp
#define AUChordMachine_hpp


#include "AserveUnitTest.hpp"

class AUTChordMachine : public AserveUnitTest
{
public:
    AUTChordMachine (AserveComs & coms);
    ~AUTChordMachine ();
    
    void runningUnitTest ();
    
};
#endif /* AUChordMachine_hpp */
