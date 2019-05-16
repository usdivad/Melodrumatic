/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
std::map<String, bool> MelodrumaticAudioProcessor::_hasInterprocessPipeBeenCreated;
std::map<String, int> MelodrumaticAudioProcessor::_numProcessesConnectedToInterprocessPipe;

//==============================================================================
MelodrumaticAudioProcessor::MelodrumaticAudioProcessor()
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
    // _interprocessPipeName = "MELODRUMATIC_INTERPROCESS_PIPE_" + generateProcessName();
    _didCurrentInstanceCreateInterprocessPipe = false;
    _processName = generateProcessName();
    _interprocessPipeSuffix = "DEFAULT";
    initializeInterprocessStaticVariables();
    createOrConnectToInterprocessPipe(); // Create pipe, or connect to existing pipe
    
    // Parameters
    addParameter(_dryWetParam = new AudioParameterFloat("dryWet", "Dry/Wet", 0, 1, 0.5));
    addParameter(_feedbackParam = new AudioParameterFloat("feedback", "Feedback", 0, 0.98, 0.5));
    // addParameter(_delayTimeParam = new AudioParameterFloat("delayTime", "Delay Time", 0.01, _maxDelayTime, 0.5));
    addParameter(_delayTimeParam = new AudioParameterFloat("delayTime", "Delay Time", _minDelayTime, _maxDelayTime, _maxDelayTime));
}

MelodrumaticAudioProcessor::~MelodrumaticAudioProcessor()
{
    // Interprocess
    
    // Decrement
    _numProcessesConnectedToInterprocessPipe[getInterprocessPipeFullName()]--;
    
    // Reset creation flag if the creator gets destroyed
    if (_didCurrentInstanceCreateInterprocessPipe)
    {
        _hasInterprocessPipeBeenCreated[getInterprocessPipeFullName()] = false;
    }
    
    // Disconnect
    disconnect();
}

//==============================================================================
const String MelodrumaticAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MelodrumaticAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MelodrumaticAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MelodrumaticAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MelodrumaticAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MelodrumaticAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MelodrumaticAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MelodrumaticAudioProcessor::setCurrentProgram (int index)
{
}

const String MelodrumaticAudioProcessor::getProgramName (int index)
{
    return {};
}

void MelodrumaticAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MelodrumaticAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Delay time
    _delayTimeInSamples = sampleRate * _delayTimeParam->get();
    _delayTimeSmoothed = _delayTimeParam->get();
    
    // Calculate circular buffer length based on sample rate
    _circularBufferLength = (int)(sampleRate * _maxDelayTime);
    
    // Initialize circular buffers based on sample rate and delay time
    // (This allows for changing sample rates in the middle of a session)
    _circularBufferLeft.reset(new float[_circularBufferLength]);
    _circularBufferRight.reset(new float[_circularBufferLength]);
    
    // Initialize write head
    _circularBufferWriteHead = 0;
}

void MelodrumaticAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MelodrumaticAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MelodrumaticAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // ================================================================
    // Interprocess
    
    // Connect (in case we lost the connection)
    bool isInterprocessConnectToPipeSuccessful = isConnected() && _numProcessesConnectedToInterprocessPipe[getInterprocessPipeFullName()] > 1;
    DBG("_numProcessesConnectedToInterprocessPipe[" << getInterprocessPipeFullName() << "]=" << _numProcessesConnectedToInterprocessPipe[getInterprocessPipeFullName()]);
    
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
                    
                    // Update our own "delayTimeParam" just to keep UI consistent
                    *_delayTimeParam = jmax(midiNote.toInteger() + 1, 1);
                    
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
        float prevDelayTimeInSamples = _delayTimeInSamples;
        // _delayTimeInSamples = getSampleRate() * _delayTimeSmoothed * _delayTimeMultiplier; // Using smoothed delay time with multiplier
        _delayTimeInSamples = getSampleRate() * (1 / midiNoteToHz(_delayTimeSmoothed)); // MIDI, with smoothing
        // _delayTimeInSamples = getSampleRate() * (1 / midiNoteToHz(_delayTimeParam->get())); // MIDI, no smoothing
        
        if (_delayTimeInSamples != prevDelayTimeInSamples) {
            DBG(_processName << " (" << _trackProperties.name << "): " << "_delayTimeInSamples=" << _delayTimeInSamples << ", sampleRate=" << getSampleRate());
        }
        
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
bool MelodrumaticAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MelodrumaticAudioProcessor::createEditor()
{
    return new MelodrumaticAudioProcessorEditor (*this);
}

//==============================================================================
void MelodrumaticAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Create XML
    std::unique_ptr<XmlElement> xml(new XmlElement("Melodrumatic"));
    
    // Populate with params
    xml->setAttribute("dryWet", _dryWetParam->get());
    xml->setAttribute("feedback", _feedbackParam->get());
    xml->setAttribute("delayTime", _delayTimeParam->get());
    xml->setAttribute("interprocessPipeSuffix", _interprocessPipeSuffix);
    
    // Copy the XML data to destination blob
    copyXmlToBinary(*xml, destData);
}

void MelodrumaticAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // Create XML from state data
    std::unique_ptr<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    
    // Set params based on state
    if (xml.get() != nullptr && xml->hasTagName("Melodrumatic")) {
        *_dryWetParam = xml->getDoubleAttribute("dryWet");
        *_feedbackParam = xml->getDoubleAttribute("feedback");
        *_delayTimeParam = xml->getDoubleAttribute("delayTime");
        
        setInterprocessPipeSuffix(xml->getStringAttribute("interprocessPipeSuffix"), true);
    }
}

//==============================================================================
void MelodrumaticAudioProcessor::connectionMade()
{
    DBG(_processName << " (" << _trackProperties.name << "): " << "IPC: Connection made");
}

void MelodrumaticAudioProcessor::connectionLost()
{
    DBG(_processName << " (" << _trackProperties.name << "): " << "IPC: Connection lost");
}

void MelodrumaticAudioProcessor::messageReceived(const MemoryBlock &message)
{
    DBG(_processName << " (" << _trackProperties.name << "): " << "IPC: Message received: " << message.toString());
    
    // Get the MIDI note from the message
    const void* messageData = message.getData();
    const BigInteger* midiNotePtr = static_cast<const BigInteger*>(messageData);
    const BigInteger midiNote = *midiNotePtr;
    
    DBG(_processName << " (" << _trackProperties.name << "): " << "The MIDI note is " << midiNote.toString(10));

    // Convert MIDI to delay time
    // TODO:
    // - Update UI accordingly
    // *_delayTimeParam = jmax(128.0 - midiNote.toInteger(), 1.0);
    *_delayTimeParam = jmax(midiNote.toInteger() + 1, 1);
}

//==============================================================================
// Custom


// Linear interpolation
float MelodrumaticAudioProcessor::lerp(float x0, float x1, float t)
{
    return (1 - t) * x0 + t * x1;
}

// Create pipe, or connect to existing pipe
bool MelodrumaticAudioProcessor::createOrConnectToInterprocessPipe()
{
    // Create pipe
    // _didCurrentInstanceCreateInterprocessPipe = false;
    DBG("_hasInterprocessPipeBeenCreated[" << getInterprocessPipeFullName() << "]=" << (_hasInterprocessPipeBeenCreated[getInterprocessPipeFullName()] ? "true" : "false"));
    if (!_hasInterprocessPipeBeenCreated[getInterprocessPipeFullName()])
    {
        _hasInterprocessPipeBeenCreated[getInterprocessPipeFullName()] = createPipe(getInterprocessPipeFullName(), _interprocessCreatePipeTimeoutMs, false);
        _didCurrentInstanceCreateInterprocessPipe = _hasInterprocessPipeBeenCreated[getInterprocessPipeFullName()];
    }
    else {
        // pass
    }
    
    // Connect to existing pipe
    if (!_didCurrentInstanceCreateInterprocessPipe) {
        bool isInterprocessConnectToPipeSuccessful = connectToPipe(getInterprocessPipeFullName(), _interprocessConnectToPipeTimeoutMs);
        if (!isInterprocessConnectToPipeSuccessful) {
            DBG(_processName << " (" << _trackProperties.name << "): " << "Unsuccessful connection to pipe " << getInterprocessPipeFullName());
        }
        else {
            DBG(_processName << " (" << _trackProperties.name << "): " << "Successfully connected to pipe " << getInterprocessPipeFullName());
            _numProcessesConnectedToInterprocessPipe[getInterprocessPipeFullName()]++;
            return true;
        }
    }
    else {
        DBG(_processName << " (" << _trackProperties.name << "): " << "Successfully created pipe " << getInterprocessPipeFullName());
        _numProcessesConnectedToInterprocessPipe[getInterprocessPipeFullName()] = 1; // Reset since we're recreating the pipe
        return true;
    }
    
    return false;
}

// Generate a random process name
String MelodrumaticAudioProcessor::generateProcessName()
{
    Random rng;
    String name;
    
    for (int i=0; i<10; i++) {
        int n = rng.nextInt(9);
        name.append(String(n), 1);
    }
    
    return name;
}

// Convert MIDI note to frequency in Hz
float MelodrumaticAudioProcessor::midiNoteToHz(float midiNote)
{
    return (440/32) * (pow(2, (midiNote-9)/12));
    // return 13.75 * (pow(2, (midiNote-9) * 0.08333333333)); // Not accurate enough :(
}

void MelodrumaticAudioProcessor::initializeInterprocessStaticVariables()
{
    _hasInterprocessPipeBeenCreated.insert({getInterprocessPipeFullName(), false});
    _numProcessesConnectedToInterprocessPipe.insert({getInterprocessPipeFullName(), 0});
}


String MelodrumaticAudioProcessor::getInterprocessPipeFullName()
{
    return _interprocessPipeBaseName + String("_") + _interprocessPipeSuffix;
}

void MelodrumaticAudioProcessor::setInterprocessPipeSuffix(String suffix, bool fromSetStateInformation)
{
    // Update variables for current pipe
    if (!fromSetStateInformation)
    {
        // Decrement current count
        _numProcessesConnectedToInterprocessPipe[getInterprocessPipeFullName()]--;
        
        // Reset creation flag for pipe if necessary
        if (_didCurrentInstanceCreateInterprocessPipe)
        {
            _hasInterprocessPipeBeenCreated[getInterprocessPipeFullName()] = false;
        }
        
        // Reset creation flag for current instance
        _didCurrentInstanceCreateInterprocessPipe = false;
    }
    
    // Set new suffix and variables
    _interprocessPipeSuffix = suffix; // Set new suffix
    initializeInterprocessStaticVariables(); // Add to map
}

String MelodrumaticAudioProcessor::getInterprocessPipeSuffix()
{
    return _interprocessPipeSuffix;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MelodrumaticAudioProcessor();
}
