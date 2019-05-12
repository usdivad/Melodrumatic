/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
bool DaalDel2AudioProcessor::_hasInterprocessPipeBeenCreated;

//==============================================================================
DaalDel2AudioProcessor::DaalDel2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        InterprocessConnection(true, 0xa1b2c3d4)
#endif
{
    // Audio
    // Circular buffers are set to null pointers because we don't know sample rate yet, and thus don't know how to instantiate the audio data. These will be set in prepareToPlay()
    _circularBufferLeft = nullptr;
    _circularBufferRight = nullptr;
    _circularBufferWriteHead = 0;
    _circularBufferLength = 0;
    
    _delayTimeInSamples = 0;
    _delayReadHead = 0;
    _delayTimeSmoothed = 0;
    
    _feedbackLeft = 0;
    _feedbackRight = 0;
    
    
    // Interprocess
    // _interprocessPipeName = "DAALDEL2_INTERPROCESS_PIPE_" + generateProcessName();
    _processName = generateProcessName();
    createOrConnectToInterprocessPipe(); // Create pipe, or connect to existing pipe
    
    // Parameters
    addParameter(_dryWetParam = new AudioParameterFloat("dryWet", "Dry/Wet", 0, 1, 0.5));
    addParameter(_feedbackParam = new AudioParameterFloat("feedback", "Feedback", 0, 0.98, 0.5));
    // addParameter(_delayTimeParam = new AudioParameterFloat("delayTime", "Delay Time", 0.01, _maxDelayTime, 0.5));
    addParameter(_delayTimeParam = new AudioParameterFloat("delayTime", "Delay Time", _minDelayTime, _maxDelayTime, _maxDelayTime));
}

DaalDel2AudioProcessor::~DaalDel2AudioProcessor()
{
    // Free up memory for circular buffers if necessary
    if (_circularBufferLeft != nullptr) {
        delete [] _circularBufferLeft;
        _circularBufferLeft = nullptr;
    }
    if (_circularBufferRight != nullptr) {
        delete [] _circularBufferRight;
        _circularBufferRight = nullptr;
    }
}

//==============================================================================
const String DaalDel2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DaalDel2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DaalDel2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DaalDel2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DaalDel2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DaalDel2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DaalDel2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DaalDel2AudioProcessor::setCurrentProgram (int index)
{
}

const String DaalDel2AudioProcessor::getProgramName (int index)
{
    return {};
}

void DaalDel2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DaalDel2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Delay time
    _delayTimeInSamples = sampleRate * _delayTimeParam->get();
    _delayTimeSmoothed = _delayTimeParam->get();
    
    // Calculate circular buffer length based on sample rate
    _circularBufferLength = (int)(sampleRate * _maxDelayTime);
    
    // Initialize circular buffers based on sample rate and delay time
    // The nullptr setting allows for changing sample rates in the middle of a session
    if (_circularBufferLeft != nullptr) { // Left
        delete [] _circularBufferLeft;
        _circularBufferLeft = nullptr;
    }
    if (_circularBufferLeft == nullptr) {
        _circularBufferLeft = new float[_circularBufferLength];
    }
    zeromem(_circularBufferLeft, _circularBufferLength * sizeof(float));
    
    if (_circularBufferRight != nullptr) { // Right
        delete [] _circularBufferRight;
        _circularBufferRight = nullptr;
    }
    if (_circularBufferRight == nullptr) {
        _circularBufferRight = new float[_circularBufferLength];
    }
    zeromem(_circularBufferLeft, _circularBufferLength * sizeof(float));
    
    // Initialize write head
    _circularBufferWriteHead = 0;
}

void DaalDel2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DaalDel2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DaalDel2AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // ================================================================
    // Interprocess
    
    // Connect (in case we lost the connection)
    bool isInterprocessConnectToPipeSuccessful = isConnected();
    if (!isInterprocessConnectToPipeSuccessful) {
        bool isInterprocessConnectToPipeSuccessful = createOrConnectToInterprocessPipe();
        // isInterprocessConnectToPipeSuccessful = connectToPipe(_interprocessPipeName, _interprocessConnectToPipeTimeoutMs);
        
        // Print connection status
        if (isInterprocessConnectToPipeSuccessful) {
            DBG(_processName << " (" << _trackProperties.name << "): " << "Reconnection to pipe succeeded");
        }
        else {
            DBG(_processName << " (" << _trackProperties.name << "): " << "Reconnection to pipe failed");
        }
    }
    
    
    // ================================================================
    // MIDI
    
    if (!midiMessages.isEmpty()) {
        // Loop through MIDI messages
        MidiBuffer::Iterator midiMessagesIterator = MidiBuffer::Iterator(midiMessages);
        bool hasNewMidiMessages = true;
        while (hasNewMidiMessages) {
            MidiMessage midiMessage;
            int midiSamplePosition;
            hasNewMidiMessages = midiMessagesIterator.getNextEvent(midiMessage, midiSamplePosition);
            if (!midiMessage.isSysEx()) {
                DBG(_processName << " (" << _trackProperties.name << "): " << "MIDI message : " << midiMessage.getDescription()); // TEMP
                
                // Only handle note-on messages
                if (midiMessage.isNoteOn()) {
                    // Construct MIDI message in memory block
                    BigInteger midiNote = BigInteger(midiMessage.getNoteNumber());
                    MemoryBlock midiMessageToSend = MemoryBlock();
                    midiMessageToSend.insert(&midiNote, sizeof(BigInteger), 0);
                    
                    // Send message if connection was successful
                    if (isInterprocessConnectToPipeSuccessful) {
                        DBG(_processName << " (" << _trackProperties.name << "): " << "Sending MIDI message " << midiNote.toString(10));
                        bool didSendMessageSucceed = sendMessage(midiMessageToSend);
                        if (didSendMessageSucceed) {
                            DBG(_processName << " (" << _trackProperties.name << "): " << "Send message succeeded");
                        }
                        else {
                            DBG(_processName << " (" << _trackProperties.name << "): " << "Send message failed");
                        }
                    }
                }
                
            }
        }
    }
    
    // ================================================================
    // Audio
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//     for (int channel = 0; channel < totalNumInputChannels; ++channel)
//     {
//         auto* channelData = buffer.getWritePointer (channel);
//
//         // ..do something to the data...
//     }
    
    // Get write pointers for left and right channels
    // TODO: Ensure this works for mono and > 2 channels!
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    // Write to circular buffer
    for (int i=0; i<buffer.getNumSamples(); i++) {
        
        // Smooth delay
        _delayTimeSmoothed = _delayTimeSmoothed - (_delayTimeSmoothAmount * (_delayTimeSmoothed - _delayTimeParam->get()));
        
        // Update delay time in samples based on sample rate, smoothed, and multiplier
        _delayTimeInSamples = getSampleRate() * _delayTimeSmoothed * _delayTimeMultiplier;
        
        // Write sample to circular buffer
        // and also add feedback
        _circularBufferLeft[_circularBufferWriteHead] = leftChannel[i] + _feedbackLeft;
        _circularBufferRight[_circularBufferWriteHead] = rightChannel[i] + _feedbackRight;
        
        // Read from delayed position in buffer
        // _delayReadHead = _circularBufferWriteHead - _delayTimeInSamples;
        _delayReadHead = _circularBufferWriteHead - _delayTimeInSamples;
        if (_delayReadHead < 0) {
            _delayReadHead += _circularBufferLength;
        }
        
        // Lerp!
        int delayReadHeadIntX0 = (int) _delayReadHead; // x0
        float delayReadHeadRemainderX0 = _delayReadHead - delayReadHeadIntX0; // t, i.e. inPhase
        int delayReadHeadIntX1 = delayReadHeadIntX0 + 1; // x1
        if (delayReadHeadIntX1 >= _circularBufferLength) {
            delayReadHeadIntX1 -= _circularBufferLength;
        }
        
        // Get current delay sample for applying feedback
        float delaySampleLeft = lerp(_circularBufferLeft[(int)delayReadHeadIntX0],
                                     _circularBufferLeft[(int)delayReadHeadIntX1],
                                     delayReadHeadRemainderX0);
        float delaySampleRight = lerp(_circularBufferRight[(int)delayReadHeadIntX0],
                                     _circularBufferRight[(int)delayReadHeadIntX1],
                                     delayReadHeadRemainderX0);
        
        // Apply feedback (for next iteration)
        _feedbackLeft = delaySampleLeft * _feedbackParam->get();
        _feedbackRight = delaySampleRight * _feedbackParam->get();
        
        // // Add the samples to the output buffer
        // buffer.addSample(0, i, delaySampleLeft);
        // buffer.addSample(1, i, delaySampleRight);
        
        // Sum the dry and wet (delayed) samples
        buffer.setSample(0, i, (buffer.getSample(0, i) * (1 - _dryWetParam->get())) +
                               (delaySampleLeft * _dryWetParam->get()));
        buffer.setSample(1, i, (buffer.getSample(1, i) * (1 - _dryWetParam->get())) +
                         (delaySampleLeft * _dryWetParam->get()));
        
        
        // Increment write head
        // _circularBufferWriteHead = (_circularBufferWriteHead + 1) % _circularBufferLength;
        _circularBufferWriteHead++;
        if (_circularBufferWriteHead >= _circularBufferLength) {
            _circularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool DaalDel2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DaalDel2AudioProcessor::createEditor()
{
    return new DaalDel2AudioProcessorEditor (*this);
}

//==============================================================================
void DaalDel2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DaalDel2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
void DaalDel2AudioProcessor::connectionMade()
{
    DBG(_processName << " (" << _trackProperties.name << "): " << "IPC: Connection made");
}

void DaalDel2AudioProcessor::connectionLost()
{
    DBG(_processName << " (" << _trackProperties.name << "): " << "IPC: Connection lost");
}

void DaalDel2AudioProcessor::messageReceived(const MemoryBlock &message)
{
    DBG(_processName << " (" << _trackProperties.name << "): " << "IPC: Message received: " << message.toString());
    
    // Get the MIDI note from the message
    const void* messageData = message.getData();
    const BigInteger* midiNotePtr = static_cast<const BigInteger*>(messageData);
    const BigInteger midiNote = *midiNotePtr;
    
    DBG(_processName << " (" << _trackProperties.name << "): " << "The MIDI note is " << midiNote.toString(10));

    
}

//==============================================================================
// Custom


// Linear interpolation
float DaalDel2AudioProcessor::lerp(float x0, float x1, float t)
{
    return (1 - t) * x0 + t * x1;
}

// Create pipe, or connect to existing pipe
bool DaalDel2AudioProcessor::createOrConnectToInterprocessPipe()
{
    // Create pipe
    bool didCurrentInstanceCreateInterprocessPipe = false;
    DBG("_hasInterprocessPipeBeenCreated=" << (_hasInterprocessPipeBeenCreated ? "true" : "false"));
    if (!_hasInterprocessPipeBeenCreated)
    {
        _hasInterprocessPipeBeenCreated = createPipe(_interprocessPipeName, _interprocessCreatePipeTimeoutMs, false);
        didCurrentInstanceCreateInterprocessPipe = _hasInterprocessPipeBeenCreated;
    }
    else {
        // pass
    }
    
    // Connect to existing pipe
    if (!didCurrentInstanceCreateInterprocessPipe) {
        bool isInterprocessConnectToPipeSuccessful = connectToPipe(_interprocessPipeName, _interprocessConnectToPipeTimeoutMs);
        if (!isInterprocessConnectToPipeSuccessful) {
            DBG(_processName << " (" << _trackProperties.name << "): " << "Unsuccessful connection to pipe " << _interprocessPipeName);
        }
        else {
            DBG(_processName << " (" << _trackProperties.name << "): " << "Successfully connected to pipe " << _interprocessPipeName);
            return true;
        }
    }
    else {
        DBG(_processName << " (" << _trackProperties.name << "): " << "Succesfully created pipe " << _interprocessPipeName);
        return true;
    }
    
    return false;
}


String DaalDel2AudioProcessor::generateProcessName()
{
    Random rng;
    String name;
    
    for (int i=0; i<10; i++) {
        int n = rng.nextInt(9);
        name.append(String(n), 1);
    }
    
    return name;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DaalDel2AudioProcessor();
}
