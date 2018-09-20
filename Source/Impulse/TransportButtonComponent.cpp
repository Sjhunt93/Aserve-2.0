/*
 *  TransportButtonComponent.cpp
 *  Axiom
 *
 *  Created by tjmitche on 06/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TransportButtonComponent.h"

const TransportButton::TransportIcons TransportButton::icons;

TransportButton::TransportButton (ButtonStyle style, int controlNumber)
 :  ShapeButton (String (controlNumber), Colours::white, Colours::grey, Colours::grey.brighter()),
    lastState (Button::buttonNormal)
{
    switch (style)
    {
        case RewindButton:      setUpIconButton (icons.rewindIcon, 4.f , 3.f);        break;
        case FastForwardButton: setUpIconButton (icons.fastForwardIcon, 4.f , 3.f);   break;
        case StopButton:        setUpIconButton (icons.stopIcon, 5.f , 5.f);          break;
        case PlayButton:        setUpIconButton (icons.playIcon, 3.f , 3.f);          break;
        case LoopButton:        setUpIconButton (icons.loopIcon, 1 , 2.f);            break;
        case RecordButton:      setUpIconButton (icons.recordIcon, 5.f , 5.f);        break;
        default:   break;
    }
}

TransportButtonComponent::TransportButtonComponent (MidiManager& m) : midiManager (m)
{
    for (int i = 0; i < NumberOfButtons; i++)
    {
        buttons.add (new TransportButton ((TransportButton::ButtonStyle)i, i + controlValueBase));

        buttons[i]->addListener (this);
        buttons[i]->setTooltip (String (i + controlValueBase));
        buttons[i]->setColour (TextButton::buttonColourId, Colours::black);
        addAndMakeVisible (buttons[i]);
    }
}

TransportButtonComponent::~TransportButtonComponent()
{

}

//==============================================================================
// called when the close button is pressed or esc is pushed
void TransportButtonComponent::resized()
{
    Rectangle<int> r (getLocalBounds());
    const int buttonWidth = getWidth() / NumberOfButtons;
    for (Button* b : buttons)
    {
        b->setBounds(r.removeFromLeft (buttonWidth).reduced (3, 8));
    }
}

void TransportButtonComponent::buttonClicked (Button* button)
{

}

void TransportButtonComponent::buttonStateChanged (Button *button)
{
    const int buttonIndex = button->getName().getIntValue() - controlValueBase;
    TransportButton* buttonPtr = buttons[buttonIndex];

    if (buttonPtr != nullptr)
    {
        if (button->getState() == Button::buttonDown || buttonPtr->lastState == Button::buttonDown)
        {
            MidiMessage m (MidiMessage::controllerEvent (1, buttonIndex + controlValueBase,
                                                         button->isDown() ? 127 : 0));
            midiManager.sendMessage (m);
        }
        buttonPtr->lastState = button->getState();
    }
}