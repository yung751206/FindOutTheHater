//
//  fetchHtml.h
//  testcurl
//
//  Created by Edison on 9/21/14.
//  Copyright (c) 2014 edison. All rights reserved.
//

#ifndef __testcurl__fetchHtml__
#define __testcurl__fetchHtml__

#include <iostream>
#include <string>
#include <curl/curl.h>

class FetchHtml{
public:
    FetchHtml();
    void setFileName(std::string fileName);
    void getHtml(std::string &address);
    void post();
private:
    const char *m_headerFileName;
    const char *m_bodyFileName;
    FILE *m_headerfile;
    FILE *m_bodyfile;
    CURL *curl_handle;
    static size_t write_data(void *ptr,size_t size, size_t nmemb, void *stream);
};
#endif /* defined(__testcurl__fetchHtml__) */
