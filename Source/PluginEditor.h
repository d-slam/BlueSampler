/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BlueSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor, public FileDragAndDropTarget
{
public:
    BlueSamplerAudioProcessorEditor (BlueSamplerAudioProcessor&);
    ~BlueSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

private:

    TextButton mLoadButton { "Load" };

    BlueSamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlueSamplerAudioProcessorEditor)
};
