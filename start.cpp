#include <iostream>
#include <string>

#include "craw/craw.h"
#include "rfcTime/rfcTime.h"

using namespace std;

int main() {
  cout << rfcTime() << endl;
  string url;
  char yon;
  while (true) {
    cout << "Enter URL:" << endl;
    cin >> url;
    cout << url << " <-- Confirmed? [y/N]" << endl;
    cin >> yon;
    if (yon == 'y') {
      break;
    }
  }
  cout << "Confirmed URL -> " << url << endl;
  vector<article> content = getWeb(url);
  for (auto i : content) cout << i.title << endl;
  return 0;
}