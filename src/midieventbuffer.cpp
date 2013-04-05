#include <cassert>
#include <iostream>

#include "midieventbuffer.h"

MidiMessage::MidiMessage(MidiStatus status, unsigned char channel, unsigned char key, unsigned char velocity)
{
  assert(channel < 16);
  assert(key < 128);
  assert(velocity < 128);

  bytes[0] = status << 4 + channel;
  bytes[1] = key;
  bytes[2] = velocity;
  bytes[3] = 0;
}

MidiEventBuffer::MidiEventBuffer()
: eventCount(0)
{
  size_t size = sizeof(VstEvents) + MAX_EVENT_COUNT * sizeof(VstEvent*);
  events = (VstEvents*)malloc(size);

  for (unsigned int i = 0; i < MAX_EVENT_COUNT; ++i)
  {
    events->events[i] = (VstEvent*)std::calloc(1, sizeof(VstMidiEvent));

    VstEvent* e = events->events[i];
    e->type = kVstMidiType;
    e->byteSize = sizeof(VstMidiEvent);
  }
}

MidiEventBuffer::~MidiEventBuffer()
{
  for (unsigned int i = 0; i < MAX_EVENT_COUNT; ++i)
  {
    free(events->events[i]);
  }

  free(events);
}

void MidiEventBuffer::send(AudioEffectX *plugin)
{
  if (eventCount > 0)
  {
    events->numEvents = eventCount;
    plugin->sendVstEventsToHost(events);
    eventCount = 0;
  }
}

void MidiEventBuffer::addNote(VstInt32 deltaFrames, Key key, VstInt32 length)
{
  addEvent(deltaFrames, length, MidiMessage(MIDI_STATUS_NOTE_ON, 0, key, 127));
  addEvent(deltaFrames + length, 0, MidiMessage(MIDI_STATUS_NOTE_OFF, 0, key, 0));
}

void MidiEventBuffer::addEvent(VstInt32 deltaFrames, VstInt32 length, MidiMessage msg)
{
  if (eventCount < MAX_EVENT_COUNT)
  {
    VstMidiEvent* e = (VstMidiEvent*)events->events[eventCount];
    ++eventCount;

    e->deltaFrames = deltaFrames;
    e->noteLength = length;

    memcpy(e->midiData, &msg, sizeof(MidiMessage));
  }
}
