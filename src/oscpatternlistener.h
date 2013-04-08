#pragma once

#include <lo/lo.h>

class Pattern;

class OscPatternListener
{
public:
  OscPatternListener(const char* port, Pattern* pattern);
  ~OscPatternListener();

private:
  lo_server_thread st;
};
