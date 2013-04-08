#include <iostream>

#include "oscpatternlistener.h"
#include "pattern.h"

const char* PATTERN_SET_PATH = "/pattern/set";
const char* PATTERN_CLEAR_PATH = "/pattern/clear";


static void error(int num, const char* msg, const char* path)
{
  std::cout << "liblo server error " << num << " at path " << path << ": "<< msg << std::endl;
}

static int stepHandler(const char* path, const char* types, lo_arg** argv, int argc, void* data, void* userData)
{
  Pattern* pattern = (Pattern*)userData;

  int32_t channel = argv[0]->i;
  int32_t step = argv[1]->i;

  if (channel >= 0 && channel < pattern->numChannels() &&
      step >= 0 && step < pattern->numSteps())
  {
    bool value = strcmp(path, PATTERN_SET_PATH) == 0;
    pattern->set(channel, step, value);
  }

  return 0;
}

OscPatternListener::OscPatternListener(const char* port, Pattern* pattern)
{
  st = lo_server_thread_new(port, error);

  lo_server_thread_add_method(st, PATTERN_SET_PATH, "ii", stepHandler, pattern);
  lo_server_thread_add_method(st, PATTERN_CLEAR_PATH, "ii", stepHandler, pattern);

  lo_server_thread_start(st);
}

OscPatternListener::~OscPatternListener()
{
  lo_server_thread_free(st);
}
