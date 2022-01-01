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
class MelodrumaticAudioProcessor  : public AudioProcessor,
                                    public InterprocessConnection
{
public:
    //==============================================================================
    // Static variables
    static std::map<String, bool> _hasInterprocessPipeBeenCreated;
    static std::map<String, int> _numProcessesConnectedToInterprocessPipe;
    
    //==============================================================================
    MelodrumaticAudioProcessor();
    ~MelodrumaticAudioProcessor();

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
    // Get the RMSE based on latest sample values
    float getRMSE();
    
private:
    //==============================================================================
    // Internal processing methods (called in processBlock())
    void processAudio(AudioBuffer<float>& buffer);
    void processMidi(MidiBuffer& midiMessages);
    
    //==============================================================================
    // Helpers
    
    // Calculate linear interpolation for given values
    float lerp(float x0, float x1, float t); // t = "inPhase"
            
    // Convert MIDI note to frequency in Hz
    float midiNoteToHz(float midiNote);
    
    // Interprocess helpers
    bool createOrConnectToInterprocessPipe();
    void initializeInterprocessStaticVariables();
    String generateProcessName();
    String getInterprocessPipeFullName();
    String getInterprocessPipeSuffix();
    void setInterprocessPipeSuffix(String suffix, bool fromSetStateInformation);
    
    
    //==============================================================================
    // Constants for delay
    const float _minDelayTime = 1.f; // Min and max time are basically MIDI values now
    const float _maxDelayTime = 127.f;
    const float _delayTimeMultiplier = 0.002f;
    const float _minDelayTimeSmoothAmount = 0.01f;
    const float _maxDelayTimeSmoothAmount = 1.f; // Normalized, 0-1
    
    
    //==============================================================================
    // Circular buffer
    std::unique_ptr<float[]> _circularBufferLeft = nullptr;
    std::unique_ptr<float[]> _circularBufferRight = nullptr;
    int _circularBufferWriteHead = 0;
    int _circularBufferLength = 0;
    
    // Delay
    float _delayTimeInSamples = 0.f;
    float _delayReadHead = 0.f;
    float _delayTimeSmoothed = 0.f;
    
    // Feedback
    float _feedbackLeft = 0.f;
    float _feedbackRight = 0.f;
    
    // RMSE
    float _rmse = 0.f;
    

    //==============================================================================
    // Interprocess
    const int _interprocessCreatePipeTimeoutMs = -1;
    const int _interprocessConnectToPipeTimeoutMs = -1; // (Infinite timeout)
    const String _interprocessPipeBaseName = "MELODRUMATIC_INTERPROCESS_PIPE";
    String _processName = generateProcessName();
    String _interprocessPipeSuffix = "DEFAULT";
    bool _didCurrentInstanceCreateInterprocessPipe = false;
    const bool _shouldCreateInterprocessPipe = false; // Skip IPC as of v0.1
    
    //==============================================================================
    TrackProperties _trackProperties = {};
    
    //==============================================================================
    // User-controlled parameters
    AudioParameterFloat* _dryWetParam = nullptr;
    AudioParameterFloat* _feedbackParam = nullptr;
    AudioParameterFloat* _delayTimeParam = nullptr;
    AudioParameterFloat* _delayTimeSmoothAmountParam = nullptr;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MelodrumaticAudioProcessor)
};
