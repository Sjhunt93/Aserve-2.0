//
//  AUTFileRead.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 05/08/2019.
//

#include "AUTFileRead.hpp"



AUTFileRead::AUTFileRead (AserveComs & coms) : AserveUnitTest(coms, "File Read", "unit test 8")
{
    
}
AUTFileRead::~AUTFileRead ()
{
    
}

void AUTFileRead::runningUnitTest ()
{
    File file(projectPath);
    file = file.getParentDirectory().getChildFile("Builds").getChildFile("MacOSX").getChildFile("build").getChildFile("Debug").getChildFile("exercise1.txt");
    
    if (file.exists()) {
        file.deleteFile();
    }
    
    std::vector<int> noteNumbers;
    for (int i = 0; i < 15; i++) {
        noteNumbers.push_back((arc4random() % 12) + 48);
    }
    {
        coms.addUnitTestMessageToLog("Writing notes numbers to file..");
        FileOutputStream stream(file);
        if (stream.openedOk()) {
            for (int num : noteNumbers) {
                stream.writeText(String(num) + String("\n"), false, false);
            }
            
        }
        coms.addUnitTestMessageToLog("File saved..");
    }
    
    coms.addUnitTestMessageToLog("waiting for note messages from project..");
    StringArray messages = getCue(20 * 1000, 15);
    if (messages.size() == 15) {
        int nInc = 0;
        
        std::vector<float> expectedSeq, recievedSeq;
        for (int i = 0; i < 15; i++) {
            String str = messages[i];
            int note = noteNumbers[i];
            if (str.startsWith("osc")) {
                
                float testF = 440.0 * pow(2.0, (note-69)/12.0);
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
                errorMessages += AserveUnitTestUtilityClass::incorrectMessageRecived("osc", str.upToFirstOccurrenceOf("|", false, false));
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
    
    
    
    
    
    if (currentState != eTestState::eEndedFail) {
        currentState = eTestState::eEndedPass;
        errorMessages = "ALL Passed";
    }
}
