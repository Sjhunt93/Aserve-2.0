/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorManager.h"
#include "LiveTextParser.h"
#include "Scope.h"
#include "AudioMain.h"
#include "MIDIIO.h"
#include "midiManager.h"
#include "ImpulseComponent.h"
#include "BitVisualiser.h"
#include "AserveComs.h"
#include "AUTMtof.hpp"
#include "UnitTestUI.hpp"
#include "OscillatorViewer.hpp"
#include "SamplerStateViewer.hpp"

#undef SHOW_CODE_INPUT

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainContentComponent   :
public AudioAppComponent,
public Timer,
public TextButton::Listener,
public MenuBarModel,
public ActionListener
{
public:
    
    //==============================================================================
    MainContentComponent() :
    oscViewer (audioMain),
    samplerViewer (audioMain),
    impulse(aserveComs),
    aserveComs(audioMain),
    MIDIIO(aserveComs),
    unitTestGUI(aserveComs)
    {
        setAudioChannels (2, 2);
        
        oscPanelEnabled = scopeLogPanelEnabled = gridPanelEnabled = impulsePanelEnabled = logEnabled = true;
        unitTestEnabled = false;
        
        addAndMakeVisible(logText);
        
        logText.setReadOnly(true);
        logText.setMultiLine(true);
        
        
        logText.setColour(TextEditor::ColourIds::backgroundColourId , Colour(25, 25, 25));
        logText.setColour(TextEditor::ColourIds::textColourId , Colour(59, 252,52));
        
        
        addAndMakeVisible(audioScope);
        addAndMakeVisible(oscViewer);
        addAndMakeVisible(samplerViewer);
        addAndMakeVisible(impulse);
        addAndMakeVisible(bitGrid);
        
        addAndMakeVisible(clearButton);
        clearButton.setButtonText("Reset");
        clearButton.addListener(this);
        
        aserveComs.addActionListener(this);
        bitGrid.addActionListener(this);
        
        setSize (1000, 700);
        
        startTimer(50);
        
        addAndMakeVisible(unitTestGUI);
        
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }
    void buttonClicked (Button* btn) override
    {
        if (btn == &clearButton) {
            audioMain.reset();
        }
    }
    void timerCallback() override
    {
        
        if (aserveComs.checkAndClearRedraw()) {
            repaint();
        }
        {
            
            StringArray totalStrings = aserveComs.getAndClearMessageLog();
            if (!totalStrings.size()) {
                return;
            }
            
            
            if (logText.getTotalNumChars() > 10000) {
                logText.clear();
            }
            
            String total = "";
            
            for (String st : totalStrings) {
                String finalString = (Time::getCurrentTime().toString(false, true) + " :: " + st + "\n");
                
                logText.setColour(TextEditor::ColourIds::textColourId , st.startsWith("ERROR") ? Colours::crimson : Colour(59, 252,52));
                
                logText.insertTextAtCaret(finalString);
            }
            logText.setCaretPosition(logText.getText().length());
        }
        
    }
    
    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        audioMain.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        
        audioMain.getNextAudioBlock(bufferToFill);
        
        if (scopeLogPanelEnabled) {
            audioScope.render(bufferToFill);
        }
    }
    
    void releaseResources() override
    {
        audioMain.releaseResources();
        MIDIIO.clearAllMidiInputs();
    }
    
    void setPanelInsets()
    {
        // insets depend on visibility of bit grid / sampler state / impulse panel
        panelLeftInset = 0;
        panelRightInset = 0;
        panelBottomInset = 0;
        
        if (impulsePanelEnabled) {  panelBottomInset = 240; }
        if (oscPanelEnabled) {      panelLeftInset = 200; }
        if (gridPanelEnabled) {     panelRightInset = 250; }
    }
    
    //=======================================================================
    void paint (Graphics& g) override
    {
        setPanelInsets(); // 'inset' parameters depend on visibility of various panels
        
        g.fillAll (Colours::black);
        
        // left and right panels
        g.setColour(Colours::darkgrey);
        g.fillRect(0,0, (int) panelLeftInset, getHeight());
        g.fillRect(getWidth() - panelRightInset, 0, panelRightInset, getHeight());
        
        // bottom panel
        g.setColour(Colours::darkgrey.darker());
        
        g.fillRect(0, getHeight() - panelBottomInset, getWidth(), panelBottomInset);
        
    }
    
    void resized() override
    {
        setPanelInsets();
        
        const int bottomDiv = (getHeight() - panelBottomInset) * 0.6;
        const int mid = getWidth() - (panelLeftInset + panelRightInset);
        
        
        logText.setBounds(panelLeftInset + 3, bottomDiv, mid - 6, (getHeight() - panelBottomInset) - (bottomDiv));
        
        
        audioScope.setBounds(panelLeftInset + 3, 5, mid - 6, bottomDiv - 6);
        clearButton.setBounds(panelLeftInset + 5, 5, 50, 25);
        
        impulse.setBounds(100, getHeight() - panelBottomInset, 800, 240);
        
        
        oscViewer.setBounds(0, 0, panelLeftInset, getHeight() - panelBottomInset);
        samplerViewer.setBounds(getWidth() - panelRightInset, 0, panelRightInset, 200);
        // now set in paint() to use startPoint offset
        bitGrid.setBounds(samplerViewer.getX() + ((samplerViewer.getWidth() - 210) * 0.5), samplerViewer.getBottom() + 10, 210, 210);
        
        
        unitTestGUI.setBounds(getWidth() - panelRightInset, 0, unitTestEnabled ? panelRightInset : 0, getHeight() - panelBottomInset);
        
    }
    
    
    
    StringArray getMenuBarNames() override
    {
        return {"MIDI", "Log", "View", "Settings"};
    }
    
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override
    {
        //we can assume the following code as there is only 1 menu..
        PopupMenu pmenu;
        if (topLevelMenuIndex == 0) // MIDI
        {
            StringArray inputs = MIDIIO.getMidiNames();
            if (!inputs.size()) {
                pmenu.addItem(1, "no inputs available", false, false);
            }
            for (int i = 0; i < inputs.size(); i++) {
                bool state = MIDIIO.getState(i);
                pmenu.addItem(i+1, inputs[i], true, state);
            }
        }
        else if (topLevelMenuIndex == 1) // Log
        {
            pmenu.addItem(1, "log enabled", true, logEnabled);
        }
        else if (topLevelMenuIndex == 2) // View
        {
            pmenu.addItem(1, "Oscillators", true, oscPanelEnabled);
            pmenu.addItem(2, "Scope & Log", true, scopeLogPanelEnabled);
            pmenu.addItem(3, "Bit grid & Samplers", true, gridPanelEnabled);
            pmenu.addItem(4, "Impulse", true, impulsePanelEnabled);
            pmenu.addItem(5, "Unit Tests", true, unitTestEnabled);
        }
        else if (topLevelMenuIndex == 3) // View
        {
            pmenu.addItem(1, "Unit test setup", true);
        }
        return pmenu;
    }
    
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override
    {
        if (topLevelMenuIndex == 0) // MIDI
        {
            MIDIIO.setState(menuItemID-1, !MIDIIO.getState(menuItemID-1));
        }
        else if (topLevelMenuIndex == 1) // Log
        {
            logEnabled = !logEnabled;
            aserveComs.enableLoggger(logEnabled);
        }
        else if (topLevelMenuIndex == 2) // View
        {
            if (menuItemID == 1) // Oscillators
            {
                oscPanelEnabled = !oscPanelEnabled;
            }
            else if (menuItemID == 2) // Scope
            {
                scopeLogPanelEnabled = !scopeLogPanelEnabled;
                clearButton.setVisible(scopeLogPanelEnabled);
                audioScope.setVisible(scopeLogPanelEnabled);
                logText.setVisible(scopeLogPanelEnabled);
            }
            else if (menuItemID == 3) // Bit grid
            {
                gridPanelEnabled = !gridPanelEnabled;
                bitGrid.setVisible(gridPanelEnabled);
            }
            else if (menuItemID == 4) // Impulse
            {
                impulsePanelEnabled = !impulsePanelEnabled;
                impulse.setVisible(impulsePanelEnabled);
            }
            else if (menuItemID == 5)
            {
                unitTestEnabled = !unitTestEnabled;
            }
            
            resized();
            repaint();
        }
        else if (topLevelMenuIndex == 3) {
            //            AlertWindow window;
            AlertWindow al("Unit Test Setup", "Only mess around here if you are having problems with your unit tests saving...", AlertWindow::QuestionIcon);
            al.addTextBlock("Enter Your project path here:");
            al.addTextEditor("Project Path", AserveUnitTest::projectPath);
            al.addTextBlock("Enter Your solutions path here:");
            al.addTextEditor("Solutions Path", AserveUnitTest::solutionsPath);
            
            al.addButton("Ok", 1);
            al.addButton("Cancel", 2);
            
            const int value = al.runModalLoop();
            if (value == 1) {
                const String project = al.getTextEditor("Project Path")->getText();
                const String solutions = al.getTextEditor("Solutions Path")->getText();
            }

        }
    }
    
    void actionListenerCallback (const String& message) override
    {
        if (message.startsWith("PIXEL")) {
            const int a = message.fromFirstOccurrenceOf(":", false, false).upToFirstOccurrenceOf(",", false, false).getIntValue();
            const int b = message.fromLastOccurrenceOf(",", false, false).getIntValue();
            bitGrid.set(b, a);
        }
        if (message.startsWith("GRID")) {
            const int x = message.fromFirstOccurrenceOf(":", false, false).upToFirstOccurrenceOf(",", false, false).getIntValue();
            const int y = message.fromLastOccurrenceOf(",", false, false).getIntValue();
            aserveComs.sendGridMessage(x, y);
        }
    }
    
    
private:
    //==============================================================================
    
    
    
    AudioMain           audioMain;
    
    //Visualiser components
    Scope               audioScope;
    OscillatorViewer    oscViewer;
    SamplerStateViewer  samplerViewer;
    BitVisualiser       bitGrid;
    ImpulseController   impulse;
    
    // MIDI OSC coms
    AserveComs          aserveComs;
    MIDIIO              MIDIIO;
    
    TextEditor          logText;
    TextButton          clearButton;

    // unit tester
    UnitTestGUI         unitTestGUI;
    
    // enable/disable logging of messages
    bool                logEnabled;
    
    // show and enable / hide and disable GUI panels
    int                 panelRightInset, panelLeftInset, panelBottomInset;
    bool                oscPanelEnabled, scopeLogPanelEnabled, gridPanelEnabled, impulsePanelEnabled, unitTestEnabled;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED

