#include <iostream>

#include "IrigBEncoder.h"

int main(int argc, char **argv) {
  using namespace std;

  IrigBTime time;
  time.seconds = 10;
  time.minutes = 20;
  time.hours = 13;
  time.days = 300;
  time.years = 40;
  time.seconds_tod = 1000;

  IrigBFrame frame = Encode(time);

  for (int i = 0; i < frame.size(); ++i) {
    auto value = frame[i];

    switch (i) {
      case 1:
      case 10:
      case 20:
      case 30:
      case 50:
      case 60:
      case 80:
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
        cout << "R ";
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
