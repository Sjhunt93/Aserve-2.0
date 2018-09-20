/*
 *  ProgButtonComponent.h
 *  Axiom
 *
 *  Created by tjmitche on 05/11/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_PROGBUTTONCOMPONENT
#define H_PROGBUTTONCOMPONENT

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiManager.h"

class ProgButtonComponent  :	public Component,
								public ButtonListener
{
public:
	//==============================================================================
	ProgButtonComponent(MidiManager& m);
	~ProgButtonComponent();
	//==============================================================================
	// called when the close button is pressed or esc is pushed
	void resized();
	void buttonClicked (Button* button);
	void buttonStateChanged(Button *button);
private:
	MidiManager& midiManager;
    const int NumberOfButtons = 9;
    const int controlValueBase = 51;
	OwnedArray <TextButton> buttons;
};



#endif //H_PROGBUTTONCOMPONENT