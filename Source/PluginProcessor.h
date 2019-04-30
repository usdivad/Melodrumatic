/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define MAX_DELAY_TIME_IN_SECONDS 2
#define DELAY_TIME_IN_SECONDS 0.5
#define FEEDBACK_RATE 0.8

//==============================================================================
/**
*/
class DaalDel2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DaalDel2AudioProcessor();
    ~DaalDel2AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    // Circular buffer
    float* _circularBufferLeft;
    float* _circularBufferRight;
    int _circularBufferWriteHead;
    int _circularBufferLength;
    
    // Delay
    float _delayTimeInSamples;
    float _delayReadHead;
    
    // Feedback
    float _feedbackLeft;
    float _feedbackRight;
    
    // Dry/wet
    float _dryWet;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalDel2AudioProcessor)
};
