#include "pattern.h"


Pattern::Pattern(unsigned int channels, unsigned int steps)
: channels(channels)
, steps(steps)
, data(channels * steps)
{
}

unsigned int Pattern::numChannels() const
{
  return channels;
}

unsigned int Pattern::numSteps() const
{
  return steps;
}

void Pattern::set(unsigned int channel, unsigned int step, bool value)
{
  data[indexOf(channel, step)] = value;
}

bool Pattern::isSet(unsigned int channel, unsigned int step) const
{
  return data[indexOf(channel, step)];
}

unsigned int Pattern::indexOf(unsigned int channel, unsigned int step) const
{
  return channel * steps + step;
}
