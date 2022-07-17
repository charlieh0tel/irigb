#include "IrigBFrame.h"

#include <absl/base/macros.h>

#include <sstream>

//
// Reference:
// FM-31 IRIG STANDARD 200-16 IRIG SERIAL TIME CODE FORMATS
// http://www.irig106.org/docs/rcc/200-16_IRIG_Serial_Time_Code_Formats.pdf
//

IrigBFrame::IrigBFrame(const IrigTime& time) {
  frame_.reserve(kFrameSize);

  // Base on Figure 5-2, Table 5-4, and Table 5-5.
  //
  // + 000 ms - Reference mark
  ABSL_ASSERT(frame_.size() == 0);
  PushSymbol(IrigSymbol::kReferenceMark);
  //
  // + 010 ms - Seconds
  ABSL_ASSERT(frame_.size() == 1);
  PushBCD7(time.seconds);
  PushSymbol(IrigSymbol::kPositionMark);
  //
  // + 100 ms - Minutes
  ABSL_ASSERT(frame_.size() == 10);
  PushBCD7(time.minutes);
  PushSymbol(IrigSymbol::kIndexMark);
  PushSymbol(IrigSymbol::kPositionMark);
  //
  // + 200 ms - Hours
  ABSL_ASSERT(frame_.size() == 20);
  PushBCD6(time.hours);
  PushSymbol(IrigSymbol::kIndexMark);
  PushSymbol(IrigSymbol::kIndexMark);
  PushSymbol(IrigSymbol::kPositionMark);
  //
  // + 300 ms - Days
  ABSL_ASSERT(frame_.size() == 30);
  PushBCD10(time.days);
  ABSL_ASSERT(frame_.size() == 42);
  for (int i = 42; i <= 48; ++i) {
    PushSymbol(IrigSymbol::kIndexMark);
  }
  ABSL_ASSERT(frame_.size() == 49);
  PushSymbol(IrigSymbol::kPositionMark);
  //
  // + 500 ms - Years
  ABSL_ASSERT(frame_.size() == 50);
  PushBCD8(time.years);
  PushSymbol(IrigSymbol::kPositionMark);
  //
  // + 600 ms - Control bits [10, 18]
  ABSL_ASSERT(frame_.size() == 60);
  for (int i = 10; i <= 18; ++i) {
    PushBit(false);
  }
  PushSymbol(IrigSymbol::kPositionMark);
  //
  // + 700 ms - Control bits [19, 27]
  ABSL_ASSERT(frame_.size() == 70);
  for (int i = 19; i <= 27; ++i) {
    PushBit(false);
  }
  PushSymbol(IrigSymbol::kPositionMark);
  //
  ABSL_ASSERT(frame_.size() == 80);
  PushBinary16(time.seconds_tod);
  PushSymbol(IrigSymbol::kIndexMark);
  PushSymbol(IrigSymbol::kPositionMark);

  ABSL_ASSERT(frame_.size() == kFrameSize);
}

std::string IrigBFrame::Dump() const {
  std::stringstream out;

  for (int i = 0; i < frame_.size(); ++i) {
    auto value = frame_[i];

    if (i % 10 == 9) {
      out << std::endl;
    }

    switch (value) {
      case IrigSymbol::kZero:
        out << "0 ";
        break;
      case IrigSymbol::kOne:
        out << "1 ";
        break;
      case IrigSymbol::kReferenceMark:
        out << "  R ";
        break;
      case IrigSymbol::kPositionMark:
        out << "P ";
        break;
      case IrigSymbol::kIndexMark:
        out << "I ";
        break;
      default:
        ABSL_ASSERT(false);
    }
  }
  out << std::endl;

  return out.str();
}

void IrigBFrame::PushSymbol(IrigSymbol symbol) { frame_.push_back(symbol); }

void IrigBFrame::PushBit(bool bit) { PushSymbol(IrigSymbol(bit)); }

void IrigBFrame::PushBCD6(int value) {
  ABSL_ASSERT(value >= 0 && value <= 39);
  int ones = value % 10;
  int tens = value / 10;

  PushBit((ones >> 0) & 1);
  PushBit((ones >> 1) & 1);
  PushBit((ones >> 2) & 1);
  PushBit((ones >> 3) & 1);
  PushSymbol(IrigSymbol::kIndexMark);
  PushBit((tens >> 0) & 1);
  PushBit((tens >> 1) & 1);
}

void IrigBFrame::PushBCD7(int value) {
  ABSL_ASSERT(value >= 0 && value <= 79);
  int ones = value % 10;
  int tens = value / 10;

  PushBit((ones >> 0) & 1);
  PushBit((ones >> 1) & 1);
  PushBit((ones >> 2) & 1);
  PushBit((ones >> 3) & 1);
  PushSymbol(IrigSymbol::kIndexMark);
  PushBit((tens >> 0) & 1);
  PushBit((tens >> 1) & 1);
  PushBit((tens >> 2) & 1);
}

void IrigBFrame::PushBCD8(int value) {
  ABSL_ASSERT(value >= 0 && value <= 99);
  int ones = value % 10;
  int tens = value / 10;

  PushBit((ones >> 0) & 1);
  PushBit((ones >> 1) & 1);
  PushBit((ones >> 2) & 1);
  PushBit((ones >> 3) & 1);
  PushSymbol(IrigSymbol::kIndexMark);
  PushBit((tens >> 0) & 1);
  PushBit((tens >> 1) & 1);
  PushBit((tens >> 2) & 1);
  PushBit((tens >> 3) & 1);
}

void IrigBFrame::PushBCD10(int value) {
  ABSL_ASSERT(value >= 0 && value <= 399);
  int ones = value % 10;
  int tens = (value / 10) % 10;
  int hundreds = value / 100;

  PushBit((ones >> 0) & 1);
  PushBit((ones >> 1) & 1);
  PushBit((ones >> 2) & 1);
  PushBit((ones >> 3) & 1);
  PushSymbol(IrigSymbol::kIndexMark);
  PushBit((tens >> 0) & 1);
  PushBit((tens >> 1) & 1);
  PushBit((tens >> 2) & 1);
  PushBit((tens >> 3) & 1);
  PushSymbol(IrigSymbol::kPositionMark);
  PushBit((hundreds >> 0) & 1);
  PushBit((hundreds >> 1) & 1);
}

void IrigBFrame::PushBinary16(int value) {
  ABSL_ASSERT(value >= 0 && value <= 0x1fffff);
  for (int i = 0; i <= 8; ++i) {
    PushBit((value >> i) & 1);
  }
  PushSymbol(IrigSymbol::kPositionMark);
  for (int i = 9; i <= 16; ++i) {
    PushBit((value >> i) & 1);
  }
}
