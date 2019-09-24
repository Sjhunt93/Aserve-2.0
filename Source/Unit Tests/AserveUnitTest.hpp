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
        "Midi Notes to Frequencies ", "Drum Sampler", "Chord Machine", "Fix NoteOff (MonoSynth)", "Scale Quantisation", "High Frequency Loop ", "Music Sequencer", "Reading From a File", "Writing To a File", "Sorting"
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
    
    static void waterMarkFile (File f);
    static void waterMarkFile2 (File f);
    static void createHiddenFile (File dir);
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

namespace AserveUnitTestUtilityClass {

    static float getFrequency (String inital)
    {
        const float freq = inital.fromFirstOccurrenceOf("|", false, false).fromFirstOccurrenceOf("|", false, false).upToFirstOccurrenceOf("|", false, false).getFloatValue();
        return freq;

    }
    static float getAmplitude (String inital)
    {
        /// osc | chan | freq | amp |
        const float amp = inital.fromFirstOccurrenceOf("|", false, false).
                                fromFirstOccurrenceOf("|", false, false).
                                fromFirstOccurrenceOf("|", false, false).
                                upToFirstOccurrenceOf("|", false, false).getFloatValue();
        return amp;
        
    }
  
    static int getChannel (String msg)
    {
        return msg.fromFirstOccurrenceOf("|", false, false).upToFirstOccurrenceOf("|", false, false).getIntValue();
    
    }
    
    static float mtof (int note)
    {
        return 440.0 * pow(2.0, (note-69)/12.0);
    }
  
    static bool compareFreq (float f1, float f2)
    {
        return ((int) f1) == ((int) f2);
    }
  
    static std::vector<float> freqFromNoteSeq (std::vector<int> notes)
    {
        std::vector<float> freqs;
        for (int n : notes) {
            freqs.push_back(mtof(n));
        }
        return freqs;
    }
    //formatForExpectedValues
    static String incorrectValueForNoteNumber (int noteNumber, String expected, String recived)
    {
        return "Incorrect value received for note number: " + String(noteNumber) + " value expected: " + expected + " value received: " + recived + "\n";
    }
    static String incorrectNumberOfMessages (String expected, String recived)
    {
        return "Incorrect number of messages received. Expected: " + expected + " received: " + recived + "\n";
    }
    static String incorrectMessageRecived (String expected, String recived)
    {
        return "Incorrect message received. Expected: " + expected + " received: " + recived + "\n";
    }
    
};


#endif /* AserveUnitTest_hpp */
