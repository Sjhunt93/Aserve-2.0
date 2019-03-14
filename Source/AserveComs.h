//
//  AserveComs.h
//  Aserve 2
//
//  Created by Samuel Hunt on 03/08/2018.
//
//

#ifndef __Aserve_2__AserveComs__
#define __Aserve_2__AserveComs__


#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioMain.h"

class AserveUnitTest;

class AserveComs :
private OSCReceiver,
private OSCReceiver::Listener<OSCReceiver::RealtimeCallback>,
public ActionBroadcaster
{
public:
    AserveComs (AudioMain &audio);
    virtual ~AserveComs ();
    
    void sendMidiMessageFromImpulse (MidiMessage message);
    void sendGridMessage (const int x, const int y);
    
    /*
     Call this to check to see if any messages need logging
     'thread safe'
     */
    bool checkAndClearRedraw ();
    /*
     Will clear the interal logger and return any messages recived
     'thread safe'
     */
    StringArray getAndClearMessageLog ();
    
    /*
     Disables log messages - good for high performance
     */
    void enableLoggger (bool state);
    
    void setUniTestPtr (AserveUnitTest * test);
private:
    void oscMessageReceived (const OSCMessage& message) override;
    void sendOsc (const int channel, const float frequency, const float amplitude, const int wavetype);
    void sendMIDI (Byte s, Byte d1, Byte d2);
    
    OSCSender sender;
    AudioMain &audio;
    
    StringArray messageLog;
    
    void addMessageToLog (String message);
    
    /* need to be thread safe */
    Atomic<int>  isMessageLocked;
    Atomic<int>  redrawNeeded; //this is set when a message is recived.. its later checked so that visuals can be updated etc..
    Atomic<int>  logEnabled;
    
    AserveUnitTest * unitTest;
    
};
#endif /* defined(__Aserve_2__AserveComs__) */
