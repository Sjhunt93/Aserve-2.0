//
//  Oscillator.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 23/10/2019.
//

#include "Oscillator.hpp"


//-------------------------------------------
// Oscillator base class
Oscillator::Oscillator() :
frequency (1000.0),
amplitude (0.0f),
sampleRate (44100.0),
tailOff(0.0)
{
    attackT     = 0.0001;
    releaseT    = 0.0001;
}
Oscillator::~Oscillator()
{
    
}

void Oscillator::prepare(double sampleRate_)
{
    sampleRate = sampleRate_;
    this->reset();
}

void Oscillator :: reset()
{
    amplitude = 0.0;        //must reset amplitude to prevent rendering when == 0
    tailOn = 1.0;
}

void Oscillator::setFrequency(const double val)
{
    frequency = val;
}

void Oscillator::setAmplitude(const float val)
{
    tailOff = 0.0;
    if (val == 0.0) {
        stop();
    }
    else {
        if (amplitude == 0.0) {
            tailOn = 1.0;
        }
        amplitude = val;
        
    }
    
}

//use this to fade out setAmplitude(0) for instintanious holdup!
void Oscillator::stop(void)
{
    if (tailOff == 0.0)//only need to begin a tailoff if it has not already begun
        tailOff = 1.0;
}


double Oscillator::nextSample(void)
{
    double sample = 0.0;
    
    
    if(amplitude > 0.0) {//don't calculate anything unless necessary

        
        if (tailOff > 0) {          //if fading out incorporate the fade
        
            sample = amplitude * (float) renderWaveShape() * tailOff;
            tailOff -= releaseT;
            
            if (tailOff <= 0.001) {
                this->reset();
            }
        }
        else {
            tailOn -= attackT;
            if (tailOn <= 0.005) {
                tailOn = 0.0;
            }
            float a = 1.0 - tailOn;
            sample = amplitude * a * (float) renderWaveShape();
        }
    }
    return sample;
}

double Oscillator::getFrequency ()
{
    return frequency;
}

String Oscillator::getStates ()
{
    return String(frequency) + " " + String(amplitude);
}

void Oscillator::setAttack (const double val)
{
    
    
    attackT = fabs(val) >= 1.0 ? 1.0 : fabs(val);
    
}
void Oscillator::setRelease (const double val)
{
    releaseT = fabs(val) >= 1.0 ? 1.0 : fabs(val);
    
}
