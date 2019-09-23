//
//  AUTNoteoff.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#ifndef AUTNoteoff_hpp
#define AUTNoteoff_hpp

#include "AserveUnitTest.hpp"

class AUTNoteoff : public AserveUnitTest
{
public:
    AUTNoteoff (AserveComs & coms);
    ~AUTNoteoff ();
    
    void runningUnitTest ();
    
};

#endif /* AUTNoteoff_hpp */
