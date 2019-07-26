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
#include "AUTDrumSampler.hpp"
#include "ComponentList.h"


class TestSelector : public Component, public ActionBroadcaster {
public:
    TestSelector (String name);
    ~TestSelector ();
    void resized ();
    void paint (Graphics & g);
    void mouseUp (const MouseEvent & event);
    void setState (AserveUnitTest::eTestState state);

    bool selected;
    const String name ();
private:
    Label label;
    AserveUnitTest::eTestState state;
};


class UnitTestGUI : public Component, public ActionListener, public Button::Listener {
  
public:
    UnitTestGUI (AserveComs & coms);
    ~UnitTestGUI ();
    
    void paint (Graphics &g) override;
    void resized () override;
    
    void actionListenerCallback (const String& message) override;

    void buttonClicked (Button* btn) override;

    TestSelector * getSelectorForName (String name);
    void resetSelectors (int idSelect);
private:
    TextButton runTest1;
    TextEditor results;
    ScopedPointer<AserveUnitTest> unitTest;
    String selectedTest;
    
    AserveComs & coms;
    
    ComponentListViewport compList;
};

#endif /* UnitTestUI_hpp */
