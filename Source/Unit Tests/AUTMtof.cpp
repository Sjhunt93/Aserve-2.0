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
        
        StringArray results =  getCue(1000, 1);
        

        const String inital = results[0];
        if (inital.startsWith("osc")) {
        
            float testF = AserveUnitTestUtilityClass::mtof(note); //440.0 * pow(2.0, (note-69)/12.0);
          
            const float freq = inital.fromFirstOccurrenceOf("|", false, false).fromFirstOccurrenceOf("|", false, false).upToFirstOccurrenceOf("|", false, false).getFloatValue();

            if ((int)testF != (int)freq) {
                currentState = eTestState::eEndedFail;
                errorMessages += "Freqency received does not match what was expected for note number: " + String(note) + "\n";
            }
            
        }
        else {
            currentState = eTestState::eEndedFail;
        }
    }
    
    if (currentState == eTestState::eEndedPass) {
        errorMessages = "ALL Passed";
    }
    
    
}
