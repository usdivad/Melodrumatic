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
    setColour(MidiKeyboardComponent::ColourIds::mouseOverKeyOverlayColourId, Colour(0x448e44ad));
}

MelodrumaticLookAndFeel::~MelodrumaticLookAndFeel()
{
    
}
