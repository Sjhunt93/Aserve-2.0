/*
 *  AserveOscillators.cpp
 *  jucedemo
 *
 *  Created by Thomas Mitchell on 25/09/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AserveOscillators.h"
#include "../JuceLibraryCode/JuceHeader.h" // this is just for the juce PI and maths stuff

#include <limits>
#include <cfloat>










//------------------------------------------------
//an oscillator class that does it all
//If I were to improvr this I would probaly implement all the rendering 
//within this class such that the frequency, amp, tailoff, etc variables were shared.

WaveOscillator::WaveOscillator() : wave(0)
{
	lastAmp = 0.0;
    ampOutOfRange = frequencyOutOfRange = false;
    lPan = rPan = 1.0;
    oscs.push_back(&sinOscillator);
    oscs.push_back(&squareOscillator);
    oscs.push_back(&sawOscillator);
    oscs.push_back(&inverseSaw);
    oscs.push_back(&triOscillator);
    oscs.push_back(&noiseOscillator);
    
}
	
WaveOscillator::~WaveOscillator()
{
	
}

void WaveOscillator::setWave(int val)
{
    waveOutOfRange = val > eNoise || val < 0;
	
    
    if (val < 0) {
        wave = eSinusoid;
    }
    else if (val > 5) {
        wave = eNoise;
    }
    else {
        wave = val;
    }
}

void WaveOscillator::setFrequency(const double val)
{
    frequencyOutOfRange = val > 44100 / 2 || val < 0;
    if (frequencyOutOfRange) {

    }
    if (std::isinf(val) || std::isnan(val)) {
        return;
    }
    for (Oscillator * osc : oscs) {
        osc->setFrequency(val);
    }
}
const double WaveOscillator::getFrequency ()
{
    return sinOscillator.getFrequency();
}

void WaveOscillator::setAmplitude(const float val)
{
    lastAmp = val;
    ampOutOfRange = lastAmp > 1.0;
    if (lastAmp > 1.0) {
        lastAmp = 1.0;
    }
    
    for (Oscillator * osc : oscs) {
        osc->setAmplitude(lastAmp);
    }
}

void WaveOscillator::prepare(double sampleRate_)
{
    for (Oscillator * osc : oscs) {
        osc->prepare(sampleRate_);
    }
}

void WaveOscillator::stop()
{
    for (Oscillator * osc : oscs) {
        osc->stop();
    }
}

double WaveOscillator::nextSample()
{
    if (wave >= 0 && wave < oscs.size()) {
        return oscs[wave]->nextSample();
    }
    return 0;
    
}


String WaveOscillator::getStates ()
{
    return sinOscillator.getStates() + " " + String(wave);
}
float WaveOscillator::getLastAmp ()
{
    return lastAmp;
}

inline float clip (float a)
{
    return (a > 1.0 ? 1.0 : (a < -1.0 ? -1.0 : a));
    
}

void WaveOscillator::setPan (float left, float right)
{
    lPan = clip(left);
    rPan = clip(right);
}
float WaveOscillator::getLPan ()
{
    return lPan;
}
float WaveOscillator::getRPan ()
{
    return rPan;
}

void WaveOscillator::setAttack (float val)
{
    for (Oscillator * osc : oscs) {
        osc->setAttack(val);
    }
}
void WaveOscillator::setRelease (float val)
{
    for (Oscillator * osc : oscs) {
        osc->setRelease(val);
    }
}
