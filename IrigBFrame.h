#ifndef _IRIG_B_FRAME_H_
#define _IRIG_B_FRAME_H_

#include <string>
#include <vector>

#include "IrigSymbol.h"
#include "IrigTime.h"

class IrigBFrame {
 public:
  typedef std::vector<IrigSymbol> Frame;
  const int kFrameSize = 100;

  IrigBFrame(const IrigTime& time);
  IrigBFrame(const IrigBFrame& frame) = default;
  IrigBFrame& operator=(const IrigBFrame& frame) = default;

  Frame GetFrame() const;

  std::string Dump() const;

 private:
  // Symbol.
  void PushSymbol(IrigSymbol symbol);
  // Bit.
  void PushBit(bool bit);
  // 6-bits BCD for hours.
  void PushBCD6(int value);
  // 7-bits BCD for seconds and minutes.
  void PushBCD7(int value);
  // 8-bits BCD for years.
  void PushBCD8(int value);
  // 10-bits BCD for days.
  void PushBCD10(int value);
  // 16-bits binary for ToD seconds.
  void PushBinary16(int value);

  Frame frame_;
};

#endif
