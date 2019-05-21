//
//  MelodrumaticLookAndFeel.h
//  Melodrumatic
//
//  Created by David Su on 5/18/19.
//  Copyright © 2019 David Su. All rights reserved.
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

class MelodrumaticLookAndFeel : public LookAndFeel_V4
{
public:
    MelodrumaticLookAndFeel();
    ~MelodrumaticLookAndFeel();
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider& slider) override;
};
