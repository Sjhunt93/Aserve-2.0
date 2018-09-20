//
//  ImpulseComponent.cpp
//  Aserve 2
//
//  Created by Samuel Hunt on 20/07/2018.
//
//

#include "ImpulseComponent.h"

//==============================================================================
ImpulseController::ImpulseController (MidiManager& m)
:   midiManager (m),
keyboard (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
pitchModComponent (m),
sliderControlComponent (m),
progButtonComponent (m),
rotaryControlComponent (m),
transportButtonComponent (m),
drumPadComponent (m)

{
    keyboard.setLowestVisibleKey (36);
    keyboard.setKeyWidth (24);
    keyboard.setOctaveForMiddleC (4);
    keyboard.setKeyPressBaseOctave (5);
    addAndMakeVisible (keyboard);
    
    keyboardState.addListener(this);
    
    addAndMakeVisible (pitchModComponent);
    addAndMakeVisible (sliderControlComponent);
    addAndMakeVisible (progButtonComponent);
    addAndMakeVisible (rotaryControlComponent);
    addAndMakeVisible (transportButtonComponent);
    addAndMakeVisible (drumPadComponent);
    
    
    connectMidiAll();

}

ImpulseController::~ImpulseController()
{
    clearAllMidiInputs();
}

void ImpulseController::paint (Graphics& g)
{
    g.setColour (Colours::darkred);
    Path p;
    p.startNewSubPath ({30.0f, 0.0f});
    p.lineTo ({0.f, 0.f});
    p.lineTo ({0.f, (float)getHeight()});
    p.lineTo ({10.f, (float)getHeight()});
    p.quadraticTo ({0.f, getHeight() * 0.5f}, {20.0f, 0.0f});
    p.closeSubPath();
    
    g.fillPath (p);
    p.applyTransform (AffineTransform::scale (-1.f, 1.f, getWidth() * 0.5f, getHeight() * 0.5f));
    g.fillPath (p);
}

void ImpulseController::resized()
{
    Rectangle<int> top (getLocalBounds().reduced (20, 0));
    Rectangle<int> bottom (top.removeFromBottom (getHeight() * 0.5));
    
    Rectangle<int> r (top.removeFromLeft (top.getWidth() * 0.5).reduced (70, 2));
    sliderControlComponent.setBounds(r.removeFromTop (top.getHeight() * 0.7));
    progButtonComponent.setBounds (r);
    
    r = top.removeFromLeft (getWidth() * 0.25).reduced (20, 2);
    rotaryControlComponent.setBounds (r.removeFromTop (top.getHeight() * 0.7));
    transportButtonComponent.setBounds(r);
    
    r = top.reduced (10, 2);
    drumPadComponent.setBounds (r.removeFromTop (top.getHeight() * 0.7).withTrimmedRight (20));
    
    pitchModComponent.setBounds (bottom.removeFromLeft (60).withWidth (55));
    keyboard.setBounds (bottom);
}

void ImpulseController::handleNoteOn (MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
    midiManager.sendMessage (m);
}

void ImpulseController::handleNoteOff (MidiKeyboardState* source,
                    int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage m (MidiMessage::noteOn(midiChannel, midiNoteNumber, 0.0f));
    midiManager.sendMessage (m);
}

void ImpulseController::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
   // std::cout << "Message Recived:: " << message.getDescription() << "\n";
    rotaryControlComponent.handleIncomingMidiMessage(source, message);
    sliderControlComponent.handleIncomingMidiMessage(source, message);
    pitchModComponent.handleIncomingMidiMessage(source, message);
    MidiMessage mm = message;
    midiManager.sendMessage(mm);
}
void ImpulseController::connectMidiAll ()
{
    StringArray newNames = MidiInput::getDevices();
    
    if (midiNames != newNames) {
        midiNames = newNames;
        //Could probably use newer new/delete methods...
        clearAllMidiInputs();
        
        for (int i = 0; i < midiNames.size(); i++) {
            inputs.push_back(MidiInput::openDevice(i, this));
            inputs[i]->start();
        }
        
    }
    
}
void ImpulseController::clearAllMidiInputs ()
{
    for (int i = 0; i < inputs.size(); i++) {
        inputs[i]->stop();
        delete inputs[i];
    }
    inputs.clear();
}