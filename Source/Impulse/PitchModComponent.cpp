/*
 *  PitchModComponent.cpp
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "PitchModComponent.h"


PitchModComponent::PitchModComponent (MidiManager& m) : midiManager (m)
{
	
	addAndMakeVisible(pitchBender);
	pitchBender.setColour(Slider::trackColourId, Colours::black);
	pitchBender.setColour(Slider::thumbColourId, Colours::darkgrey);
	pitchBender.addListener(this);
	pitchBender.setTooltip("A18");
	addAndMakeVisible(modWheel);
	modWheel.setSliderStyle(Slider::LinearVertical);
	modWheel.setRange(0, 127, 1);
	modWheel.setColour(Slider::trackColourId, Colours::black);
	modWheel.setColour(Slider::thumbColourId, Colours::darkgrey);
	modWheel.addListener(this);
	modWheel.setTooltip("A19");
}

PitchModComponent::~PitchModComponent()
{

}

void PitchModComponent::resized()
{
    Rectangle<int> r (getLocalBounds());
    
	pitchBender.setBounds(r.removeFromLeft (getWidth() * 0.5f));
	modWheel.setBounds(r);
}

void PitchModComponent::sliderValueChanged (Slider* slider)
{
	if (slider == &pitchBender)
	{
        MidiMessage m (MidiMessage::pitchWheel (1, pitchBender.getValue()));
        midiManager.sendMessage (m);
	}
	else if(slider == &modWheel)
	{
        MidiMessage m (MidiMessage::controllerEvent (1, 1, modWheel.getValue()));
        midiManager.sendMessage (m);
	}
}

void PitchModComponent::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    const MessageManagerLock lock; //this is called from MIDI thread..
    if (message.isController()) {
        
        if (message.getControllerNumber() == 1) {
            modWheel.setValue(message.getControllerValue(), dontSendNotification);
        }
    }
    
    if (message.isPitchWheel()) {
        pitchBender.setValue(message.getPitchWheelValue(), dontSendNotification);
    }
}