#include "VolumenAutomatic.h"


Volumen::Volumen() {

}

Volumen::~Volumen() {


}

/**
* @brief function to process the samples and multiply by inGain in order to
*        increase the volume
	return time in seconds
*/
void Volumen::processVolumen(float* inAudio, float* outAudio,
	float inGain, float inNumSamplesToRender) {

	/*convierto a scala de decibeles el valor del slider(o incremento de volumen)*/
	inGain = juce::Decibels::decibelsToGain(inGain, -60.0f);

	/**
	itera todos los samples entrantes 1 por 1
	inputsampletorender puede ser 512,1024 o whatever depende del buffer6
	*/
	for (int i = 0; i < inNumSamplesToRender; i++) {
		/*Cada muestra de entrada se multiplicara por la ganncia y esto sera la salida*/
		outAudio[i] = inAudio[i] * inGain; 
	}
	/*goto pluginProcessor*/
}




