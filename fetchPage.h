//
//  fetchPage.h
//  testcurl
//
//  Created by Edison on 9/22/14.
//  Copyright (c) 2014 edison. All rights reserved.
//

#ifndef __testcurl__fetchPage__
#define __testcurl__fetchPage__

#include <vector>
#include <string>
#include "fetchID.h"
#include "fetchHtml.h"

class FetchPage{
public:
    FetchPage(int num,std::string board,std::string keyWord);
    void processPage();
    std::vector<std::string> getPage();
    void saveData(std::string fileName);
private:
    void setIndex();
    std::vector<std::string> m_page;
    std::string m_board;
    int m_num;
    int m_index;
    FetchID m_id;
    FetchHtml m_fHtml;
};


#endif /* defined(__testcurl__fetchPage__) */
