#pragma once

#include <vector>

class Pattern
{
public:
  Pattern(unsigned int channels, unsigned int steps);

  unsigned int numChannels() const;
  unsigned int numSteps() const;

  void set(unsigned int channel, unsigned int step, bool value = true);
  bool isSet(unsigned int channel, unsigned int step) const;

private:
  unsigned int indexOf(unsigned int channel, unsigned int step) const;

  unsigned int channels;
  unsigned int steps;
  std::vector<bool> data;
};
