//
//  AserveUnitTest.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#include "AserveUnitTest.hpp"


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

AserveUnitTest::eTestState AserveUnitTest::getResult ()
{
    return currentState;
}

String AserveUnitTest::getErrors ()
{
    return errorMessages;
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
            File hCopy = fFolder.getChildFile("IAP.h");
            File cppCopy = fFolder.getChildFile("IAP.cpp");
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
