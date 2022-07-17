#ifndef _IRIG_TIME_H_
#define _IRIG_TIME_H_

struct IrigTime {
  int seconds;
  int minutes;
  int hours;
  int days;
  int years;
  int seconds_tod;
};

#endif
