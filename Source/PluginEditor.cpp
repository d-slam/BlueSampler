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
	g.fillAll(Colours::black);

	g.setColour(Colours::white);
	g.setFont(15.0f);

	if (audioProcessor.getNumSamplerSounds() > 0)
	{
		g.fillAll(Colours::red);
		g.drawText("sample loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
	}
	else
	{
		g.drawText("load a sample", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
	}

}

void BlueSamplerAudioProcessorEditor::resized()
{
	//mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool BlueSamplerAudioProcessorEditor::isInterestedInFileDrag(const StringArray& files)
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

void BlueSamplerAudioProcessorEditor::filesDropped(const StringArray& files, int x, int y)
{
	for (auto file : files)
	{
		if (isInterestedInFileDrag(file))
		{
			audioProcessor.loadFile(file);
		}
	}
	repaint();
}
