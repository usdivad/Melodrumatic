/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DaalDel2AudioProcessorEditor  : public AudioProcessorEditor,
                                      public Timer
{
public:
    DaalDel2AudioProcessorEditor (DaalDel2AudioProcessor&);
    ~DaalDel2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalDel2AudioProcessor& processor;
    
    //==============================================================================
    // Sliders
    Slider _dryWetSlider;
    Slider _feedbackSlider;
    Slider _delayTimeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalDel2AudioProcessorEditor)
};
