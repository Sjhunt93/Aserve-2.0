//
//  TriOsc.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#include "TriOsc.hpp"





TriOscillator::TriOscillator()
{
    this->setFrequency( frequency );
    this->reset();
    
}
TriOscillator::~TriOscillator()
{
    
}

void TriOscillator::reset(void)
{
    for (int i = 0; i < 10; i++) {
        sineOscs[i].reset();
    }
}
void TriOscillator::setFrequency(const double val)
{
    for (int i = 0; i < 10; i++) {
        sineOscs[i].setFrequency(val*(i*2+1));
    }
}
void TriOscillator::prepare(double sampleRate_)
{
    sampleRate = sampleRate_;
    this->reset();
    this->updateHarmonics();
}
double TriOscillator::renderWaveShape(void)
{
    double sample = 0;
    static const double inverses[10] = {1,-1,1,-1,1,-1,1,-1,1,-1};
    for (int i = 0; i < 10; i++) {
        
        if (sineOscs[i].getFrequency() > sampleRate * 0.5) {
            break;
        }
        double amp = 1.0 / ((i*2+1) * (i*2+1));
        double s = sineOscs[i].renderWaveShape() * (amp * inverses[i]);
        sample += s;
    }
    return sample;
}
void TriOscillator::updateHarmonics( void )
{
    
}







