//
//  AUTAlgorithms.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 06/08/2019.
//

#ifndef AUTAlgorithms_hpp
#define AUTAlgorithms_hpp

#include "AserveUnitTest.hpp"

class AUTAlgorithms : public AserveUnitTest
{
public:
    AUTAlgorithms (AserveComs & coms);
    ~AUTAlgorithms ();
    
    void runningUnitTest ();
    
};
#endif /* AUTAlgorithms_hpp */
