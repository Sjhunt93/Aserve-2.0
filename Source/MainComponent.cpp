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
public TextEditorListener,
public Timer,
public TextButton::Listener,
public MenuBarModel,
public ActionListener
{
public:
    
    //==============================================================================
    MainContentComponent() :
    aserveComs(audioMain), MIDIIO(aserveComs), impulse(aserveComs),
      logEnabled(true),
      oscPanelEnabled(true),
      scopeLogPanelEnabled(true),
      gridPanelEnabled(true),
    oscViewer (audioMain),
    samplerViewer (audioMain),
      impulsePanelEnabled(true),
    unitTestGUI(aserveComs)
    {
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
#ifdef SHOW_CODE_INPUT
        addAndMakeVisible(entryLabel);
#endif
        addAndMakeVisible(logText);
        
        logText.setReadOnly(true);
        entryLabel.setMultiLine(false);
        logText.setMultiLine(true);
        entryLabel.addListener(this);
        
        
        logText.setColour(TextEditor::ColourIds::backgroundColourId , Colour(25, 25, 25));
        entryLabel.setColour(TextEditor::ColourIds::backgroundColourId , Colour(25, 25, 25));
        
        entryLabel.setColour(TextEditor::ColourIds::textColourId , Colour(59, 252,52));
        logText.setColour(TextEditor::ColourIds::textColourId , Colour(59, 252,52));

        entryLabel.setColour(TextEditor::ColourIds::highlightedTextColourId , Colour(59, 252,52));
        entryLabel.setColour(TextEditor::ColourIds::highlightColourId , Colour(59, 252,52).withAlpha((UInt8)120));
        
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

        if( scopeLogPanelEnabled )  audioScope.render(bufferToFill);
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
//        const int bottomRemained  = getHeight() - bottomDiv;
        
#ifdef SHOW_CODE_INPUT
        entryLabel.setBounds(panelLeftInset + 3, bottomDiv, mid - 6, 40.0);
        logText.setBounds(entryLabel.getX(), entryLabel.getBottom() + 5, entryLabel.getWidth(), (getHeight() - 240) - (entryLabel.getBottom() + 10));
#else
        logText.setBounds(panelLeftInset + 3, bottomDiv, mid - 6, (getHeight() - panelBottomInset) - (bottomDiv));
#endif
        
        audioScope.setBounds(panelLeftInset + 3, 5, mid - 6, bottomDiv - 6);
        clearButton.setBounds(panelLeftInset + 5, 5, 50, 25);
      
        impulse.setBounds(100, getHeight() - panelBottomInset, 800, 240);
        unitTestGUI.setBounds(getWidth() - 100, 0, 100, bottomDiv);
        
        oscViewer.setBounds(0, 0, panelLeftInset, getHeight() - panelBottomInset);
        samplerViewer.setBounds(getWidth() - panelRightInset, 0, panelRightInset, 200);
        // now set in paint() to use startPoint offset
        bitGrid.setBounds(samplerViewer.getX() + ((samplerViewer.getWidth() - 210) * 0.5), samplerViewer.getBottom() + 10, 210, 210);
    }
    
    void textEditorReturnKeyPressed (TextEditor& txt) override
    {
        /*
         Only used if the live code input is enabled
         */
        String toParse = entryLabel.getText();
        if (toParse == "play") {
//            audioMain.loadFile(0, "/Users/sj4-hunt/Music/iTunes/iTunes Media/Music/Dream Theater/Awake/01 6_00.wav");
//
//            audioMain.playFile(0, 1.0);
        }
        if (toParse == "sample") {
            audioMain.triggerSampledNote(0, arc4random() % 12 + 60, 120);
        }
        if (toParse == "killall();") {
            audioMain.stopAll();
        }
        
        LiveTextParser::Parsed parsed = LiveTextParser::parse(toParse);
        String thingToAdd = Time::getCurrentTime().toString(false, true) + " :: ";
        //+ logText.getText() + "\n";
        if (parsed.error.length()) {
            thingToAdd += entryLabel.getText() + " :: ERROR! " + parsed.error + "\n";
        }
        else {
            thingToAdd += entryLabel.getText() + "\n";
         //   entryLabel.clear();
            
            if (parsed.type == LiveTextParser::eOsc) {
                int channel = parsed.data[0];
                float frequency = parsed.data[1];
                float amp = parsed.data[2];
                int wavetype = parsed.data[3];
                if (channel >= 0 && channel < 16 /*&& amp >= 0 && amp <= 1.0*/) {
                    
                    
                    audioMain.getOscs().setFrequency(channel, frequency);
                    audioMain.getOscs().setAmplitude(channel, amp);
                    audioMain.getOscs().setWaveform(channel, wavetype);
                }
            }
            if (parsed.type == LiveTextParser::eLpf) {
                float lpf = parsed.data[0];
                if (lpf > 20.0 && lpf < 20000) {
                    audioMain.setLPF(lpf);
                }
                
                
            }
            repaint();

        }
        logText.setText(thingToAdd + logText.getText());
    }
    
  
    StringArray getMenuBarNames() override
    {
        return {"MIDI", "Log", "View"};
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
            pmenu.addItem(4, "Impulse", true, unitTestEnabled);
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

    // Your private member variables go here...
    
    TextEditor          entryLabel, logText;
    TextButton          clearButton;
    
    AudioMain           audioMain;
    
    //Visualiser components
    Scope               audioScope;
    OscillatorViewer    oscViewer;
    SamplerStateViewer  samplerViewer;
    
    BitVisualiser       bitGrid;
    
    AserveComs          aserveComs;
    MIDIIO              MIDIIO;
    ImpulseController   impulse;
//    AserveUnitTest      unitTester;
    UnitTestGUI         unitTestGUI;
    
    
    std::vector<int>    midi;
  
    // enable/disable logging of messages
    bool                logEnabled;
  
    // show and enable / hide and disable GUI panels
    int                 panelRightInset, panelLeftInset, panelBottomInset;
    bool                oscPanelEnabled, scopeLogPanelEnabled, gridPanelEnabled, impulsePanelEnabled;
    bool                unitTestEnabled;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
