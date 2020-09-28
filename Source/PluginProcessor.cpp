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
	), mAPVTS(*this, nullptr, "PARAMETERS", createParameters())


#endif
{
	mWaveForm.setSize(1, 0);				//WICHTIG!! suscht schreits assart wenn beim starten in editor in readpointer holsch!!!!

	mFormatManager.registerBasicFormats();

	mAPVTS.state.addListener(this);

	for (int i = 0; i < mNumVoices; ++i)
	{
		mSampler.addVoice(new SamplerVoice());
	}
}

BlueSamplerAudioProcessor::~BlueSamplerAudioProcessor()
{
	if (mFormatReader != nullptr)					//wtf ober assert werd net gschmissn...ERST wenn >2 files gloden hosch Maulter...
	{
		mFormatReader->~AudioFormatReader();
	}
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

	updateADSR();
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

	if (mShouldUpdate)
	{
		updateADSR();
	}


	MidiMessage m;
	MidiBuffer::Iterator it{ midiMessages };
	int sample;
	while (it.getNextEvent(m, sample))
	{
		if (m.isNoteOn())
			mIsNotePlayed = true;

		else if (m.isNoteOff())
			mIsNotePlayed = false;
	}

	mSampleCount = mIsNotePlayed ? mSampleCount += buffer.getNumSamples() : 0;				//verkürtzt if statement(if mIsNotePlayed than inc... des nochn : isch es else (mSampleCount=0)


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
	mSampler.clearSounds();

	FileChooser chooser{ "PLease File" };
	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		mFormatReader = mFormatManager.createReaderFor(file);
	}

	BigInteger range;
	range.setRange(0, 128, true);

	mSampler.addSound(new SamplerSound("Sampler", *mFormatReader, range, 60, 0.1, 0.1, 10));	//rotzt assert, check tuts how to implement mit unique_ptr
}
void BlueSamplerAudioProcessor::loadFile(const String& path)
{
	mSampler.clearSounds();

	auto file = File(path);
	mFormatReader = mFormatManager.createReaderFor(file);


	auto sampleLenght = static_cast<int>(mFormatReader->lengthInSamples);						//potentieller channel assert
	mWaveForm.setSize(1, sampleLenght);
	mFormatReader->read(&mWaveForm, 0, sampleLenght, 0, true, false);


	//auto buffer = mWaveForm.getReadPointer(0);												//debug out
	//for (int sample = 0; sample < mWaveForm.getNumSamples(); ++sample) 
	//{
	//	DBG(buffer[sample]);
	//}

	BigInteger range;
	range.setRange(0, 128, true);

	mSampler.addSound(new SamplerSound("Sampler", *mFormatReader, range, 60, 0.1, 0.1, 10));		//same, funkt ober mit den bleedsinn in destructor

	updateADSR();

}

void BlueSamplerAudioProcessor::updateADSR()
{

	mADSRParams.attack = mAPVTS.getRawParameterValue("ATTACK")->load();
	mADSRParams.decay = mAPVTS.getRawParameterValue("DECAY")->load();
	mADSRParams.sustain = mAPVTS.getRawParameterValue("SUSTAIN")->load();
	mADSRParams.release = mAPVTS.getRawParameterValue("RELEASE")->load();


	//DBG("Attack " << attack << "decay " << decay << "sustain " << sustain << "release " << release);
	for (int i = 0; i < mSampler.getNumSounds(); ++i)
	{
		if (auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get()));
		{
			auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get());				//scheint ausn scope zu sein, donn castmor holt nomol
			sound->setEnvelopeParameters(mADSRParams);
		}
	}
}


AudioProcessorValueTreeState::ParameterLayout BlueSamplerAudioProcessor::createParameters()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

	parameters.push_back(std::make_unique<AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
	parameters.push_back(std::make_unique<AudioParameterFloat>("DECAY", "Decay", 0.0f, 3.0f, 2.0f));
	parameters.push_back(std::make_unique<AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
	parameters.push_back(std::make_unique<AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 2.0f));


	return { parameters.begin(),parameters.end() };
}

void BlueSamplerAudioProcessor::valueTreePropertyChanged(ValueTree& treeWhoesPropertyChanged, const Identifier& property)
{
	mShouldUpdate = true;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new BlueSamplerAudioProcessor();
}



