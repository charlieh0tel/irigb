#ifndef _IRIG_B_H_
#define _IRIG_B_H_

#include <vector>

enum class IrigBSymbols {
  kZero = 0,			/* 2 ms */
  kOne = 1,			/* 5 ms */
  kReferenceMark = -1,		/* 8 ms */
  kPositionMark = -2,		/* 8 ms */
  kIndexMark = -3,		/* 2 ms (same as zero) */
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
