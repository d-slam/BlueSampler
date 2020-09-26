/*
  ==============================================================================

	WaveThumbnail.cpp
	Created: 26 Sep 2020 4:33:09pm
	Author:  damsc

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(BlueSamplerAudioProcessor& p) : audioProcessor(p)
{


}

WaveThumbnail::~WaveThumbnail()
{}

void WaveThumbnail::paint(juce::Graphics& g)
{

	g.fillAll(Colours::cadetblue.darker());

	auto waveform = audioProcessor.getWaveForm();

	if (waveform.getNumSamples() > 0)
	{
		Path p;
		mAudioPoints.clear();

		auto ratio = waveform.getNumSamples() / getWidth();
		auto buffer = waveform.getReadPointer(0);

		//scale x
		for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
		{
			mAudioPoints.push_back(buffer[sample]);
		}

		g.setColour(Colours::green);
		p.startNewSubPath(0, getHeight() / 2);


		//scale y
		for (int sample = 0; sample < mAudioPoints.size(); ++sample)
		{
			auto point = jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
			p.lineTo(sample, point);
		}

		g.strokePath(p, PathStrokeType(2.0f));

		g.setColour(Colours::white);
		g.setFont(15.0f);

		auto textBounds = getLocalBounds().reduced(10, 10);
		g.drawFittedText(mFileName, textBounds, Justification::topRight, 1);

		auto playHeadPosition = jmap<int>(audioProcessor.getSampleCount(), 0, audioProcessor.getWaveForm().getNumSamples(), 0, getWidth());
		g.setColour(Colours::white);
		g.drawLine(playHeadPosition, 0, playHeadPosition, getHeight(), 2.0f);

		g.setColour(Colours::black.withAlpha(0.2f));
		g.fillRect(0, 0, playHeadPosition, getHeight());

	}
	else
	{
		g.setColour(Colours::white);
		g.setFont(40.0f);
		g.drawFittedText("Drop Sample!", getLocalBounds(), Justification::centred, 1);
	}

}

void WaveThumbnail::resized()
{


}

bool WaveThumbnail::isInterestedInFileDrag(const StringArray& files)
{
	for (auto file : files)
	{
		if (file.contains(".wav") || file.contains(".mp3"))
		{
			return true;
		}
	}
	return false;
}

void WaveThumbnail::filesDropped(const StringArray& files, int x, int y)
{
	for (auto file : files)
	{
		if (isInterestedInFileDrag(file))
		{
			auto myFile = std::make_unique<File>(file);
			mFileName = myFile->getFileNameWithoutExtension();
			audioProcessor.loadFile(file);
		}
	}
	repaint();
}