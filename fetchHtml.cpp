//
//  fetchHtml.cpp
//  testcurl
//
//  Created by Edison on 9/21/14.
//  Copyright (c) 2014 edison. All rights reserved.
//

#include "fetchHtml.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>

size_t FetchHtml::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    int written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

FetchHtml::FetchHtml(){
    const char* cookieFile = "";
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
//    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, cookieFile);   //enable cookie engine

}

void FetchHtml::setFileName(std::string fileName){
    m_headerFileName = "head.out";
    m_bodyFileName = fileName.c_str();
}

void FetchHtml::getHtml(std::string &address){
    curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_URL, address.c_str());
    /* open the files */
    m_headerfile = fopen(m_headerFileName,"wb");
    if (m_headerfile == NULL) {
        std::cout<<"cannot open header file"<<std::endl;
    }
    m_bodyfile = fopen(m_bodyFileName,"wb");
    if (m_bodyfile == NULL) {
        std::cout<<"cannot open body file"<<std::endl;
    }
    curl_easy_setopt(curl_handle,   CURLOPT_HEADERDATA, m_headerfile);
    curl_easy_setopt(curl_handle,   CURLOPT_WRITEDATA, m_bodyfile);
    curl_easy_perform(curl_handle);
    fclose(m_headerfile);
    fclose(m_bodyfile);
}

void FetchHtml::post(){
    const char *data="yes=yes";
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);

    CURLcode res;
    m_headerfile = fopen(m_headerFileName,"wb");
    if (m_headerfile == NULL) {
        std::cout<<"cannot open header file"<<std::endl;
    }
    m_bodyfile = fopen(m_bodyFileName,"wb");
    if (m_bodyfile == NULL) {
        std::cout<<"cannot open body file"<<std::endl;
    }
    
    res = curl_easy_perform(curl_handle);
    /* Check for errors */
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    fclose(m_headerfile);
    fclose(m_bodyfile);
}