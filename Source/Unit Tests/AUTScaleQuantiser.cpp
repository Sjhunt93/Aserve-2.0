//
//  AUTScaleQuantiser.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#include "AUTScaleQuantiser.hpp"
#include "AUTMtof.hpp"


AUTScaleQuantiser::AUTScaleQuantiser (AserveComs & coms) : AserveUnitTest(coms, "Scale Quantiser", "unit test 5")
{
    
}
AUTScaleQuantiser::~ AUTScaleQuantiser()
{
    
}


inline int majorScale(int note)
{
    int pitch = note % 12;
    if (pitch == 1 || pitch == 3 || pitch == 6 || pitch == 8 || pitch == 10)
    {
        note++;
    }
    return note;
}
void AUTScaleQuantiser::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    for (int note = 0; note < 127 && currentState != eTestState::eEndedFail; note++) {
        MidiMessage message(0x90, note, 127);
        coms.sendMidiMessageFromImpulse(message);
        
        StringArray results =  getCue(1000, 1);
        const String inital = results[0];
        if (inital.startsWith("osc")) {
            
            int tNote = majorScale(note);
            float testF = 440.0 * pow(2.0, (tNote-69)/12.0);
            float freqIn = AserveUnitTestUtilityClass::getFrequency(inital);
            
            if (!AserveUnitTestUtilityClass::compareFreq(testF, freqIn)) {
                currentState = eTestState::eEndedFail;
                errorMessages += AserveUnitTestUtilityClass::incorrectValueForNoteNumber(note, String(testF), String(freqIn));
                errorMessages += "Note Number :" + String(note) + " should output: " + String(tNote) + "\n";
                currentState = eTestState::eEndedFail;
                return;
            }
            
        }
        else {
            currentState = eTestState::eEndedFail;
            errorMessages += AserveUnitTestUtilityClass::incorrectMessageRecived("Osc", inital.upToFirstOccurrenceOf("|", false, false));
            return;
        }
    }
    
    if (currentState == eTestState::eEndedPass) {
        errorMessages = "ALL Passed";
    }
    
    
}
