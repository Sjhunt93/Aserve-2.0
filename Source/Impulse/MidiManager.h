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


class MidiManager {
public:
    MidiManager(AserveComs & _aserveComs) : aserveComs(_aserveComs)
    {
        
    }
    void sendMessage (MidiMessage& message)
    {
        aserveComs.sendMidiMessageFromImpulse(message);
    }
private:
    AserveComs & aserveComs;

};

#endif  // MIDIMANAGER_H_INCLUDED
