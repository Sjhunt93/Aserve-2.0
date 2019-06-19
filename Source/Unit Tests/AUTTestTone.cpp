//
//  AUTMtof.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#include "AUTTestTone.hpp"


AUTTestTone::AUTTestTone (AserveComs & coms) : AserveUnitTest(coms, "Test Tone")
{
    
}
AUTTestTone::~AUTTestTone ()
{
    
}

void AUTTestTone::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    
    StringArray tt;
    int counter = 0;
    do {
        tt = getAndClearMessageLog();
        Thread::sleep(1000);
    } while (!tt.size() && counter < 20);
    
    const String inital = tt[0];
    if (inital.startsWith("osc")) {
        const float freq = inital.fromFirstOccurrenceOf("|", false, false).fromFirstOccurrenceOf("|", false, false).upToFirstOccurrenceOf("|", false, false).getFloatValue();
        if ((int)freq != 503) {
            currentState = eTestState::eEndedFail;
        }
    }
    else {
        currentState = eTestState::eEndedFail;
    }
    
    
    if (currentState == eTestState::eEndedPass) {
        errorMessages = "ALL Passed";
    }
    

    
}
