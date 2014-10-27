//
//  fetchID.h
//  testcurl
//
//  Created by Edison on 9/21/14.
//  Copyright (c) 2014 edison. All rights reserved.
//

#ifndef __testcurl__fetchID__
#define __testcurl__fetchID__
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include "fetchHtml.h"

struct data{
    data(){
        push = shush = arrow = 0;
    }
    std::string id;
    int push;
    int shush;
    int arrow;
};

class FetchID{
public:
    FetchID(std::string keyWord);
    void processHtml(std::vector<std::string>& page,FetchHtml& m_fHtml);
    void saveData(std::string idFileName);
    void saveCSV(std::string idFileName);
    void printData();

private:
    void copyDatatoVector();
    void processComment(std::string page,FetchHtml& m_fHtml);
    std::unordered_map<std::string,std::unordered_map<std::string,int>> m_data;
    std::regex m_keyWordTag;
    std::vector<data> m_vectorData;
    
};

#endif /* defined(__testcurl__fetchID__) */
