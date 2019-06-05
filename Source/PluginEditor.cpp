/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MelodrumaticAudioProcessorEditor::MelodrumaticAudioProcessorEditor (MelodrumaticAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
      _midiKeyboardState(),
      _midiKeyboardComponent(_midiKeyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    // ================================================================
    // Editor window
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    int editorWidth = 800;
    int editorHeight = 360; // 320
    setSize (editorWidth, editorHeight);
    
    // Look and feel
    // setLookAndFeel(&_lookAndFeel); // We run into assertion errors since the LookAndFeel gets destroyed before the editor itself
    
    // Resizable
    setResizable(false, false);
    
    // ================================================================
    // Get params from processor
    auto& params = processor.getParameters();
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterFloat* delayTimeSmoothAmountParam = (AudioParameterFloat*)params.getUnchecked(3);
    
    // ================================================================
    // Shared slider values
    float rotarySliderWidth = 75;
    float rotarySliderHeight = 75;
    float rotarySliderXMargin = 100;
    // float rotarySliderXOffset = (editorWidth * 0.5) + rotarySliderWidth;
    float rotarySliderXOffset = (editorWidth * 0.5) - (((rotarySliderWidth*3) + (rotarySliderXMargin*2)) / 3);
    float rotarySliderY = 265;
    
    // ================================================================
    // Dry/Wet
    _dryWetSlider.setBounds(rotarySliderXOffset, rotarySliderY, rotarySliderWidth, rotarySliderHeight);
    _dryWetSlider.setLookAndFeel(&_lookAndFeel);
    _dryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _dryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _dryWetSlider.setRange(dryWetParam->range.start, dryWetParam->range.end);
    _dryWetSlider.setValue(dryWetParam->get());
    addAndMakeVisible(_dryWetSlider);
    
    _dryWetSlider.onValueChange = [this, dryWetParam]
    {
        *dryWetParam = _dryWetSlider.getValue();
    };
    _dryWetSlider.onDragStart = [dryWetParam]
    {
        dryWetParam->beginChangeGesture();
    };
    _dryWetSlider.onDragEnd = [dryWetParam]
    {
        dryWetParam->endChangeGesture();
    };
    
    _dryWetLabel.setText("Dry/Wet", NotificationType::dontSendNotification);
    _dryWetLabel.setJustificationType(Justification::centred);
    _dryWetLabel.setFont(_lookAndFeel.getGSRegularFont());
    _dryWetLabel.attachToComponent(&_dryWetSlider, false);
    addAndMakeVisible(_dryWetLabel);
    
    // ================================================================
    // Feedback
    _feedbackSlider.setBounds(rotarySliderXOffset + (rotarySliderXMargin * 1), rotarySliderY, rotarySliderWidth, rotarySliderHeight);
    _feedbackSlider.setLookAndFeel(&_lookAndFeel);
    _feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _feedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _feedbackSlider.setRange(feedbackParam->range.start, feedbackParam->range.end);
    _feedbackSlider.setValue(feedbackParam->get());
    addAndMakeVisible(_feedbackSlider);
    
    _feedbackSlider.onValueChange = [this, feedbackParam]
    {
        *feedbackParam = _feedbackSlider.getValue();
    };
    _feedbackSlider.onDragStart = [feedbackParam]
    {
        feedbackParam->beginChangeGesture();
    };
    _feedbackSlider.onDragEnd = [feedbackParam]
    {
        feedbackParam->endChangeGesture();
    };
    
    _feedbackLabel.setText("Feedback", NotificationType::dontSendNotification);
    _feedbackLabel.setJustificationType(Justification::centred);
    _feedbackLabel.setFont(_lookAndFeel.getGSRegularFont());
    _feedbackLabel.attachToComponent(&_feedbackSlider, false);
    addAndMakeVisible(_feedbackLabel);
    
    // ================================================================
    // Delay time smooth amount
    _delayTimeSmoothAmountSlider.setBounds(rotarySliderXOffset + (rotarySliderXMargin * 2), rotarySliderY, rotarySliderWidth, rotarySliderHeight);
    _delayTimeSmoothAmountSlider.setLookAndFeel(&_lookAndFeel);
    _delayTimeSmoothAmountSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _delayTimeSmoothAmountSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _delayTimeSmoothAmountSlider.setSkewFactor(5.0);
    _delayTimeSmoothAmountSlider.setRange(delayTimeSmoothAmountParam->range.start, delayTimeSmoothAmountParam->range.end);
    _delayTimeSmoothAmountSlider.setValue(delayTimeSmoothAmountParam->get());
    addAndMakeVisible(_delayTimeSmoothAmountSlider);
    
    _delayTimeSmoothAmountSlider.onValueChange = [this, delayTimeSmoothAmountParam]
    {
        *delayTimeSmoothAmountParam = _delayTimeSmoothAmountSlider.getValue();
        DBG("delayTimeSmoothAmountParam=" << *delayTimeSmoothAmountParam);
    };
    _delayTimeSmoothAmountSlider.onDragStart = [delayTimeSmoothAmountParam]
    {
        delayTimeSmoothAmountParam->beginChangeGesture();
    };
    _delayTimeSmoothAmountSlider.onDragEnd = [delayTimeSmoothAmountParam]
    {
        delayTimeSmoothAmountParam->endChangeGesture();
    };
    
    _delayTimeSmoothAmountLabel.setText("Glissando", NotificationType::dontSendNotification);
    _delayTimeSmoothAmountLabel.setJustificationType(Justification::centred);
    _delayTimeSmoothAmountLabel.setFont(_lookAndFeel.getGSRegularFont());
    _delayTimeSmoothAmountLabel.attachToComponent(&_delayTimeSmoothAmountSlider, false);
    addAndMakeVisible(_delayTimeSmoothAmountLabel);
    
    // ================================================================
    // Interprocess pipe text editor
    // _interprocessPipeSuffixTextEditor.setBounds(400, 175, 100, 20);
    // _interprocessPipeSuffixTextEditor.setText(processor.getInterprocessPipeSuffix(), NotificationType::dontSendNotification);
    // _interprocessPipeSuffixTextEditor.setColour(Label::ColourIds::outlineColourId, Colours::white);
    // _interprocessPipeSuffixTextEditor.setJustificationType(Justification::centred);
    // _interprocessPipeSuffixTextEditor.setFont(_lookAndFeel.getGSRegularFont());
    // _interprocessPipeSuffixTextEditor.setEditable(true);
    // // _interprocessPipeIdLabel.attachToComponent(&_delayTimeSlider, true);
    // addAndMakeVisible(_interprocessPipeSuffixTextEditor);
    //
    // _interprocessPipeSuffixTextEditor.onTextChange = [this]
    // {
    //     processor.setInterprocessPipeSuffix(_interprocessPipeSuffixTextEditor.getText(), false);
    // };
    //
    // _interprocessPipeSuffixLabel.setText("Plugin Pair ID", NotificationType::dontSendNotification);
    // _interprocessPipeSuffixLabel.setJustificationType(Justification::centred);
    // _interprocessPipeSuffixLabel.setFont(_lookAndFeel.getGSRegularFont());
    // _interprocessPipeSuffixLabel.attachToComponent(&_interprocessPipeSuffixTextEditor, false);
    // addAndMakeVisible(_interprocessPipeSuffixLabel);
    
    // ================================================================
    // MIDI keyboard
    float midiKeyboardComponentX = 20;
    float midiKeyboardComponentY = 110;
    float midiKeyboardComponentWidth = editorWidth - (midiKeyboardComponentX * 2);
    _midiKeyboardComponent.setBounds(midiKeyboardComponentX, midiKeyboardComponentY, midiKeyboardComponentWidth, 100);
    _midiKeyboardComponent.setAvailableRange(0, 127);
    _midiKeyboardComponent.setLowestVisibleKey(0);
    _midiKeyboardComponent.setKeyWidth(midiKeyboardComponentWidth / (128.0 - (5 * (128.0/12)))); // White keys only
    // _midiKeyboardComponent.setOctaveForMiddleC(2);
    // _midiKeyboardComponent.setBlackNoteWidthProportion(0.8);
    _midiKeyboardComponent.setLookAndFeel(&_lookAndFeel);
    addAndMakeVisible(_midiKeyboardComponent);
    
    // MIDI keyboard state
    _midiKeyboardState.addListener(this);
    
    // ================================================================
    // Delay time
    _delayTimeSlider.setBounds(midiKeyboardComponentX, midiKeyboardComponentY + 85, midiKeyboardComponentWidth, 50);
    _delayTimeSlider.setLookAndFeel(&_lookAndFeel);
    // _delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _delayTimeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    // _delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    _delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end); // Allow for microtones
    // _delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end, 1); // No microtones
    _delayTimeSlider.setValue(delayTimeParam->get());
    addAndMakeVisible(_delayTimeSlider);
    
    _delayTimeSlider.onValueChange = [this, delayTimeParam]
    {
        *delayTimeParam = _delayTimeSlider.getValue();
        DBG("delayTimeParam=" << *delayTimeParam);
    };
    _delayTimeSlider.onDragStart = [delayTimeParam]
    {
        delayTimeParam->beginChangeGesture();
    };
    _delayTimeSlider.onDragEnd = [delayTimeParam]
    {
        delayTimeParam->endChangeGesture();
    };
    
    _delayTimeLabel.setText("Target Pitch = MIDI Note = Delay Time", NotificationType::dontSendNotification);
    _delayTimeLabel.setJustificationType(Justification::centred);
    _delayTimeLabel.setFont(_lookAndFeel.getGSRegularFont());
    _delayTimeLabel.attachToComponent(&_midiKeyboardComponent, false);
    addAndMakeVisible(_delayTimeLabel);
    
    // ================================================================
    // Title & logo
    
    // Old approach using text (now we have images with bloom & plume)
    // float titleX = 20;
    // float titleY = titleX / 2.0;
    // _titleLabel.setBounds(titleX, titleY, 150, 50);
    // _titleLabel.setText("Melodrumatic", NotificationType::dontSendNotification);
    // _titleLabel.setJustificationType(Justification::centred);
    // _titleLabel.setFont(_lookAndFeel.getPCDFont());
    // addAndMakeVisible(_titleLabel);
    
    // Hyperlinks
    URL titleURL = URL("https://github.com/usdivad/Melodrumatic");
    URL logoURL = URL("https://usdivad.com/");
    
    // TODO: Don't hard-code this (it's directly copied from paint() atm)
    //       We can easily just turn them into member variables
    int titleImageSourceWidth = 1024;
    int titleImageSourceHeight = 341;
    float titleImageScale = 0.25;
    int titleImageDestWidth = (int) (titleImageSourceWidth * titleImageScale);
    int titleImageDestHeight = (int) (titleImageSourceHeight * titleImageScale);
    
    int logoImageSourceWidth = 1024;
    int logoImageSourceHeight = 1024;
    float logoImageScale = 0.075;
    int logoImageDestWidth = (int) (logoImageSourceWidth * logoImageScale);
    int logoImageDestHeight = (int) (logoImageSourceHeight * logoImageScale);
    
    _titleHyperlink.setURL(titleURL);
    _titleHyperlink.setBounds(15, 0, titleImageDestWidth, titleImageDestHeight);
    addAndMakeVisible(_titleHyperlink);
    
    _logoHyperlink.setURL(logoURL);
    _logoHyperlink.setBounds(getWidth() - logoImageDestWidth - 15, 10, logoImageDestWidth, logoImageDestHeight);
    addAndMakeVisible(_logoHyperlink);
    
    
    // ================================================================
    // Timer
    startTimer(20);
}

MelodrumaticAudioProcessorEditor::~MelodrumaticAudioProcessorEditor()
{
}

//==============================================================================
void MelodrumaticAudioProcessorEditor::paint (Graphics& g)
{
    // RMSE visualizer
    float rmse = processor.getRMSE();
    float rmseNormalized = jmin(jmax(rmse, 0.0f), 1.0f);
    Colour vizColourMin = Colour(0xff000000);
    Colour vizColourMax = Colour(0xff8e44ad);
    
    // Using interpolation
    Colour vizColour = vizColourMin.interpolatedWith(vizColourMax, rmseNormalized);
    
    // Using alpha
    // vizColour = vizColour.withAlpha(rmseNormalized); // Use RMSE to adjust alpha
    // vizColour = vizColour.withMultipliedAlpha(1.0f);
    
    g.setColour(vizColour);
    DBG("vizColour=" << vizColour.toDisplayString(true));
    // g.fillEllipse(getWidth() * 0.5, 5, 50, 50); // Test element for color
    
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    // g.fillAll(Colour(0xff000000));
    g.fillAll(vizColour); // Let's just use the whole background as a visualizer!

    // g.setColour (Colours::white);
    // g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    
    // ================================
    // Images!
    
    // Title
    Image titleImage = ImageCache::getFromMemory(BinaryData::MelodrumaticTitle_png, BinaryData::MelodrumaticTitle_pngSize);

    int titleImageSourceWidth = 1024;
    int titleImageSourceHeight = 341;
    float titleImageScale = 0.25;
    int titleImageDestWidth = (int) (titleImageSourceWidth * titleImageScale);
    int titleImageDestHeight = (int) (titleImageSourceHeight * titleImageScale);
    
    g.drawImage(titleImage, 15, 0, titleImageDestWidth, titleImageDestHeight, 0, 0, titleImageSourceWidth, titleImageSourceHeight);
    
    
    // Logo / byline
    Image logoImage = ImageCache::getFromMemory(BinaryData::DavidSuLogo_png, BinaryData::DavidSuLogo_pngSize);
    
    int logoImageSourceWidth = 1024;
    int logoImageSourceHeight = 1024;
    float logoImageScale = 0.075;
    int logoImageDestWidth = (int) (logoImageSourceWidth * logoImageScale);
    int logoImageDestHeight = (int) (logoImageSourceHeight * logoImageScale);
    
    g.drawImage(logoImage, getWidth() - logoImageDestWidth - 15, 10, logoImageDestWidth, logoImageDestHeight, 0, 0, logoImageSourceWidth, logoImageSourceHeight);
    
    
    
}

void MelodrumaticAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


//==============================================================================
void MelodrumaticAudioProcessorEditor::timerCallback()
{
    // Update GUI based on params
    auto& params = processor.getParameters();
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterFloat* delayTimeSmoothAmountParam = (AudioParameterFloat*)params.getUnchecked(3);
    
    _dryWetSlider.setValue(dryWetParam->get());
    _feedbackSlider.setValue(feedbackParam->get());
    _delayTimeSlider.setValue(delayTimeParam->get());
    _delayTimeSmoothAmountSlider.setValue(delayTimeSmoothAmountParam->get());
    
    int midiNote = jmax((int) delayTimeParam->get() - 1, 1); // Subtract one to get back to original note
    // DBG("_delayTimeSlider value = " << _delayTimeSlider.getValue());
    if (!_midiKeyboardState.isNoteOn(1, midiNote))
    {
        _midiKeyboardState.allNotesOff(1);
        _midiKeyboardState.noteOn(1, midiNote, 1.0);
    }
    
    
    // RMSE
    float currRMSE = processor.getRMSE();
    if (currRMSE != _prevRMSE) // Only repaint if difference from prev is noticeably different
    {
        DBG("RMSE=" << processor.getRMSE());
        repaint();
    }
    _prevRMSE = currRMSE;
}

void MelodrumaticAudioProcessorEditor::handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    // Get params from processor
    auto& params = processor.getParameters();
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    
    // Set params
    int midiNote = jmin(midiNoteNumber + 1, 127); // Add one for internal representation
    *delayTimeParam = midiNote;
    
    // Update GUI
    _delayTimeSlider.setValue(delayTimeParam->get());
    
    DBG("noteOn: " << midiNoteNumber);
}

void MelodrumaticAudioProcessorEditor::handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    // pass
}
