/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "VolumenAutomatic.h"



//==============================================================================
/**
*/
class Plug1AudioProcessor  : public juce::AudioProcessor , private juce::Timer
{
public:
    //==============================================================================
    Plug1AudioProcessor();
    ~Plug1AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    /*aqui creo los objetos a declarar*/
    void initializeDSP();
    void timerCallback() override;
    
    /*Creo slider*/
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState::ParameterLayout initializeGUI();

    /*!< functions to get and set value ot specifictime*/
    void setDataToSpecificTime(double value);
    double getDataToSpecificTime();

private:


    /*!< dos objetos(dos canales) -> dos punteros inteligentes a la clase*/
    float tickCounter = 1.0; /*1ms*/
    float specificValueTime = 30.0; /*!< -60  a 24 */
    const float minimunCounterSpecificValue = -60.0;
    const float maximunCounterSpecificValue = 24.0;
    float counterSpecificValue = -60.0;  /*0 volume*/
  
    
    std::unique_ptr<Volumen> ptrVolumen[2]; /*array de punteros de tipo volumen -> RAII*/
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plug1AudioProcessor)
};
