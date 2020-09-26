/*
  ==============================================================================

    ADSRComponent.h
    Created: 26 Sep 2020 5:19:34pm
    Author:  damsc

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(BlueSamplerAudioProcessor&);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;

    Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mDecayAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mSustainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mReleaseAttachment;

    BlueSamplerAudioProcessor& audioProcessor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
