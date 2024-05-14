/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Plug1AudioProcessorEditor::Plug1AudioProcessorEditor (Plug1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);


    depthSlider.addListener(this);      /*!< link callback sliderValueChanged*/
    depthSlider.setRange(10.0, 200.0, 1.0);
    setDataSlider(audioProcessor.getDataToSpecificTime());  /*!< initialize slider with default specific time*/

}

Plug1AudioProcessorEditor::~Plug1AudioProcessorEditor()
{
}

//==============================================================================
void Plug1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    

    /*creating slider*/
    addAndMakeVisible(depthSlider);
    depthSlider.setBounds(80, 100, 300, 22);
    depthLabel.attachToComponent(&depthSlider, false); /*!< attach label name to slider*/
    depthLabel.setJustificationType(juce::Justification::centred);
}


void Plug1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


/**
* @brief function to set time duration of audio plugin to slider
    return time in seconds
*/
void Plug1AudioProcessorEditor::setDataSlider(double value) {

    depthSlider.setValue(value, juce::NotificationType::dontSendNotification);
    
}

/**
* @brief function to get time duration of audio plugin from slider
    return time in seconds
*/
double Plug1AudioProcessorEditor::getDataSlider() {
    return (int)depthSlider.getValue();
}

void Plug1AudioProcessorEditor::sliderValueChanged(juce::Slider*)
{
    DBG("sliderValueChanged " << depthSlider.getValue());
    sliderValue = (int)depthSlider.getValue();
    audioProcessor.setDataToSpecificTime(sliderValue); /*set data from slider to volumen audio processor*/

}