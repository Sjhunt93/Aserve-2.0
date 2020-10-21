//
//  SineOsc.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#ifndef SineOsc_hpp
#define SineOsc_hpp

#include "Oscillator.hpp"


class SineOscillator : public Oscillator
{
private:
    double currentAngle, angleDelta;
    
public:
    SineOscillator();
    virtual ~SineOscillator();
    void reset(void);
    void setFrequency(const double val);
    double renderWaveShape(void);
};

#endif /* SineOsc_hpp */
