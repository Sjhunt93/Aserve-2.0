//
//  AUFileWrite.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#include "AUFileWrite.hpp"


AUTFileWrite::AUTFileWrite (AserveComs & coms) : AserveUnitTest(coms, "File Write", "unit test 9")
{
    
}
AUTFileWrite::~AUTFileWrite ()
{
    
}

void AUTFileWrite::runningUnitTest ()
{

    
    std::vector<int> noteNumbers;
    std::vector<int> velcotiyNumbers;
    const int totalEvents = 15;
    
    coms.addUnitTestMessageToLog("Starting Test, sending notes messages");
    
    for (int i = 0; i < totalEvents; i++) {
        const int note = (arc4random() % 12) + 48;
        
        noteNumbers.push_back(note);
        noteNumbers.push_back(note);
        velcotiyNumbers.push_back(127);
        velcotiyNumbers.push_back(0);
        
        MidiMessage messageOn(0x90, note, 127);
        coms.sendMidiMessageFromImpulse(messageOn);
        
        Thread::sleep(150);
        
        MidiMessage messageOff(0x90, note, 0);
        coms.sendMidiMessageFromImpulse(messageOff);
        
        Thread::sleep(150);
    }
    
    Thread::sleep(4000);
    
    {StringArray array = getCue(500, 100);} //clear the cache
    
    coms.addUnitTestMessageToLog("waiting for file save..");
    coms.sendMidiMessageFromImpulse(MidiMessage::controllerEvent(1, 117, 127));
    
    Thread::sleep(5000);
    
    {StringArray array = getCue(500, 100);} //clear the cache
    
    
    
    
    File file(projectPath);
    file = file.getParentDirectory().getChildFile("Builds").getChildFile("MacOSX").getChildFile("build").getChildFile("Debug").getChildFile("notes.txt");
    
    coms.addUnitTestMessageToLog("opening file..");
    if (file.exists()) {
        FileInputStream stream(file);
        if (stream.openedOk()) {
            std::vector<int> notesInFile, velocitysInFile;
            while (!stream.isExhausted()) {
                String str = stream.readNextLine();
                if (!str.contains(",")) {
                    errorMessages += "File not formatted correctly: should be note,velocity. Line value: " + str;
                    currentState = eTestState::eEndedFail;
                    return;
                }
                notesInFile.push_back(str.upToFirstOccurrenceOf(",", false, false).getIntValue());
                velocitysInFile.push_back(str.fromFirstOccurrenceOf(",", false, false).getIntValue());
            }
            
            if (notesInFile.size() != totalEvents * 2) {
                errorMessages += "Incorrect number of notes in file: Expected: " + String(totalEvents*2) + " Actual: " + String(notesInFile.size()) + "\n";
                currentState = eTestState::eEndedFail;
                return;
            }
            
            coms.addUnitTestMessageToLog("comparing values in file..");
            bool failFlag = false;
            for (int i = 0; i < noteNumbers.size() && !failFlag; i++) {
                
                failFlag |= (notesInFile[i] != noteNumbers[i]);
                failFlag |= (velcotiyNumbers[i] != velocitysInFile[i]);
            }
            if (failFlag) {
                errorMessages += "ERROR! incorrect sequence sent back.";
                currentState = eTestState::eEndedFail;
                
                errorMessages += "\nExpected Notes: ";
                for (int i = 0; i < noteNumbers.size(); i++) {
                    errorMessages += String(noteNumbers[i]) + ", ";
                }
                errorMessages += "\nNotes in file: ";
                for (int i = 0; i < notesInFile.size(); i++) {
                    errorMessages += String(notesInFile[i]) + ", ";
                }
                
                errorMessages += "\nExpected Velocities: ";
                for (int i = 0; i < velcotiyNumbers.size(); i++) {
                    errorMessages += String(noteNumbers[i]) + ", ";
                }
                errorMessages += "\nVelocity values in file: ";
                for (int i = 0; i < velocitysInFile.size(); i++) {
                    errorMessages += String(notesInFile[i]) + ", ";
                }
                currentState = eTestState::eEndedFail;
                return;
            }
            
       }
    }
    else
    {
        errorMessages += "ERROR! File does not exist in expected location \n";
        currentState = eTestState::eEndedFail;
        return;
    }
    

    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
}
