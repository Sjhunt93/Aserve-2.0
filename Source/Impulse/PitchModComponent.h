/*
 *  PitchModComponent.h
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_PITCHMOD
#define H_PITCHMOD

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MidiManager.h"

class AxiomSlider : public Slider
{
public:
    //==========================================================================
    AxiomSlider(const String& name) : Slider (name)
	{
		setSliderStyle(Slider::LinearVertical);
	}
    
    //==========================================================================
	void paint(Graphics &g)
	{
		g.setColour(Colours::darkgrey);
		g.fillRoundedRectangle(1, 0, getWidth()-2, getHeight(), getWidth()/2);
		g.setColour(Colours::lightgrey);
        g.fillRect(0, getHeight()*(1-((float)getValue()/(getMaximum()-getMinimum())))-getWidth()/2,
                   getWidth(), getWidth());
		//g.fillEllipse(0,
		//			  getHeight()*(1-((float)getValue()/(getMaximum()-getMinimum())))-getWidth()/2,
		//			  getWidth(), getWidth());
		
	}

private:
	
};


class PitchBender : public Slider
{
public:
    //==========================================================================
    PitchBender()
	{
		setSliderStyle (Slider::LinearVertical);
		setRange (0, 16383, 1);
		setValue (8192, dontSendNotification);
	}
    
    //==========================================================================
	
	void mouseUp(const MouseEvent &e)
	{
		setValue (8192, dontSendNotification);
	}
private:
	
};


//==============================================================================
class PitchModComponent  :	public Component,
	public Slider::Listener
{
public:
    //==========================================================================
    PitchModComponent (MidiManager& m);
    ~PitchModComponent();
    //==========================================================================
	void resized();
	void sliderValueChanged (Slider* slider);
    
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);
private:
    MidiManager& midiManager;
	PitchBender pitchBender;
	Slider modWheel;
};


#endif //H_PITCHMOD