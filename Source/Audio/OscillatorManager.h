/*
 *  OscillatorManager.h
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_OSCILLATORMANAGER
#define H_OSCILLATORMANAGER

#include "../../JuceLibraryCode/JuceHeader.h"
#include "AserveOscillators.h"

/**
 Manages Aserve's oscillators
 */

class OscillatorManager : public AudioSource
{
public:
    enum OscillatorManagerConstants 
    {
        NumOscillators = 24,
        NumWaveforms = WaveOscillator::eNumWaveforms
    };
    enum eOscillatorMode
    {
      eNormal = 0,
        eFm8,
    };
    /** 
     Constructor 
     */
    OscillatorManager();
    
    /** 
     Destructor 
     */
    ~OscillatorManager();

    /** 
     Sets the signal's amplitude. 
     */
    void setAmplitude (const int oscillatorNumber, const float newAmplitude);
    
    /** 
     Sets the signal's frequency. 
     */
    void setFrequency (const int oscillatorNumber, const double newFrequencyHz);
    
    /**
     Set waveform of the specified oscillator
     */
    void setWaveform (const int oscillatorNumber, const int wave);
    
    /** 
     Stops all oscillators in back with a tailoff 
     */
    void stopAll(void);
	
    /** 
     Stops the specified oscillator with a tailoff 
     */
    void stop(const int oscillatorNumber);
	
    /*
     
     */
    void setPanning (const int oscillatorNumber, const float left, const float right);
    
    /*
     
     */
    void setAttackIncrement (const int oscillatorNumber, const double attack);
    void setReleaseIncrement (const int oscillatorNumber, const double release);

    //AudioSouce Callbacks
    virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    virtual void releaseResources();
    virtual void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    String getState (const int oscillatorNumber);
    bool isActive (const int oscillatorNumber);
    bool isOutOfRange (const int oscillatorNumber);
    
    //oscillator routing
    void setOscillatorRoutingMode (const eOscillatorMode mode);
    eOscillatorMode getOscillatorRoutingMode ();
    
private:
    OwnedArray<WaveOscillator>  oscillators;
    float                       coreFreq[24];
    float                       coreAmp[24];
    eOscillatorMode             oscillatorMode;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorManager)
};  


#endif //H_OSCILLATORMANAGER
