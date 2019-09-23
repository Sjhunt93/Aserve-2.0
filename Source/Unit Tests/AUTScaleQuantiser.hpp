//
//  AUTScaleQuantiser.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#ifndef AUTScaleQuantiser_hpp
#define AUTScaleQuantiser_hpp

#include "AserveUnitTest.hpp"

class AUTScaleQuantiser : public AserveUnitTest
{
public:
    AUTScaleQuantiser (AserveComs & coms);
    ~AUTScaleQuantiser ();
    
    void runningUnitTest ();
    
};

#endif /* AUTScaleQuantiser_hpp */
