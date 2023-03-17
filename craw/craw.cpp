#include "craw.h"

#include <Windows.h>
#include <wininet.h>

#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "wininet.lib")

std::vector<article> parseHtml(std::string html) {
  std::vector<article> outVect;
  for (int i = 0; i < html.size(); i++) {
    if (html[i] == '<') {
      std::string type = "", val = "", link = "", title = "";
      int cur_ind_t = i + 1, cur_ind_v;
      for (;; cur_ind_t++) {
        if (html[cur_ind_t] == '>') {
          cur_ind_v = cur_ind_t + 1;
          break;
        } else {
          type += html[cur_ind_t];
        }
      }
      if (type == "h2") {
        for (;; cur_ind_v++) {
          if (html[cur_ind_v] == '<' && html[cur_ind_v + 1] == '/' &&
              html[cur_ind_v + 2] == 'h' && html[cur_ind_v + 3] == '2') {
            break;
          } else {
            val += html[cur_ind_v];
          }
        }
        long link_s = 999;
        for (int ii = 0; ii < val.size(); ii++) {
          if (val[ii] == '"' && link_s == 999) {
            link_s = ii;
          } else if (val[ii] == '"' && link_s != 999) {
            int title_s = ii;
            for (;; title_s++) {
              if (val[title_s] == '>') {
                title_s++;
                break;
              }
            }
            for (;; title_s++) {
              if (val[title_s] == '<') {
                break;
              } else {
                title += val[title_s];
              }
            }
            struct article target {
              title, link
            };
            outVect.push_back(target);
          } else if (ii > link_s) {
            link += val[ii];
          }
        }
      }
    }
  }
  return outVect;
}

std::vector<article> getWeb(std::string url) {
  LPCSTR url_c = url.c_str();
  TCHAR l_url_c = strlen(url_c);
  TCHAR szHostName[128];
  TCHAR szUrlPath[256];
  URL_COMPONENTS crackedURL = {0};
  crackedURL.dwStructSize = sizeof(URL_COMPONENTS);
  crackedURL.lpszHostName = szHostName;
  crackedURL.dwHostNameLength = ARRAYSIZE(szHostName);
  crackedURL.lpszUrlPath = szUrlPath;
  crackedURL.dwUrlPathLength = ARRAYSIZE(szUrlPath);
  InternetCrackUrl(url_c, (DWORD)l_url_c, 0, &crackedURL);
  LPCSTR browser = "Microsoft InternetExplorer";
  HINTERNET hInternet =
      InternetOpen(browser, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (hInternet == NULL) throw std::runtime_error("-1");

  HINTERNET hHttpSession =
      InternetConnect(hInternet, crackedURL.lpszHostName, crackedURL.nPort,
                      NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
  if (hHttpSession == NULL) {
    InternetCloseHandle(hInternet);
    throw std::runtime_error("-2");
  }
  LPCSTR meth = "GET", blan = "";
  HINTERNET hHttpRequest = HttpOpenRequest(
      hHttpSession, meth, crackedURL.lpszUrlPath, NULL, blan, NULL, 0, 0);
  if (hHttpRequest == NULL) {
    InternetCloseHandle(hHttpSession);
    InternetCloseHandle(hInternet);
    throw std::runtime_error("-3");
  }
  DWORD dwRetCode = 0;
  DWORD dwSizeOfRq = sizeof(DWORD);
  if (!HttpSendRequest(hHttpRequest, NULL, 0, NULL, 0) ||
      !HttpQueryInfo(hHttpRequest,
                     HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                     &dwRetCode, &dwSizeOfRq, NULL) ||
      dwRetCode >= 400) {
    InternetCloseHandle(hHttpRequest);
    InternetCloseHandle(hHttpSession);
    InternetCloseHandle(hInternet);
    throw std::runtime_error("-4");
  }
  std::string content;
  DWORD dwError;
  DWORD dwBytesRead;
  DWORD nCurrentBytes = 0;
  char szBuffer[1024] = {0};
  while (TRUE) {
    if (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer),
                         &dwBytesRead)) {
      if (dwBytesRead == 0) {
        break;
      }
      nCurrentBytes += dwBytesRead;
      content += szBuffer;
    } else {
      dwError = GetLastError();
      break;
    }
  }
  InternetCloseHandle(hHttpRequest);
  InternetCloseHandle(hHttpSession);
  InternetCloseHandle(hInternet);
  return parseHtml(content);
}