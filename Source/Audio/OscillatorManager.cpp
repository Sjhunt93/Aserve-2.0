/*
 *  OscillatorManager.cpp
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscillatorManager.h"

OscillatorManager::OscillatorManager()
{
    for (int i = 0; i < NumOscillators; i++) 
    {
        oscillators.add(new WaveOscillator());
        coreFreq[i] = coreAmp[i] = 0;
    }
    
    oscillatorMode = eNormal;
    
}

OscillatorManager::~OscillatorManager()
{
}

//==============================================================================
void OscillatorManager::setAmplitude (const int oscillatorIndex, const float newAmplitude)
{
    if (oscillatorIndex >= 0 && oscillatorIndex < NumOscillators) {

    coreAmp[oscillatorIndex] = newAmplitude;
    if (oscillatorMode == eFm8) {
        if (oscillatorIndex >= 16) {
            oscillators[oscillatorIndex]->setAmplitude(newAmplitude > 1.0 ? 1.0 : newAmplitude);
            return;
        }
    }
    oscillators[oscillatorIndex]->setAmplitude(newAmplitude);
    }
}

void OscillatorManager::stop (const int oscillatorIndex)
{
    oscillators[oscillatorIndex]->stop();  
}

void OscillatorManager::stopAll (void)
{
    for (int oscillatorIndex = NumOscillators; --oscillatorIndex >= 0;) {
		oscillators[oscillatorIndex]->stop();
    }
}

void OscillatorManager::setFrequency (const int oscillatorIndex, const double newFrequencyHz)
{
    if (oscillatorIndex >= 0 && oscillatorIndex < NumOscillators) {
        oscillators[oscillatorIndex]->setFrequency(newFrequencyHz);
        coreFreq[oscillatorIndex] = newFrequencyHz; //used in the FM/AM mode
    }
    
}

void OscillatorManager::setWaveform (const int oscillatorIndex, const int wave)
{
    if (oscillatorIndex >= 0 && oscillatorIndex < NumOscillators) {
        oscillators[oscillatorIndex]->setWave(wave);
    }
    
}

void OscillatorManager::setPanning (const int oscillatorIndex, const float left, const float right)
{
    if (oscillatorIndex >= 0 && oscillatorIndex < NumOscillators) {
        oscillators[oscillatorIndex]->setPan(left, right);
    }
}

//==============================================================================
void OscillatorManager::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate_)
{
	for (int oscillatorIndex = NumOscillators; --oscillatorIndex >= 0;)
		oscillators[oscillatorIndex]->prepare(sampleRate_);
}

void OscillatorManager::releaseResources()
{
    
}

void OscillatorManager::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    const eOscillatorMode mode = oscillatorMode;
    
    for (int i = 0; i < info.numSamples; ++i)
    {
        float sampleL = 0.0f;
        float sampleR = 0.0f;
        /* read all 24 oscialltors*/
        if (mode == eNormal) {
            for (int oscillatorIndex = NumOscillators; --oscillatorIndex >= 0;) {
                const float sample = (float)(oscillators[oscillatorIndex]->nextSample());
                sampleL += sample * oscillators[oscillatorIndex]->getLPan();
                sampleR += sample * oscillators[oscillatorIndex]->getRPan();
            }
        }
        else if (mode == eFm8) {
            for (int index = 16; index < NumOscillators; ++index) {
                float carrier = (float)oscillators[index]->nextSample();
                carrier *= (coreFreq[index] * coreAmp[index]); //our iFD.
                carrier += coreFreq[index-16];
                oscillators[index-16]->setFrequency(carrier);
            }
            for (int index = 0; index < NumOscillators-8; ++index) {
                sampleL += (float)(oscillators[index]->nextSample());
                sampleR = sampleL;
            }
        }

        *info.buffer->getWritePointer (0, info.startSample + i) = sampleL;
        *info.buffer->getWritePointer (1, info.startSample + i) = sampleR;
//        for (int j = info.buffer->getNumChannels(); --j >= 0;)
//            *info.buffer->getWritePointer (j, info.startSample + i) = sample;
    }
}

String OscillatorManager::getState (const int oscillatorNumber)
{
    return oscillators[oscillatorNumber]->getStates();
}

bool OscillatorManager::isActive (const int oscillatorNumber)
{
    return oscillators[oscillatorNumber]->getLastAmp() > 0.0;
}

bool OscillatorManager::isOutOfRange (const int oscillatorNumber)
{
    return oscillators[oscillatorNumber]->ampOutOfRange || oscillators[oscillatorNumber]->frequencyOutOfRange || oscillators[oscillatorNumber]->waveOutOfRange;
}

void OscillatorManager::setOscillatorRoutingMode (const eOscillatorMode mode)
{
    oscillatorMode = mode;
}
OscillatorManager::eOscillatorMode OscillatorManager::getOscillatorRoutingMode ()
{
    return oscillatorMode;
}

void OscillatorManager::setAttackIncrement (const int oscillatorIndex, const double attack)
{
    if (oscillatorIndex >= 0 && oscillatorIndex < NumOscillators) {
        oscillators[oscillatorIndex]->setAttack(attack);
    }
}
void OscillatorManager::setReleaseIncrement (const int oscillatorIndex, const double release)
{
    if (oscillatorIndex >= 0 && oscillatorIndex < NumOscillators) {
        oscillators[oscillatorIndex]->setRelease(release);
    }
}

void OscillatorManager::reset ()
{
    for (int oscillatorIndex = NumOscillators; --oscillatorIndex >= 0;)
    {
        oscillators[oscillatorIndex]->setAttack(defaultAttackInc);
        oscillators[oscillatorIndex]->setRelease(defaultAttackInc);
        oscillators[oscillatorIndex]->setPan(1.0, 1.0);
        oscillators[oscillatorIndex]->setAmplitude(0);
        oscillators[oscillatorIndex]->setFrequency(0);
    }
}
