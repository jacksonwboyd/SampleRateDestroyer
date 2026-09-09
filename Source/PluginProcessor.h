#pragma once
#include <JuceHeader.h>

class SampleRateDestroyerAudioProcessor final : public juce::AudioProcessor
{
public:
    SampleRateDestroyerAudioProcessor();
    ~SampleRateDestroyerAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    double currentSampleRate = 44100.0;
    std::vector<float> heldSamples;
    double phase = 0.0;
    float previousKnob = -1.0f;

    float getTargetSampleRate(float knob) const noexcept;
    void resetReducerState();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleRateDestroyerAudioProcessor)
};
