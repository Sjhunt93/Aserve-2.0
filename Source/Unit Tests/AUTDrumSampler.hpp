//
//  AUTDrumSampler.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 24/07/2019.
//

#ifndef AUTDrumSampler_hpp
#define AUTDrumSampler_hpp

#include "AserveUnitTest.hpp"

class AUTDrumSampler : public AserveUnitTest
{
public:
    AUTDrumSampler (AserveComs & coms);
    ~AUTDrumSampler ();
    
    void runningUnitTest ();
    
};
#endif /* AUTDrumSampler_hpp */
