//
//  UnitTestUI.cpp
//  Aserve 2.0 - App
//
//  Created by Samuel Hunt on 19/06/2019.
//

#include "UnitTestUI.hpp"


TestSelector::TestSelector (String name)
{
//    addAndMakeVisible(label);
    label.setText(name, dontSendNotification);
    selected = false;
}
TestSelector::~TestSelector ()
{
    
}
void TestSelector::resized ()
{
    label.setBounds(3, 3, getWidth() - 40, getHeight() - 6);
}
void TestSelector::paint (Graphics & g)
{
    
    
    g.fillAll(Colours::lightgrey);
    g.setColour(Colours::black);
    g.drawText(label.getText(), 3, 3, getWidth() - 40, getHeight() - 6, Justification::left);
    
    if (selected) {
        g.setColour(Colours::greenyellow);
        g.drawRect(0, 0, getWidth(), getHeight(), 3.0);
    }
    
    if (state == AserveUnitTest::eEndedFail) {
        g.setColour(Colours::red);
    }
    else if (state == AserveUnitTest::eEndedPass) {
        g.setColour(Colours::green);
    }
    else if (state == AserveUnitTest::eRunning) {
        g.setColour(Colours::orange);
    }
    else {
        g.setColour(Colours::darkgrey);
    }
    g.fillEllipse(getWidth() - getHeight() - 3, 3, getHeight()-6, getHeight()-6);

}
void TestSelector::setState (AserveUnitTest::eTestState s)
{
    state = s;
    repaint();
}

void TestSelector::mouseUp (const MouseEvent & event)
{
    sendActionMessage("Selected:" + label.getText());
}

const String TestSelector::name ()
{
    return label.getText();
}

UnitTestGUI::UnitTestGUI (AserveComs & _coms) : coms(_coms)
{
    addAndMakeVisible(runTest1);
    runTest1.setButtonText("Run Selected Test");
    runTest1.addListener(this);
    
    
    
    StringArray array = AserveUnitTest::getTestList();
    for (auto a : array) {
        TestSelector * ts = new TestSelector(a);

        ts->addActionListener(this);
        compList.getComponentList()->addNewComponet(ts);
        ts->setState(AserveUnitTest::getStateForTest(a));
        
    }
    addAndMakeVisible(compList);
    addAndMakeVisible(results);
    results.setText("Errors will be shown here: ");
    results.setMultiLine(true);
}
UnitTestGUI::~UnitTestGUI ()
{
    
}

void UnitTestGUI::paint (Graphics &g)
{
    g.fillAll(Colours::darkgrey);
    g.setColour(Colours::white);
    g.drawText("Unit Tests", 0, 0, getWidth(), 50, Justification::centred);
}
void UnitTestGUI::resized ()
{
    
    
    compList.setBounds(3, 43, getWidth() - 6, getHeight() - 200);
    runTest1.setBounds(5, compList.getBottom() + 5, 100, 30);
    const int dif = (getHeight() - runTest1.getBottom()) - 10;
    
    results.setBounds(5, runTest1.getBottom() + 5, getWidth() - 10, dif);
}

void UnitTestGUI::actionListenerCallback (const String& message)
{
    if (message.startsWith("finished") && unitTest != nullptr) {
        std::cout << unitTest->getErrors() << " : Unit result: " << unitTest->getResult() << "\n";
//        if (unitTest->getResult() == 3) {
//            currentTest->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
//        }
//        if (unitTest->getResult() == 2) {
//            currentTest->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
//        }
        unitTest->saveToFile();
        results.setText(unitTest->getErrors());
        
        TestSelector * selector = getSelectorForName(selectedTest);
        if (selector != nullptr) {
            selector->setState(unitTest->getResult());
        }
        unitTest = nullptr;
        coms.reset();
        runTest1.setButtonText("Run Selected Test");

    }
    if (message.startsWith("Selected:")) {
        String testName = message.fromFirstOccurrenceOf(":", false, false);
        /*
        unitTest = AserveUnitTest::allocateForTest(testName, coms);
        if (unitTest != nullptr) {
            unitTest->addActionListener(this);
        }
         */
        selectedTest = testName;
        StringArray sArray = AserveUnitTest::getTestList();
        resetSelectors(sArray.indexOf(testName));
        
    }
}


void UnitTestGUI::buttonClicked (Button* btn)
{
    // if a test is running
    if (unitTest != nullptr) {
        unitTest->signalThreadShouldExit();
        while (unitTest->isThreadRunning()) {
            Thread::sleep(10);
        }
        unitTest = nullptr;
        coms.reset();
        runTest1.setButtonText("Run Selected Test");
        
        TestSelector * selector = getSelectorForName(selectedTest);
        if (selector != nullptr) {
            selector->setState(AserveUnitTest::eNone);
        }
        results.setText("Test Stopped!");
        return;
    }
  
    if (btn == &runTest1) {
        coms.reset();
        
        unitTest = AserveUnitTest::allocateForTest(selectedTest, coms);
        if (unitTest != nullptr) {
            runTest1.setButtonText("Stop Test");
            results.setText("");

            unitTest->addActionListener(this);
            unitTest->startUnitTest(1000);
            TestSelector * selector = getSelectorForName(selectedTest);
            if (selector != nullptr) {
                selector->setState(AserveUnitTest::eRunning);
            }
        }
        else
        {
          results.setText("ERROR - cannot start test");
        }
    }
    

}
//
TestSelector * UnitTestGUI::getSelectorForName (String name)
{
    for (int i = 0; i < compList.getComponentList()->getTotalComponents(); i++) {
        Component * comp = compList.getComponentList()->getComponentAtPostion(i);
        TestSelector * tt = dynamic_cast<TestSelector *>(comp);
        if (tt != nullptr) {
            if (tt->name() == name) {
                return tt;
            }
        }
    }
    return nullptr;
}

void UnitTestGUI::resetSelectors (int idSelect)
{
    for (int i = 0; i < compList.getComponentList()->getTotalComponents(); i++) {
        Component * comp = compList.getComponentList()->getComponentAtPostion(i);
        TestSelector * tt = dynamic_cast<TestSelector *>(comp);
        if (tt != nullptr) {
            tt->selected = false;
            if (i == idSelect) {
            tt->selected = true;
            }
            tt->repaint();
        }
        
    }
}
