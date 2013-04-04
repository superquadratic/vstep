#include <algorithm>
#include <vector>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

#include "UnitTest++.h"

#include "pattern.h"


struct Note
{
  Note(unsigned int channel, unsigned int step)
  : channel(channel)
  , step(step)
  {
  }

  int operator==(const Note& rhs)
  {
    return rhs.channel == channel and rhs.step == step;
  }

  unsigned int channel;
  unsigned int step;
};


TEST(PatternIsInitializedToOff)
{
  Pattern pattern(4, 16);

  for (unsigned int channel = 0; channel < pattern.numChannels(); ++channel)
  {
    for (unsigned int step = 0; step < pattern.numSteps(); ++step)
    {
      CHECK_EQUAL(false, pattern.isSet(channel, step));
    }
  }
}


TEST(PatternStepsCanBeSet)
{
  Pattern pattern(4, 16);

  std::vector<Note> notes = boost::assign::list_of
    (Note(0, 0))
    (Note(0, 4))
    (Note(0, 8))
    (Note(0, 12))
    (Note(1, 2))
    (Note(1, 6))
    (Note(1, 10))
    (Note(1, 14));

  BOOST_FOREACH(const Note& note, notes)
  {
    pattern.set(note.channel, note.step);
  }

  for (unsigned int channel = 0; channel < pattern.numChannels(); ++channel)
  {
    for (unsigned int step = 0; step < pattern.numSteps(); ++step)
    {
      std::vector<Note>::iterator found = std::find(notes.begin(), notes.end(), Note(channel, step));
      bool isSet = (found != notes.end());
      CHECK_EQUAL(isSet, pattern.isSet(channel, step));
    }
  }
}
