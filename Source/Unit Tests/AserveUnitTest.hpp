//
//  AserveUnitTest.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 07/03/2019.
//

#ifndef AserveUnitTest_hpp
#define AserveUnitTest_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AserveComs.h"



namespace AUT
{
    static const int nbrOfTests = 10;
    static const char * unitTestNames [nbrOfTests] = {
        "mtof", "Drum Sampler", "Chord Machine", "Fix noteoff", "Scale Quantisation", "HF test", "Music Sequencer", "Reading from a file", "Writing to a file", "Sorting"
    };
}


class AserveUnitTest : public Thread, public ActionBroadcaster {
public:
    
    static const String TEST_TIMEOUT;
    
    enum eTestState {
        eNone = 0,
        eRunning,
        eEndedPass,
        eEndedFail,
    };
    
    struct Test {
        String name;
        String folder;
        int state;
    };
    
    //Note the unit test will register itself with aserveComs, then disable itself after exiting.
    AserveUnitTest (AserveComs & coms, String unitTestName, String folderName);
    virtual ~AserveUnitTest ();
    
    //Thread will call this, then call runningUnitTest

    void startUnitTest (const int timeout); //how long will the test wait before aborting..
    void run();
    
    void testOscMessageReceived (const OSCMessage& message);
    void testMessageReceived (String message, std::vector<String> params);

    //Will run unit test.
    virtual void runningUnitTest () = 0;
    
    eTestState getResult ();
    String getErrors ();
    
    StringArray getAndClearMessageLog ();
    StringArray getCue (int timeout, int messagesToWaitFor, const int lagInMs = 10); //wait this long for a message then return with what ever is recieved
    
    void saveToFile ();
    static File fodlerForTestResults (String name);

    static String solutionsPath;
    static String projectPath;

    static StringArray getTestList ();
    static AserveUnitTest * allocateForTest (String t, AserveComs & coms); //will return nullptr
    static eTestState getStateForTest (String t);
    
    
    
protected:
    AserveComs & coms;
    const String testName;
    String errorMessages;
    
    int timeout;
    eTestState currentState;
    int defaultTimeOutInc;
    String folderName;
private:
    StringArray messageQue;
    Atomic<int> simpleMutex; //used a simple thread safe thingy..
    

};

#endif /* AserveUnitTest_hpp */
