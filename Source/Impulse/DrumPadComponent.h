/*
 *  DrumPadComponent.h
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_DRUMPADCOMPONENT
#define H_DRUMPADCOMPONENT

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiManager.h"

class DrumButton : public Button
{
public:
	DrumButton (const String& buttonName = String::empty)
     :  Button (buttonName),
        lastState (Button::buttonNormal)
	{
		
	}
	void paintButton (Graphics& g,bool isMouseOverButton,bool isButtonDown)
	{
		if(isMouseOverButton) g.setColour (Colours::darkgrey);
		if(isButtonDown) g.setColour (Colours::lightgrey);

		g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 2);
	}
    Button::ButtonState lastState;
};

class DrumPadComponent  :	public Component,
							public ButtonListener
{
public:
	//==============================================================================
	DrumPadComponent(MidiManager& m);
	~DrumPadComponent();
	//==============================================================================
	// called when the close button is pressed or esc is pushed
	void resized();
	void buttonClicked (Button* button);
	void buttonStateChanged(Button *button); 
	
private:
	const int NumberOfButtons = 8;
	int buttonNoteNumbers[8];
	OwnedArray <DrumButton> buttons;
	MidiManager& midiManager;
};



#endif //H_DRUMPADCOMPONENT