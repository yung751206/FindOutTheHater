//
//  fetchPage.cpp
//  testcurl
//
//  Created by Edison on 9/22/14.
//  Copyright (c) 2014 edison. All rights reserved.
//

#include "fetchPage.h"
#include "fetchHtml.h"
#include <fstream>
#include <string>
#include <regex>
#include <iostream>

FetchPage::FetchPage(int num,std::string board,std::string keyWord):m_id(keyWord),m_fHtml(){
    m_board = board;
    m_num = num;
    setIndex();
}

void FetchPage::setIndex(){
    std::string url = "https://www.ptt.cc/bbs/"+ m_board + "/index.html";
    std::cout<<url<<std::endl;
    std::regex urlTag("(<a class=\"btn wide\" href=\")(.[^\"]*)(.*上頁)");
    std::smatch tagResult;
    std::ifstream input;
    std::string line;
    std::string result = "";
    std::string index;
    m_fHtml.setFileName("body.out");
    m_fHtml.getHtml(url);
    m_fHtml.post();
    input.open("body.out");
    if(input.is_open()){
        while(input.good()){
            getline(input,line);
            std::regex_search(line,tagResult,urlTag);
            if(tagResult[2].str().length()>0){
                index = tagResult[2].str();
                for(int i=0;i<7;i++){
                    if( isdigit(index[index.length()-11+i]) ){
                        result = result + index[index.length()-11+i];
                    }
                }
            }
        }
    }
    input.close();
    m_index = stoi(result)+1;
    std::cout<<m_index<<std::endl;
}

void FetchPage::processPage(){
    std::ifstream input;
    std::string line;
    std::regex titleTag("(<div class=\"title\">)");
    std::regex linkTag("(<a href=\")(.[^<]*)(\">)");
    std::string temp;
    std::smatch titleResult;
    std::smatch linkResult;

    
    for(int i=0;i<m_num;i++){
        temp = "https://www.ptt.cc/bbs/"+ m_board + "/index" + std::to_string(m_index-i) + ".html";
        std::cout<<temp<<std::endl;
        m_fHtml.setFileName("page.out");
        m_fHtml.getHtml(temp);
        input.open("page.out");
        if(input.is_open()){
            while(input.good()){
                getline(input,line);
                std::regex_search(line,titleResult,titleTag);
                if(titleResult[1].str().length()>0){
                    getline(input,line);
                    getline(input,line);
                    std::regex_search(line,linkResult,linkTag);
                    if(linkResult[2].str().length()>0){
                        m_page.push_back("https://www.ptt.cc"+linkResult[2].str());
                    }
                }
            }
        }
        else{
            std::cout<<"fail to open file"<<std::endl;
        }
        input.close();
        m_id.processHtml(m_page,m_fHtml);
        m_page.clear();
        std::cout<<"page "<<i<<" finished"<<std::endl;
    }
    m_id.saveCSV("data.csv");
}

void FetchPage::saveData(std::string fileName){
    std::ofstream output;
    output.open(fileName);
    if(output.is_open()){
        for(auto i:m_page){
            output<<i;
            output<<std::endl;
        }
    }
    else{
        std::cout<<"Fail to write m_page to file"<<std::endl;
    }
    output.close();
    m_page.clear();
}

std::vector<std::string> FetchPage::getPage(){
    return m_page;
}