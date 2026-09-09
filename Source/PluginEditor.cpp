#include "PluginEditor.h"

SampleRateDestroyerAudioProcessorEditor::SampleRateDestroyerAudioProcessorEditor(SampleRateDestroyerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(260, 260);

    knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    knob.setRange(0.0, 1.0, 0.001);
    knob.setValue(1.0);
    knob.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                             juce::MathConstants<float>::pi * 2.75f,
                             true);
    knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffd8d8d8));
    knob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff303030));
    knob.setColour(juce::Slider::thumbColourId, juce::Colour(0xfff2f2f2));
    addAndMakeVisible(knob);

    title.setText("SAMPLE RATE", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    title.setColour(juce::Label::textColourId, juce::Colour(0xffdddddd));
    title.setFont(juce::FontOptions(13.0f, juce::Font::bold));
    addAndMakeVisible(title);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, "sampleRate", knob);
}

void SampleRateDestroyerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff111111));

    auto bounds = getLocalBounds().toFloat().reduced(14.0f);
    g.setColour(juce::Colour(0xff1b1b1b));
    g.fillRoundedRectangle(bounds, 8.0f);

    g.setColour(juce::Colour(0xff2b2b2b));
    g.drawRoundedRectangle(bounds, 8.0f, 1.0f);
}

void SampleRateDestroyerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    title.setBounds(area.removeFromTop(48).reduced(20, 10));
    knob.setBounds(area.reduced(36, 16));
}
