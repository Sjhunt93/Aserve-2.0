//
//  AUTDrumSampler.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 24/07/2019.
//

#include "AUTDrumSampler.hpp"
#include <array>

AUTDrumSampler::AUTDrumSampler (AserveComs & coms) : AserveUnitTest(coms, "Drum Sampler", "unit test 2")
{
    
}
AUTDrumSampler::~AUTDrumSampler ()
{
    
}

void AUTDrumSampler::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    
    std::array<int, 4> notes = {67, 69, 71, 72};
    for (int note : notes) {
        MidiMessage messageOn(0x90, note, 127);
        coms.sendMidiMessageFromImpulse(messageOn);
        MidiMessage messageOff(0x90, note, 0);
        coms.sendMidiMessageFromImpulse(messageOff);

    }
    
    StringArray results =  getCue(2000, 8);
    if (results.size() != 4) {
        currentState = eTestState::eEndedFail;
        errorMessages += "Test failed: expected 4 messages recived: " + String(results.size());
    }
    else {
        for (int i = 0; i < results.size(); i++) {
            const String inital = results[i];
            if (inital.startsWith("playSample")) {
                const int noteNum = inital.fromFirstOccurrenceOf("|", false, false).getIntValue();
                if (noteNum != i) {
                    errorMessages += "Test failed: expected sample number: " + String(i) + " for note number : " + String(notes[i]) + " recived sample number: " + String(noteNum) + " instead";
                    currentState = eTestState::eEndedFail;
                }

                
            }
        }
    }
    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
    
}
