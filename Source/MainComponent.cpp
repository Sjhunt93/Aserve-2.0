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
    MainContentComponent() : aserveComs(audioMain), MIDIIO(aserveComs), impulse(aserveComs)
    {
        setSize (1000, 840);

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
        
        addAndMakeVisible(impulse);
        addAndMakeVisible(bitGrid);
        
        addAndMakeVisible(clearButton);
        clearButton.setButtonText("Reset");
        clearButton.addListener(this);
        
        aserveComs.addActionListener(this);
        bitGrid.addActionListener(this);
        
        logEnabled = true;
        startTimer(50);

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
            
#if 0
            String final;
            bool setRed = false;
            for (String st : totalStrings) {
                final += (Time::getCurrentTime().toString(false, true) + " :: " + st + "\n");
                if (st.startsWith("ERROR")) {
                    setRed = true;
                }
            }
            
            String lastStr = logText.getText();
            if (lastStr.length() > 1000) {
                lastStr = lastStr.substring(lastStr.length() - 1000, lastStr.length());
                lastStr = lastStr.fromFirstOccurrenceOf("\n", false, false);
            }
            
            logText.setText( lastStr);
            logText.setCaretPosition(logText.getText().length());
            
            logText.setColour(TextEditor::ColourIds::textColourId , setRed ? Colours::crimson : Colour(59, 252,52));
            
            logText.insertTextAtCaret(final);
#else

            if (logText.getTotalNumChars() > 10000) {
                logText.clear();
            }
            
            String total = "";
            
            for (String st : totalStrings) {
                String final = (Time::getCurrentTime().toString(false, true) + " :: " + st + "\n");
                
             //   logText.setCaretPosition(logText.getText().length());

                logText.setColour(TextEditor::ColourIds::textColourId , st.startsWith("ERROR") ? Colours::crimson : Colour(59, 252,52));
                
                logText.insertTextAtCaret(final);
            }
            logText.setCaretPosition(logText.getText().length());
#endif
        }

    }

    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()

        audioMain.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        
        bufferToFill.clearActiveBufferRegion();
        
        audioMain.getNextAudioBlock(bufferToFill);

        audioScope.render(bufferToFill);
        
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
        audioMain.releaseResources();
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        
        const int leftInset = 200;
        const int rightInset = 250;
//        const int bottomDiv = (getHeight() - 240) * 0.6;
//        const int mid = getWidth() - (leftInset + rightInset);
        
        g.fillAll (Colours::black);
        
        g.setColour(Colours::darkgrey);
        g.fillRect(0,0, (int) leftInset, getHeight());
        
        g.fillRect(getWidth() - rightInset, 0, rightInset, getHeight());
        
        
        
        g.setColour(Colours::white);
        const int mode = audioMain.getOscs().getOscillatorRoutingMode();
        if (mode == OscillatorManager::eNormal) {
            g.drawText("Oscillator State (normal)", 5, 5, leftInset-10, 30, Justification::centred);
        }
        if (mode == OscillatorManager::eFm8) {
            g.drawText("Oscillator State (FM 8)", 5, 5, leftInset-10, 30, Justification::centred);
        }
        
        float startPoint = 40.0;
        
        float height = ((getHeight() - 240) - startPoint) / ((float)OscillatorManager::OscillatorManagerConstants::NumOscillators);
        
        g.setColour(Colours::darkgrey.darker());
        g.fillRect(0, getHeight() - 240, getWidth(), 240);
        
        Colour offCol = Colours::white.withAlpha(0.2f);
        
        for (int i = 0; i < OscillatorManager::OscillatorManagerConstants::NumOscillators; i++) {
            g.setColour(Colours::black);
            g.drawLine(0, startPoint, leftInset, startPoint);
            
            g.setColour( audioMain.getOscs().isActive(i) ? (  audioMain.getOscs().isOutOfRange(i) ? Colours::red : Colours::white )  : offCol );
            
            g.drawText(/*String("Oscillator: ") +*/ String(i) + " | " + audioMain.getOscs().getState(i) , 5, startPoint, leftInset, height, Justification::left);
            
            startPoint += height;
            
        }
        
        {
            g.setColour(Colours::white);
            g.drawText("Sampler State", getWidth()-rightInset, 5, rightInset , 30, Justification::centred);
            
            float startPoint = 40.0;

            
            for (int i = 0; i < AudioMain::eMaxFileTracks; i++) {
                g.setColour(Colours::black);
                g.drawLine(getWidth() - rightInset, startPoint, getWidth(), startPoint);
                
                g.setColour(audioMain.getAudioFileNames()[i] != "Empty" ? Colours::white : Colours::white.withAlpha(0.2f));
                
                g.drawText(/*"Sampler: " +*/ String(i) + " | " + audioMain.getAudioFileNames()[i]
                           , (getWidth()-rightInset) + 5, startPoint, rightInset-10, height, Justification::left);
                
                startPoint += height;

                
            }
            g.setColour(Colours::black);
            g.drawLine(getWidth() - rightInset, startPoint, getWidth(), startPoint, 3.0);

            g.setColour(Colours::white);
            g.drawText("Pitched Sampler State: ", (getWidth()-rightInset) + 5, startPoint, rightInset-10, 30, Justification::centred);
            g.drawText("Bit Gird", (getWidth()-rightInset) + 5, bitGrid.getY() - 38, rightInset-10, 30, Justification::centred);
            
            startPoint += 30;

            for (int i = 0; i < AudioMain::eMaxSamplerTracks; i++) {
                g.setColour(Colours::black);
                g.drawLine(getWidth() - rightInset, startPoint, getWidth(), startPoint);
                
                g.setColour(audioMain.getResampledNames()[i] != "Empty" ? Colours::white : Colours::white.withAlpha(0.2f));
                
                g.drawText(/*"Sampler: " +*/ String(i) + " | " + audioMain.getResampledNames()[i]
                           , (getWidth()-rightInset) + 5, startPoint, rightInset-10, height, Justification::left);
                
                startPoint += height;
                
                
            }
            g.setColour(Colours::black);
            g.drawLine(getWidth() - rightInset, startPoint, getWidth(), startPoint, 3.0);
            
        }
    }
    void paintOverChildren (Graphics& g) override
    {
        g.setColour(Colours::lightgrey);
        g.setFont(10);
        g.drawText("Version: " + String(ProjectInfo::versionString), audioScope.getX() + 5, audioScope.getBottom() - 20, 100, 20, Justification::left);
        
    }
    void resized() override
    {

        const int leftInset = 200;
        const int rightInset = 250;
        const int bottomDiv = (getHeight() - 240) * 0.6;
        const int mid = getWidth() - (leftInset + rightInset);
        const int bottomRemained  = getHeight() - bottomDiv;
        
        
#ifdef SHOW_CODE_INPUT
        entryLabel.setBounds(leftInset + 3, bottomDiv, mid - 6, 40.0);
        logText.setBounds(entryLabel.getX(), entryLabel.getBottom() + 5, entryLabel.getWidth(), (getHeight() - 240) - (entryLabel.getBottom() + 10));
#else
//        logText.setBounds(leftInset+3, bottomDiv+5, mid-6, (getHeight() - 240));
        logText.setBounds(leftInset + 3, bottomDiv, mid - 6, (getHeight() - 240) - (bottomDiv));
  
#endif
        
        audioScope.setBounds(leftInset + 3, 5, mid - 6, bottomDiv - 6);
        
        impulse.setBounds(100, getHeight() - 240, 800, 240);
        bitGrid.setBounds((getWidth() - rightInset) + 20, bottomDiv + 10, 210, 210);
        
        clearButton.setBounds(leftInset + 5, 5, 50, 25);
    }
    
    void textEditorReturnKeyPressed (TextEditor& txt) override
    {
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
        return {"MIDI", "Log"};
    }
    
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override
    {
        //we can assume the following code as there is only 1 menu..
        PopupMenu pmenu;
        if (topLevelMenuIndex == 0) {
            StringArray inputs = MIDIIO.getMidiNames();
            if (!inputs.size()) {
                pmenu.addItem(1, "no inputs available", false, false);
            }
            for (int i = 0; i < inputs.size(); i++) {
                bool state = MIDIIO.getState(i);
                pmenu.addItem(i+1, inputs[i], true, state);
            }
        }
        else {
            pmenu.addItem(1, "log enabled", true, logEnabled);
        }
        

        return pmenu;
    }
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override
    {
        if (topLevelMenuIndex == 0) {
            MIDIIO.setState(menuItemID-1, !MIDIIO.getState(menuItemID-1));
            printf("Clicked %i \n", MIDIIO.getState(menuItemID-1));
        }
        else if (topLevelMenuIndex == 1) {
            
            logEnabled = !logEnabled;
            aserveComs.enableLoggger(logEnabled);
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
    
    TextEditor          entryLabel;
    TextEditor          logText;
    TextButton          clearButton;
    
    AudioMain           audioMain;
    Scope               audioScope;

    BitVisualiser       bitGrid;
    
    AserveComs          aserveComs;
    MIDIIO              MIDIIO;
    ImpulseController   impulse;
    
    
    std::vector<int>    midi;
    bool                logEnabled;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
