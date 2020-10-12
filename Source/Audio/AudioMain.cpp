//
//  AudioMain.cpp
//  Aserve 2
//
//  Created by Samuel Hunt on 17/07/2018.
//
//

#include "AudioMain.h"


const String AudioMain::ERROR_POSTED = "ERROR:";
const String AudioMain::UPDATE_GUI = "GUI_UP:";

AudioMain::AudioMain ()
{
    filter[0].makeInactive();
    filter[1].makeInactive();
    
    audioFiles = new AudioFilePlayerManager();
    audioFiles->addListener(this);
    
    mixerSource.addInputSource(&oscs, false);
    mixerSource.addInputSource(audioFiles, false);

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 16; i++) {
            resampleSynth[j].addVoice(new SamplerVoice());
        }
    }
    formatManager.registerBasicFormats();

//    setResampleSynthSound(0, "/Users/sj4-hunt/Documents/pianoSample.wav", 60, 0, 0);
    prevHBCuttoff_ = prevHBQ_ = prevHBGain_ = 0;
    lpfCuttoff = lastLpf = 21000;
    hpfCuttoff = lastHpf = 20;
    
    for (int i = 0; i < eMaxFileTracks; i++) {
        audioNames.add("Empty");
    }
    for (int i = 0; i < eMaxSamplerTracks; i++) {
        resampledNames.add("Empty");
    }
}
AudioMain::~AudioMain ()
{
    
}

void AudioMain::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    oscs.prepareToPlay(samplesPerBlockExpected, sampleRate);
    audioFiles->prepareToPlay(samplesPerBlockExpected, sampleRate);
    this->sampleRate = sampleRate;
    
    for (int i = 0; i < eSizes::eMaxSamplerTracks; i++) {
        messageCollector[i].reset(sampleRate);
        resampleSynth[i].setCurrentPlaybackSampleRate(sampleRate);
    }
    
}

void AudioMain::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
//    oscs.getNextAudioBlock(bufferToFill);
    mixerSource.getNextAudioBlock(bufferToFill);
//    audioFiles->getNextAudioBlock(bufferToFill);
    
    if (lastLpf != lpfCuttoff) {
        lastLpf = lpfCuttoff;
        filter[0].setCoefficients(IIRCoefficients::makeLowPass(sampleRate, lpfCuttoff));
        filter[1].setCoefficients(IIRCoefficients::makeLowPass(sampleRate, lpfCuttoff));
    }
    if (lastHpf != hpfCuttoff) {
        lastHpf = hpfCuttoff;
        filter[2].setCoefficients(IIRCoefficients::makeHighPass(sampleRate, hpfCuttoff));
        filter[3].setCoefficients(IIRCoefficients::makeHighPass(sampleRate, hpfCuttoff));
    }
    
    for (int i = 0; i < eSizes::eMaxSamplerTracks; i++) {
        MidiBuffer incomingMidi;
        messageCollector[i].removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        resampleSynth[i].renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
    }
    
    //First do LPF
    filter[0].processSamples(bufferToFill.buffer->getWritePointer(0), bufferToFill.buffer->getNumSamples());
    filter[1].processSamples(bufferToFill.buffer->getWritePointer(1), bufferToFill.buffer->getNumSamples());
    
    filter[2].processSamples(bufferToFill.buffer->getWritePointer(0), bufferToFill.buffer->getNumSamples());
    filter[3].processSamples(bufferToFill.buffer->getWritePointer(1), bufferToFill.buffer->getNumSamples());
    
    
    //Then do BPF
    for (int i = 0; i < bufferToFill.buffer->getNumSamples(); i++) {
        for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); chan++) {
            *bufferToFill.buffer->getWritePointer(chan, i) =
            (filter[4+chan].processSingleSampleRaw(*bufferToFill.buffer->getReadPointer(chan, i)) * prevHBGain_) +
            *bufferToFill.buffer->getReadPointer(chan, i);
            
            *bufferToFill.buffer->getWritePointer(chan, i) = *bufferToFill.buffer->getWritePointer(chan, i) * masterGain;//
        }
    }
    
}


void AudioMain::releaseResources()
{
    oscs.releaseResources();
    audioFiles->releaseResources();
    
}
OscillatorManager & AudioMain::getOscs ()
{
    return oscs;
}


void AudioMain::setLPF (const int cuttoff)
{
    lpfCuttoff = cuttoff;
}
void AudioMain::setHPF (const int cuttoff)
{
    hpfCuttoff = cuttoff;
}
void AudioMain::setHighBand (const float cuttoff, const float q, const float gain)
{
    if ( !(cuttoff == prevHBCuttoff_ && q == prevHBQ_)) { //dont update
        if (cuttoff * 2 < sampleRate && q > 0.0  ) {
            filter[4].setCoefficients(IIRCoefficients::makeBandPass(sampleRate, cuttoff, q));
            filter[5].setCoefficients(IIRCoefficients::makeBandPass(sampleRate, cuttoff, q));
        }
    }
    
    prevHBQ_ = q;
    prevHBGain_ = gain;
    prevHBCuttoff_ = cuttoff;

}
void AudioMain::setHighBandGain (const float gain)
{
    
}

void AudioMain::audioFileNameChanged(const int audioFileIndex, const String &path)
{
    //no need to do anything here yet.
}
void AudioMain::audioFilePlayStateChanged(const int audioFileIndex, const AudioFilePlayerManager::PlayState state)
{
    //no need to do anything here yet.
}

void AudioMain::stopAll(void)
{
    oscs.stopAll();
    audioFiles->stopAll();
}

void AudioMain::playFile(const int audioFileIndex)
{
    audioFiles->togglePlayState(audioFileIndex);
}

void AudioMain::changeAudioFileState (const int audioFileIndex, const int state)
{
    
}

void AudioMain::playFile(const int audioFileIndex, float gain)
{
    audioFiles->stop(audioFileIndex);
    if (gain > 1.0) {
        gain = 1.0;
    }
    gain > 0.f ? audioFiles->play(audioFileIndex, gain) : audioFiles->stop(audioFileIndex);

}

void AudioMain::loadFile(const int audioFileIndex, const String &newFilePath)
{
    audioFiles->loadAudioFile(audioFileIndex, newFilePath);
    audioNames.set(audioFileIndex, newFilePath.fromLastOccurrenceOf("/", false, false));
}

void AudioMain::setResampleSynthSound (const int index, const String &filePath, const int originalPitch, double attack, double release)
{

    if (index >= 0 && index < eMaxSamplerTracks) {
        resampleSynth[index].clearSounds();
        BigInteger range;
        for (int i = 0; i < 127; i++) {
            range.setBit(i);
        }
        
        AudioFormatReader* reader = formatManager.createReaderFor (File(filePath));
        SynthesiserSound * sound = new SamplerSound("Sample", *reader, range, originalPitch,attack,release, 5.0);
        
        resampleSynth[index].addSound(sound);
        delete reader;
        
            resampledNames.set(index, filePath.fromLastOccurrenceOf("/", false, false));
    }
    
    
    
}

void AudioMain::triggerSampledNote (const int index, const int note, const int velocity)
{
    MidiMessage m(0x90, note, velocity);
    m.setTimeStamp(Time::getMillisecondCounter());
    messageCollector[index].handleIncomingMidiMessage(nullptr, m);
}

void AudioMain::reset ()
{
    stopAll();
    
    prevHBCuttoff_ = prevHBQ_ = prevHBGain_ = 0;
    lpfCuttoff = 21000;
    hpfCuttoff = 20;
    
    for (int i = 0; i < 8; i++) {
        filter[i].reset();
    }
}

StringArray AudioMain::getAudioFileNames ()
{
    return audioNames;
}
StringArray AudioMain::getResampledNames ()
{
    return resampledNames;
}
