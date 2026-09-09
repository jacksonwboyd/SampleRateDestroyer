#include "PluginProcessor.h"
#include "PluginEditor.h"

SampleRateDestroyerAudioProcessor::SampleRateDestroyerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameters())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SampleRateDestroyerAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "sampleRate", 1 },
        "Sample Rate",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f),
        1.0f));
    return layout;
}

bool SampleRateDestroyerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto in  = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();

    if (in != out)
        return false;

    return out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo();
}

void SampleRateDestroyerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    heldSamples.assign(static_cast<size_t>(getTotalNumInputChannels()), 0.0f);
    phase = 0.0;
    previousKnob = -1.0f;
    juce::ignoreUnused(samplesPerBlock);
}

void SampleRateDestroyerAudioProcessor::resetReducerState()
{
    std::fill(heldSamples.begin(), heldSamples.end(), 0.0f);
    phase = 0.0;
}

float SampleRateDestroyerAudioProcessor::getTargetSampleRate(float knob) const noexcept
{
    // 0.0 = extremely crushed, 1.0 = transparent.
    // Log mapping makes the useful lo-fi range occupy most of the knob.
    const float minRate = 350.0f;
    const float maxRate = static_cast<float>(currentSampleRate);
    return minRate * std::pow(maxRate / minRate, juce::jlimit(0.0f, 1.0f, knob));
}

void SampleRateDestroyerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                      juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midi);

    const int channels = juce::jmin(buffer.getNumChannels(), static_cast<int>(heldSamples.size()));
    const int numSamples = buffer.getNumSamples();
    const float knob = apvts.getRawParameterValue("sampleRate")->load();

    if (previousKnob < 0.0f || std::abs(knob - previousKnob) > 0.0001f)
    {
        // Avoid zippering/reset clicks only when the parameter has moved materially.
        // The actual held-sample state remains continuous.
        previousKnob = knob;
    }

    const double targetRate = static_cast<double>(getTargetSampleRate(knob));
    const double increment = targetRate / currentSampleRate;

    if (knob >= 0.9999f)
    {
        // Exact top position is a true bypass: no coloration, no stateful reduction.
        return;
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        phase += increment;

        if (phase >= 1.0)
        {
            phase -= std::floor(phase);
            for (int ch = 0; ch < channels; ++ch)
                heldSamples[static_cast<size_t>(ch)] = buffer.getSample(ch, sample);
        }

        for (int ch = 0; ch < channels; ++ch)
            buffer.setSample(ch, sample, heldSamples[static_cast<size_t>(ch)]);
    }
}

void SampleRateDestroyerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary(*xml, destData);
}

void SampleRateDestroyerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor* SampleRateDestroyerAudioProcessor::createEditor()
{
    return new SampleRateDestroyerAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SampleRateDestroyerAudioProcessor();
}
