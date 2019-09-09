//
//  AserveUnitTest.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#include "AserveUnitTest.hpp"
#include "AUTMtof.hpp"
#include "AUTDrumSampler.hpp"
#include "AUTTestTone.hpp"
#include "AUChordMachine.hpp"
#include "AUTNoteoff.hpp"
#include "AUTScaleQuantiser.hpp"
#include "AUTHighFreqTest.hpp"
#include "AUTVectorSequencer.hpp"
#include "AUTFileRead.hpp"
#include "AUFileWrite.hpp"
#include "AUTAlgorithms.hpp"

const String AserveUnitTest::TEST_TIMEOUT = "Test Timeout - please check your code for excessive sleeps.\n";

#ifdef DEBUG
    String AserveUnitTest::solutionsPath = "/Users/sj4-hunt/Downloads/IAP-2018-2019-master 6/Solutions/Unit Tests";
    String AserveUnitTest::projectPath = "/Users/sj4-hunt/Downloads/IAP-2018-2019-master 6/iapProj/Source";
#endif

AserveUnitTest::AserveUnitTest (AserveComs & _coms, String unitTestName, String fn) : Thread("Unit Tester"), coms(_coms), testName(unitTestName), folderName(fn)
{
    timeout = 0;
    currentState = eNone;
    defaultTimeOutInc = 100;
    coms.setUniTestPtr(this);
}

AserveUnitTest::~AserveUnitTest ()
{
    coms.setUniTestPtr(nullptr);
}

//Thread will call this, then call runningUnitTest

void AserveUnitTest::startUnitTest (const int to)
{
    timeout = to;
    errorMessages = "";
    simpleMutex.set(0);
    messageQue.clear();
    
    currentState = eRunning;
    startThread(3); //lowish priority.
    
}
void AserveUnitTest::run()
{
    while (!threadShouldExit() && timeout > 0 && currentState == eRunning) {
        runningUnitTest();
        Thread::sleep(defaultTimeOutInc);
        timeout -= defaultTimeOutInc;
    }
    if (timeout < 0) {
        errorMessages += TEST_TIMEOUT;
    }
    sendActionMessage("finished");

}

void AserveUnitTest::testMessageReceived (String message, std::vector<String> params)

//void AserveUnitTest::testOscMessageReceived (const OSCMessage& message)
{
    
    String decodedMsg = "";
    
    decodedMsg = message;
    for (String v : params) {
        decodedMsg += "|" + String(v);
    }
    
    /*
     Decode message then lock the thread and add  message to que so running thread can pull out info...
     */
    
    while (simpleMutex.get()) { //will keep trying while message is locked by other operation
        
    }
    simpleMutex.set(true);
    messageQue.add(decodedMsg);
    simpleMutex.set(false);
}

StringArray AserveUnitTest::getAndClearMessageLog ()
{
    while (simpleMutex.get()) { //will keep trying while message is locked by other operation
        
    }
    simpleMutex.set(true);
    StringArray messages = messageQue;
    messageQue.clear();
    
    
    simpleMutex.set(false);
    return messages;
}

StringArray AserveUnitTest::getCue (int timeout, int messagesToWaitFor, const int lagInMs)
{
    
    StringArray tt;
    
    if (threadShouldExit()) {
        return tt;
    }
    
    int counter = 0;
    const bool expectsMessages = messagesToWaitFor > 0;
    
    auto ending = [&]() -> bool {
        if (expectsMessages) {
            return tt.size() < messagesToWaitFor && counter < timeout;
        }
        else {
            return counter < timeout;
        }
    };
    
    do {
        tt.addArray(getAndClearMessageLog());
        Thread::sleep(lagInMs);
        counter += lagInMs;
    } while (ending() && !threadShouldExit());
    
    
    
    
    if (!tt.size() && expectsMessages) { //if no messages are expected then this test should simply timeout..
        errorMessages += "No messages received test timed out!\n";
    }
    
    return tt;
}

AserveUnitTest::eTestState AserveUnitTest::getResult ()
{
    return currentState;
}

String AserveUnitTest::getErrors ()
{
    return errorMessages;
}

 File AserveUnitTest::fodlerForTestResults (String name)
{
    File root(solutionsPath);
    
    StringArray list = getTestList();
    const int index  = list.indexOf(name);
    if (index >= 0) {
        String name = "unit test " + String(index+1);
        File fFolder = root.getChildFile(name);
        return fFolder;
    }
    
    return File("");
}

void AserveUnitTest::saveToFile ()
{
    File root(solutionsPath);
    File fFolder = root.getChildFile(folderName);
    if (!fFolder.exists() ) {
        fFolder.createDirectory();
    }
    else if (fFolder.isDirectory()) {
        //
        
        File codeFolder = File(projectPath);
        File cpp = codeFolder.getChildFile("IAP.cpp");
        File h = codeFolder.getChildFile("IAP.h");
        if (cpp.exists() && h.exists()) {
            
            String prePend = (getResult() == eEndedPass) ? "Pass_" : "Failed_";
            
            File hCopy = fFolder.getChildFile(prePend + "IAP.h");
            File cppCopy = fFolder.getChildFile(prePend + "IAP.cpp");
            if (hCopy.exists()) {
                hCopy.deleteFile();
            }
            if (cppCopy.exists()) {
                cppCopy.deleteFile();
            }
            h.copyFileTo(hCopy);
            cpp.copyFileTo(cppCopy);
        }
    }
}


/*static*/ StringArray AserveUnitTest::getTestList ()
{
    StringArray array;
    for (int i = 0; i < AUT::nbrOfTests; i++) {
        array.add(AUT::unitTestNames[i]);
    }
    return array;
}

AserveUnitTest * AserveUnitTest::allocateForTest (String t, AserveComs & coms)
{
    StringArray list = getTestList();
    const int index  = list.indexOf(t);
    switch (index) {
        case 0:
            return new AUTMtof(coms);
        case 1:
            return new AUTDrumSampler(coms);
        case 2:
            return new AUTChordMachine(coms);
        case 3:
            return new AUTNoteoff(coms);
        case 4:
            return new AUTScaleQuantiser(coms);
        case 5:
            return new AUTHighFreqTest(coms);
        case 6:
            return new AUTVectorSequencer(coms);
        case 7:
            return new AUTFileRead(coms);
        case 8:
            return new AUTFileWrite(coms);
        case 9:
            return new AUTAlgorithms(coms);
        default:
            break;
    }
    return nullptr;
}

/*static*/ AserveUnitTest::eTestState AserveUnitTest::getStateForTest (String t)
{
    File f = fodlerForTestResults(t);
    if (f.exists() && f.isDirectory()) {
        if (f.getChildFile("Pass_IAP.cpp").exists()) {
            return eTestState::eEndedPass;
        }
        else if (f.getChildFile("Failed_IAP.cpp").exists()) {
            return eTestState::eEndedFail;
            
        }
    }
    return eTestState::eNone;
}
