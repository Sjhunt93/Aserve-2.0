/*
 *  SliderControlComponent.h
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_SLIDERCONTROLCOMPONENT
#define H_SLIDERCONTROLCOMPONENT

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiManager.h"

class SliderControlComponent  :	public Component,
	public Slider::Listener
{
public:
	//==============================================================================
	SliderControlComponent(MidiManager& m);
	~SliderControlComponent();
	//==============================================================================
	// called when the close button is pressed or esc is pushed
	void resized();
	void sliderValueChanged (Slider* slider);

	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);
private:
    MidiManager& midiManager;
	const int NumberOfSliders = 9;
	OwnedArray <Slider> sliders;
};



#endif //H_SLIDERCONTROLCOMPONENT