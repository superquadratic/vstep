#include "vstep.h"

//------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new VStep(audioMaster);
}

//------------------------------------------------------------------------------
VStep::VStep(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, 1, 0) // 1 program, 0 parameters
{
  setUniqueID('VSTP');

  setNumInputs(2);
  setNumOutputs(2);
  isSynth();

  canProcessReplacing();
  canDoubleReplacing();

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
  memcpy(outputs[0], inputs[0], sampleFrames);
  memcpy(outputs[1], inputs[1], sampleFrames);
}

//------------------------------------------------------------------------------
void VStep::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
  memcpy(outputs[0], inputs[0], sampleFrames);
  memcpy(outputs[1], inputs[1], sampleFrames);
}
