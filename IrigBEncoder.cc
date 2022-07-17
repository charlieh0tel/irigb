#include "IrigBEncoder.h"

#include <absl/base/macros.h>

//
// Reference:
// FM-31 IRIG STANDARD 200-16 IRIG SERIAL TIME CODE FORMATS
// http://www.irig106.org/docs/rcc/200-16_IRIG_Serial_Time_Code_Formats.pdf
//


namespace {
void PushBit(IrigBFrame& frame, bool bit) { frame.push_back(IrigBSymbols(bit)); }

// 7-bits BCD for seconds and minutes.
void PushBCD7(IrigBFrame& frame, int value) {
  ABSL_ASSERT(value >= 0 && value <= 79);
  int ones = value % 10;
  int tens = value / 10;

  PushBit(frame, (ones >> 0) & 1);
  PushBit(frame, (ones >> 1) & 1);
  PushBit(frame, (ones >> 2) & 1);
  PushBit(frame, (ones >> 3) & 1);
  frame.push_back(IrigBSymbols::kIndexMark);
  PushBit(frame, (tens >> 0) & 1);
  PushBit(frame, (tens >> 1) & 1);
  PushBit(frame, (tens >> 2) & 1);
}

// 6-bits BCD for hours.
void PushBCD6(IrigBFrame& frame, int value) {
  ABSL_ASSERT(value >= 0 && value <= 39);
  int ones = value % 10;
  int tens = value / 10;

  PushBit(frame, (ones >> 0) & 1);
  PushBit(frame, (ones >> 1) & 1);
  PushBit(frame, (ones >> 2) & 1);
  PushBit(frame, (ones >> 3) & 1);
  frame.push_back(IrigBSymbols::kIndexMark);
  PushBit(frame, (tens >> 0) & 1);
  PushBit(frame, (tens >> 1) & 1);
}

// 10-bits BCD for days.
void PushBCD10(IrigBFrame& frame, int value) {
  ABSL_ASSERT(value >= 0 && value <= 399);
  int ones = value % 10;
  int tens = (value / 10) % 10;
  int hundreds = value / 100;

  PushBit(frame, (ones >> 0) & 1);
  PushBit(frame, (ones >> 1) & 1);
  PushBit(frame, (ones >> 2) & 1);
  PushBit(frame, (ones >> 3) & 1);
  frame.push_back(IrigBSymbols::kIndexMark);
  PushBit(frame, (tens >> 0) & 1);
  PushBit(frame, (tens >> 1) & 1);
  PushBit(frame, (tens >> 2) & 1);
  PushBit(frame, (tens >> 3) & 1);
  frame.push_back(IrigBSymbols::kPositionMark);
  PushBit(frame, (hundreds >> 0) & 1);
  PushBit(frame, (hundreds >> 1) & 1);
}

// 8-bits BCD for days.
void PushBCD8(IrigBFrame& frame, int value) {
  ABSL_ASSERT(value >= 0 && value <= 99);
  int ones = value % 10;
  int tens = value / 10;

  PushBit(frame, (ones >> 0) & 1);
  PushBit(frame, (ones >> 1) & 1);
  PushBit(frame, (ones >> 2) & 1);
  PushBit(frame, (ones >> 3) & 1);
  frame.push_back(IrigBSymbols::kIndexMark);
  PushBit(frame, (tens >> 0) & 1);
  PushBit(frame, (tens >> 1) & 1);
  PushBit(frame, (tens >> 2) & 1);
  PushBit(frame, (tens >> 3) & 1);
}

// 16-bits binary for ToD seconds.
void PushBinary16(IrigBFrame& frame, int value) {
  ABSL_ASSERT(value >= 0 && value <= 0x1fffff);
  for (int i = 0; i <= 8; ++i) {
    PushBit(frame, (value >> i) & 1);
  }
  frame.push_back(IrigBSymbols::kPositionMark);
  for (int i = 9; i <= 16; ++i) {
    PushBit(frame, (value >> i) & 1);
  }
}
}  // namespace

IrigBFrame Encode(const IrigBTime& time) {
  IrigBFrame frame;

  frame.reserve(kIrigBFrameSize);

  // Base on Figure 5-2, Table 5-4, and Table 5-5.
  //
  // + 000 ms - Reference mark
  ABSL_ASSERT(frame.size() == 0);
  frame.push_back(IrigBSymbols::kReferenceMark);
  //
  // + 010 ms - Seconds
  ABSL_ASSERT(frame.size() == 1);
  PushBCD7(frame, time.seconds);
  frame.push_back(IrigBSymbols::kPositionMark);
  //
  // + 100 ms - Minutes
  ABSL_ASSERT(frame.size() == 10);
  PushBCD7(frame, time.minutes);
  frame.push_back(IrigBSymbols::kIndexMark);
  frame.push_back(IrigBSymbols::kPositionMark);
  //
  // + 200 ms - Hours
  ABSL_ASSERT(frame.size() == 20);
  PushBCD6(frame, time.hours);
  frame.push_back(IrigBSymbols::kIndexMark);
  frame.push_back(IrigBSymbols::kIndexMark);
  frame.push_back(IrigBSymbols::kPositionMark);
  //
  // + 300 ms - Days
  ABSL_ASSERT(frame.size() == 30);
  PushBCD10(frame, time.days);
  ABSL_ASSERT(frame.size() == 42);
  for (int i = 42; i <= 48; ++i) {
    frame.push_back(IrigBSymbols::kIndexMark);
  }
  ABSL_ASSERT(frame.size() == 49);
  frame.push_back(IrigBSymbols::kPositionMark);
  //
  // + 500 ms - Years
  ABSL_ASSERT(frame.size() == 50);
  PushBCD8(frame, time.years);
  frame.push_back(IrigBSymbols::kPositionMark);
  //
  // + 600 ms - Control bits [10, 18]
  ABSL_ASSERT(frame.size() == 60);
  for (int i = 10; i <= 18; ++i) {
    PushBit(frame, false);
  }
  frame.push_back(IrigBSymbols::kPositionMark);
  //
  // + 700 ms - Control bits [19, 27]
  ABSL_ASSERT(frame.size() == 70);
  for (int i = 19; i <= 27; ++i) {
    PushBit(frame, false);
  }
  frame.push_back(IrigBSymbols::kPositionMark);
  //
  ABSL_ASSERT(frame.size() == 80);
  PushBinary16(frame, time.seconds_tod);
  frame.push_back(IrigBSymbols::kIndexMark);
  frame.push_back(IrigBSymbols::kPositionMark);

  ABSL_ASSERT(frame.size() == kIrigBFrameSize);
  return frame;
}
