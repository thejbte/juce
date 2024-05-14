#pragma once
#include <JuceHeader.h>


class Volumen {

public:
	Volumen();
	~Volumen();


	void processVolumen(float *inAudio, float *outAudio, float inGain, float inNumSamplesToRender);


};