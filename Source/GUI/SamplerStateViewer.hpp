//
//  SamplerStateViewer.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 24/07/2019.
//

#ifndef SamplerStateViewer_hpp
#define SamplerStateViewer_hpp

#include "../../JuceLibraryCode/JuceHeader.h"
#include "AudioMain.h"

class SamplerStateViewer : public Component {
public:
    SamplerStateViewer (AudioMain& audio) : audioMain(audio) {}
    ~SamplerStateViewer () {}
    
    void paint (Graphics & g);
private:
    AudioMain & audioMain;
};
#endif /* SamplerStateViewer_hpp */
