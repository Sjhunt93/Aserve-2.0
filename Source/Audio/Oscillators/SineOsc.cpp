//
//  SineOsc.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#include "SineOsc.hpp"
//-------------------------------------------
//Sinusoidal Oscilator - Not Necessary
SineOscillator::SineOscillator()
:
currentAngle (0.0),
angleDelta (0.0)
{
    
}
SineOscillator::~SineOscillator()
{
    
}



void SineOscillator :: reset()
{
    amplitude = 0.0;
    angleDelta = 0.0;
}

void SineOscillator::setFrequency(const double val)
{
    frequency = val;
    angleDelta = 0;
}




double SineOscillator::renderWaveShape(void)
{
    const double sample = sin(currentAngle);
    
    if (angleDelta == 0.0)        //if the frequency ha been changed reset the stepsize
        angleDelta = double_Pi * 2.0 / (sampleRate / frequency);
    
    currentAngle += angleDelta;
    const double twoPi = double_Pi*2.0;
    if(currentAngle >= twoPi)
        currentAngle -= twoPi;
    
    return sample;
}

