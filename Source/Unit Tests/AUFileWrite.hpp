//
//  AUFileWrite.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#ifndef AUFileWrite_hpp
#define AUFileWrite_hpp

#include "AserveUnitTest.hpp"

class AUTFileWrite : public AserveUnitTest
{
public:
    AUTFileWrite (AserveComs & coms);
    ~AUTFileWrite ();
    
    void runningUnitTest ();
    
};
#endif /* AUFileWrite_hpp */
