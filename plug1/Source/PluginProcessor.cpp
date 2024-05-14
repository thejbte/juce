/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Plug1AudioProcessor::Plug1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), /*!< inicializador gui */
                        parameters(*this/*apunta a esta clase*/, nullptr, "PARAMETERS", initializeGUI())
#endif
{
    /*Cuando invoca el constructor inicializa los canales */
    initializeDSP();
    //startTimerHz(); // f=1/T  1/5  = 0.2  T= 1/f
    startTimer((int)(tickCounter*1000.0));
    counterSpecificValue = minimunCounterSpecificValue;
}


Plug1AudioProcessor::~Plug1AudioProcessor()
{
    stopTimer();
}

/**
* @brief function callback of timer to increase the volume variable
*/
void Plug1AudioProcessor::timerCallback()
{   //  -60 -----> 24  db
    //   0  -----> 10 sg (specific time)
    //tick = 1 sg (default)   84/(10/1) = 8.4db/s       0 1 2 3 4 5 6 7 8 9 
    //tick = 5 sg   84/(10/5) = 42 db/s       0   5  10

    float step = (maximunCounterSpecificValue - minimunCounterSpecificValue ) / (specificValueTime / tickCounter);
    counterSpecificValue += step;

    if (counterSpecificValue < maximunCounterSpecificValue)
    {
        DBG(" counter tick " << counterSpecificValue <<  " specificValueTime " << specificValueTime << " value " << step);
    }
    else {
        counterSpecificValue = minimunCounterSpecificValue;
    }

}


void Plug1AudioProcessor::initializeDSP() {
    for (int i = 0; i < getTotalNumOutputChannels(); i++) {
        //iteramos cada canal
        ptrVolumen[i] = std::unique_ptr<Volumen>(new Volumen()); // creo(asigno memoria) un nuevo puntero de tipo volumen 

    }
}

/**
* @brief function to get time duration of audio plugin from silence to full volume
    return time in seconds
*/
double Plug1AudioProcessor::getDataToSpecificTime() {
    return  specificValueTime;
}

/**
* @brief function to set time duration of audio plugin from silence to full volume
    @param specific time in seconds
*/
void Plug1AudioProcessor::setDataToSpecificTime(double value) {
    specificValueTime = value;
    
}

/**
* @brief function to initialize the slide to volume (manual)
    @param specific time in seconds
*/
juce::AudioProcessorValueTreeState::ParameterLayout Plug1AudioProcessor::initializeGUI() {

    /*lugar donde guardar el o los slider -> vector*/
    /*vector de tipo unique pointer y el puntero sera de Rangeaudioparamentr*/
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    /*slide ID, nombreLabel, valor minimo(en Db,,)*/
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Volumen",
        "volumen", -60.0f, 24.0f, 0.0f));

    return { params.begin(), params.end() }; /*devuelvo donde arranca y finaliza*/
}

//==============================================================================
const juce::String Plug1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plug1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plug1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plug1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plug1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plug1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plug1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plug1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Plug1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Plug1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Plug1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    counterSpecificValue = -60.0;  /*!< minimun value  to volume (silence) before start */
}

void Plug1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plug1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

/*Aca llegan nuestras samples*/
void Plug1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...

        /*!< here is where  I increase the volumen*/
        ptrVolumen[channel]->processVolumen(channelData,
            channelData, //*parameters.getRawParameterValue("Volumen"),
            (float)counterSpecificValue       
            ,buffer.getNumSamples());

    }
}

//==============================================================================
bool Plug1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Plug1AudioProcessor::createEditor()
{
    return new Plug1AudioProcessorEditor (*this);
}

//==============================================================================
void Plug1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plug1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plug1AudioProcessor();
}
