//
//  AserveHeader.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/12/20.
//

#ifndef AserveHeader_hpp
#define AserveHeader_hpp

#include <stdio.h>



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
    
    static const String aSetRegister = aserve + "register";
}

namespace AserveRegisterMap {
    
    //registers start at 0x0
    typedef const int aRegister;
    
    static const int step = OscillatorManager::OscillatorManagerConstants::NumOscillators;
    
    //  --------
    //  address             property                        value range
    //
    //  0x00 - 0x0F         resets and reserved messages    n/a
    //  --------------      Oscillator controls, these are banked sequentially ------------------------------------
    
    //  0x10 - 0x2F         oscillator frequency            0 - sample rate / 2.0
    //  0x30 - 0x4F         oscillator amplitude            0 - 1.0
    //  0x50 - 0x6F         oscillator waveforms            0 - 6
    //  0x70 - 0x8F         oscillator pan                  -1.0 - 1.0 (full left = -1.0, full right = 1.0)
    //  0x90 - 0xAF         oscillator attack               0 - 1.0
    //  0xB0 - 0xCF         oscillator release              0 - 1.0
    
    //  --------------      Filter controls, these are global ------------------------------------

    //  0x100               low pass filter cutoff          20 - sample rate / 2.0
    //  0x101               high pass filter cutoff         20 - sample rate / 2.0
    //  0x102               band pass filter cutoff         20 - sample rate / 2.0
    //  0x103               band pass filter Q              > 0
    //  0x103               band pass filter gain           > 0
    
    //  --------------      Aserve bit grid, these are global ------------------------------------

    
    //  0x200 - 0x2FF       bit grid states                 encoded row by row, = x + y * 16
    
    //  There are 3 sets of registers for setting custom colours on the bit grid visualiser
    //  these are stored in rows and then column
    //  For example to set the red component of x:5, y:7 if would be register 0x275
    //  generic format 0x (R/G/B) (Y) (X)
    
    //  0x300 - 0x3FF       bit grid visualiser colours     red values
    //  0x400 - 0x4FF       bit grid visualiser colours     green values
    //  0x500 - 0x5FF       bit grid visualiser colours     blue values
    
    
    //  ------------------------------------------------------------------------------------------
    
    aRegister masterRest            = 0x0;
    //Oscillators
    aRegister oscillatorPitch       = 0x10;
    aRegister oscillatorAmp         = 0x30;
    aRegister oscillatorWave        = 0x50;
    aRegister oscillatorPan         = 0x70;
    aRegister oscillatorAttack      = 0x90;
    aRegister oscillatorRelease     = 0xB0;
    
    aRegister lpfCutoff             = 0x100;
    aRegister hpfCutoff             = 0x101;
    
    aRegister bpfCutoff             = 0x102;
    aRegister bpfQ                  = 0x103;
    aRegister bpfGain               = 0x104;
    
    //unused
    aRegister brfCutoff             = 0x105;
    aRegister brfQ                  = 0x106;
    aRegister brfGain               = 0x107;
    
    
    aRegister bitGridStates             = 0x200;
    aRegister bitGridRedValues          = 0x300;
    aRegister bitGridGreenValues        = 0x400;
    aRegister bitGridBlueValues         = 0x500;
    
    // not yet added
    // pixel grid
    // mode
    
}

#endif /* AserveHeader_hpp */
