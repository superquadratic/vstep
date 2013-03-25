#pragma once

#include "public.sdk/source/vst2.x/audioeffectx.h"

//-------------------------------------------------------------------------------------------------------
class VStep : public AudioEffectX
{
public:
	VStep(audioMasterCallback audioMaster);
	~VStep();

	// Processing
	virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName(char* name);
	virtual void getProgramName(char* name);

	// Info
	virtual bool getEffectName(char* name);
	virtual bool getVendorString(char* text);
	virtual bool getProductString(char* text);
	virtual VstInt32 getVendorVersion();

protected:
	char programName[kVstMaxProgNameLen + 1];
};
