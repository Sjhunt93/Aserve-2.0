//
//  Oscillator.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 23/10/2019.
//

#ifndef Oscillator_hpp
#define Oscillator_hpp

#include "../JuceLibraryCode/JuceHeader.h"


class Oscillator
{
public:
    Oscillator ();
    virtual ~Oscillator ();
    virtual void reset (void);
    virtual void setFrequency (const double val);
    void setAmplitude (const float val);
    virtual void prepare (double sampleRate_);
    void stop (void);
    double nextSample (void);
    virtual double renderWaveShape (void) = 0;
    double getFrequency ();
    String getStates ();
    
    void setAttack (const double val);
    void setRelease (const double val);
protected:
    double frequency, amplitude, sampleRate, tailOff, tailOn; //used by derrived classes
    double attackT, releaseT; //by defaults this are fixed but can be hacked by a register..
};

#endif /* Oscillator_hpp */
