//
//  AUTVectorSequencer.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#include "AUTVectorSequencer.hpp"

AUTVectorSequencer::AUTVectorSequencer (AserveComs & coms) : AserveUnitTest(coms, "Vector Sequencer", "unit test 7")
{
    
}
AUTVectorSequencer::~AUTVectorSequencer ()
{
    
}




void AUTVectorSequencer::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    using namespace AserveUnitTestUtilityClass;
    
    std::vector<int> notes;
    for (int i = 0; i < 5; i++) {
        notes.push_back(arc4random() % 12 + 60);
    }
    //coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 1, 127));
    for (int i = 0; i < 5; i++) {
        
        MidiMessage messageOn(0x90, notes[i], 127);
        coms.sendMidiMessageFromImpulse(messageOn);
        
#if 0
        StringArray messages = getCue(400, 2);
        if (messages.size() == 1) {
            
        }
        else {
            errorMessages += incorrectNumberOfMessages(String(1), String(messages.size()));
            currentState = eTestState::eEndedFail;
            return;
        }
#endif
        
        MidiMessage messageOff(0x90, notes[i], 0);
        coms.sendMidiMessageFromImpulse(messageOff);
    }
    
    
    {StringArray array = getCue(500, 100);} //clear the cache
    
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 115, 127));
    
    StringArray messages = getCue(2000, 10);
    
    if (messages.size() > 5) {
        int nInc = 0;
        std::vector<float> expectedSeq, recievedSeq;
        for (String str : messages) {
            if (str.startsWith("osc")) {
                
                float testF = 440.0 * pow(2.0, (notes[nInc%5]-69)/12.0);
                float freqIn = AserveUnitTestUtilityClass::getFrequency(str);
                
                expectedSeq.push_back(testF);
                recievedSeq.push_back(freqIn);
                if (!AserveUnitTestUtilityClass::compareFreq(testF, freqIn)) {
                    
                    errorMessages += "ERROR! incorrect sequence sent back.";
                    currentState = eTestState::eEndedFail;
                    
                }
                nInc++;
            }
            else {
                errorMessages += incorrectMessageRecived("osc", str.upToFirstOccurrenceOf("|", false, false));
                currentState = eTestState::eEndedFail;
                return;
            }
        }
        if (currentState == eTestState::eEndedFail) {
            
            errorMessages += "\nExpected Seq: ";
            for (int i = 0; i < expectedSeq.size(); i++) {
                errorMessages += String(expectedSeq[i]) + ", ";
            }
            errorMessages += "\nreceived Seq: ";
            for (int i = 0; i < recievedSeq.size(); i++) {
                errorMessages += String(recievedSeq[i]) + ", ";
            }
            
            return;
        }
    }
    else if (messages.size() == 0) {
        errorMessages += "ERROR! No messages received on playback";
        currentState = eTestState::eEndedFail;
        return;
    }
    else {
        errorMessages += "ERROR! Not enough messages received during playback";
        currentState = eTestState::eEndedFail;
        return;
    }
    
    
    
//    else {
//        errorMessages += incorrectNumberOfMessages(String(5), String(messages.size())) + "During playback";
//        currentState = eTestState::eEndedFail;
//        return;
//    }
    
    
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 114, 127));
    
    
    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
    
}


