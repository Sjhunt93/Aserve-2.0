//
//  AUTHighFreqTest.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#ifndef AUTHighFreqTest_hpp
#define AUTHighFreqTest_hpp

#include "AserveUnitTest.hpp"

class AUTHighFreqTest : public AserveUnitTest
{
public:
    AUTHighFreqTest (AserveComs & coms);
    ~AUTHighFreqTest ();
    
    void runningUnitTest ();
    
};

#endif /* AUTHighFreqTest_hpp */
