#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <regex>

// لیست کلمات کلیدی
static const std::unordered_set<std::string> KEYWORDS = {
    "start","finish","then","if","repeat","var",
    "int","float","do","read","print","void",
    "return","dummy","program"
};

// لیست عملگرهای رابطه‌ای
static const std::unordered_set<std::string> RELOPS = {
    "==","!=","=>","="
};

// تبدیل به lowercase
inline std::string toLower(const std::string &s) {
    std::string out=s;
    for(char &c:out) c=std::tolower(c);
    return out;
}

// بررسی شناسه
inline bool isIdentifier(const std::string &s) {
    static const std::regex idRegex("^[a-zA-Z][a-zA-Z0-9]{0,7}$");
    return std::regex_match(s, idRegex);
}

// بررسی عملگر رابطه‌ای
inline bool isRelOp(const std::string &s) {
    return RELOPS.count(s)>0;
}

// اسکن یک سطر و چاپ توکن‌ها
inline void scanLine(const std::string &line, int row) {
    std::istringstream iss(line);
    std::string word;
    int col=1;
    while(iss>>word) {
        std::string wlow=toLower(word), type;
        if(KEYWORDS.count(wlow))       type="KEYWORD";
        else if(isRelOp(wlow))         type="RELOP";
        else if(isIdentifier(word))    type="IDENTIFIER";
        else                           type="ERROR";
        std::cout<<"("<<type<<",\""<<word<<"\",row="<<row<<",col="<<col<<")\n";
        col += word.length()+1;
    }
}
