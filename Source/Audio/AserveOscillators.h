/*
 *  AserveOscillators.h
 *
 *  Created by Thomas Mitchell on 25/09/2008.
 *
 */
#ifndef H_ASERVEOSCILLATORS
#define H_ASERVEOSCILLATORS
#include "../../JuceLibraryCode/JuceHeader.h"

#include "Oscillator.hpp"
#include "SineOsc.hpp"
#include "SquareOsc.hpp"
#include "SawOsc.hpp"
#include "TriOsc.hpp"



//noise is so simple it can just stay here...
class Noise : public Oscillator {
public:
    double renderWaveShape(void)
    {
        return (((arc4random() % 2000) * 0.001) - 1.0) * 0.7;
    }
    
};


class WaveOscillator
{
public:
    enum eWaveTypes
    {
        eSinusoid = 0,
        eSquare,
        eSawtooth,
        eInverseSawtooth,
        eTri,
        eNoise,
        eNumWaveforms
    };
    
    WaveOscillator();
    ~WaveOscillator();
    void setWave(int val);
    void setFrequency(const double val);
    const double getFrequency ();
    void setAmplitude(const float val);
    void prepare(double sampleRate_);
    void stop();
    double nextSample();
    
    String getStates ();
    float getLastAmp ();
    /*
     Range 0.0 - 1.0
     */
    void setPan (float left, float right);
    float getLPan ();
    float getRPan ();
    
    void setAttack (float val);
    void setRelease (float val);
    
    bool frequencyOutOfRange;
    bool ampOutOfRange;
    bool waveOutOfRange;
private:
    int wave;                            //sets the current wavesahpe SIN = 0, SQUR = 1, SAW = 2
    SineOscillator      sinOscillator;
    SquareOscillator    squareOscillator;
    SawOscillator 	    sawOscillator;
    InverseSaw      	inverseSaw;
    TriOscillator   	triOscillator;
    Noise           	noiseOscillator;
    std::vector<Oscillator *> oscs;
    float               lastAmp;
    float               lPan, rPan;
};

#endif   // H_ASERVEOSCILLATORS

