#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include "fetchID.h"
#include "fetchHtml.h"
#include "fetchPage.h"


using std::endl;
using std::cout;



int main(){
    std::string board = "Gossiping";
    std::string keyWord ="連勝文";
    int pages = 500;
    FetchPage fPage(pages,board,keyWord);
    fPage.processPage();
    return 0;
}


