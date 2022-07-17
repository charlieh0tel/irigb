#include <absl/base/macros.h>

#include <iostream>

#include "IrigBFrame.h"
#include "IrigTime.h"

int main(int argc, char **argv) {
  using namespace std;

  IrigTime time;
  time.seconds = 42;
  time.minutes = 18;
  time.hours = 21;
  time.days = 173;
  time.years = 3;
  time.seconds_tod = 0b10010101110110010;

  auto frame = IrigBFrame(time);

  cout << frame.Dump() << endl;
}
