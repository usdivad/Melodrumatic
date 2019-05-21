//
//  MelodrumaticLookAndFeel.cpp
//  Melodrumatic
//
//  Created by David Su on 5/18/19.
//  Copyright © 2019 David Su. All rights reserved.
//

#include "MelodrumaticLookAndFeel.h"

//==============================================================================

MelodrumaticLookAndFeel::MelodrumaticLookAndFeel()
{
    // Slider
    setColour(Slider::thumbColourId, Colour(0xff8e44ad));
    
    // MIDI keyboard
    setColour(MidiKeyboardComponent::ColourIds::keyDownOverlayColourId, Colour(0xff8e44ad));
    setColour(MidiKeyboardComponent::ColourIds::mouseOverKeyOverlayColourId, Colour(0xffd2b4de));
}

MelodrumaticLookAndFeel::~MelodrumaticLookAndFeel()
{
    
}

void MelodrumaticLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                                                 float sliderPosProportional, float rotaryStartAngle,
                                                 float rotaryEndAngle, Slider& slider)
{
    // ================================================================
    // Rotary
    float radius = jmin(width * 0.5, height * 0.5) - 6.0;
    float centerX = x + (width * 0.5);
    float centerY = y + (height * 0.5);
    float rx = centerX - radius;
    float ry = centerY - radius;
    float rw = radius * 2.0; // Width
    float angle = rotaryStartAngle + (sliderPosProportional * (rotaryEndAngle - rotaryStartAngle));
    float rotaryOutlineThickness = 6.0;
    
    // Fill
    g.setColour(Colour(0xff5b2c6f));
    g.fillEllipse(rx, ry, rw, rw);
    
    // Outline
    g.setColour(Colour(0xffd2b4de));
    g.drawEllipse(rx, ry, rw, rw, 6.0);
    
    
    // ================================================================
    // Pointer
    Path p;
    float pointerLength = radius * 0.66;
    float pointerThickness = 6.0;
    p.addRectangle(-pointerThickness * 0.5, -radius + (rotaryOutlineThickness * 0.5), pointerThickness, pointerLength);
    p.applyTransform(AffineTransform::rotation(angle).translated(centerX, centerY));
    
    g.setColour(Colour(0xffffffff));
    g.fillPath(p);
}

