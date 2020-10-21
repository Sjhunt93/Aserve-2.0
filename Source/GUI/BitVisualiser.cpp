//
//  BitVisualiser.cpp
//  Aserve 2
//
//  Created by Samuel Hunt on 20/07/2018.
//
//

#include "BitVisualiser.h"



BitVisualiser::BitVisualiser ()
{

    reset();
    startTimer(1000/50);
}
BitVisualiser::~BitVisualiser ()
{
    
}
void BitVisualiser::timerCallback()
{
    /* this allows us to repaint once a change has happend and therefore avoid any issues with threads
    */
    if (paintUpdate) {
        repaint();
        paintUpdate = false;
    }
}

void BitVisualiser::reset ()
{
    mutexLock.lock();
    
    for (int i = 0; i < 16; i++) {
        pixelGrid[i] = 0;
    }

    
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            int index = x + y*16;
            if (new_piskel_data[0][index]) {
                // pixelGrid[y] |= 1 << x;
            }
            overloadedColours[index] = Colour::fromRGBA(0, 0, 0, 0);
        }
    }
    paintUpdate = true;
    mutexLock.unlock();
}

void BitVisualiser::paint (Graphics & g)
{
//    jassert(getWidth() == getHeight());
    
    while (mutexLock.try_lock()) {

    }
//    mutexLock.lock();
    
    float size = ((getWidth()-2) / 16);
  g.fillAll(Colours::darkgrey);
    g.setColour(Colours::white);
    g.drawText("Pixel Grid", 0, 0, getWidth(), 30, Justification::centred);
    
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            if (pixelGrid[y] & 1 << (15-x)) {
                int index = x + y*16;
                if (overloadedColours[index].getAlpha()) {
                    g.setColour(overloadedColours[index]);
                }
                else {
                    g.setColour(Colours::white);
                }
                g.fillRect(1+ x*size, 30 + y*size, size - 1, size - 1);
            }
        }
    }
    
    g.setColour(Colours::black);
    for (int i = 0; i < 15; i++) {

        g.drawLine(size*i + size, 30, size *i + size, getHeight()-2);
        
        g.drawLine(0, 30 + (size*i + size), getWidth()-2, 30 + (size*i + size));
    }
    
    mutexLock.unlock();
}
void BitVisualiser::set (uint16 pixelRow, int row)
{
    if (row < 16 && row >= 0) {
        pixelGrid[row] = pixelRow;
        repaint();
    }

}
void BitVisualiser::mouseDown (const MouseEvent& event)
{
    float size = ((getWidth()-2) / 16);
    
    int x = event.x / size;
    int y = ( event.y-30) / size;
    if (x > 15) {
        x = 15;
    }
    if (y > 15) {
        y = 15;
    }
    sendActionMessage("GRID:" + String(x) + "," + String(y));
}

void BitVisualiser::setThreadSafe (const int index, const int red, const int green, const int blue)
{
    mutexLock.lock();

    if (index >= 0 && index < 256) {
//        overloadedColours[index] = Colour::fromRGB(red, green, blue);
        const int oldR = overloadedColours[index].getRed();
        const int oldG = overloadedColours[index].getGreen();
        const int oldB = overloadedColours[index].getBlue();
        
        if (red != -1) {
            overloadedColours[index] = Colour::fromRGB(red, oldG, oldB);
        }
        if (green != -1) {
            overloadedColours[index] = Colour::fromRGB(oldR, green, oldB);
        }
        if (blue != -1) {
            overloadedColours[index] = Colour::fromRGB(oldR, oldG, blue);
        }
    }
    paintUpdate = true;
    mutexLock.unlock();
    

}
void BitVisualiser::updateThreadSafe (int x, int y, bool state)
{
    mutexLock.lock();
    x = 15-x;
    if (x >= 0 && x < 16) {
        if (y >= 0 && y < 16) {
            if (state) {
                pixelGrid[y] |= 1 << x;
            }
            else {
                pixelGrid[y] = pixelGrid[y] & ~(1 << x);
            }
            
        }
    }
    
    paintUpdate = true;
    mutexLock.unlock();

    
}

void BitVisualiser::setThreadSafe (UInt16 pixelRow, int row)
{
    mutexLock.lock();
    pixelGrid[row] = pixelRow;
    paintUpdate = true;
    mutexLock.unlock();
    
    
}

