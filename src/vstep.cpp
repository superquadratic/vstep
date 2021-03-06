#include <cassert>
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
, keyForChannel(pattern.numChannels())
, pattern(8, 16) // 8 channels, 16 steps
, listener("7770", &pattern)
, keepAlive(0)
{
  setUniqueID('VSTP');

  setNumInputs(0);
  setNumOutputs(2);
  isSynth();

  canProcessReplacing();
  canDoubleReplacing();

  vst_strncpy(programName, "Default", kVstMaxProgNameLen);

  for (unsigned int channel = 0; channel < pattern.numChannels(); ++channel)
  {
    keyForChannel[channel] = 36 + channel;
  }
}

//------------------------------------------------------------------------------
VStep::~VStep()
{
  // nothing to do here
}

//------------------------------------------------------------------------------
VstInt32 VStep::canDo(char* feature)
{
  if (!strcmp(feature, "sendVstEvents")) return 1;
  if (!strcmp(feature, "sendVstMidiEvent")) return 1;
  if (!strcmp(feature, "receiveVstEvents")) return -1;
  if (!strcmp(feature, "receiveVstMidiEvent")) return -1;
  if (!strcmp(feature, "receiveVstTimeInfo")) return 1;
  if (!strcmp(feature, "offline")) return -1;
  if (!strcmp(feature, "midiProgramNames")) return -1;
  if (!strcmp(feature, "bypass")) return -1;

  return AudioEffectX::canDo(feature);
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
  memset(outputs[0], 0, sizeof(outputs) * sampleFrames);
  memset(outputs[1], 0, sizeof(outputs) * sampleFrames);

  VstTimeInfo* info = getTimeInfo(kVstTempoValid | kVstPpqPosValid);

  if (info->flags & kVstTransportPlaying)
  {
    float beatsPerSecond = info->tempo / 60.0f;
    float secondsPerBuffer = sampleFrames / info->sampleRate;
    float beatsPerBuffer = secondsPerBuffer * beatsPerSecond;

    float currentPpq = info->ppqPos;
    float nextPpq = currentPpq + beatsPerBuffer;
    float nextStepPpq = 0.25f * ceilf(info->ppqPos * 4);

    while (nextStepPpq >= currentPpq && nextStepPpq < nextPpq)
    {
      unsigned int step = static_cast<unsigned int>(nextStepPpq * 4) % pattern.numSteps();

      float framesPerBeat = info->sampleRate / beatsPerSecond;
      VstInt32 deltaFrames = (nextStepPpq - currentPpq) * framesPerBeat;

      assert(deltaFrames < sampleFrames);

      for (unsigned int channel = 0; channel < pattern.numChannels(); ++channel)
      {
        if (pattern.isSet(channel, step))
        {
          Key key = keyForChannel[channel];
          midiEventBuffer.addNote(deltaFrames, key, 127, 100);
        }
      }

      nextStepPpq += 0.25f;
    }

    if (hostIsAbletonLive())
    {
      // Ableton Live deactivates the plugin if it produces silence and no MIDI events
      // for some time. As a workaround, we keep sending program changes for controller 0.
      // See http://fogbugz.nuedge.net/default.asp?W159.

      midiEventBuffer.addControlChange(0, 0, keepAlive);
      keepAlive = 1 - keepAlive;
    }

    midiEventBuffer.send(this);
  }
}

//------------------------------------------------------------------------------
void VStep::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
  memset(outputs[0], 0, sizeof(outputs) * sampleFrames);
  memset(outputs[1], 0, sizeof(outputs) * sampleFrames);
}

//------------------------------------------------------------------------------
bool VStep::hostIsAbletonLive()
{
  static int isAbleton = -1;

  if (isAbleton == -1)
  {
    char vendor[65];
    getHostVendorString(vendor);

    if (strcmp(vendor, "Ableton") == 0)
    {
      isAbleton = 1;
    }
    else
    {
      isAbleton = 0;
    }
  }

  return isAbleton == 1;
}
