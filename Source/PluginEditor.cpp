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

	mAttackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mAttackSlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
	mAttackSlider.addListener(this);
	addAndMakeVisible(&mAttackSlider);

	mDecaySlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mDecaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mDecaySlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
	mDecaySlider.addListener(this);
	addAndMakeVisible(&mDecaySlider);

	mSustainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mSustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mSustainSlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	mSustainSlider.setRange(0.0f, 1.0f, 0.01f);
	mSustainSlider.addListener(this);
	addAndMakeVisible(&mSustainSlider);

	mReleaseSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mReleaseSlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	mReleaseSlider.setRange(0.0f, 5.0f, 0.01f);
	mReleaseSlider.addListener(this);
	addAndMakeVisible(&mReleaseSlider);

	mAttackLabel.setFont(16.0f);
	mAttackLabel.setText("Attack", dontSendNotification);
	mAttackLabel.setJustificationType(Justification::centredTop);
	mAttackLabel.setColour(Label::ColourIds::textColourId, Colours::yellow);
	mAttackLabel.attachToComponent(&mAttackSlider, false);

	mDecayLabel.setFont(16.0f);
	mDecayLabel.setText("Decay", dontSendNotification);
	mDecayLabel.setJustificationType(Justification::centredTop);
	mDecayLabel.setColour(Label::ColourIds::textColourId, Colours::yellow);
	mDecayLabel.attachToComponent(&mDecaySlider, false);

	mSustainLabel.setFont(16.0f);
	mSustainLabel.setText("Sustain", dontSendNotification);
	mSustainLabel.setJustificationType(Justification::centredTop);
	mSustainLabel.setColour(Label::ColourIds::textColourId, Colours::yellow);
	mSustainLabel.attachToComponent(&mSustainSlider, false);

	mReleaseLabel.setFont(16.0f);
	mReleaseLabel.setText("Release", dontSendNotification);
	mReleaseLabel.setJustificationType(Justification::centredTop);
	mReleaseLabel.setColour(Label::ColourIds::textColourId, Colours::yellow);
	mReleaseLabel.attachToComponent(&mReleaseSlider, false);

	setSize(600, 200);
}

BlueSamplerAudioProcessorEditor::~BlueSamplerAudioProcessorEditor() {}

//==============================================================================
void BlueSamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(Colours::black);

	g.setColour(Colours::yellow);

	if (mShouldBePainting == true)
	{
		Path p;
		mAudioPoints.clear();

		auto waveform = audioProcessor.getWaveForm();

		auto ratio = waveform.getNumSamples() / getWidth();
		auto buffer = waveform.getReadPointer(0);

		//scale x
		for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
		{
			mAudioPoints.push_back(buffer[sample]);
		}
		p.startNewSubPath(0, getHeight() / 2);
		//scale y
		for (int sample = 0; sample < mAudioPoints.size(); ++sample)
		{
			auto point = jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, 200, 0);
			p.lineTo(sample, point);
		}

		g.strokePath(p, PathStrokeType(2.0f));
		mShouldBePainting = false;

		//waveform.clear();
		//buffer = nullptr;

	}

	//g.setColour(Colours::white);
	//g.setFont(15.0f);

	//if (audioProcessor.getNumSamplerSounds() > 0)
	//{
	//	g.fillAll(Colours::red);
	//	g.drawText("sample loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
	//}
	//else
	//{
	//	g.drawText("load a sample", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
	//}

}

void BlueSamplerAudioProcessorEditor::resized()
{

	//mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);

	const auto startX = 0.6f;
	const auto startY = 0.6f;
	const auto dialWidth = 0.1f;
	const auto dialHeight = 0.4f;

	mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
	mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
	mSustainSlider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
	mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);

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
			mShouldBePainting = true;
			audioProcessor.loadFile(file);
		}
	}
	repaint();
}

void BlueSamplerAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &mAttackSlider) 
	{
		audioProcessor.getADSRParams().attack = mAttackSlider.getValue();
	}
	else if(slider == &mDecaySlider)
	{
		audioProcessor.getADSRParams().decay = mDecaySlider.getValue();

	}
	else if (slider == &mSustainSlider)
	{
		audioProcessor.getADSRParams().sustain = mSustainSlider.getValue();
		;
	}
	else if (slider == &mReleaseSlider)
	{
		audioProcessor.getADSRParams().release = mReleaseSlider.getValue();

	}
	audioProcessor.updateADSR();
}
