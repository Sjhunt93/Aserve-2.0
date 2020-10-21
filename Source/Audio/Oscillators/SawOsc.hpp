//
//  SawOsc.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#ifndef SawOsc_hpp
#define SawOsc_hpp

#include "Oscillator.hpp"


class SawOscillator : public Oscillator
{
private:
    void updateHarmonics( void );
    
    unsigned int nHarmonics_, m_;
    double rate_, phase_;
    double p_,a_,C2_;
    double state_, lastOutput_;
    
public:
    SawOscillator();
    ~SawOscillator();
    
    void reset(void);
    void setFrequency(const double val);
    void prepare(double sampleRate_);
    double renderWaveShape(void);
};

class InverseSaw : public SawOscillator {
public:
    InverseSaw () : SawOscillator() {}
    double renderWaveShape(void) {
        return SawOscillator::renderWaveShape() * -1;
    }
    
};

#endif /* SawOsc_hpp */
