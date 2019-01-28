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

#ifdef JUCE_WINDOWS
typedef unsigned char Byte;
#endif

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
    
    bool checkAndClearRedraw ();
    StringArray getAndClearMessageLog ();
    
    void enableLoggger (bool state);
private:
    void oscMessageReceived (const OSCMessage& message) override;
    void sendOsc (const int channel, const float frequency, const float amplitude, const int wavetype);
    void sendMIDI (Byte s, Byte d1, Byte d2);
    
    OSCSender sender;
    AudioMain &audio;
    
    StringArray messageLog;
    
    void addMessageToLog (String message);
    
    Atomic<int>  isMessageLocked;
    Atomic<int>  redrawNeeded; //this is set when a message is recived.. its later checked so that visuals can be updated etc..
    Atomic<int>  logEnabled;
    
};
#endif /* defined(__Aserve_2__AserveComs__) */
