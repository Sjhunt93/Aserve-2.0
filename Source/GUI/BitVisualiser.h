//
//  BitVisualiser.h
//  Aserve 2
//
//  Created by Samuel Hunt on 20/07/2018.
//
//

#ifndef __Aserve_2__BitVisualiser__
#define __Aserve_2__BitVisualiser__

#include "../../JuceLibraryCode/JuceHeader.h"


static const uint32_t new_piskel_data[1][256] = {
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000,
        0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000,
        0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000,
        0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000,
        0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000,
        0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000,
        0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
    }
};

class BitVisualiser : public Component, public ActionBroadcaster {
public:
    BitVisualiser ();
    ~BitVisualiser ();
    

    void paint (Graphics & g);
    void set (uint16 pixelRow, int row);
    
    void mouseDown (const MouseEvent& event);

    
private:
	uint16 pixelGrid[16];
};


#endif /* defined(__Aserve_2__BitVisualiser__) */
