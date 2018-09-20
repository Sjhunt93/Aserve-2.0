/*
  ==============================================================================

    MidiManager.h
    Created: 25 Nov 2016 12:58:41pm
    Author:  Tom Mitchell

  ==============================================================================
*/

#ifndef MIDIMANAGER_H_INCLUDED
#define MIDIMANAGER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AserveComs.h"

/*
 I have kept this class much the same as the original MIDI so that it can be easily re-instated at a later data.
 
 Instead i parse the MIDI to create the OSC messages.
 */

#undef USE_MIDI
//#define USE_OSC

class MidiManager {
public:
    MidiManager(AserveComs & _aserveComs) : aserveComs(_aserveComs)
#ifdef USE_MIDI
    : midiOutput (MidiOutput::createNewDevice("Virtual Impulse\n"))
#endif
    {
#ifdef USE_MIDI
        midiOutput->startBackgroundThread();
#endif
    }
    
    void sendMessage (MidiMessage& message)
    {
#ifdef USE_MIDI
       midiOutput->sendBlockOfMessages (MidiBuffer (message), Time::getMillisecondCounter(), 44100);
#endif
        aserveComs.sendMidiMessageFromImpulse(message);
        
        
    }
    
private:
#ifdef USE_MIDI
    ScopedPointer<MidiOutput> midiOutput;	//this will be our virtual midi input
#endif
    
    AserveComs & aserveComs;

};

#endif  // MIDIMANAGER_H_INCLUDED
