//
//  SawOsc.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 10/6/20.
//

#include "SawOsc.hpp"



//-------------------------------------------
//Saw Oscilator
//Adapted from The Synthesis ToolKit in C++ (STK)
//By Perry R. Cook and Gary P. Scavone, 1995-2007.

SawOscillator::SawOscillator()
: nHarmonics_(0),
lastOutput_(0)
{
    this->setFrequency( frequency );
    this->reset();
}
SawOscillator::~SawOscillator()
{
    
}

void SawOscillator :: reset()
{
    amplitude = 0.0;
    phase_ = 0.0f;
    state_ = 0.0;
    lastOutput_ = 0;
}

void SawOscillator::prepare(double sampleRate_)
{
    sampleRate = sampleRate_;
    this->reset();
    this->updateHarmonics();
}

void SawOscillator::setFrequency(const double val)
{
    frequency = val;
    // By using an even value of the parameter M, we get a bipolar blit
    // waveform at half the blit frequency.  Thus, we need to scale the
    // frequency value here by 0.5. (GPS, 2006).
    
    this->updateHarmonics();
    
    p_ = sampleRate / frequency;
    C2_ = 1 / p_;
    rate_ = double_Pi * C2_;
    this->updateHarmonics();
}

void SawOscillator::updateHarmonics( void )
{
    if ( nHarmonics_ <= 0 )
    {
        unsigned int maxHarmonics = (unsigned int) floor( 0.5 * p_ );
        m_ = 2 * maxHarmonics + 1;
    }
    else
        m_ = 2 * nHarmonics_ + 1;
    
    a_ = m_ / p_;
}

double SawOscillator :: renderWaveShape( void )
{
    // The code below implements the BLIT algorithm of Stilson and
    // Smith, followed by a summation and filtering operation to produce
    // a sawtooth waveform.  After experimenting with various approaches
    // to calculate the average value of the BLIT over one period, I
    // found that an estimate of C2_ = 1.0 / period (in samples) worked
    // most consistently.  A "leaky integrator" is then applied to the
    // difference of the BLIT output and C2_. (GPS - 1 October 2005)
    
    // A fully  optimized version of this code would replace the two sin
    // calls with a pair of fast sin oscillators, for which stable fast
    // two-multiply algorithms are well known. In the spirit of STK,
    // which favors clarity over performance, the optimization has
    // not been made here.
    
    // Avoid a divide by zero, or use of a denormalized divisor
    // at the sinc peak, which has a limiting value of m_ / p_.
    
    double denominator = sin( phase_ );
    
    if ( fabs(denominator) <= std::numeric_limits<double>::epsilon() )
        lastOutput_ = a_;
    else
    {
        lastOutput_ =  sin( m_ * phase_ );
        lastOutput_ /= p_ * denominator;
    }
    
    lastOutput_ += state_ - C2_;
    state_ = lastOutput_ * 0.995;
    
    phase_ += rate_;
    if ( phase_ >= double_Pi ) phase_ -= double_Pi;
    
    return lastOutput_;
}




