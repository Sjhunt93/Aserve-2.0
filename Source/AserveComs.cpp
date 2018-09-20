//
//  AserveComs.cpp
//  Aserve 2
//
//  Created by Samuel Hunt on 03/08/2018.
//
//

#include "AserveComs.h"


namespace AserveOSC
{
    
    static const String aserve = "/aserve/";
    
    static const String note = aserve + "note";
    static const String control = aserve + "control";
    static const String mod = aserve + "modwheel";
    static const String pitchBend = aserve + "pitchbend";
    static const String aftertouch = aserve + "aftertouch";
    static const String pressure = aserve + "channelpressure";
    static const String MIDI = aserve + "midi";
    
    static const String oscilator = "/aserve/osc";
    static const String sample = "/aserve/sample";
    static const String pitchedSample = "/aserve/samplepitch";
    static const String setPixelGrid = "/aserve/pixelgrid";
    static const String loadsample = aserve + "loadsample";
    static const String loadPitchedSample = aserve + "loadpitchedsample";
    static const String lpf = aserve + "lpf";
    static const String bpf = aserve + "bpf";
    static const String brf = aserve + "brf";
    
    static const String loadDefaultSounds = aserve + "loaddefaults";
    static const String reset = aserve + "reset";
}



AserveComs::AserveComs (AudioMain &_audio) : audio(_audio)
{
    if (! connect (9002)) { //listen
        //alert
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error port 9002 already in use", "Please restart aserve.");
        std::cout << "Error opening 9002 for listening \n";
    }
    
    if (! sender.connect ("127.0.0.1", 9001)) {
        //error
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error port 9001 already in use", "Please restart aserve.");
        std::cout << "Error opening 9001 for reciving \n";
    }
    
    // tell the component to listen for OSC messages matching this address:
    addListener (this);
    
    isMessageLocked.set(false);
}
AserveComs::~AserveComs ()
{
    
}

void AserveComs::sendMidiMessageFromImpulse (MidiMessage midiMessage)
{
    if (midiMessage.isNoteOnOrOff()) {
        OSCMessage message(AserveOSC::note);
        message.addArgument(midiMessage.getNoteNumber());
        message.addArgument(midiMessage.getVelocity());
        message.addArgument(midiMessage.getChannel());
        sender.send(message);
    }
    else if (midiMessage.isController()) {
        const int cc = midiMessage.getControllerNumber();
        const int value = midiMessage.getControllerValue();
        if (cc == 1) {
            OSCMessage message(AserveOSC::mod, value);
            sender.send(message);
        }
        else {
            OSCMessage message(AserveOSC::control, cc, value);
            sender.send(message);
        }
    }
    else if (midiMessage.isPitchWheel()) {
        OSCMessage message(AserveOSC::pitchBend, midiMessage.getPitchWheelValue());
        sender.send(message);
    }

    // Also send pure MIDI aswell..
    if (midiMessage.getRawDataSize() == 3) {
        OSCMessage message(AserveOSC::MIDI, midiMessage.getRawData()[0], midiMessage.getRawData()[1], midiMessage.getRawData()[2]);
    }
    else if (midiMessage.getRawDataSize() == 2) {
        OSCMessage message(AserveOSC::MIDI, midiMessage.getRawData()[0], midiMessage.getRawData()[1], 0);
    }

    //plus we allways send MIDI for the more advance stuff..
    
}

bool AserveComs::checkAndClearRedraw ()
{
    int local = redrawNeeded.get();
    redrawNeeded.set(false);
    return (bool) local;
}

StringArray AserveComs::getAndClearMessageLog ()
{
    while (isMessageLocked.get()) { //will keep trying while message is locked by other operation
        
    }
    isMessageLocked.set(true);
    
    StringArray d = messageLog;
    messageLog.clear();
    isMessageLocked.set(false);
    return d;
}

void AserveComs::addMessageToLog (String message)
{
    while (isMessageLocked.get()) { //will keep trying while message is locked by other operation
        
    }
    isMessageLocked.set(true);
    messageLog.add(message);
    isMessageLocked.set(false);
}


void AserveComs::oscMessageReceived (const OSCMessage& message)
{
    redrawNeeded.set(true);
    //std::cout << message.getAddressPattern().toString() << "\n";
    
    //this could probably be refactored with string arrays etc..
    
    if (message.getAddressPattern().toString().startsWith(AserveOSC::oscilator)) {
        if (message.size() == 4) {
            //we have to do loads of error checking, dont want asserts being thrown...
            if (message[0].isInt32() && message[1].isFloat32() && message[2].isFloat32() && message[3].isInt32()) {
                const int c = message[0].getInt32();
                const float f = message[1].getFloat32();
                const float a = message[2].getFloat32();
                const float w = message[3].getInt32();
                String errorA = "";
                String errorB = "";
                if (c >= 0 && c < OscillatorManager::NumOscillators) {
                    audio.getOscs().setFrequency(c, f);
                    audio.getOscs().setAmplitude(c, a);
                    audio.getOscs().setWaveform(c, w);
                    bool range = audio.getOscs().isOutOfRange(c);
                    if (range) {
                        errorA = "ERROR! Oscillator: " + String(c) + " configured with out of range values";
                    }
                }
                else {
                    errorB = "ERROR! Oscillator: " + String(c) + " is not a valid oscillator index";
                }
                String message = "aserveOscillator(" + String(c) + ", " + String(f) + ", " + String(a) + ", " + String(w) + ");";
                addMessageToLog(message);
                if (errorB.isNotEmpty()) {
                    addMessageToLog(errorB);
                }
                if (errorA.isNotEmpty()) {
                    addMessageToLog(errorA);
                }

                

            }
            
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::loadsample)) {
        
        if (message.size() == 2) {
            if (message[0].isInt32() && message[1].isString()) {
                
                
                String errorA = "";
                String errorB = "";
                
                const int index = message[0].getInt32();
                const String path = message[1].getString();
                
                if (index >= 0 && index < AudioMain::eMaxFileTracks) {
                    
                    File f(path);
                    if (f.existsAsFile()) {
                        audio.loadFile(index, path);
                    }
                    else {
                        errorB = "ERROR! - " + path + " - Does not exist as a file!";
                    }
                    
                }
                else {
                    errorA = "ERROR! - " + String(index) + " - Is an out of range channel value!";
                }

                String message = "aserveLoadSample(" + String(index) + ", " + String(path) +  ");";
                addMessageToLog(message);
                if (errorB.isNotEmpty()) {
                    addMessageToLog(errorB);
                }
                if (errorA.isNotEmpty()) {
                    addMessageToLog(errorA);
                }
                
            }
        }
        
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::loadPitchedSample)) {

        if (message.size() == 5) {
            if (message[0].isInt32() && message[1].isString() && message[2].isInt32()
                && message[3].isFloat32() && message[4].isFloat32()) {
                
                audio.setResampleSynthSound(message[0].getInt32(), message[1].getString(), message[2].getInt32(),
                                            message[3].getFloat32(), message[4].getFloat32());
            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::pitchedSample)) {
        if (message.size() == 3) {
            if (message[0].isInt32() && message[1].isInt32() && message[2].isFloat32()) {
                //we have to cast back up to int 0-127 because of the way the synth accepts values..
                const int index = message[0].getInt32();
                const int note = message[1].getInt32();
                const float amp = message[2].getFloat32();
                
                String errorA = "";
                String errorB = "";
                
                

                
                if (index >= 0 && index < AudioMain::eMaxFileTracks) {
                    
                    if (note > 127 || note < 0 || amp < 0.0 || amp > 1.0) {
                        errorB = "ERROR! Sampler values out of range!";
                    }
                    audio.triggerSampledNote(index, note, amp);
                }
                else {
                    errorB = "ERROR! Sampler: " + String(index) + " is not a valid sampler index";
                }
                String message = "aservePlaySample(" + String(index) + ", " + String(note) + ", " + String(amp) + ");";
                addMessageToLog(message);
                if (errorB.isNotEmpty()) {
                    addMessageToLog(errorB);
                }

            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::sample)) {
        if (message.size() == 2) {
            if (message[0].isInt32() && message[1].isFloat32()) {
                int chan = message[0].getInt32();
                float amp = message[1].getFloat32();
                audio.playFile( chan, amp );
                
                
                
                String message = "aservePlaySample(" + String(chan) + ", " + String(amp) +  ");";
                addMessageToLog(message);
            }
            

        }
    }

    else if (message.getAddressPattern().toString().startsWith(AserveOSC::lpf)) {
        if (message.size() == 1) {
            if (message[0].isFloat32()) {
                audio.setLPF(message[0].getFloat32());
            }
        }

    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::bpf)) {
        if (message.size() == 3) {
            if (message[0].isFloat32() && message[1].isFloat32() && message[2].isFloat32()) {
                audio.setHighBand(message[0].getFloat32(), message[1].getFloat32(), message[2].getFloat32());
            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::brf)) {
        if (message.size() == 3) {
            if (message[0].isFloat32() && message[1].isFloat32() && message[2].isFloat32()) {
//                audio.setHighBand(message[0].getFloat32(), message[1].getFloat32(), message[2].getFloat32());
            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::reset)) {
        audio.reset();
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::loadDefaultSounds)) {
        
        File froot = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile ).getChildFile("Contents").getChildFile("Resources");
        

        audio.loadFile(0, froot.getChildFile("bd.wav").getFullPathName());
        audio.loadFile(1, froot.getChildFile("sd.wav").getFullPathName());
        audio.loadFile(2, froot.getChildFile("chh.wav").getFullPathName());
        audio.loadFile(3, froot.getChildFile("ohh.wav").getFullPathName());

    }

    
}

