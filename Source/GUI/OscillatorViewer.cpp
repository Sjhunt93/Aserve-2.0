//
//  OscillatorViewer.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 24/07/2019.
//

#include "OscillatorViewer.hpp"
#include "OscillatorManager.h"

void OscillatorViewer::paint (Graphics & g)
{
    float startPoint = 40.0;
    float height = getHeight() - startPoint;
    height /= ((float)OscillatorManager::OscillatorManagerConstants::NumOscillators);
    if (height <= 10) {
        height = 10;
    }
    
    g.setColour(Colours::white);
    const int mode = audioMain.getOscs().getOscillatorRoutingMode();
    const int width = getWidth() - 10;
    if (mode == OscillatorManager::eNormal) {
        g.drawText("Oscillator State (normal)", 5, 5, width, 30, Justification::centred);
    }
    if (mode == OscillatorManager::eFm8) {
        g.drawText("Oscillator State (FM 8)", 5, 5, width, 30, Justification::centred);
    }
    
    Colour offCol = Colours::white.withAlpha(0.2f);
    
    for (int i = 0; i < OscillatorManager::OscillatorManagerConstants::NumOscillators; i++) {
        g.setColour(Colours::black);
        g.drawLine(0, startPoint, getWidth(), startPoint);
        
        g.setColour( audioMain.getOscs().isActive(i) ? (  audioMain.getOscs().isOutOfRange(i) ? Colours::red : Colours::white )  : offCol );
        
        g.drawText(/*String("Oscillator: ") +*/ String(i) + " | " + audioMain.getOscs().getState(i) , 5, startPoint, width, height, Justification::left);
        
        startPoint += height;
        
    }
    
}

