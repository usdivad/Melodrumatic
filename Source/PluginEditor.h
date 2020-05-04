/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MelodrumaticLookAndFeel.h"

//==============================================================================
/**
*/
class MelodrumaticAudioProcessorEditor  : public AudioProcessorEditor,
                                          public Timer,
                                          public MidiKeyboardStateListener
{
public:
    MelodrumaticAudioProcessorEditor (MelodrumaticAudioProcessor&);
    ~MelodrumaticAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void timerCallback() override;
    void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MelodrumaticAudioProcessor& processor;
    
    //==============================================================================
    
    // Overall look and feel
    MelodrumaticLookAndFeel _lookAndFeel;
    
    // Sliders
    Slider _dryWetSlider;
    Slider _feedbackSlider;
    Slider _delayTimeSlider;
    Slider _delayTimeSmoothAmountSlider;
    
    Label _dryWetLabel;
    Label _feedbackLabel;
    Label _delayTimeLabel;
    Label _delayTimeSmoothAmountLabel;
    
    // Text
    Label _titleLabel;
    Label _interprocessPipeSuffixTextEditor; // Yes, it's technically a label, but it's editable
    Label _interprocessPipeSuffixLabel;
    Label _versionLabel;
    
    // Keys!
    MidiKeyboardState _midiKeyboardState;
    MidiKeyboardComponent _midiKeyboardComponent;
    
    // Hyperlinks
    HyperlinkButton _titleHyperlink;
    HyperlinkButton _logoHyperlink;
    
    // Misc
    float _prevRMSE;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MelodrumaticAudioProcessorEditor)
};
