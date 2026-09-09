#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SampleRateDestroyerAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SampleRateDestroyerAudioProcessorEditor(SampleRateDestroyerAudioProcessor&);
    ~SampleRateDestroyerAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SampleRateDestroyerAudioProcessor& processor;
    juce::Slider knob;
    juce::Label title;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleRateDestroyerAudioProcessorEditor)
};
