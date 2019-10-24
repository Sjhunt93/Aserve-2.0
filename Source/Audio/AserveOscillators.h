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


class SineOscillator : public Oscillator
{
private:
    double currentAngle, angleDelta;
    
public:
    SineOscillator();
    virtual ~SineOscillator();
    void reset(void);
    void setFrequency(const double val);
    double renderWaveShape(void);
};

class SquareOscillator : public Oscillator
{
private:
    void updateHarmonics( void );
    
    unsigned int nHarmonics_, m_;
    double rate_, phase_;
    double p_,a_;
    double lastBlitOutput_, dcbState_, lastOutput_;
    
public:
    SquareOscillator();
    virtual ~SquareOscillator();
    
    void reset(void);
    void prepare(double sampleRate_);
    void setFrequency(const double val);
    double renderWaveShape(void);
};

class SawOscillator : public Oscillator
{
private:
    void updateHarmonics( void );
    
    unsigned int nHarmonics_, m_;
    double rate_, phase_;
    double p_,a_,C2_;
    double state_, lastOutput_;
    
public:
    SawOscillator();
    ~SawOscillator();
    
    void reset(void);
    void setFrequency(const double val);
    void prepare(double sampleRate_);
    double renderWaveShape(void);
};

class InverseSaw : public SawOscillator {
public:
    InverseSaw () : SawOscillator() {}
    double renderWaveShape(void) {
        return SawOscillator::renderWaveShape() * -1;
    }
    
};

class Noise : public Oscillator {
public:
    double renderWaveShape(void)
    {
        return (((arc4random() % 2000) * 0.001) - 1.0) * 0.7;
    }
    
};

class TriOscillator : public Oscillator
{
private:
    void updateHarmonics( void );
    
    
    SineOscillator sineOscs[10];
    
    
public:
    TriOscillator();
    ~TriOscillator();
    
    void reset(void);
    void setFrequency(const double val);
    void prepare(double sampleRate_);
    double renderWaveShape(void);
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

