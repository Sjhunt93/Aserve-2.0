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
    for (int i = 0; i < 16; i++) {
        pixelGrid[i] = 0;
           // pixelGrid[i] = arc4random() % 65536;
    }

    
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            int index = x + y*16;
            if (new_piskel_data[0][index]) {
               // pixelGrid[y] |= 1 << x;
            }
        }
    }
    
    
}
BitVisualiser::~BitVisualiser ()
{
    
}

void BitVisualiser::paint (Graphics & g)
{
    jassert(getWidth() == getHeight());
    
    float size = ((getWidth()-2) / 16);
    
  //  g.fillAll(Colours::black);
    
    g.setColour(Colours::white);
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            if (pixelGrid[y] & 1 << x) {
                g.fillRect(1+ x*size, 1 + y*size, size - 1, size - 1);
            }
        }
    }
    
}
void BitVisualiser::set (UInt16 pixelRow, int row)
{
    pixelGrid[row] = pixelRow;
    repaint();
}