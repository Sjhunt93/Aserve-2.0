/*
 *  RotControlComponent.h
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_ROTARYCONTROLCOMPONENT
#define H_ROTARYCONTROLCOMPONENT

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiManager.h"

class RotaryControlComponent  :		public Component,
									public SliderListener
{
public:
	//==============================================================================
	RotaryControlComponent(MidiManager& m);
	~RotaryControlComponent();
	//==============================================================================
	// called when the close button is pressed or esc is pushed
	void resized();
	void sliderValueChanged (Slider* slider);
	
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);

    
private:
    MidiManager& midiManager;
	const int NumberOfEncoders = 8;
	OwnedArray <Slider> sliders;
};



#endif //H_ROTARYCONTROLCOMPONENT