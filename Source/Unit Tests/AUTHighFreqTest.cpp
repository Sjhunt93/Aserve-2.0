//
//  AUTHighFreqTest.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#include "AUTHighFreqTest.hpp"



AUTHighFreqTest::AUTHighFreqTest (AserveComs & coms) : AserveUnitTest(coms, "Fix Noteoff", "unit test 4")
{
    
}
AUTHighFreqTest::~AUTHighFreqTest ()
{
    
}




void AUTHighFreqTest::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    using namespace AserveUnitTestUtilityClass;
    
    
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 1, 127));
    const int numberOfEventsToCollect = ((17300 - 20) / 17) + 1;
    
    StringArray msgs = getCue(15000, numberOfEventsToCollect+5, 10);
    float lastFreq = 20;
    for (int i = 0; i < msgs.size(); i++) {
        String str = msgs[i];
        if (str.startsWith("osc")) {
            const float f = getFrequency(str);
            if (i == 0) {
                if (! compareFreq(20, f)) {
                    errorMessages += "ERROR! First value should be 20hz: first value received " + String(f);
                    currentState = eTestState::eEndedFail;
                    return;
                }
            }
            else if (f > 17300) {
                errorMessages += "ERROR! Frequency value should not exceed 17300hz: value received " + String(f);
                currentState = eTestState::eEndedFail;
                return;
            }
            else {
                if (! compareFreq(lastFreq+17, f)) {
                    int inc = f - lastFreq;
                    errorMessages += "ERROR! Frequency value increment not 17hz: value received " + String(inc);
                    currentState = eTestState::eEndedFail;
                    return;
                }
                lastFreq = f;
            }
            
            
            
        }
    }
    if (msgs.size() != numberOfEventsToCollect) {
        errorMessages += incorrectNumberOfMessages(String(numberOfEventsToCollect), String(msgs.size()));
        currentState = eTestState::eEndedFail;
        return;
    }
    
    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
    
}
