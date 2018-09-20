/*
 *  SliderControlComponent.cpp
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SliderControlComponent.h"


SliderControlComponent::SliderControlComponent (MidiManager& m) : midiManager(m) 
{
	for (int i = 0; i < NumberOfSliders; i++)
	{
		sliders.add (new Slider (String (41 + i)));
		sliders[i]->setSliderStyle (Slider::LinearVertical);
		sliders[i]->setRange (0, 127, 1);
		sliders[i]->addListener (this);
		sliders[i]->setColour (Slider::trackColourId, Colours::black);
		sliders[i]->setColour (Slider::thumbColourId, Colours::darkgrey);
        sliders[i]->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
		addAndMakeVisible (sliders[i]);
        
        sliders[i]->setTooltip (sliders.getLast()->getName());
	}
}

SliderControlComponent::~SliderControlComponent()
{

}

//==============================================================================
// called when the close button is pressed or esc is pushed
void SliderControlComponent::resized()
{
    Rectangle<int> r (getLocalBounds());
	float sliderWidth = getWidth() / static_cast<float> (NumberOfSliders);
    for (Slider* s : sliders)
	{
		s->setBounds (r.removeFromLeft (sliderWidth).reduced (1, 3));
	}
}

void SliderControlComponent::sliderValueChanged (Slider* slider)
{
    MidiMessage m (MidiMessage::controllerEvent(1, slider->getName().getIntValue(), slider->getValue()));
    midiManager.sendMessage (m);
}

void SliderControlComponent::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    const MessageManagerLock lock; //this is called from MIDI thread..
    if (message.isController()) {
        for (int i = 0; i < NumberOfSliders; i++) {
            if (sliders[i]->getName().getIntValue() == message.getControllerNumber()) {
                sliders[i]->setValue(message.getControllerValue(), dontSendNotification);
            }
        }
        
    }
}