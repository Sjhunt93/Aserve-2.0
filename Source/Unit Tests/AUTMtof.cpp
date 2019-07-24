//
//  AUTMtof.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#include "AUTMtof.hpp"


AUTMtof::AUTMtof (AserveComs & coms) : AserveUnitTest(coms, "mtof", "unit test 1")
{
    
}
AUTMtof::~AUTMtof ()
{
    
}

void AUTMtof::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    for (int note = 0; note < 127 && currentState != eTestState::eEndedFail; note++) {
        MidiMessage message(0x90, note, 127);
        coms.sendMidiMessageFromImpulse(message);
        StringArray tt;
        int counter = 0;
        do {
            tt = getAndClearMessageLog();
            Thread::sleep(10);
            counter++;
        } while (!tt.size() && counter < 100);
        
        const String inital = tt[0];
        if (inital.startsWith("osc")) {
            float testF = 440.0 * pow(2.0, (note-69)/12.0);
            const float freq = inital.fromFirstOccurrenceOf("|", false, false).fromFirstOccurrenceOf("|", false, false).upToFirstOccurrenceOf("|", false, false).getFloatValue();
//            std::cout << testF << " - " << inital << "\n";
            if ((int)testF != (int)freq) {
//            if (testF != freq) {

                currentState = eTestState::eEndedFail;

            }
            
        }
        else {
            currentState = eTestState::eEndedFail;
        }
    }
    
    if (currentState == eTestState::eEndedPass) {
        errorMessages = "ALL Passed";
    }
    
    /*
    MidiMessage message(0x90, 69, 127);
    coms.sendMidiMessageFromImpulse(message);
    
    StringArray tt;
    
    int counter = 0;
    do {
        tt = getAndClearMessageLog();
        Thread::sleep(100);
        counter++;
    } while (!tt.size() && counter < 5);
    
    const String inital = tt[0];
    if (inital.startsWith("osc")) {
        currentState = eTestState::eEndedPass;
    }
    else {
        currentState = eTestState::eEndedFail;
    }
    */
    
}
