#pragma once
#include <iostream>
#include <string>
<<<<<<< HEAD
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
=======

class Wildcard {
public:
    Wildcard(){_path="Phone_Directory.txt";}
    Wildcard(std::string path){
        _path=path;
    }

    void autocreate_Directory(int number_of_record);

private:
    std::string _path;
>>>>>>> origin/Development
};