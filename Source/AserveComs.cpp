//
//  AserveComs.cpp
//  Aserve 2
//
//  Created by Samuel Hunt on 03/08/2018.
//
//

#include "AserveComs.h"
#include "AserveUnitTest.hpp"

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
    static const String pixelGridClicked = aserve + "clickedpixelgrid";
    static const String loadsample = aserve + "loadsample";
    static const String loadPitchedSample = aserve + "loadpitchedsample";
    static const String lpf = aserve + "lpf";
    static const String hpf = aserve + "hpf";
    static const String bpf = aserve + "bpf";
    static const String brf = aserve + "brf";
    
    static const String loadDefaultSounds = aserve + "loaddefaults";
    static const String reset = aserve + "reset";

    static const String mode = aserve + "mode";
    
    static const String pan = aserve + "pan";
    
    static const String fPath = aserve + "path";
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
    logEnabled.set(true);
    unitTest = nullptr;
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
        sender.send(message);
    }
    else if (midiMessage.getRawDataSize() == 2) {
        OSCMessage message(AserveOSC::MIDI, midiMessage.getRawData()[0], midiMessage.getRawData()[1], 0);
        sender.send(message);
    }

    //plus we allways send MIDI for the more advance stuff..
    
    if (unitTest != nullptr) {
        addMessageToLog(String("Unit Test -> Send: ") + midiMessage.getDescription());
    }
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
void AserveComs:: enableLoggger (bool state)
{
    if (!state) {
        addMessageToLog("Message Logging Disabled");
    }
    logEnabled.set(state);
    if (state) {
        addMessageToLog("Message Logging Enabled");
    }
}

void AserveComs::addMessageToLog (String message)
{
    if (logEnabled.get()) {
        while (isMessageLocked.get()) { //will keep trying while message is locked by other operation
            
        }
        isMessageLocked.set(true);
        if (messageLog.size() < 30) {
            messageLog.add(message);
            
            
        }
        if (messageLog.size() == 30 && unitTest != nullptr) {
            messageLog.add("ERROR! Message log overloaded!");
        }
        
        isMessageLocked.set(false);

    }
}


void AserveComs::oscMessageReceived (const OSCMessage& message)
{
    redrawNeeded.set(true);
    //std::cout << message.getAddressPattern().toString() << "\n";
    
    //this could probably be refactored with string arrays etc..
    
    
    String errorA = "";
    String errorB = "";
    String errorC = "";
    String errorD = "";
    
    if (message.getAddressPattern().toString().startsWith(AserveOSC::oscilator)) {
        if (message.size() == 4) {
            //we have to do loads of error checking, dont want asserts being thrown...
            if (message[0].isInt32() && message[1].isFloat32() && message[2].isFloat32() && message[3].isInt32()) {
                const int c = message[0].getInt32();
                const float f = message[1].getFloat32();
                const float a = message[2].getFloat32();
                const float w = message[3].getInt32();

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
                if (unitTest) {
                    unitTest->testMessageReceived("osc", {String(c),String(f),String(a),String(w)});
                }
                
                // send to 
            }
            
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::loadsample)) {
        
        if (message.size() == 2) {
            if (message[0].isInt32() && message[1].isString()) {
                
                

                
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
                
            }
        }
        
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::loadPitchedSample)) {

        if (message.size() == 5) {
            if (message[0].isInt32() && message[1].isString() && message[2].isInt32()
                && message[3].isFloat32() && message[4].isFloat32()) {
 
                const int index = message[0].getInt32();
                const String path = message[1].getString();
                
                if (index >= 0 && index < AudioMain::eMaxSamplerTracks) {
                    
                    File f(path);
                    if (f.existsAsFile()) {
                        audio.setResampleSynthSound(index, path, message[2].getInt32(),
                                                    message[3].getFloat32(), message[4].getFloat32());
                    }
                    else {
                        errorB = "ERROR! - " + path + " - Does not exist as a file!";
                    }
                }
                else {
                    errorA = "ERROR! - " + String(index) + " - Is an out of range channel value!";
                }
                
                //note we wont check the other 3 parameters which are original pitch, attach, decay. These should simply produce interesting effects for the students.
                
                String log = "aserveLoadPitchedSample(" + String(index) + ", " + path + ", " + String(message[2].getInt32()) + ", " + String(message[3].getFloat32()) + ", " + String(message[4].getFloat32()) + ");";
                
                addMessageToLog(log);
                
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
                
                if (index >= 0 && index < AudioMain::eMaxSamplerTracks) {
                    
                    if (note > 127 || note < 0 || amp < 0.0 || amp > 1.0) {
                        errorA = "ERROR! Sampler values out of range!";
                    }
                    else {
                        audio.triggerSampledNote(index, note, amp * 127.0);
                    }
                }
                else {
                    errorB = "ERROR! Sampler: " + String(index) + " is not a valid sampler index";
                }
                String message = "aservePlayPitchedSample(" + String(index) + ", " + String(note) + ", " + String(amp) + ");";
                addMessageToLog(message);


            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::sample)) {
        if (message.size() == 2) {
            if (message[0].isInt32() && message[1].isFloat32()) {
                
                const int index = message[0].getInt32();
                const float amp = message[1].getFloat32();
                
                
                if (index >= 0 && index < AudioMain::eMaxFileTracks) {
                    
                    if (amp < 0.0 || amp > 1.0) {
                        errorA = "ERROR! Sampler values out of range!";
                    }
                    else {
                        audio.playFile(index, amp);
                    }
                }
                else {
                    errorB = "ERROR! Sampler: " + String(index) + " is not a valid sampler index";
                }
                
                
                String message = "aservePlaySample(" + String(index) + ", " + String(amp) +  ");";
                addMessageToLog(message);
                
                if (unitTest) {
                    unitTest->testMessageReceived("playSample", {String(index),String(amp)});
                }
            }

        }
    }

    else if (message.getAddressPattern().toString().startsWith(AserveOSC::lpf)) {
        if (message.size() == 1) {
            if (message[0].isFloat32()) {
                const float cuttoff = message[0].getFloat32();
                if (cuttoff >= 20 && cuttoff < 22050) {
                    audio.setLPF(cuttoff);
                }
                else {
                    errorA = "ERROR! LPF cuttoff out of range: " + String(cuttoff);
                }
                String message = "aserveLPF(" + String(cuttoff) + ");";
                addMessageToLog(message);
            }
        }

    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::hpf)) {
        if (message.size() == 1) {
            if (message[0].isFloat32()) {
                const float cuttoff = message[0].getFloat32();
                if (cuttoff >= 20 && cuttoff < 22050) {
                    audio.setHPF(cuttoff);
                }
                else {
                    errorA = "ERROR! HPF cuttoff out of range: " + String(cuttoff);
                }
                String message = "aserveHPF(" + String(cuttoff) + ");";
                addMessageToLog(message);
            }
        }
        
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::bpf)) {
        if (message.size() == 3) {
            if (message[0].isFloat32() && message[1].isFloat32() && message[2].isFloat32()) {
                
                const float cuttoff = message[0].getFloat32();
                const float q = message[1].getFloat32();
                const float gain = message[2].getFloat32();
                
                if (cuttoff >= 20 && cuttoff < 22050 && q >= 0) {
                    audio.setHighBand(message[0].getFloat32(), message[1].getFloat32(), message[2].getFloat32());
                }
                else {
                    errorA = "ERROR! BPF values out of range: " + String(cuttoff);

                }
                String message = "aserveBPF(" + String(cuttoff) + ", " + String(q) + ", " + String(gain) + ");";
                addMessageToLog(message);
                

            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::brf)) {
        if (message.size() == 3) {
            if (message[0].isFloat32() && message[1].isFloat32() && message[2].isFloat32()) {

                const float cuttoff = message[0].getFloat32();
                const float q = message[1].getFloat32();
                const float gain = message[2].getFloat32();
                
                if (cuttoff >= 20 && cuttoff < 22050 && q >= 0) {
//                    audio.setHighBand(message[0].getFloat32(), message[1].getFloat32(), message[2].getFloat32());
                }
                else {
                    errorA = "ERROR! PRF values out of range: " + String(cuttoff);
                    
                }
                String message = "aserveBRF(" + String(cuttoff) + ", " + String(q) + ", " + String(gain) + ");";
                addMessageToLog(message);
                
                
            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::reset)) {
        reset();
//        addMessageToLog("aserveReset()");
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::loadDefaultSounds)) {
        
        File froot = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile ).getChildFile("Contents").getChildFile("Resources");
        

        audio.loadFile(0, froot.getChildFile("bd.wav").getFullPathName());
        audio.loadFile(1, froot.getChildFile("sd.wav").getFullPathName());
        audio.loadFile(2, froot.getChildFile("chh.wav").getFullPathName());
        audio.loadFile(3, froot.getChildFile("ohh.wav").getFullPathName());
        
        //double attack, double release)
        audio.setResampleSynthSound(0, froot.getChildFile("pianoSample.wav").getFullPathName(), 60, 0.01, 0.3);
        
        addMessageToLog("aserveLoadDefaultSounds();");
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::MIDI)) {
        if (message.size() == 3) {
            if (message[0].isInt32() && message[1].isInt32() && message[2].isInt32()) {
                const int s = message[0].getInt32();
                const int d1 = message[1].getInt32();
                const int d2 = message[2].getInt32();
                //send midi
                const MidiMessage m(s, d1, d2);
                sendActionMessage("MIDI:" + String(s) + "," + String(d1) + "," + String(d2));
                
            }

        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::setPixelGrid)) {
        if (message.size() == 2) {
            if (message[0].isInt32() && message[1].isInt32()) {
                const int a = message[0].getInt32();
                const int b = message[1].getInt32();
                sendActionMessage("PIXEL:" + String(a) + "," + String(b));

            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::mode)) {
        if (message.size() == 1) {
            if (message[0].isInt32()) {
                const int mode = message[0].getInt32();
                if (mode >= OscillatorManager::eOscillatorMode::eNormal) {
                    switch (mode) {
                        case OscillatorManager::eOscillatorMode::eNormal:
                            audio.getOscs().setOscillatorRoutingMode(OscillatorManager::eOscillatorMode::eNormal);
                            break;
                        case OscillatorManager::eOscillatorMode::eFm8:
                            audio.getOscs().setOscillatorRoutingMode(OscillatorManager::eOscillatorMode::eFm8);
                            break;
                        default:
                            break;
                    }
                }
                else {
                    errorA = "ERROR! Incorrect Mode Operation: " + String(mode);
                }
                String message = "aserveOscillatorMode(" + String(mode) + ");";
                addMessageToLog(message);
            }
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::pan)) {
        if (message.size() == 3) {
            //we have to do loads of error checking, dont want asserts being thrown...
            if (message[0].isInt32() && message[1].isFloat32() && message[2].isFloat32()) {
                const int c = message[0].getInt32();
                const float l = message[1].getFloat32();
                const float r = message[2].getFloat32();
            
                
                if (c >= 0 && c < OscillatorManager::NumOscillators) {
                    audio.getOscs().setPanning(c, l, r);

                    if (fabs(l) > 1.0 || fabs(r) > 1.0) {
                        errorA = "ERROR! PAN: " + String(c) + " configured with out of range values";
                    }
                }
                else {
                    errorB = "ERROR! PAN: " + String(c) + " is not a valid oscillator index";
                }
                String message = "aservePanOscillator(" + String(c) + ", " + String(l) + ", " + String(r) + ");";
                addMessageToLog(message);
            }
            
        }
    }
    else if (message.getAddressPattern().toString().startsWith(AserveOSC::fPath)) {
        if (message.size() == 1) {
            if (message[0].isString()) {
                File fPath = message[0].getString();
                if (fPath.exists()) {
                    AserveUnitTest::projectPath = fPath.getFullPathName();
                    File fSol = fPath.getParentDirectory().getParentDirectory();
                    fSol = fSol.getChildFile("Solutions");
                    fSol = fSol.getChildFile("Unit Tests");
                    if (fPath.exists()) {
                        AserveUnitTest::solutionsPath = fSol.getFullPathName();
                    }
                    
                }
            }
        }
    }

    if (errorD.isNotEmpty()) {
        addMessageToLog(errorD);
    }
    if (errorC.isNotEmpty()) {
        addMessageToLog(errorC);
    }
    if (errorB.isNotEmpty()) {
        addMessageToLog(errorB);
    }
    if (errorA.isNotEmpty()) {
        addMessageToLog(errorA);
    }
    

    
}

void AserveComs::sendGridMessage (const int x, const int y)
{
    OSCMessage message(AserveOSC::pixelGridClicked, x, y);
    sender.send(message);
}

void AserveComs::setUniTestPtr (AserveUnitTest * test)
{
    unitTest = test;
}

void AserveComs::reset ()
{
    audio.reset();
}

void AserveComs::addUnitTestMessageToLog (String message)
{
    addMessageToLog(String("Unit Test -> Log: ") + message);

}
