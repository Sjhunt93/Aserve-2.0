//
//  AserveUnitTest.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#include "AserveUnitTest.hpp"


const String AserveUnitTest::TEST_TIMEOUT = "Test Timeout - please check your code for excessive sleeps.\n";

AserveUnitTest::AserveUnitTest (AserveComs & _coms, String unitTestName) : Thread("Unit Tester"), coms(_coms), testName(unitTestName)
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
