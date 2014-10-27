//
//  fetchID.cpp
//  testcurl
//
//  Created by Edison on 9/21/14.
//  Copyright (c) 2014 edison. All rights reserved.
//

#include "fetchID.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <iomanip>
#include <algorithm>
#include "fetchHtml.h"

bool CompareData(const data& A, const data& B)
{
    if (A.shush > B.shush)
        return true;
    else if(A.shush < B.shush)
        return false;
    if (A.arrow > B.arrow)
        return true;
    else if(A.arrow < B.arrow)
        return false;
    if (A.push > B.push)
        return true;
    else if(A.push < B.push)
        return false;
    if (A.id < B.id)
        return true;
    else if(A.id > B.id)
        return false;
    return false;
}

FetchID::FetchID(std::string keyWord){
    std::string regexKeyWord = "";
    std::string delimiter = " ";
    std::string token;
    size_t pos = 0;
    if( keyWord.find(delimiter) == std::string::npos){
        std::cout<<"Key word is "<<keyWord<<std::endl;
        m_keyWordTag = keyWord;

    }
    else{
        while( (pos = keyWord.find(delimiter)) != std::string::npos){
            token = keyWord.substr(0,pos);
            keyWord.erase(0,pos+delimiter.length());
            if(regexKeyWord == ""){
                regexKeyWord = token;
            }
            else{
                regexKeyWord = regexKeyWord + "|" + token;
            }
        }
        regexKeyWord = regexKeyWord + "|" + keyWord;
        std::cout<<"Key words are "<<regexKeyWord<<std::endl;
        //    regexKeyWord = "[Tt][Oo][Yy][Zz]";
        m_keyWordTag = regexKeyWord;
    }

}

void FetchID::processHtml(std::vector<std::string>& page,FetchHtml& m_fHtml){
    std::ifstream inputBody;
    std::string line;
    std::smatch keyWordResult;
    

    for(auto i:page){
        m_fHtml.setFileName("source.out");
        m_fHtml.getHtml(i);
        std::cout<<i<<std::endl;
        inputBody.open("source.out");
        if(inputBody.is_open()){
            while(inputBody.good()){
                getline(inputBody,line);
                std::regex_search(line,keyWordResult,m_keyWordTag);
                if(keyWordResult[0].str().length()>0){
                    std::cout<<line<<std::endl;
                    inputBody.close();
                    processComment(i,m_fHtml);
                    break;
                }
            }
            inputBody.close();
        }
        else{
            std::cout<<"cannot open body file"<<std::endl;
        }
    }

}

void FetchID::processComment(std::string page,FetchHtml& m_fHtml){
    std::ifstream inputBody;
    std::string line;
    std::regex idTag("(<span class=\"f3 hl push-userid\">)(.[^<]*)(</[^>]*>)");
    std::regex pushTag("(<span class=\".*hl push-tag\">)(.[^<]*)(</[^>]*>)");
    std::smatch idResult;
    std::smatch pushResult;
    m_fHtml.setFileName("body.out");
    m_fHtml.getHtml(page);
    inputBody.open("body.out");
    if(inputBody.is_open()){
        while(inputBody.good()){
            getline(inputBody,line);
            std::regex_search(line,idResult,idTag);
            std::regex_search(line,pushResult,pushTag);
            if(idResult[2].str().length()>0){
                if(pushResult[2].str().length()>0){
                    auto searchId = m_data.find(idResult[2].str());
                    if(searchId != m_data.end()){
                        auto searchPush = searchId->second.find(pushResult[2].str());
                        if(searchPush != searchId->second.end()){
                            searchPush->second++;
                        }
                        else{
                            m_data[idResult[2].str()][pushResult[2].str()] = 1;
                        }
                    }
                    else{
                        std::unordered_map<std::string,int> temp_map;
                        temp_map[pushResult[2].str()] = 1;
                        m_data[idResult[2].str()] = temp_map;
                    }
                }
            }
        }
    }
    else{
        std::cout<<"cannot open body file"<<std::endl;
    }
    inputBody.close();
}

void FetchID::copyDatatoVector(){
    data temp;
    for(auto i:m_data){
        temp.id = i.first;
        for(auto j:i.second){
            if(j.first == "推 "){
                temp.push = j.second;
            }
            else if(j.first == "噓 "){
                temp.shush = j.second;
            }
            else if(j.first == "→ "){
                temp.arrow = j.second;
            }
        }
        m_vectorData.push_back(temp);
        temp.push = 0;
        temp.shush = 0;
        temp.arrow = 0;
    }
    
//    for(auto i:m_vectorData){
//        std::cout<<i.id<<" "<<i.shush<<" "<<i.arrow<<" "<<i.push<<std::endl;
//    }
//    printData();
}

void FetchID::saveData(std::string idFileName){
    copyDatatoVector();
    std::sort(m_vectorData.begin(),m_vectorData.end(),CompareData);
    std::ofstream output;
    output.open(idFileName);
    if(output.is_open()){
        for(auto i:m_vectorData){
            output<<i.id;
            output<<std::setw(4)<<" 噓"<<std::setw(4)<<i.shush;
            output<<std::setw(4)<<" ->"<<std::setw(4)<<i.arrow;
            output<<std::setw(4)<<" 推"<<std::setw(4)<<i.push<<std::endl;
        }
    }
    else{
        std::cout<<"Fail to write m_data to file"<<std::endl;
    }
    output.close();
}

void FetchID::saveCSV(std::string idFileName){
    copyDatatoVector();
    std::sort(m_vectorData.begin(),m_vectorData.end(),CompareData);
    std::ofstream output;
    output.open(idFileName);
    if(output.is_open()){
        output<<"id,shush,->,push,"<<std::endl;
        for(auto i:m_vectorData){
            output<<i.id<<","<<i.shush<<","<<i.arrow<<","<<i.push<<std::endl;;
        }
    }
    else{
        std::cout<<"Fail to write m_data to file"<<std::endl;
    }
    output.close();
}

void FetchID::printData(){
    for(auto i:m_data){
        std::cout<<i.first<<" ";
        for(auto j:i.second){
            std::cout<<j.first<<" "<<j.second<<" ";
        }
        std::cout<<std::endl;
    }
}

