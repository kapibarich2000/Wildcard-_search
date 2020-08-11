#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

class Wildcard {
public:
    Wildcard();

    void autoCreate_Directory(int number_of_record);
    void load_Directory();
    const void search(std::string& str_find);
protected:
    std::string _path;
    std::unordered_multimap<std::string,std::string> _container_forNameSerch;
    std::unordered_map<std::string,std::string>  _container_forNumberSerch;

    const void searchByStar(std::string& str_find);
    const bool isPhoneNumber(std::string &str_find);
};