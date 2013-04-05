#include "public.sdk/source/vst2.x/audioeffectx.h"

enum MidiStatus
{
  MIDI_STATUS_NOTE_OFF = 0x8,
  MIDI_STATUS_NOTE_ON = 0x9,
  MIDI_STATUS_CONTROL_CHANGE = 0xb
};

class MidiMessage
{
public: 
  MidiMessage(MidiStatus status, unsigned char channel, unsigned char key, unsigned char velocity);

private:
  unsigned char bytes[4];
};

class MidiEventBuffer
{
public:
  typedef unsigned char Key;
  typedef unsigned char Controller;

  MidiEventBuffer();
  ~MidiEventBuffer();

  void send(AudioEffectX* plugin);

  void addNote(VstInt32 deltaFrames, Key key, unsigned char velocity, VstInt32 length);
  void addControlChange(VstInt32 deltaFrames, Controller controller, unsigned char value);

private:
  static const unsigned char MAX_EVENT_COUNT = 64;

  void addEvent(VstInt32 deltaFrames, VstInt32 length, MidiMessage msg);

  VstInt32 eventCount;
  VstEvents* events;
};
