/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Plug1AudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    Plug1AudioProcessorEditor (Plug1AudioProcessor&);
    ~Plug1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


    /*!< functions to get and set the slider value and callback to receive value changed*/
    void setDataSlider(double value);
    double getDataSlider();
    void sliderValueChanged(juce::Slider*) override;

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plug1AudioProcessor& audioProcessor;

    juce::Slider depthSlider{ juce::Slider::LinearHorizontal, juce::Slider::TextBoxLeft };
    juce::Label depthLabel{ {}, "Specific Time [seconds]:" };
    double sliderValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plug1AudioProcessorEditor)
};
