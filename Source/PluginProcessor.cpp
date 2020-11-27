/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudiopluginAudioProcessor::AudiopluginAudioProcessor()
     #ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties()
            #if ! JucePlugin_IsMidiEffect
                #if ! JucePlugin_IsSynth
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
                #endif
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
            #endif
        ), parameters(*this, nullptr, juce::Identifier("TESTI"), createParameterLayout())
         , chorus(*this, samplerate)
         , delay(*this, samplerate)
         , distortion(*this, samplerate)
    #endif
{

}

AudiopluginAudioProcessor::~AudiopluginAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout AudiopluginAudioProcessor::createParameterLayout()
{
    // Here we can programatically add parameters to the parameter layout.
    AudioProcessorValueTreeState::ParameterLayout params;

    // Delay 
    // ID, name, min, max, default


    return params;
}

//==============================================================================
const String AudiopluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudiopluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudiopluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudiopluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudiopluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudiopluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudiopluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudiopluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudiopluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudiopluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudiopluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // store this for later use
    this->samplerate = sampleRate;
    chorus.prepareToPlay(sampleRate);
    delay.prepareToPlay(sampleRate);
    distortion.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumInputChannels());
}

void AudiopluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudiopluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void AudiopluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    const int numSamplesInInput = buffer.getNumSamples();

    // Mono processing
    if (totalNumInputChannels == 1 && totalNumOutputChannels == 1)
    {
    }
    // Stereo processing
    // This is essentially the mono version but doubled up for both channels
    // We could generalise and refactor the processing to a separate function and call it for both channels as needed
    // instead of copying the code like this.
    else if (totalNumOutputChannels == 2)
    {

        float* leftData = buffer.getWritePointer(0);
        float* rightData = buffer.getWritePointer(1);


        for (int i = 0; i < numSamplesInInput; i++)
        {
            //chorus.process(leftData[i], rightData[i]);
            //delay.process(leftData[i], rightData[i]);
        }

        distortion.process(buffer);
    }
}

//==============================================================================
bool AudiopluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudiopluginAudioProcessor::createEditor()
{
    return new AudiopluginAudioProcessorEditor (*this, parameters);
}

void AudiopluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudiopluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudiopluginAudioProcessor();
}