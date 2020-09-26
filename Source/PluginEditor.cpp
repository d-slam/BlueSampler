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
	mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);

	mADSRComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
}

void BlueSamplerAudioProcessorEditor::timerCallback()
{
	repaint();
}




