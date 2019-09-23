//
//  AUTMtof.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#ifndef AUTTestTone_hpp
#define AUTTestTone_hpp

#include "AserveUnitTest.hpp"

class AUTTestTone : public AserveUnitTest
{
public:
    AUTTestTone (AserveComs & coms);
    ~AUTTestTone ();
    
    void runningUnitTest ();
    
};

#endif /* AUTMtof_hpp */
