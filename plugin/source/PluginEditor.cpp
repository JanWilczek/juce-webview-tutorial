#include "JuceWebViewTutorial/PluginEditor.h"
#include <optional>
#include "JuceWebViewTutorial/PluginProcessor.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_extra/juce_gui_extra.h"

namespace audio_plugin {
namespace {
std::vector<std::byte> streamToVector(juce::InputStream& stream) {
  const auto sizeInBytes = static_cast<size_t>(stream.getTotalLength());
  std::vector<std::byte> result(sizeInBytes);
  stream.setPosition(0);
  [[maybe_unused]] const auto bytesRead =
      stream.read(result.data(), result.size());
  jassert(bytesRead == static_cast<ssize_t>(sizeInBytes));
  return result;
}
}  // namespace

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      webView(juce::WebBrowserComponent::Options{}.withResourceProvider(
          [this](const auto& url) { return getResource(url); })) {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(webView);

  // WebBrowserComponent can display any URL
  // webView.goToURL("https://juce.com");

  // This is necessary if we want to use a ResourceProvider
  webView.goToURL(webView.getResourceProviderRoot());

  setResizable(true, true);
  setSize(800, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::resized() {
  webView.setBounds(getBounds().withTrimmedLeft(getWidth() / 2));
}

auto AudioPluginAudioProcessorEditor::getResource(const juce::String& url) const
    -> std::optional<Resource> {
  DBG("ResourceProvider called with " + url);

  static const auto resourceFilesRoot =
      juce::File::getSpecialLocation(
          juce::File::SpecialLocationType::currentApplicationFile)
          .getParentDirectory()
          .getParentDirectory()
          .getChildFile("public");

  DBG("Resource files root is " + resourceFilesRoot.getFullPathName());

  if (url == "/") {
    const auto indexHtml =
        resourceFilesRoot.getChildFile("index.html").createInputStream();
    jassert(indexHtml != nullptr);
    return Resource{streamToVector(*indexHtml), juce::String{"text/html"}};
  }

  return std::nullopt;
}
}  // namespace audio_plugin
