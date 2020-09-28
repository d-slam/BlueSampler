/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BlueSamplerAudioProcessorEditor::BlueSamplerAudioProcessorEditor(BlueSamplerAudioProcessor& p) : AudioProcessorEditor(&p), mWaveThumbnail(p), mADSRComponent(p), audioProcessor(p)
{
	auto tapImage = ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize);

	if (!tapImage.isNull())
		mImageComponent.setImage(tapImage, RectanglePlacement::stretchToFit);
	jassert(!tapImage.isNull());

	addAndMakeVisible(&mImageComponent);
	addAndMakeVisible(&mWaveThumbnail);
	addAndMakeVisible(&mADSRComponent);
	startTimer(50);
	setSize(600, 400);
}

BlueSamplerAudioProcessorEditor::~BlueSamplerAudioProcessorEditor()
{
	stopTimer();
}

//==============================================================================
void BlueSamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(Colours::black);
}

void BlueSamplerAudioProcessorEditor::resized()
{

	mImageComponent.setBoundsRelative(0.0f, 0.0f, 0.35f, 0.25f);

	mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);

	mADSRComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
}

void BlueSamplerAudioProcessorEditor::timerCallback()
{
	repaint();
}




