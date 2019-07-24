//
//  UnitTestUI.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 19/06/2019.
//

#include "UnitTestUI.hpp"

UnitTestGUI::UnitTestGUI (AserveComs & _coms) : coms(_coms)
{
    addAndMakeVisible(runTest1);
    addAndMakeVisible(runTest2);
    runTest1.setButtonText("Run Test 1");
    runTest2.setButtonText("Run Test 2");
    
    runTest1.addListener(this);
    runTest2.addListener(this);
    currentTest = nullptr;
}
UnitTestGUI::~UnitTestGUI ()
{
    
}

void UnitTestGUI::paint (Graphics &g)
{
    g.fillAll(Colours::white);
    g.drawText("Unit Test", 0, 0, getWidth(), 50, Justification::centred);
}
void UnitTestGUI::resized ()
{
    runTest1.setBounds(0, 100, 100, 30);
    runTest2.setBounds(0, runTest1.getBottom() + 5, 100, 30);
}

void UnitTestGUI::actionListenerCallback (const String& message)
{
    if (message.startsWith("finished") && unitTest != nullptr) {
        std::cout << unitTest->getErrors() << " : Unit result: " << unitTest->getResult() << "\n";
        if (unitTest->getResult() == 3) {
            currentTest->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
        }
        if (unitTest->getResult() == 2) {
            currentTest->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        }
        unitTest = nullptr;
    }
}

void UnitTestGUI::runUnitTest1()
{
    
}
void UnitTestGUI::runUnitTest2()
{
}

void UnitTestGUI::buttonClicked (Button* btn)
{
    if (unitTest != nullptr) {
        return;
    }
    if (btn == &runTest1) {
        unitTest = new AUTMtof(coms);
    }
    if (btn == &runTest2) {
        unitTest = new AUTTestTone(coms);
    }
    currentTest = btn;
    unitTest->addActionListener(this);
    unitTest->startUnitTest(1000);

}
//
