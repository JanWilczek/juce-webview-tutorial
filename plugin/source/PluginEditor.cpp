#include "JuceWebViewTutorial/PluginEditor.h"
#include "JuceWebViewTutorial/PluginProcessor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(webView);
  setResizable(true, true);

  setSize(400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::resized() {
  webView.setBounds(getBounds().withTrimmedLeft(getWidth() / 2));
}
} // namespace audio_plugin
