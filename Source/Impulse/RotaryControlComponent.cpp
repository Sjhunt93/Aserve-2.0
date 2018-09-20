/*
 *  RotControlComponent.cpp
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RotaryControlComponent.h"

RotaryControlComponent::RotaryControlComponent (MidiManager& m) : midiManager(m)
{
    for (int i = 0; i < NumberOfEncoders; i++)
    {
        sliders.add (new Slider (String (21 + i)));
        sliders[i]->setSliderStyle (Slider::Rotary);
        sliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 0.0, 0.0);
        sliders[i]->setRange (0, 127, 1);
        sliders[i]->addListener (this);
        sliders[i]->setColour(Slider::rotarySliderFillColourId, Colours::black);
//        sliders[i]->setColour (Slider::trackColourId, Colours::black);
//        sliders[i]->setColour (Slider::thumbColourId, Colours::darkgrey);
        sliders[i]->setTooltip (String (i));
        addAndMakeVisible (sliders[i]);
        sliders[i]->setTooltip (sliders.getLast()->getName());
    }
}

RotaryControlComponent::~RotaryControlComponent()
{
    
}

//==============================================================================
// called when the close button is pressed or esc is pushed
void RotaryControlComponent::resized()
{
    Rectangle<int> topRow (getLocalBounds());
    Rectangle<int> bottomRow (topRow.removeFromBottom (getHeight() / 2));
    int sliderWidth = getWidth() / 4;
	for (int i = 0; i < NumberOfEncoders; i++)
	{
		if (i < 4)
            sliders[i]->setBounds (topRow.removeFromLeft (sliderWidth).reduced(1, 3));
        else
            sliders[i]->setBounds (bottomRow.removeFromLeft (sliderWidth).reduced(1, 3));
	}
}

void RotaryControlComponent::sliderValueChanged (Slider* slider)
{
    MidiMessage m (MidiMessage::controllerEvent(1, slider->getName().getIntValue(), slider->getValue()));
    midiManager.sendMessage (m);
}

void RotaryControlComponent::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    const MessageManagerLock lock; //this is called from MIDI thread..
    if (message.isController()) {
        for (int i = 0; i < NumberOfEncoders; i++) {
            if (sliders[i]->getName().getIntValue() == message.getControllerNumber()) {
                sliders[i]->setValue(message.getControllerValue(), dontSendNotification);
            }
        }
        
    }
}