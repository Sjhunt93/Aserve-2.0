//
//  AUTNoteoff.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#include "AUTNoteoff.hpp"

AUTNoteoff::AUTNoteoff (AserveComs & coms) : AserveUnitTest(coms, "Fix Noteoff", "unit test 4")
{
    
}
AUTNoteoff::~AUTNoteoff ()
{
    
}




void AUTNoteoff::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    
    const int testNoteA = arc4random() % 60 + 36;
    const int testNoteB = testNoteA + (arc4random() % 3) + 1;
    
    
    
    using namespace AserveUnitTestUtilityClass;
    
    auto testCondition  = [&](int note, bool amp) -> bool
    {
        StringArray results = getCue(1000, 3);
        if (results.size() == 1) {
            String str = results[0];
            if (str.startsWith("osc")) {
                float f = getFrequency(str);
                float a = getAmplitude(str);
                
                if (!compareFreq(f, (note > 0) ? mtof(note) : 0))
                {
                    errorMessages += incorrectValueForNoteNumber(note, String(f), String(mtof(testNoteA))) + " Frequency";
                    currentState = eTestState::eEndedFail;
                    return false;
                }
                bool ampCondition = amp ? a > 0.0 : a == 0.0; //so if amp is true, then amplitude recieved should be positive else it should be zero
                if (  ! ampCondition ) {
                    errorMessages += incorrectValueForNoteNumber(note, String(a), amp ? "1.0" : "0.0") + " Amplitude";
                    currentState = eTestState::eEndedFail;
                    return false;
                }
            }
            
        }
        else {
            errorMessages += incorrectNumberOfMessages(String(1), String(results.size()));
            currentState = eTestState::eEndedFail;
            return false;
        }
        return true;

    };
    
    { // first noteon message
        MidiMessage messageOn(0x90, testNoteA, 127);
        coms.sendMidiMessageFromImpulse(messageOn);
        if (!testCondition(testNoteA, true)) {
            return;
        }
    }
    
    { // second noteon message
        MidiMessage messageOn(0x90, testNoteB, 127);
        coms.sendMidiMessageFromImpulse(messageOn);
        if (!testCondition(testNoteB, true)) {
            return;
        }
    }
    { // first noteoff message
        MidiMessage messageOn(0x90, testNoteA, 0);
        coms.sendMidiMessageFromImpulse(messageOn);
        StringArray results = getCue(1000, 0);
      
        if (results.size() != 0) {
        
          const String inital = results[0];
          if ( inital.startsWith("osc") )
          {
            errorMessages += "ERROR: Oscillator state should only be updated when note off note number matches the oscillator playing\n";
            
            const float newAmp = inital.fromFirstOccurrenceOf("|", false, false).fromFirstOccurrenceOf("|", false, false).fromFirstOccurrenceOf("|", false, false).upToFirstOccurrenceOf("|", false, false).getFloatValue();
          
            if ( newAmp != 0 )
              errorMessages += "ERROR: Oscillator amplitude should be zero when note off note number matches the oscillator playing\n";
          }
          
          
            //            errorMessages += incorrectNumberOfMessages("0", String(results.size()));
            currentState = eTestState::eEndedFail;
            return;
        }
    }
    { // second noteoff message
        MidiMessage messageOn(0x90, testNoteB, 0);
        coms.sendMidiMessageFromImpulse(messageOn);
        if (!testCondition(0, false)) {
            return;
        }
    }
    
    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
    
}
