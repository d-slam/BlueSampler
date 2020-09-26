/*
  ==============================================================================

	WaveThumbnail.h
	Created: 26 Sep 2020 4:33:09pm
	Author:  damsc

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class WaveThumbnail : public juce::Component, public FileDragAndDropTarget
{
public:
	WaveThumbnail(BlueSamplerAudioProcessor&);
	~WaveThumbnail() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	bool isInterestedInFileDrag(const StringArray& files) override;
	void filesDropped(const StringArray& files, int x, int y) override;

private:
	std::vector<float> mAudioPoints;
	String mFileName{ "" };

	BlueSamplerAudioProcessor& audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveThumbnail)
};
