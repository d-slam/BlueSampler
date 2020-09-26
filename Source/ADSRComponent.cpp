/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 26 Sep 2020 5:19:34pm
    Author:  damsc

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(BlueSamplerAudioProcessor& p): audioProcessor(p)
{
	mAttackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mAttackSlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	addAndMakeVisible(&mAttackSlider);
	mAttackAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", mAttackSlider);

	mDecaySlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mDecaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mDecaySlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	addAndMakeVisible(&mDecaySlider);
	mDecayAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", mDecaySlider);

	mSustainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mSustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mSustainSlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	addAndMakeVisible(&mSustainSlider);
	mSustainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", mSustainSlider);

	mReleaseSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	mReleaseSlider.setColour(Slider::ColourIds::thumbColourId, Colours::blue);
	addAndMakeVisible(&mReleaseSlider);
	mReleaseAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", mReleaseSlider);


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

}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
	g.fillAll(Colours::black);
}

void ADSRComponent::resized()
{
	const auto startX = 0.6f;
	const auto startY = 0.2f;
	const auto dialWidth = 0.1f;
	const auto dialHeight = 0.75f;

	mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
	mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
	mSustainSlider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
	mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);
}
