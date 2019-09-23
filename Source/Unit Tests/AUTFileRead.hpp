//
//  AUTFileRead.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#ifndef AUTFileRead_hpp
#define AUTFileRead_hpp

#include "AserveUnitTest.hpp"

class AUTFileRead : public AserveUnitTest
{
public:
    AUTFileRead (AserveComs & coms);
    ~AUTFileRead ();
    
    void runningUnitTest ();
    
};
#endif /* AUTFileRead_hpp */
