#include "vstep.h"

//------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new VStep(audioMaster);
}

//------------------------------------------------------------------------------
VStep::VStep(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, 1, 1) // 1 program, 1 parameter only
{
  setUniqueID('VSTP');

  setNumInputs(2);
  setNumOutputs(2);
  isSynth();

  canProcessReplacing();
  canDoubleReplacing();

  fGain = 1.f;      // default to 0 dB
  vst_strncpy(programName, "Default", kVstMaxProgNameLen);  // default program name
}

//------------------------------------------------------------------------------
VStep::~VStep()
{
  // nothing to do here
}

//------------------------------------------------------------------------------
void VStep::setProgramName(char* name)
{
  vst_strncpy(programName, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------------
void VStep::getProgramName(char* name)
{
  vst_strncpy(name, programName, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------------
void VStep::setParameter(VstInt32 index, float value)
{
  fGain = value;
}

//------------------------------------------------------------------------------
float VStep::getParameter(VstInt32 index)
{
  return fGain;
}

//------------------------------------------------------------------------------
void VStep::getParameterName(VstInt32 index, char* label)
{
  vst_strncpy(label, "Gain", kVstMaxParamStrLen);
}

//------------------------------------------------------------------------------
void VStep::getParameterDisplay(VstInt32 index, char* text)
{
  dB2string(fGain, text, kVstMaxParamStrLen);
}

//------------------------------------------------------------------------------
void VStep::getParameterLabel(VstInt32 index, char* label)
{
  vst_strncpy(label, "dB", kVstMaxParamStrLen);
}

//------------------------------------------------------------------------------
bool VStep::getEffectName(char* name)
{
  vst_strncpy(name, "Gain", kVstMaxEffectNameLen);
  return true;
}

//------------------------------------------------------------------------------
bool VStep::getProductString(char* text)
{
  vst_strncpy(text, "Gain", kVstMaxProductStrLen);
  return true;
}

//------------------------------------------------------------------------------
bool VStep::getVendorString(char* text)
{
  vst_strncpy(text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
  return true;
}

//------------------------------------------------------------------------------
VstInt32 VStep::getVendorVersion()
{ 
  return 1000; 
}

//------------------------------------------------------------------------------
void VStep::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
  float* in1  =  inputs[0];
  float* in2  =  inputs[1];
  float* out1 = outputs[0];
  float* out2 = outputs[1];

  while (--sampleFrames >= 0)
  {
      (*out1++) = (*in1++) * fGain;
      (*out2++) = (*in2++) * fGain;
  }
}

//------------------------------------------------------------------------------
void VStep::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
  double* in1  =  inputs[0];
  double* in2  =  inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];
  double dGain = fGain;

  while (--sampleFrames >= 0)
  {
      (*out1++) = (*in1++) * dGain;
      (*out2++) = (*in2++) * dGain;
  }
}
