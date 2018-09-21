//
//  MIDIIO.h
//  Aserve 2
//
//  Created by Samuel Hunt on 21/09/2018.
//
//

#ifndef __Aserve_2__MIDIIO__
#define __Aserve_2__MIDIIO__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AserveComs.h"

class ImpulseController;
class MIDIIO : public MidiInputCallback {
public:
    
    MIDIIO(AserveComs & _aserveComs);
    void sendMessage (MidiMessage& message);
    
    void connectMidiAll ();
    void clearAllMidiInputs ();
    
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);
    
    StringArray getMidiNames ();
    
    void setImpulseComponent (ImpulseController * impulse);
    void sendMIDI (MidiMessage message);
    
    bool getState (int index);
    void setState (int index, bool state);

private:
    AserveComs & aserveComs;
    ScopedPointer<MidiOutput> midiOutput;	//this will be our virtual midi output
    
    StringArray midiNames;
    std::vector<bool> states;
    std::vector<MidiInput *> inputs;
    ImpulseController * impulse;
    
};
#endif /* defined(__Aserve_2__MIDIIO__) */
