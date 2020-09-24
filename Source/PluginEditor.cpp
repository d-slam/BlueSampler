/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BlueSamplerAudioProcessorEditor::BlueSamplerAudioProcessorEditor(BlueSamplerAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p)
{
	mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
	addAndMakeVisible(&mLoadButton);

	setSize(200, 200);
}

BlueSamplerAudioProcessorEditor::~BlueSamplerAudioProcessorEditor()
{}

//==============================================================================
void BlueSamplerAudioProcessorEditor::paint(juce::Graphics& g)
{

	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void BlueSamplerAudioProcessorEditor::resized()
{
	mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}
