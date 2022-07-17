#ifndef _IRIG_SYMBOL_H_
#define _IRIG_SYMOBL_H_

enum class IrigSymbol {
  kZero = 0,
  kOne = 1,
  kReferenceMark = -1,
  kPositionMark = -2,
  kIndexMark = -3,
  kInvalid = -9999
};

#endif
