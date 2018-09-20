/*
 *  DrumPadComponent.cpp
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DrumPadComponent.h"

DrumPadComponent::DrumPadComponent(MidiManager& m) : midiManager (m)
{
    const int noteMap[] = {67, 69, 71, 72, 60, 62, 64, 65};
    
    for (int i = 0; i < NumberOfButtons; i++)
    {
        buttons.add (new DrumButton (String (noteMap[i])));
        buttons[i]->addListener (this);
        buttons[i]->setTooltip (String (noteMap[i]));
        addAndMakeVisible (buttons[i]);
    }
}

DrumPadComponent::~DrumPadComponent()
{

}

//==============================================================================
// called when the close button is pressed or esc is pushed
void DrumPadComponent::resized()
{
    Rectangle<int> topRow (getLocalBounds());
    Rectangle<int> bottomRow (topRow.removeFromBottom (getHeight() / 2));
    int sliderWidth = getWidth() / 4;
    for (int i = 0; i < NumberOfButtons; i++)
    {
        if (i < 4)
            buttons[i]->setBounds (topRow.removeFromLeft (sliderWidth).reduced (2, 4));
        else
            buttons[i]->setBounds (bottomRow.removeFromLeft (sliderWidth).reduced (2, 4));
    }
}

void DrumPadComponent::buttonClicked (Button* button)
{

}

void DrumPadComponent::buttonStateChanged(Button *button)
{
    int buttonIndex = 0;
    while (buttons[buttonIndex] != button && buttonIndex < NumberOfButtons)
        buttonIndex++;
    DrumButton* buttonPtr = buttons[buttonIndex];
    
    if (buttonPtr != nullptr)
    {
        if (button->getState() == Button::buttonDown || buttonPtr->lastState == Button::buttonDown)
        {
            MidiMessage m (MidiMessage::noteOn (10, buttonPtr->getName().getIntValue(),
                                                (uint8)(button->isDown() ? 127 : 0)));
            midiManager.sendMessage (m);
        }
        buttonPtr->lastState = button->getState();
    }
}