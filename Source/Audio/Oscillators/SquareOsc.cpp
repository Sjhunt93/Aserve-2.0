//
//  SquareOsc.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#include "SquareOsc.hpp"


//-------------------------------------------
//Square Oscilator
//Adapted from The Synthesis ToolKit in C++ (STK)
//By Perry R. Cook and Gary P. Scavone, 1995-2007.

SquareOscillator::SquareOscillator()
: nHarmonics_(0),
lastOutput_(0)
{
    this->setFrequency( frequency );
    this->reset();
}
SquareOscillator::~SquareOscillator()
{
    
}

void SquareOscillator :: reset()
{
    amplitude = 0.0;
    phase_ = 0.0;
    lastOutput_ = 0;
    dcbState_ = 0.0;
    lastBlitOutput_ = 0;
}

void SquareOscillator::prepare(double sampleRate_)
{
    sampleRate = sampleRate_;
    this->reset();
    this->updateHarmonics();
}

void SquareOscillator::setFrequency(const double val)
{
    frequency = val;
    // By using an even value of the parameter M, we get a bipolar blit
    // waveform at half the blit frequency.  Thus, we need to scale the
    // frequency value here by 0.5. (GPS, 2006).
    p_ = 0.5 * sampleRate / frequency;
    rate_ = double_Pi / p_;
    this->updateHarmonics();
}

void SquareOscillator::updateHarmonics( void )
{
    // Make sure we end up with an even value of the parameter M here.
    if ( nHarmonics_ <= 0 )
    {
        unsigned int maxHarmonics = (unsigned int) floor( 0.5 * p_ );
        m_ = 2 * (maxHarmonics + 1);
    }
    else
        m_ = 2 * (nHarmonics_ + 1);
    
    a_ = m_ / p_;
}

double SquareOscillator :: renderWaveShape( void )
{
    double temp = lastBlitOutput_;
    const double twoPi = double_Pi*2.0;
    // A fully  optimized version of this would replace the two sin calls
    // with a pair of fast sin oscillators, for which stable fast
    // two-multiply algorithms are well known. In the spirit of STK,
    // which favors clarity over performance, the optimization has
    // not been made here.
    
    // Avoid a divide by zero, or use of a denomralized divisor
    // at the sinc peak, which has a limiting value of 1.0.
    double denominator = sin( phase_ );
    if ( fabs( denominator )  < std::numeric_limits<double>::epsilon() ) {
        // Inexact comparison safely distinguishes betwen *close to zero*, and *close to PI*.
        if ( phase_ < 0.1f || phase_ > twoPi - 0.1f )
            lastBlitOutput_ = a_;
        else
            lastBlitOutput_ = -a_;
    }
    else {
        lastBlitOutput_ =  sin( m_ * phase_ );
        lastBlitOutput_ /= p_ * denominator;
    }
    
    lastBlitOutput_ += temp;
    
    // Now apply DC blocker.
    lastOutput_ = lastBlitOutput_ - dcbState_ + 0.999 * lastOutput_;
    dcbState_ = lastBlitOutput_;
    
    phase_ += rate_;
    if ( phase_ >= twoPi ) phase_ -= twoPi;
    
    return lastOutput_;
}
