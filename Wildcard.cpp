#include <iostream>
#include "Wildcard.h"
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>


Wildcard::Wildcard() {
    _path="Phone_Directory.txt";
    _isCoincidence= false;
}



std::string generatePhoneNumber(){
    int part1 = 1000+rand()%9000;
    int part2 = 1000+rand()%(10000-1000);
    int part3 = 10+rand()%(100-10);

    part1 = 1000+rand()%9000;
    part2 = 1000+rand()%9000;
    part3 = 10+rand()%(100-10);

    std::string phoneNumber ="+7"+std::to_string(part1)+std::to_string(part2)+std::to_string(part3);
    return phoneNumber;

}



void Wildcard::autoCreate_Directory(int number_of_record) {

    if (number_of_record>0){
        std::ofstream fout(_path,std::ios_base::out|std::ios_base::trunc);
        if (!fout.is_open()){
            std::cout<<"Something went wrong with opening the file!"<<std::endl;
        }
        else{
            // creating an array of last names
            std::array<std::string,20> l_names{
                "Smirnov", "Ivanov",
                "Kuznetsov", "Sokolov",
                "Popov",  "Lebedev",
                "Morozov",  "Perov",
                "Volkov",  "Solovyov",
                "Vasiliev",  "Zaitsev",
                "Pavlov",  "Semenov",
                "Golubev",  "Vinogradov",
                "Vorobyov",   "Novikov",
                "Yershov",   "Borisov",
            };


            // creating unique phone numbers
            std::vector<long long> phoneNumbers;
            std::string Number =  generatePhoneNumber();
            phoneNumbers.push_back(atoll(Number.c_str()));
            for (int i = 1; i < number_of_record; ++i) {
                phoneNumbers.push_back(atoll(generatePhoneNumber().c_str()));
                while (phoneNumbers[i]==phoneNumbers[i-1]){
                    phoneNumbers[i]=atoll(generatePhoneNumber().c_str());
                }
            }

            // writing to file
            for (int i = 0; i < number_of_record; ++i) {
                fout<<"+"<<phoneNumbers[i]<<" "<<l_names[rand()%20]<<std::endl;
            }

        }
    }
    else{
        std::cout<<"It's impossible to create "<<number_of_record<<" records!"<<std::endl;
    }

}



void Wildcard::load_Directory() {
    std::ifstream fin(_path,std::ios_base::in);
    if (!fin.is_open()){
        std::cout<<"Something went wrong with opening the file!"<<std::endl;
    }
    else{
        std::string phone;
        std::string name;
        while (1){
            fin>>phone;
            fin>>name;
            _container_forNameSerch.emplace(name,phone);
            _container_forNumberSerch.emplace(phone,name);
            if (fin.eof()) break;
        }
    }
    fin.close();
}



const void Wildcard::search(std::string& str_find) {

    std::cout<<"Result:"<<std::endl;

    int countStars=0;
    for (auto i=0;i<str_find.size();++i) {
        if (str_find[i]=='*') countStars++;
    }

    if (countStars==2) searchByDoubleStar(str_find);

    else if(countStars==1) searchByStar(str_find);

    else{
        if(isPhoneNumber(str_find)){
            if (_container_forNumberSerch.find(str_find)!=_container_forNumberSerch.end()){
                std::cout<<_container_forNumberSerch.find(str_find)->second<<std::endl;
            } else showNoData();
        }
        else{
            _isCoincidence= false;
            for (auto it: _container_forNameSerch){
                if (it.first==str_find){
                    std::cout<<it.second<<std::endl;
                    _isCoincidence= true;
                }
            }
            if (!_isCoincidence) showNoData();
        }
    }
}



const void Wildcard::searchByStar(std::string &str_find) {

    _isCoincidence= false;
    if(isPhoneNumber(str_find)){
        for (auto it:_container_forNumberSerch) {
            for (int i = 0,border=str_find.find('*')-1; i !=str_find.find('*'); ++i) {
                if (str_find[i]!=it.first.at(i)){
                    break;
                }
                if (i==border){
                    std::cout<<"Phone number: "<<it.first<<"   Last name: "<<it.second<<std::endl;
                    _isCoincidence= true;
                }

            }
        }
    }else{
        for (auto it:_container_forNameSerch) {
            for (int i = 0,border=str_find.find('*')-1; i !=str_find.find('*'); ++i) {
                if (str_find[i]!=it.first.at(i)){
                    break;
                }
                if (i==border){
                    std::cout<<"Last name: "<<it.first<<"   Phone number: "<<it.second<<std::endl;
                    _isCoincidence= true;
                }

            }
        }
    }
    if (!_isCoincidence) showNoData();
}



const void Wildcard::searchByDoubleStar(std::string &str_find) {
    std::stringstream ss;
    ss.str(str_find);
    std::string item;
    std::pair<std::string,std::string> keys;

    int numberOfKey=1;
    while (getline(ss, item, ' '))
    {
        if (numberOfKey==1) {
            keys.first = item;
            numberOfKey++;
        }
        else keys.second=item;
    }
    //std::cout<<"first key: "<<keys.first<<"  second key: "<<keys.second<<std::endl;

    _isCoincidence= false;
    if(isPhoneNumber(keys.first)){
        for (auto it1:_container_forNumberSerch) {
            for (int i = 0, border=keys.first.find('*')-1; i !=keys.first.find('*'); ++i) {
                if (keys.first[i] != it1.first.at(i)) {
                    break;
                }
                if (i == border){ // If the first key found a coincidence
                    for (int j = 0, border = keys.second.find('*') - 1; j < keys.second.find('*'); ++j) {
                        if (keys.second[j] != it1.second.at(j)) {
                            break;
                        }
                        if (j == border) { //If the second key found a coincidence
                            std::cout << "Phone number: " << it1.first << "   Last name: " << it1.second << std::endl;
                            _isCoincidence = true;
                        }
                    }
                }
            }
        }
    } else {
        for (auto it1:_container_forNameSerch) {
            for (int i = 0, border=keys.first.find('*')-1; i !=keys.first.find('*'); ++i) {
                if (keys.first[i] != it1.first.at(i)) {
                    break;
                }
                if (i == border){ // If the first key found a coincidence
                    for (int j = 0, border = keys.second.find('*') - 1; j < keys.second.find('*'); ++j) {
                        if (keys.second[j] != it1.second.at(j)) {
                            break;
                        }
                        if (j == border) { //If the second key found a coincidence
                            std::cout << "Last name: " << it1.first << "   Phone number: " << it1.second << std::endl;
                            _isCoincidence = true;
                        }
                    }
                }
            }
        }
    }
    if (!_isCoincidence) showNoData();
}



const bool Wildcard::isPhoneNumber(std::string &str_find) {
    return str_find.at(0)=='+';
}
const void Wildcard::showNoData() {
    std::cout<<"There is no such data !"<<std::endl;
}

