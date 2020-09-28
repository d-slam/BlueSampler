/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"

//==============================================================================
/**
*/
class BlueSamplerAudioProcessorEditor : public juce::AudioProcessorEditor, public Timer
{
public:
	BlueSamplerAudioProcessorEditor(BlueSamplerAudioProcessor&);
	~BlueSamplerAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	void timerCallback() override;

private:

	ImageComponent mImageComponent;
	WaveThumbnail mWaveThumbnail;
	ADSRComponent mADSRComponent;

	BlueSamplerAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlueSamplerAudioProcessorEditor)
};
