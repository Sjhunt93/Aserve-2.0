//
//  SquareOsc.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#ifndef SquareOsc_hpp
#define SquareOsc_hpp

#include "Oscillator.hpp"


class SquareOscillator : public Oscillator
{
private:
    void updateHarmonics( void );
    
    unsigned int nHarmonics_, m_;
    double rate_, phase_;
    double p_,a_;
    double lastBlitOutput_, dcbState_, lastOutput_;
    
public:
    SquareOscillator();
    virtual ~SquareOscillator();
    
    void reset(void);
    void prepare(double sampleRate_);
    void setFrequency(const double val);
    double renderWaveShape(void);
};

#endif /* SquareOsc_hpp */
