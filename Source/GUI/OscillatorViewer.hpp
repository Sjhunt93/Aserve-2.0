//
//  OscillatorViewer.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 24/07/2019.
//

#ifndef OscillatorViewer_hpp
#define OscillatorViewer_hpp

#include "../../JuceLibraryCode/JuceHeader.h"
#include "AudioMain.h"
class OscillatorViewer : public Component {
public:
    OscillatorViewer (AudioMain& audio) : audioMain(audio) {}
    ~OscillatorViewer () {}
    
    void paint (Graphics & g);
private:
    AudioMain & audioMain;
};

#endif /* OscillatorViewer_hpp */
