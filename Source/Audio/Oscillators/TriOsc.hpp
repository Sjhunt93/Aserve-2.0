//
//  TriOsc.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#ifndef TriOsc_hpp
#define TriOsc_hpp

#include "SineOsc.hpp"


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

#endif /* TriOsc_hpp */
