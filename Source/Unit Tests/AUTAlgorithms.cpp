//
//  AUTAlgorithms.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 06/08/2019.
//

#include "AUTAlgorithms.hpp"


AUTAlgorithms::AUTAlgorithms (AserveComs & coms) : AserveUnitTest(coms, "Algorithms", "unit test 10")
{
    
}
AUTAlgorithms::~AUTAlgorithms ()
{
    
}




void AUTAlgorithms::runningUnitTest ()
{
#define CLEAR_CACHE {StringArray array = getCue(500, 100);}
    auto checkSequence = [&](StringArray messages, std::vector<int> testSequence) -> bool
    {
        if (messages.size() == 5) {
            
            std::vector<float> expectedSeq, recievedSeq;
            for (int nInc = 0; nInc < 5; nInc++) {
                String str = messages[nInc];
                if (str.startsWith("osc")) {
                    
                    float testF = 440.0 * pow(2.0, (testSequence[nInc]-69)/12.0);
                    float freqIn = AserveUnitTestUtilityClass::getFrequency(str);
                    
                    expectedSeq.push_back(testF);
                    recievedSeq.push_back(freqIn);
                    if (!AserveUnitTestUtilityClass::compareFreq(testF, freqIn)) {
                        
                        errorMessages += "ERROR! incorrect sequence sent back.";
                        currentState = eTestState::eEndedFail;
                        
                    }
                }
                else {
                    errorMessages += AserveUnitTestUtilityClass::incorrectMessageRecived("osc", str.upToFirstOccurrenceOf("|", false, false));
                    currentState = eTestState::eEndedFail;
                    return false;
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
                
                return false;
            }
        }
        else if (messages.size() == 0) {
            errorMessages += "ERROR! No messages received on playback";
            currentState = eTestState::eEndedFail;
            return false;
        }
        else {
            errorMessages += "ERROR! Not enough messages received during playback";
            currentState = eTestState::eEndedFail;
            return false;
        }
        return true;
    };
    
    
    /// test

    currentState = eTestState::eEndedPass;
    using namespace AserveUnitTestUtilityClass;
    
    std::vector<int> notes;
    for (int i = 0; i < 5; i++) {
        notes.push_back(arc4random() % 12 + 60);
    }
    
    coms.addUnitTestMessageToLog("Sending sequence..");
    for (int i = 0; i < 5; i++) {
        
        MidiMessage messageOn(0x90, notes[i], 127);
        coms.sendMidiMessageFromImpulse(messageOn);
        Thread::sleep(100);
        MidiMessage messageOff(0x90, notes[i], 0);
        coms.sendMidiMessageFromImpulse(messageOff);
        Thread::sleep(100);
    }
    
    
    ///======================================================================== normal
    coms.addUnitTestMessageToLog("Checking normal sequence playback..");
    CLEAR_CACHE
    
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 115, 127));
    
    
    if ( !checkSequence(getCue(2000, 5), notes) )
    {
        return;
    }
    ///======================================================================== fowards
    coms.addUnitTestMessageToLog("Checking sorted sequence playback fowards..");
    
    CLEAR_CACHE
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 112, 127)); //sort foward
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 115, 127)); // playback
    
    std::sort(notes.begin(), notes.end());
    if ( !checkSequence(getCue(2000, 5), notes) )
    {
        return;
    }
    ///======================================================================== reverse
    coms.addUnitTestMessageToLog("Checking sorted sequence playback backwards..");

    CLEAR_CACHE
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 113, 127)); //sort reverse
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 115, 127)); // playback
    
    std::sort(notes.begin(), notes.end());
    std::reverse(notes.begin(), notes.end());
    if ( !checkSequence(getCue(2000, 5), notes) )
    {
        return;
    }
    ///======================================================================== shuffle
    coms.addUnitTestMessageToLog("Checking shuffled sequence..");
    
    CLEAR_CACHE
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 116, 127)); //shuffle
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 115, 127)); // playback
    
#if 0
    String msgs = errorMessages;
    
    if ( checkSequence(getCue(2000, 5), notes) ) //test conditions reversed.
    {
        msgs += "ERROR! Playing a sorted sequence, when sequence should be shuffled..";
        errorMessages = msgs;
        currentState = eTestState::eEndedFail;
        return;
    }
    errorMessages = msgs;
#else
    
    StringArray messagesA = getCue(2000, 5);
    
    CLEAR_CACHE
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 116, 127)); //shuffle
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 115, 127)); // playback
    
    StringArray messagesB = getCue(2000, 5);
    
    if (messagesA == messagesB) {
        errorMessages += "ERROR! Playing the same sequence twice, when sequence should be shuffled..";
        currentState = eTestState::eEndedFail;
        return;
    }
    
    
#endif
    
    

    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
    
}
