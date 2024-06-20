#include "JuceWebViewTutorial/PluginEditor.h"
#include "JuceWebViewTutorial/PluginProcessor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(webView);
  webView.goToURL("https://juce.com");

  setResizable(true, true);
  setSize(800, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::resized() {
  webView.setBounds(getBounds().withTrimmedLeft(getWidth() / 2));
}
} // namespace audio_plugin
