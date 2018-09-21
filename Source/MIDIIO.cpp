//
//  MIDIIO.cpp
//  Aserve 2
//
//  Created by Samuel Hunt on 21/09/2018.
//
//

#include "MIDIIO.h"
#include "ImpulseComponent.h"




MIDIIO::MIDIIO(AserveComs & _aserveComs) : aserveComs(_aserveComs), midiOutput (MidiOutput::createNewDevice("Virtual Impulse"))
{
    midiOutput->startBackgroundThread();
    connectMidiAll();
    impulse = nullptr;
}

void MIDIIO::sendMessage (MidiMessage& message)
{
//    midiOutput->sendBlockOfMessages (MidiBuffer (message), Time::getMillisecondCounter(), 44100);
//    aserveComs.sendMidiMessageFromImpulse(message);
}

void MIDIIO::connectMidiAll ()
{
    StringArray newNames = MidiInput::getDevices();
    
        //Could probably use newer new/delete methods...
        clearAllMidiInputs();
        states.clear();
        midiNames.clear();
        
        
        for (int i = 0; i < newNames.size(); i++) {
            if (newNames[i] != "Virtual Impulse") {
                inputs.push_back(MidiInput::openDevice(i, this));
                inputs[i]->start();
                states.push_back(true);
                midiNames.add(newNames[i]);
            }

        }
    

}
void MIDIIO::clearAllMidiInputs ()
{
    for (int i = 0; i < inputs.size(); i++) {
        inputs[i]->stop();
        delete inputs[i];
    }
    inputs.clear();
}

void MIDIIO::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    for (int i = 0; i < midiNames.size(); i++) {
        if (source->getName() == midiNames[i]) {
            if (states[i]) {
                aserveComs.sendMidiMessageFromImpulse(message);
                break;
            }
        }
    }
    
    if (impulse) {
        impulse->handleIncomingMidiMessage(nullptr, message);
    }
    //need to feed this to virtual impulse
}

StringArray MIDIIO::getMidiNames ()
{
    return midiNames;
}

void MIDIIO::setImpulseComponent (ImpulseController * _impulse)
{
    _impulse = impulse;
}

void MIDIIO::sendMIDI (MidiMessage message)
{
    midiOutput->sendMessageNow(message);
}

bool MIDIIO::getState (int index)
{
    return states[index];
}
void MIDIIO::setState (int index, bool state)
{
    states[index] = state;
}