#include <iostream>
#include <absl/base/macros.h>

#include "IrigBEncoder.h"

int main(int argc, char **argv) {
  using namespace std;

  IrigBTime time;
  time.seconds = 42;
  time.minutes = 18;
  time.hours = 21;
  time.days = 173;
  time.years = 3;
  time.seconds_tod = 0b10010101110110010;

  IrigBFrame frame = Encode(time);

  for (int i = 0; i < frame.size(); ++i) {
    auto value = frame[i];

    if (i % 10 == 9) {
        cout << endl;
    }

    switch (value) {
      case IrigBSymbols::kZero:
        cout << "0 ";
        break;
      case IrigBSymbols::kOne:
        cout << "1 ";
        break;
      case IrigBSymbols::kReferenceMark:
        cout << "  R ";
        break;
      case IrigBSymbols::kPositionMark:
        cout << "P ";
        break;
      case IrigBSymbols::kIndexMark:
        cout << "I ";
        break;
      default:
        ABSL_ASSERT(false);
    }
  }
  cout << endl;
}
