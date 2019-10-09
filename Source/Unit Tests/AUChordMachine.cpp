//
//  AUChordMachine.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 29/07/2019.
//

#include "AUChordMachine.hpp"
#include <array>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
AUTChordMachine::AUTChordMachine (AserveComs & coms) : AserveUnitTest(coms, "Chord Machine", "unit test 3")
{
    
}
AUTChordMachine::~AUTChordMachine ()
{
    
}

inline std::vector<int> getMajorChord (int root)
{
    return {root, root+4, root+7};
}

inline std::vector<int> getMinorChord (int root)
{
    return {root, root+3, root+7};
}


void AUTChordMachine::runningUnitTest ()
{
    currentState = eTestState::eEndedPass;
    
    std::array<int, 4> notes = {67, 69, 71, 72};
    std::array<int, 4> chordRoots = {60, 65, 67, 57};
//    std::random_shuffle(notes.begin(), notes.end());
    
    for (int i = 0; i < notes.size(); i++) {
        MidiMessage messageOn(0x90, notes[i], 127);
        coms.sendMidiMessageFromImpulse(messageOn);
        
        StringArray results =  getCue(2000, 3);
        std::vector<float> freqReceived;
        std::vector<int> channels;
      
        if ( results.size() ) // process each result
        {
          for (String str : results) {
              if (str.startsWith("osc")) {
                  channels.push_back( AserveUnitTestUtilityClass::getChannel(str) );
                  freqReceived.push_back(AserveUnitTestUtilityClass::getFrequency(str));
              }
              else {
                  currentState = eTestState::eEndedFail;
                  errorMessages += "Test failed: incorrect message type recieved";
                  return;
              }
          }
        }
        else  // no results, test fails with timeout
        {
          currentState = eTestState::eEndedFail;
          return;
        }
      
        if (channels.size() == 3) {
            auto chan = channels;
            std::sort(chan.begin(), chan.end());
            auto last = std::unique(chan.begin(), chan.end());
            chan.erase(last, chan.end());
            
            if (chan.size() != 3) {
                currentState = eTestState::eEndedFail;
                errorMessages += "Test failed: messages recieved on the same oscillator channel. Each note of the chord should be on a different channel ";
                return;
            }
        }
        else {
            errorMessages += "Test failed: Incorrect number of note messages recived. Chords for this test should only have 3 notes." + String(" Received ") + String(channels.size()) + String(" note messages");
        }
        
        std::vector<float> testSetFreq;
        if (chordRoots[i] == 57 ) { //there is one minor chord...
            testSetFreq = AserveUnitTestUtilityClass::freqFromNoteSeq(getMinorChord(chordRoots[i]));
        }
        else {
            testSetFreq = AserveUnitTestUtilityClass::freqFromNoteSeq(getMajorChord(chordRoots[i]));
        }
        
        std::sort(freqReceived.begin(), freqReceived.end());
        std::sort(testSetFreq.begin(), testSetFreq.end());
        for (int j = 0; j < testSetFreq.size() && freqReceived.size(); j++) {
            if ((int)testSetFreq[j] != (int)freqReceived[j]) {
                currentState = eTestState::eEndedFail;
//                errorMessages += "Freqency received does not match what was expected for note number: " + String(note) + "\n";
                errorMessages += "Test failed: Incorrect chord played for note number: " + String(notes[i]);
            }
        }
        
        Thread::sleep(500);
        
    }
    
    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
    
}
