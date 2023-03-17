#include <string>
#include <vector>

#ifndef __CRAW_H__
#define __CRAW_H__

struct article {
  std::string title;
  std::string link;
};

std::vector<article> getWeb(std::string url_c);

#endif  // __CRAW_H__