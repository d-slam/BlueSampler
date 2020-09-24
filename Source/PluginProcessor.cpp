/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BlueSamplerAudioProcessor::BlueSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	mFormatManager.registerBasicFormats();

	for (int i = 0; i < mNumVoices; ++i)
	{
		mSampler.addVoice(new SamplerVoice());
	}
}

BlueSamplerAudioProcessor::~BlueSamplerAudioProcessor()
{
	mFormatReader = nullptr;
}

//==============================================================================
const juce::String BlueSamplerAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool BlueSamplerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool BlueSamplerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool BlueSamplerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double BlueSamplerAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int BlueSamplerAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int BlueSamplerAudioProcessor::getCurrentProgram()
{
	return 0;
}

void BlueSamplerAudioProcessor::setCurrentProgram(int index)
{}

const juce::String BlueSamplerAudioProcessor::getProgramName(int index)
{
	return {};
}

void BlueSamplerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{}

//==============================================================================
void BlueSamplerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	mSampler.setCurrentPlaybackSampleRate(sampleRate);
}

void BlueSamplerAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BlueSamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void BlueSamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();


	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool BlueSamplerAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BlueSamplerAudioProcessor::createEditor()
{
	return new BlueSamplerAudioProcessorEditor(*this);
}

//==============================================================================
void BlueSamplerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{

}

void BlueSamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{

}

void BlueSamplerAudioProcessor::loadFile()
{
	FileChooser chooser{ "PLease File" };
	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		mFormatReader = mFormatManager.createReaderFor(file);
	}

	BigInteger range;
	range.setRange(0, 128, true);

	mSampler.addSound(new SamplerSound("Sampler", *mFormatReader, range, 60, 0.1, 0.1, 10));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new BlueSamplerAudioProcessor();
}