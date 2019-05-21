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
    setSize (640, 320);
    
    // ================================================================
    // Get params from processor
    auto& params = processor.getParameters();
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    
    // ================================================================
    // Dry/Wet
    _dryWetSlider.setBounds(0, 175, 100, 100);
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
    _dryWetLabel.attachToComponent(&_dryWetSlider, false);
    addAndMakeVisible(_dryWetLabel);
    
    // ================================================================
    // Feedback
    _feedbackSlider.setBounds(100, 175, 100, 100);
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
    _feedbackLabel.attachToComponent(&_feedbackSlider, false);
    addAndMakeVisible(_feedbackLabel);
    
    // ================================================================
    // Delay time
    _delayTimeSlider.setBounds(200, 150, 150, 150);
    _delayTimeSlider.setLookAndFeel(&_lookAndFeel);
    _delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    _delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end, 1);
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
    _delayTimeLabel.attachToComponent(&_delayTimeSlider, false);
    addAndMakeVisible(_delayTimeLabel);
    
    // ================================================================
    // Interprocess pipe text editor
    _interprocessPipeSuffixTextEditor.setBounds(400, 175, 100, 20);
    _interprocessPipeSuffixTextEditor.setText(processor.getInterprocessPipeSuffix(), NotificationType::dontSendNotification);
    _interprocessPipeSuffixTextEditor.setColour(Label::ColourIds::outlineColourId, Colours::white);
    _interprocessPipeSuffixTextEditor.setJustificationType(Justification::centred);
    _interprocessPipeSuffixTextEditor.setEditable(true);
    // _interprocessPipeIdLabel.attachToComponent(&_delayTimeSlider, true);
    addAndMakeVisible(_interprocessPipeSuffixTextEditor);
    
    _interprocessPipeSuffixTextEditor.onTextChange = [this] {
        processor.setInterprocessPipeSuffix(_interprocessPipeSuffixTextEditor.getText(), false);
    };
    
    _interprocessPipeSuffixLabel.setText("Plugin Pair ID", NotificationType::dontSendNotification);
    _interprocessPipeSuffixLabel.setJustificationType(Justification::centred);
    _interprocessPipeSuffixLabel.attachToComponent(&_interprocessPipeSuffixTextEditor, false);
    addAndMakeVisible(_interprocessPipeSuffixLabel);
    
    // ================================================================
    // MIDI keyboard
    _midiKeyboardComponent.setBounds(20, 20, 600, 100);
    _midiKeyboardComponent.setLookAndFeel(&_lookAndFeel);
    addAndMakeVisible(_midiKeyboardComponent);
    
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
