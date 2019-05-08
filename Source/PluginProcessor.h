/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class DaalDel2AudioProcessor  : public AudioProcessor,
                                public InterprocessConnection
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

    
    //==============================================================================
    // InterprocessConnection virtual methods
    void connectionMade() override;
    void connectionLost() override;
    void messageReceived(const MemoryBlock& message) override;
    
    //==============================================================================
    float lerp(float x0, float x1, float t); // t = "inPhase"
    
    
private:
    //==============================================================================
    // Constants for delay
    const float _minDelayTime = 0.1; // Min and max time are no longer in seconds due to multiplier
    const float _maxDelayTime = 60.0;
    const float _delayTimeSmoothAmount = 0.001;
    const float _delayTimeMultiplier = 0.001;
    
    //==============================================================================
    // Circular buffer
    float* _circularBufferLeft;
    float* _circularBufferRight;
    int _circularBufferWriteHead;
    int _circularBufferLength;
    
    // Delay
    float _delayTimeInSamples;
    float _delayReadHead;
    float _delayTimeSmoothed;
    
    // Feedback
    float _feedbackLeft;
    float _feedbackRight;
    

    //==============================================================================
    // Interprocess
    const String _interprocessPipeName = "DAALDEL2_INTERPROCESS_PIPE";
    const int _interprocessPipeTimeoutMs = 50;
    
    //==============================================================================
    // User-controlled parameters
    AudioParameterFloat* _dryWetParam;
    AudioParameterFloat* _feedbackParam;
    AudioParameterFloat* _delayTimeParam;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalDel2AudioProcessor)
};
