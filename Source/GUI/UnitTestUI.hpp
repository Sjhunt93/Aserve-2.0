//
//  UnitTestUI.hpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 19/06/2019.
//

#ifndef UnitTestUI_hpp
#define UnitTestUI_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AserveComs.h"
#include "AUTMtof.hpp"
#include "AUTTestTone.hpp"
class UnitTestGUI : public Component, public ActionListener, public Button::Listener {
  
public:
    UnitTestGUI (AserveComs & coms);
    ~UnitTestGUI ();
    
    void paint (Graphics &g) override;
    void resized ();
    
    void actionListenerCallback (const String& message);

    void buttonClicked (Button* btn);

private:
    TextButton runTest1, runTest2;
    TextEditor results;
    ScopedPointer<AserveUnitTest> unitTest;
    
    void runUnitTest1();
    void runUnitTest2();
    AserveComs & coms;
    
    Button * currentTest;
};

#endif /* UnitTestUI_hpp */
