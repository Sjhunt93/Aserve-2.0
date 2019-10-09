//
//  AUTVectorSequencer.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#ifndef AUTVectorSequencer_hpp
#define AUTVectorSequencer_hpp

#include "AserveUnitTest.hpp"

class AUTVectorSequencer : public AserveUnitTest
{
public:
    AUTVectorSequencer (AserveComs & coms);
    ~AUTVectorSequencer ();
    
    void runningUnitTest ();
    
};
#endif /* AUTVectorSequencer_hpp */
