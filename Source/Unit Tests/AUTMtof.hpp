//
//  AUTMtof.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#ifndef AUTMtof_hpp
#define AUTMtof_hpp

#include "AserveUnitTest.hpp"

class AUTMtof : public AserveUnitTest
{
public:
    AUTMtof (AserveComs & coms);
    ~AUTMtof ();
    
    void runningUnitTest ();
    
};

#endif /* AUTMtof_hpp */
