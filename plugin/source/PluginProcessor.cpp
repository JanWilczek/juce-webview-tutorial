#include "JuceWebViewTutorial/PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include "JuceWebViewTutorial/PluginEditor.h"
#include "JuceWebViewTutorial/ParameterIDs.hpp"
#include <cmath>
#include <functional>

namespace audio_plugin {
namespace {
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
  using namespace juce;
  AudioProcessorValueTreeState::ParameterLayout layout;

  layout.add(std::make_unique<AudioParameterFloat>(
      id::GAIN, "gain", NormalisableRange<float>{0.f, 1.f, 0.01f, 0.9f}, 1.f));

  layout.add(std::make_unique<AudioParameterBool>(
      id::BYPASS, "bypass", false,
      AudioParameterBoolAttributes{}.withLabel("Bypass")));

  layout.add(std::make_unique<AudioParameterChoice>(
      id::DISTORTION_TYPE, "distortion type",
      StringArray{"none", "tanh(kx)/tanh(k)", "sigmoid"}, 0));

  return layout;
}

void forEachSampleIn(juce::AudioBuffer<float>& buffer,
                     std::function<void(float&)> function) {
  for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
    auto channelToWrite = buffer.getWritePointer(channel);
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
      function(channelToWrite[sample]);
    }
  }
}
}  // namespace

AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      state{*this, nullptr, "PARAMETERS", createParameterLayout()},
      bypass{dynamic_cast<juce::AudioParameterBool*>(
          state.getParameter(id::BYPASS.getParamID()))},
      distortionType{dynamic_cast<juce::AudioParameterChoice*>(
          state.getParameter(id::DISTORTION_TYPE.getParamID()))} {
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

const juce::String AudioPluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() {
  return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  if (bypass->get()) {
    return;
  }

  if (distortionType->getIndex() == 1) {
    // tanh(kx)/tanh(k)
    forEachSampleIn(buffer, [](float& sample) {
      constexpr auto SATURATION = 5.f;
      static const auto normalizationFactor = std::tanh(SATURATION);
      sample = std::tanh(SATURATION * sample) / normalizationFactor;
    });
  } else if (distortionType->getIndex() == 2) {
    // sigmoid
    forEachSampleIn(buffer, [](float& sample) {
      constexpr auto SATURATION = 5.f;
      sample = 2.f / (1.f + std::exp(-SATURATION * sample)) - 1.f;
    });
  }

  buffer.applyGain(*state.getRawParameterValue(id::GAIN.getParamID()));
}

bool AudioPluginAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data,
                                                    int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::ignoreUnused(data, sizeInBytes);
}
}  // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}
