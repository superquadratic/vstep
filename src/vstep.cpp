#include <cmath>
#include <iostream>

#include "vstep.h"

//------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new VStep(audioMaster);
}

//------------------------------------------------------------------------------
VStep::VStep(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, 1, 0) // 1 program, 0 parameters
, pattern(8, 16) // 8 channels, 16 steps
{
  setUniqueID('VSTP');

  setNumInputs(0);
  setNumOutputs(2);
  isSynth();

  canProcessReplacing();
  canDoubleReplacing();

  vst_strncpy(programName, "Default", kVstMaxProgNameLen);

  pattern.set(0, 0);
  pattern.set(0, 4);
  pattern.set(0, 8);
  pattern.set(0, 12);
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
  vst_strncpy(name, "vStep", kVstMaxEffectNameLen);
  return true;
}

//------------------------------------------------------------------------------
bool VStep::getProductString(char* text)
{
  vst_strncpy(text, "vStep", kVstMaxProductStrLen);
  return true;
}

//------------------------------------------------------------------------------
bool VStep::getVendorString(char* text)
{
  vst_strncpy(text, "superquadratic.net", kVstMaxVendorStrLen);
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
  VstTimeInfo* info = getTimeInfo(0xffff);

  float beatsPerSecond = info->tempo / 60.0f;
  float secondsPerBuffer = sampleFrames / info->sampleRate;
  float beatsPerBuffer = secondsPerBuffer * beatsPerSecond;

  float currentPpq = info->ppqPos;
  float nextPpq = currentPpq + beatsPerBuffer;

  if (info->flags & kVstTransportPlaying)
  {
    float nextStepPpq = 0.25f * ceilf(info->ppqPos * 4);

    while (nextStepPpq >= currentPpq && nextStepPpq < nextPpq)
    {
      unsigned int step = static_cast<unsigned int>(nextStepPpq * 4) % pattern.numSteps();

      nextStepPpq += 0.25f;

      std::cout << "step: " << step << std::endl;

      for (VstInt32 i = 0; i < sampleFrames; ++i) {
        if ((i / 100) % 2 == 0) {
          outputs[0][i] = 1.0f;
          outputs[1][i] = 1.0f;
        }
        else
        {
          outputs[0][i] = 0.0f;
          outputs[1][i] = 0.0f;
        }
      }

      return;
    }
  }

  memset(outputs[0], 0, sizeof(outputs) * sampleFrames);
  memset(outputs[1], 0, sizeof(outputs) * sampleFrames);
}

//------------------------------------------------------------------------------
void VStep::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
  memset(outputs[0], 0, sizeof(outputs) * sampleFrames);
  memset(outputs[1], 0, sizeof(outputs) * sampleFrames);
}
