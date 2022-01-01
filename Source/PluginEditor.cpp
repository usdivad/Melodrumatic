/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{

// Editor window
constexpr int kEditorWidth = 800;
constexpr int kEditorHeight = 360;

// Shared slider values
constexpr float kRotarySliderWidth = 75.f;
constexpr float kRotarySliderHeight = 75.f;
constexpr float kRotarySliderXMargin = 100.f;
constexpr float kRotarySliderXOffset = (kEditorWidth * 0.5f) - (((kRotarySliderWidth*3.f) + (kRotarySliderXMargin*2.f)) / 3.f);
constexpr float kRotarySliderY = 265.f;

// Title
constexpr int kTitleImageSourceX = 0;
constexpr int kTitleImageSourceY = 0;
constexpr int kTitleImageSourceWidth = 1024;
constexpr int kTitleImageSourceHeight = 341;
constexpr float kTitleImageScale = 0.25;
constexpr int kTitleImageDestX = 15;
constexpr int kTitleImageDestY = 0;
constexpr int kTitleImageDestWidth = (int) (kTitleImageSourceWidth * kTitleImageScale);
constexpr int kTitleImageDestHeight = (int) (kTitleImageSourceHeight * kTitleImageScale);

// Logos
constexpr int kLogoImageSourceWidth = 1024;
constexpr int kLogoImageSourceHeight = 1024;
constexpr int kLogoImageSourceX = 0;
constexpr int kLogoImageSourceY = 0;
constexpr float kLogoImageScale = 0.075f;
constexpr int kLogoImageDestWidth = (int) (kLogoImageSourceWidth * kLogoImageScale);
constexpr int kLogoImageDestHeight = (int) (kLogoImageSourceHeight * kLogoImageScale);
constexpr int kLogoImageDestXRightOffset = kLogoImageDestWidth + 15;
constexpr int kLogoImageDestY = 10;

}

//==============================================================================
MelodrumaticAudioProcessorEditor::MelodrumaticAudioProcessorEditor (MelodrumaticAudioProcessor& p)
    : AudioProcessorEditor (&p), _processor (p),
      _midiKeyboardState(),
      _midiKeyboardComponent(_midiKeyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{

    // Initialize UI elements
    initEditorWindow();
    initPluginInfoUI();
    initDelayTimeUI();
    initAdditionalParamsUI();
    initInterprocessUI();
    
    // Start timer for UI updates
    startTimer(20);
}

MelodrumaticAudioProcessorEditor::~MelodrumaticAudioProcessorEditor()
{
}

//==============================================================================
void MelodrumaticAudioProcessorEditor::paint (Graphics& g)
{
    // RMSE visualizer
    const float rmse = _processor.getRMSE();
    const float rmseNormalized = jmin(jmax(rmse, 0.0f), 1.0f);
    const Colour vizColourMin = Colour(0xff000000);
    const Colour vizColourMax = Colour(0xff8e44ad);
    
    const Colour vizColour = vizColourMin.interpolatedWith(vizColourMax, rmseNormalized);
    
    g.fillAll(vizColour); // Let's use the whole background as a visualizer!

    // ================================
    // Images!
    
    // Title
    Image titleImage = ImageCache::getFromMemory(BinaryData::MelodrumaticTitle_png, BinaryData::MelodrumaticTitle_pngSize);
    
    g.drawImage(titleImage,
                kTitleImageDestX, kTitleImageDestY,
                kTitleImageDestWidth, kTitleImageDestHeight,
                kTitleImageSourceX, kTitleImageSourceY,
                kTitleImageSourceWidth, kTitleImageSourceHeight);
    
    
    // Logo / byline
    Image logoImage = ImageCache::getFromMemory(BinaryData::DavidSuLogo_png, BinaryData::DavidSuLogo_pngSize);
    
    const int logoImageDestX = getWidth() - kLogoImageDestXRightOffset;
    
    g.drawImage(logoImage,
                logoImageDestX, kLogoImageDestY,
                kLogoImageDestWidth, kLogoImageDestHeight,
                kLogoImageSourceX, kLogoImageSourceY,
                kLogoImageSourceWidth, kLogoImageSourceHeight);
    
    
    
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
    auto& params = _processor.getParameters();
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterFloat* delayTimeSmoothAmountParam = (AudioParameterFloat*)params.getUnchecked(3);
    
    _dryWetSlider.setValue(dryWetParam->get());
    _feedbackSlider.setValue(feedbackParam->get());
    _delayTimeSlider.setValue(delayTimeParam->get());
    _delayTimeSmoothAmountSlider.setValue(delayTimeSmoothAmountParam->get());
    
    int midiNote = jmax((int) delayTimeParam->get() - 1, 1); // Subtract one to get back to original note
    if (!_midiKeyboardState.isNoteOn(1, midiNote))
    {
        _midiKeyboardState.allNotesOff(1);
        _midiKeyboardState.noteOn(1, midiNote, 1.0);
    }
    
    
    // RMSE
    float currRMSE = _processor.getRMSE();
    if (currRMSE != _prevRMSE) // Only repaint if difference from prev is noticeably different
    {
        repaint();
    }
    _prevRMSE = currRMSE;
}

void MelodrumaticAudioProcessorEditor::handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    // Get params from processor
    auto& params = _processor.getParameters();
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

void MelodrumaticAudioProcessorEditor::initEditorWindow()
{
    setSize (kEditorWidth, kEditorHeight);
    setResizable(false, false);
}

void MelodrumaticAudioProcessorEditor::initPluginInfoUI()
{
    // ================================================================
    // Hyperlinks
    const URL titleURL = URL("https://github.com/usdivad/Melodrumatic");
    const URL logoURL = URL("https://usdivad.com/");
    
    _titleHyperlink.setURL(titleURL);
    _titleHyperlink.setBounds(kTitleImageDestX, kTitleImageDestY,
                              kTitleImageDestWidth, kTitleImageDestHeight);
    addAndMakeVisible(_titleHyperlink);
    
    const int logoImageDestX = getWidth() - kLogoImageDestXRightOffset;
    _logoHyperlink.setURL(logoURL);
    _logoHyperlink.setBounds(logoImageDestX, kLogoImageDestY,
                             kLogoImageDestWidth, kLogoImageDestHeight);
    addAndMakeVisible(_logoHyperlink);

    // ================================================================
    // Version label
    _versionLabel.setBounds(15, getHeight() * 0.9f, 75, 25);
    _versionLabel.setJustificationType(Justification::centredLeft);
    _versionLabel.setFont(_lookAndFeel.getGSRegularFont());
    _versionLabel.setColour(Label::ColourIds::textColourId, Colours::white);
    _versionLabel.setText(JucePlugin_VersionString, NotificationType::dontSendNotification);
    addAndMakeVisible(_versionLabel);
}

void MelodrumaticAudioProcessorEditor::initDelayTimeUI()
{
    // ================================================================
    // Get params from processor
    auto& params = _processor.getParameters();
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    
    // ================================================================
    // MIDI keyboard
    const float midiKeyboardComponentX = 20;
    const float midiKeyboardComponentY = 110;
    const float midiKeyboardComponentWidth = kEditorWidth - (midiKeyboardComponentX * 2);
    _midiKeyboardComponent.setBounds(midiKeyboardComponentX, midiKeyboardComponentY, midiKeyboardComponentWidth, 100);
    _midiKeyboardComponent.setAvailableRange(0, 127);
    _midiKeyboardComponent.setLowestVisibleKey(0);
    _midiKeyboardComponent.setKeyWidth(midiKeyboardComponentWidth / (128.0 - (5 * (128.0/12)))); // White keys only
    _midiKeyboardComponent.setLookAndFeel(&_lookAndFeel);
    addAndMakeVisible(_midiKeyboardComponent);
    
    // MIDI keyboard state
    _midiKeyboardState.addListener(this);
    
    // ================================================================
    // Slider
    _delayTimeSlider.setBounds(midiKeyboardComponentX, midiKeyboardComponentY + 85, midiKeyboardComponentWidth, 50);
    _delayTimeSlider.setLookAndFeel(&_lookAndFeel);
    _delayTimeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    _delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end); // Allow for microtones
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
    
    // ================================================================
    // Label
    _delayTimeLabel.setText("Target Pitch = MIDI Note = Delay Time", NotificationType::dontSendNotification);
    _delayTimeLabel.setJustificationType(Justification::centred);
    _delayTimeLabel.setFont(_lookAndFeel.getGSRegularFont());
    _delayTimeLabel.attachToComponent(&_midiKeyboardComponent, false);
    addAndMakeVisible(_delayTimeLabel);
}

void MelodrumaticAudioProcessorEditor::initAdditionalParamsUI()

{
    // ================================================================
    // Get params from processor
    auto& params = _processor.getParameters();
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* delayTimeSmoothAmountParam = (AudioParameterFloat*)params.getUnchecked(3);
    
    // ================================================================
    // Dry/Wet
    _dryWetSlider.setBounds(kRotarySliderXOffset, kRotarySliderY, kRotarySliderWidth, kRotarySliderHeight);
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
    _feedbackSlider.setBounds(kRotarySliderXOffset + (kRotarySliderXMargin * 1), kRotarySliderY, kRotarySliderWidth, kRotarySliderHeight);
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
    _delayTimeSmoothAmountSlider.setBounds(kRotarySliderXOffset + (kRotarySliderXMargin * 2), kRotarySliderY, kRotarySliderWidth, kRotarySliderHeight);
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
}

void MelodrumaticAudioProcessorEditor::initInterprocessUI()
{
    if (!_processor.getShouldCreateInterprocessPipe())
    {
        return;
    }
    
    _interprocessPipeSuffixTextEditor.setBounds(400, 175, 100, 20);
    _interprocessPipeSuffixTextEditor.setText(_processor.getInterprocessPipeSuffix(), NotificationType::dontSendNotification);
    _interprocessPipeSuffixTextEditor.setColour(Label::ColourIds::outlineColourId, Colours::white);
    _interprocessPipeSuffixTextEditor.setJustificationType(Justification::centred);
    _interprocessPipeSuffixTextEditor.setFont(_lookAndFeel.getGSRegularFont());
    _interprocessPipeSuffixTextEditor.setEditable(true);
    addAndMakeVisible(_interprocessPipeSuffixTextEditor);
    
    _interprocessPipeSuffixTextEditor.onTextChange = [this]
    {
        _processor.setInterprocessPipeSuffix(_interprocessPipeSuffixTextEditor.getText(), false);
    };
    
    _interprocessPipeSuffixLabel.setText("Plugin Pair ID", NotificationType::dontSendNotification);
    _interprocessPipeSuffixLabel.setJustificationType(Justification::centred);
    _interprocessPipeSuffixLabel.setFont(_lookAndFeel.getGSRegularFont());
    _interprocessPipeSuffixLabel.attachToComponent(&_interprocessPipeSuffixTextEditor, false);
    addAndMakeVisible(_interprocessPipeSuffixLabel);
}
