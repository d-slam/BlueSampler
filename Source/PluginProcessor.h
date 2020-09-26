/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class BlueSamplerAudioProcessor : public juce::AudioProcessor, public ValueTree::Listener
{
public:
	//==============================================================================
	BlueSamplerAudioProcessor();
	~BlueSamplerAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	void loadFile();
	void loadFile(const String& path);

	int getNumSamplerSounds() { return mSampler.getNumSounds(); }
	AudioBuffer<float>& getWaveForm() { return mWaveForm; }

	void updateADSR();

	ADSR::Parameters& getADSRParams() { return mADSRParams; };
	AudioProcessorValueTreeState& getAPVTS() { return mAPVTS; };

	float attack{ 0.0f };
	float decay{ 0.0f };
	float sustain{ 0.0f };
	float release{ 0.0f };

private:

	Synthesiser mSampler;
	const int mNumVoices{ 3 };
	AudioBuffer<float> mWaveForm;

	AudioFormatManager mFormatManager;
	AudioFormatReader* mFormatReader{ nullptr };


	ADSR::Parameters mADSRParams;
	AudioProcessorValueTreeState mAPVTS;
	AudioProcessorValueTreeState::ParameterLayout createParameters();

	void valueTreePropertyChanged(ValueTree& treeWhoesPropertyChanged, const Identifier& property)override;

	std::atomic<bool> mShouldUpdate{ false };


	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlueSamplerAudioProcessor)
};
