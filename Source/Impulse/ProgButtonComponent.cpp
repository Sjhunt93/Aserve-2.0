/*
 *  ProgButtonComponent.cpp
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ProgButtonComponent.h"

ProgButtonComponent::ProgButtonComponent(MidiManager& m) : midiManager (m)
{
	for (int i = 0; i < NumberOfButtons; i++)
	{
		buttons.add (new TextButton (String (controlValueBase + i)));
        buttons[i]->addListener (this);
		buttons[i]->setButtonText (String::empty);
		buttons[i]->setColour (TextButton::buttonColourId, Colours::black);
        buttons[i]->setColour (TextButton::buttonOnColourId, Colours::darkred);
        buttons[i]->setClickingTogglesState (true);
        buttons[i]->setTooltip (String (controlValueBase + i));
        buttons[i]->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
		addAndMakeVisible (buttons[i]);
	}
}

ProgButtonComponent::~ProgButtonComponent()
{

}

//==============================================================================
// called when the close button is pressed or esc is pushed
void ProgButtonComponent::resized()
{
	const float buttonWidth = getWidth() / (float)NumberOfButtons;
    Rectangle<int> r (getLocalBounds());
    for (Button* b : buttons)
		b->setBounds (r.removeFromLeft (buttonWidth).reduced (3, 8));
}

void ProgButtonComponent::buttonClicked (Button* button)
{
	
}

void ProgButtonComponent::buttonStateChanged(Button *button)
{
	if(button->isDown() && button->isOver())
	{
        MidiMessage m (MidiMessage::controllerEvent (1,
                                                     button->getName().getIntValue(),
                                                     127 * (int)!button->getToggleState()));
        midiManager.sendMessage (m);
	}
}