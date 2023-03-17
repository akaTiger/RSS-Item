#include <ctime>
#include <string>

#include "rfcTime.h"

std::string wdParse(int wd) {
  switch (wd) {
    case 1:
      return std::string("Mon");
      break;
    case 2:
      return std::string("Tue");
      break;
    case 3:
      return std::string("Wed");
      break;
    case 4:
      return std::string("Thu");
      break;
    case 5:
      return std::string("Fri");
      break;
    case 6:
      return std::string("Sat");
      break;
    case 7:
      return std::string("Sun");
      break;
  }
  return std::string("NUL");
}

std::string mParse(int m) {
  switch (m) {
    case 0:
      return std::string("Jan");
      break;
    case 1:
      return std::string("Feb");
      break;
    case 2:
      return std::string("Mar");
      break;
    case 3:
      return std::string("Apr");
      break;
    case 4:
      return std::string("May");
      break;
    case 5:
      return std::string("Jun");
      break;
    case 6:
      return std::string("Jul");
      break;
    case 7:
      return std::string("Aug");
      break;
    case 8:
      return std::string("Sep");
      break;
    case 9:
      return std::string("Oct");
      break;
    case 10:
      return std::string("Nov");
      break;
    case 11:
      return std::string("Dec");
      break;
  }
  return std::string("NUL");
}

std::string tParse(int t) {
  std::string tStr = std::to_string(t);
  if (tStr.length() < 2) {
    return std::string(2 - tStr.length(), '0') + tStr;
  } else {
    return tStr;
  }
}

std::string rfcTime() {
  time_t curr_time;
  curr_time = time(NULL);
  tm *tm_gmt = gmtime(&curr_time);
  std::string curt = wdParse(tm_gmt->tm_wday) + ", " + tParse(tm_gmt->tm_mday) +
                     " " + mParse(tm_gmt->tm_mon) + " " +
                     std::to_string(tm_gmt->tm_year + 1900) + " " +
                     tParse(tm_gmt->tm_hour) + ":" + tParse(tm_gmt->tm_min) +
                     ":" + tParse(tm_gmt->tm_sec) + " GMT";
  return curt;
}