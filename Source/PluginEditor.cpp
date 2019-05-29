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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    int editorWidth = 800;
    int editorHeight = 320;
    setSize (editorWidth, editorHeight);
    
    // ================================================================
    // Get params from processor
    auto& params = processor.getParameters();
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterFloat* delayTimeSmoothAmountParam = (AudioParameterFloat*)params.getUnchecked(3);
    
    // ================================================================
    // Dry/Wet
    _dryWetSlider.setBounds(0, 150, 100, 100);
    _dryWetSlider.setLookAndFeel(&_lookAndFeel);
    _dryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _dryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _dryWetSlider.setRange(dryWetParam->range.start, dryWetParam->range.end);
    _dryWetSlider.setValue(dryWetParam->get());
    addAndMakeVisible(_dryWetSlider);
    
    _dryWetSlider.onValueChange = [this, dryWetParam] {
        *dryWetParam = _dryWetSlider.getValue();
    };
    _dryWetSlider.onDragStart = [dryWetParam] {
        dryWetParam->beginChangeGesture();
    };
    _dryWetSlider.onDragEnd = [dryWetParam] {
        dryWetParam->endChangeGesture();
    };
    
    _dryWetLabel.setText("Dry/Wet", NotificationType::dontSendNotification);
    _dryWetLabel.setJustificationType(Justification::centred);
    _dryWetLabel.setFont(_lookAndFeel.getGSRegularFont());
    _dryWetLabel.attachToComponent(&_dryWetSlider, false);
    addAndMakeVisible(_dryWetLabel);
    
    // ================================================================
    // Feedback
    _feedbackSlider.setBounds(100, 150, 100, 100);
    _feedbackSlider.setLookAndFeel(&_lookAndFeel);
    _feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _feedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _feedbackSlider.setRange(feedbackParam->range.start, feedbackParam->range.end);
    _feedbackSlider.setValue(feedbackParam->get());
    addAndMakeVisible(_feedbackSlider);
    
    _feedbackSlider.onValueChange = [this, feedbackParam] {
        *feedbackParam = _feedbackSlider.getValue();
    };
    _feedbackSlider.onDragStart = [feedbackParam] {
        feedbackParam->beginChangeGesture();
    };
    _feedbackSlider.onDragEnd = [feedbackParam] {
        feedbackParam->endChangeGesture();
    };
    
    _feedbackLabel.setText("Feedback", NotificationType::dontSendNotification);
    _feedbackLabel.setJustificationType(Justification::centred);
    _feedbackLabel.setFont(_lookAndFeel.getGSRegularFont());
    _feedbackLabel.attachToComponent(&_feedbackSlider, false);
    addAndMakeVisible(_feedbackLabel);
    
    // ================================================================
    // Delay time smooth amount
    _delayTimeSmoothAmountSlider.setBounds(200, 150, 100, 100);
    _delayTimeSmoothAmountSlider.setLookAndFeel(&_lookAndFeel);
    _delayTimeSmoothAmountSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _delayTimeSmoothAmountSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _delayTimeSmoothAmountSlider.setRange(delayTimeSmoothAmountParam->range.start, delayTimeSmoothAmountParam->range.end);
    _delayTimeSmoothAmountSlider.setValue(delayTimeSmoothAmountParam->get());
    addAndMakeVisible(_delayTimeSmoothAmountSlider);
    
    _delayTimeSmoothAmountSlider.onValueChange = [this, delayTimeSmoothAmountParam] {
        *delayTimeSmoothAmountParam = _delayTimeSmoothAmountSlider.getValue();
        DBG("delayTimeSmoothAmountParam=" << *delayTimeSmoothAmountParam);
    };
    _delayTimeSmoothAmountSlider.onDragStart = [delayTimeSmoothAmountParam] {
        delayTimeSmoothAmountParam->beginChangeGesture();
    };
    _delayTimeSmoothAmountSlider.onDragEnd = [delayTimeSmoothAmountParam] {
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
    // _interprocessPipeSuffixTextEditor.onTextChange = [this] {
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
    float midiKeyboardComponentWidth = editorWidth - (midiKeyboardComponentX * 2);
    _midiKeyboardComponent.setBounds(midiKeyboardComponentX, 25, midiKeyboardComponentWidth, 100);
    _midiKeyboardComponent.setAvailableRange(0, 127);
    _midiKeyboardComponent.setLowestVisibleKey(0);
    _midiKeyboardComponent.setKeyWidth(midiKeyboardComponentWidth / (128.0 - (5 * (128.0/12)))); // White keys only
    // _midiKeyboardComponent.setOctaveForMiddleC(2);
    // _midiKeyboardComponent.setBlackNoteWidthProportion(0.8);
    _midiKeyboardComponent.setLookAndFeel(&_lookAndFeel);
    addAndMakeVisible(_midiKeyboardComponent);
    
    // ================================================================
    // Delay time
    _delayTimeSlider.setBounds(midiKeyboardComponentX, 110, midiKeyboardComponentWidth, 50);
    _delayTimeSlider.setLookAndFeel(&_lookAndFeel);
    // _delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _delayTimeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    // _delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    _delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end); // Allow for microtones
    // _delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end, 1); // No microtones
    _delayTimeSlider.setValue(delayTimeParam->get());
    addAndMakeVisible(_delayTimeSlider);
    
    _delayTimeSlider.onValueChange = [this, delayTimeParam] {
        *delayTimeParam = _delayTimeSlider.getValue();
        DBG("delayTimeParam=" << *delayTimeParam);
    };
    _delayTimeSlider.onDragStart = [delayTimeParam] {
        delayTimeParam->beginChangeGesture();
    };
    _delayTimeSlider.onDragEnd = [delayTimeParam] {
        delayTimeParam->endChangeGesture();
    };
    
    _delayTimeLabel.setText("Delay Time (MIDI Note)", NotificationType::dontSendNotification);
    _delayTimeLabel.setJustificationType(Justification::centred);
    _delayTimeLabel.setFont(_lookAndFeel.getGSRegularFont());
    _delayTimeLabel.attachToComponent(&_midiKeyboardComponent, false);
    addAndMakeVisible(_delayTimeLabel);
    
    // ================================================================
    // Title
    // TODO: Eventually make this an image with bloom & plume
    _titleLabel.setBounds(30, editorHeight - 60, 150, 50);
    _titleLabel.setText("Melodrumatic", NotificationType::dontSendNotification);
    _titleLabel.setJustificationType(Justification::centred);
    _titleLabel.setFont(_lookAndFeel.getPCDFont());
    addAndMakeVisible(_titleLabel);
    
    
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // g.setColour (Colours::white);
    // g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
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
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    _delayTimeSlider.setValue(delayTimeParam->get());
    
    int midiNote = jmax((int) delayTimeParam->get() - 1, 1); // Subtract one to get back to original note
    // DBG("_delayTimeSlider value = " << _delayTimeSlider.getValue());
    if (!_midiKeyboardState.isNoteOn(1, midiNote))
    {
        _midiKeyboardState.allNotesOff(1);
        _midiKeyboardState.noteOn(1, midiNote, 1.0);
    }
}
