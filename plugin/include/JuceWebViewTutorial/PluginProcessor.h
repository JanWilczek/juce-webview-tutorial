#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace webview_plugin {
class AudioPluginAudioProcessor : public juce::AudioProcessor {
public:
  AudioPluginAudioProcessor();
  ~AudioPluginAudioProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  using AudioProcessor::processBlock;

  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  juce::AudioProcessorValueTreeState& getState() { return state; }
  std::atomic<float> outputLevelLeft;

private:
  struct Parameters {
    juce::AudioParameterFloat* gain{nullptr};
    juce::AudioParameterBool* bypass{nullptr};
    juce::AudioParameterChoice* distortionType{nullptr};
  };

  [[nodiscard]] static juce::AudioProcessorValueTreeState::ParameterLayout
  createParameterLayout(Parameters&);

  Parameters parameters;
  juce::AudioProcessorValueTreeState state;
  juce::dsp::BallisticsFilter<float> envelopeFollower;
  juce::AudioBuffer<float> envelopeFollowerOutputBuffer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
}  // namespace webview_plugin
