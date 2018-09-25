//
//  AudioMain.h
//  Aserve 2
//
//  Created by Samuel Hunt on 17/07/2018.
//
//

#ifndef __Aserve_2__AudioMain__
#define __Aserve_2__AudioMain__

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorManager.h"
#include "AudioFilePlayerManager.h"

//This is a container for holding all audio related things...

class AudioMain : public AudioFilePlayerManager::Listener, public ActionBroadcaster {
public:
    
    enum eSizes {
        eMaxSamplerTracks = 4,
        eMaxFileTracks = 4,
    };
    
    static const String ERROR_POSTED;
    static const String UPDATE_GUI;
    
    AudioMain ();
    ~AudioMain ();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    
    void audioFileNameChanged(const int audioFileIndex, const String &path);
    void audioFilePlayStateChanged(const int audioFileIndex, const AudioFilePlayerManager::PlayState state);
    
    /*
     Note that most of this is not technically thread safe.. however because they only acesses primitive values, it is unlikely that they will cause issue, will check this later with someone who knows more than me...
     */
    OscillatorManager &getOscs ();
    void setLPF (const int cuttoff);
    void setHPF (const int cuttoff);

    void setHighBand (const float cuttoff, const float q, const float gain);
    void setHighBandGain (const float gain);
    
    void stopAll(void);
    
    void playFile(const int audioFileIndex);
    void playFile(const int audioFileIndex, float gain);
    void changeAudioFileState (const int audioFileIndex, const int state);
    void loadFile(const int audioFileIndex, const String &newFilePath);
    
    void setResampleSynthSound (const int index, const String &filePath, const int originalPitch, double attack, double release);
    void triggerSampledNote (const int index, const int note, const int velocity);
    
    void reset ();
    
    StringArray getAudioFileNames ();
    StringArray getResampledNames ();
    
private:
    AudioFormatManager formatManager;
    OscillatorManager oscs;
    IIRFilter filter[8];
    double sampleRate;
    ScopedPointer <AudioFilePlayerManager> audioFiles;  //audio file playback
    MixerAudioSource mixerSource;           //all audio objects will be mixed into this
    
    Synthesiser     resampleSynth[eMaxSamplerTracks];
    MidiMessageCollector messageCollector[eMaxSamplerTracks];

    float prevHBCuttoff, prevHBQ, prevHBGain;
    
    float lpfCuttoff;
    float hpfCuttoff;
    
    float lastLpf, lastHpf;
    
    
    StringArray audioNames;
    StringArray resampledNames;
};

#endif /* defined(__Aserve_2__AudioMain__) */
