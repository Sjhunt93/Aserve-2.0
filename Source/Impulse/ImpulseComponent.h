//
//  ImpulseComponent.h
//  Aserve 2
//
//  Created by Samuel Hunt on 20/07/2018.
//
//

#ifndef __Aserve_2__ImpulseComponent__
#define __Aserve_2__ImpulseComponent__


#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiManager.h"
#include "PitchModComponent.h"
#include "SliderControlComponent.h"
#include "ProgButtonComponent.h"
#include "RotaryControlComponent.h"
#include "TransportButtonComponent.h"
#include "DrumPadComponent.h"

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class ImpulseController   :  public Component,
public MidiKeyboardStateListener
{
public:
    //==============================================================================
    ImpulseController (AserveComs& aComs);
    ~ImpulseController();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;

    
    //Handle messages from real aserve..
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);
    
private:
    TooltipWindow tooltipWindow;
    MidiManager midiManager;
    
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboard;
    
    //
    PitchModComponent pitchModComponent;
    SliderControlComponent sliderControlComponent;
    ProgButtonComponent progButtonComponent;
    RotaryControlComponent rotaryControlComponent;
    TransportButtonComponent transportButtonComponent;
    DrumPadComponent drumPadComponent;
    

    
    //==============================================================================
  };
#endif /* defined(__Aserve_2__ImpulseComponent__) */
