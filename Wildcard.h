#pragma once
#include <iostream>
#include <string>

class Wildcard {
public:
    Wildcard(){_path="Phone_Directory.txt";}
    Wildcard(std::string path){
        _path=path;
    }

    void autocreate_Directory(int number_of_record);

private:
    std::string _path;
};