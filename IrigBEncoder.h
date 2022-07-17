#ifndef _IRIG_B_H_
#define _IRIG_B_H_

#include <vector>

enum class IrigBSymbols {
  kZero = 0,
  kOne = 1,
  kReferenceMark = -1,
  kPositionMark = -2,
  kIndexMark = -3,
  kInvalid = -9999
};

const int kIrigBFrameSize = 100;

typedef std::vector<IrigBSymbols> IrigBFrame;

struct IrigBTime {
  int seconds;
  int minutes;
  int hours;
  int days;
  int years;
  int seconds_tod;
};

IrigBFrame Encode(const IrigBTime& time);

#endif
