//
//  SamplerStateViewer.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 24/07/2019.
//

#include "SamplerStateViewer.hpp"
#include "OscillatorManager.h"

void SamplerStateViewer::paint (Graphics & g)
{
    // what is startpoint?
    
    // height adjusted by 'bottom' inset - to make room for impulse midi keyboard panel
    float height = getHeight();//((getHeight() - panelBottomInset) - startPoint);
    height /= 12.0;
    if (height <= 10) {
        height = 10;
    }
    g.setColour(Colours::white);
    g.drawText("Sampler State", 0, 5, getWidth() , height*2, Justification::centred);
    
    float startPoint = height*2;
    
    for (int i = 0; i < AudioMain::eMaxFileTracks; i++) {
        g.setColour(Colours::black);
        g.drawLine(0, startPoint, getWidth(), startPoint);
        
        g.setColour(audioMain.getAudioFileNames()[i] != "Empty" ? Colours::white : Colours::white.withAlpha(0.2f));
        
        g.drawText(/*"Sampler: " +*/ String(i) + " | " + audioMain.getAudioFileNames()[i]
                   , 5, startPoint, getWidth() - 10, height, Justification::left);
        
        startPoint += height;
        
    }
    g.setColour(Colours::black);
    g.drawLine(0, startPoint, getWidth(), startPoint, 3.0);
    
    g.setColour(Colours::white);
    g.drawText("Pitched Sampler State: ", 5, startPoint, getWidth()-10, height*2, Justification::centred);
    
    startPoint += height*2;
    
    for (int i = 0; i < AudioMain::eMaxSamplerTracks; i++) {
        g.setColour(Colours::black);
        g.drawLine(0, startPoint, getWidth(), startPoint);
        
        g.setColour(audioMain.getResampledNames()[i] != "Empty" ? Colours::white : Colours::white.withAlpha(0.2f));
        
        g.drawText(/*"Sampler: " +*/ String(i) + " | " + audioMain.getResampledNames()[i]
                   , 5, startPoint, getWidth()-10, height, Justification::left);
        
        startPoint += height;
    }
    
    g.setColour(Colours::black);
    g.drawLine(0, startPoint, getWidth(), startPoint, 3.0);
    
}

