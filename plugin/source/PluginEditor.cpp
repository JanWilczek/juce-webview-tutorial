#include "JuceWebViewTutorial/PluginEditor.h"
#include <juce_events/juce_events.h>
#include <optional>
#include <ranges>
#include "JuceWebViewTutorial/PluginProcessor.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "JuceWebViewTutorial/ParameterIDs.hpp"

namespace audio_plugin {
namespace {
std::vector<std::byte> streamToVector(juce::InputStream& stream) {
  // Workaround to make ssize_t work cross-platform.
  using namespace juce;
  const auto sizeInBytes = static_cast<size_t>(stream.getTotalLength());
  std::vector<std::byte> result(sizeInBytes);
  stream.setPosition(0);
  [[maybe_unused]] const auto bytesRead =
      stream.read(result.data(), result.size());
  jassert(bytesRead == static_cast<ssize_t>(sizeInBytes));
  return result;
}

static const char* getMimeForExtension(const juce::String& extension) {
  static const std::unordered_map<juce::String, const char*> mimeMap = {
      {{"htm"}, "text/html"},
      {{"html"}, "text/html"},
      {{"txt"}, "text/plain"},
      {{"jpg"}, "image/jpeg"},
      {{"jpeg"}, "image/jpeg"},
      {{"svg"}, "image/svg+xml"},
      {{"ico"}, "image/vnd.microsoft.icon"},
      {{"json"}, "application/json"},
      {{"png"}, "image/png"},
      {{"css"}, "text/css"},
      {{"map"}, "application/json"},
      {{"js"}, "text/javascript"},
      {{"woff2"}, "font/woff2"}};

  if (const auto it = mimeMap.find(extension.toLowerCase());
      it != mimeMap.end())
    return it->second;

  jassertfalse;
  return "";
}

juce::Identifier getExampleEventId() {
  static const juce::Identifier id{"exampleevent"};
  return id;
}
}  // namespace

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      gainSliderAttachment{processorRef.state, id::GAIN.getParamID(),
                           gainSlider},
      webView{
          juce::WebBrowserComponent::Options{}
              .withBackend(
                  juce::WebBrowserComponent::Options::Backend::webview2)
              .withWinWebView2Options(
                  juce::WebBrowserComponent::Options::WinWebView2{}
                      .withBackgroundColour(juce::Colours::white))
              .withResourceProvider(
                  [this](const auto& url) { return getResource(url); })
              .withInitialisationData("vendor", JUCE_COMPANY_NAME)
              .withInitialisationData("pluginName", JUCE_PRODUCT_NAME)
              .withInitialisationData("pluginVersion", JUCE_PRODUCT_VERSION)
              .withUserScript("console.log(\"C++ backend here: This is run "
                              "before any other loading happens\");")
              .withEventListener(
                  "exampleJavaScriptEvent",
                  [this](juce::var objectFromFrontend) {
                    labelUpdatedFromJavaScript.setText(
                        "example JavaScript event occurred with value " +
                            objectFromFrontend.getProperty("emittedCount", 0)
                                .toString(),
                        juce::dontSendNotification);
                  })
              .withNativeFunction(
                  juce::Identifier{"nativeFunction"},
                  [this](const juce::Array<juce::var>& args,
                         juce::WebBrowserComponent::NativeFunctionCompletion
                             completion) {
                    nativeFunction(args, std::move(completion));
                  })} {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(webView);

  // WebBrowserComponent can display any URL
  // webView.goToURL("https://juce.com");

  // This is necessary if we want to use a ResourceProvider
  webView.goToURL(webView.getResourceProviderRoot());

  runJavaScriptButton.onClick = [this] {
    constexpr auto JAVASCRIPT_TO_RUN{"console.log(\"Hello from C++!\")"};
    webView.evaluateJavascript(
        JAVASCRIPT_TO_RUN,
        [](juce::WebBrowserComponent::EvaluationResult result) {
          if (const auto* resultPtr = result.getResult()) {
            DBG("JavaScript evaluation result: " + resultPtr->toString());
          } else {
            DBG("JavaScript evaluation failed because " +
                result.getError()->message);
          }
        });
  };
  addAndMakeVisible(runJavaScriptButton);

  emitJavaScriptEventButton.onClick = [this] {
    static const juce::var valueToEmit{42.0};
    webView.emitEventIfBrowserIsVisible(getExampleEventId(), valueToEmit);
  };
  addAndMakeVisible(emitJavaScriptEventButton);

  addAndMakeVisible(labelUpdatedFromJavaScript);

  gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
  addAndMakeVisible(gainSlider);

  setResizable(true, true);
  setSize(800, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::resized() {
  auto bounds = getBounds();
  webView.setBounds(bounds.removeFromRight(getWidth() / 2));
  runJavaScriptButton.setBounds(bounds.removeFromTop(50).reduced(5));
  emitJavaScriptEventButton.setBounds(bounds.removeFromTop(50).reduced(5));
  labelUpdatedFromJavaScript.setBounds(bounds.removeFromTop(50).reduced(5));
  gainSlider.setBounds(bounds.removeFromTop(50).reduced(5));
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

  [[maybe_unused]] static auto printRootOnce = [] {
    DBG("Resource files root is " + resourceFilesRoot.getFullPathName());
    return true;
  }();

  const auto resourceToRetrieve =
      url == "/" ? "index.html" : url.fromFirstOccurrenceOf("/", false, false);

  const auto resource =
      resourceFilesRoot.getChildFile(resourceToRetrieve).createInputStream();
  if (resource) {
    const auto extension =
        resourceToRetrieve.fromLastOccurrenceOf(".", false, false);
    return Resource{streamToVector(*resource), getMimeForExtension(extension)};
  }

  return std::nullopt;
}

void AudioPluginAudioProcessorEditor::nativeFunction(
    const juce::Array<juce::var>& args,
    juce::WebBrowserComponent::NativeFunctionCompletion completion) {
  using namespace std::views;
  juce::String concatenatedString;
  for (const auto& string : args | transform(&juce::var::toString)) {
    concatenatedString += string;
  }
  labelUpdatedFromJavaScript.setText(
      "Native function called with args: " + concatenatedString,
      juce::dontSendNotification);
  completion("nativeFunction callback: All OK!");
}
}  // namespace audio_plugin
