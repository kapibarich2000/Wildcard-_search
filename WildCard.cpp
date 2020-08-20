#include <iostream>
#include "WildCard.h"
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>


WildCard::WildCard():_path("Phone_Directory.txt"),_isCoincidence(false) {}

bool WildCard::isPhoneNumber(std::string &str)const {
    return str.at(0)=='+';
}
void WildCard::showNoData()const {
    std::cout<<"There is no such data !"<<std::endl;
}

std::string generatePhoneNumber(){
    // Bug: without this repetitions not generate "good random" phone numbers
    int part1 = 1000+rand()%9000;
    int part2 = 1000+rand()%(10000-1000);
    int part3 = 10+rand()%(100-10);

    part1 = 1000+rand()%9000;
    part2 = 1000+rand()%9000;
    part3 = 10+rand()%(100-10);

    std::string phoneNumber ="+7"+std::to_string(part1)+std::to_string(part2)+std::to_string(part3);
    return phoneNumber;

}

void WildCard::autoCreate_Directory(int number_of_record) {

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

void WildCard::load_Directory() {
    std::ifstream fin(_path,std::ios_base::in);
    if (!fin.is_open()){
        throw WildCardException(WildCardException::ErrorsType::LOAD_ERROR);
    }
    else{
        std::string phone;
        std::string name;
        while (true){
            fin>>phone;
            fin>>name;
            _container_forNameSearch.emplace(name,phone);
            _container_forNumberSearch.emplace(phone,name);
            if (fin.eof()) break;
        }
    }
    fin.close();
}

void WildCard::prepareWords(std::string& str_find){

    std::stringstream ss;
    ss.str(str_find);
    std::string word;

    while (getline(ss, word, ' '))
    {
        // first word - phone
        // second word - l_name

        if (isPhoneNumber(word)){

            _words.first._str_word = word;
            findStars(_words.first);

            if (_words.first._str_word.find('&')!=std::string::npos || _words.first._str_word.find('*') != std::string::npos)
                _words.first._needSearch= true;

        } else{

            _words.second._str_word=word;
            findStars(_words.second);

            if (_words.second._str_word.find('&')!=std::string::npos||_words.second._str_word.find('*')!=std::string::npos)
                _words.second._needSearch= true;
        }
    }
}

void WildCard::findStars(Word& word){
    if (*word._str_word.begin()=='*'){
        word._areThereStars.first= true;
    }

    if(word._str_word[word._str_word.size()-1]=='*') {
        word._areThereStars.second = true;
    }
}


void WildCard::startSearch(std::string& str_find) {
    std::cout<<"Result:"<<std::endl;
    _isCoincidence= false;

    prepareWords(str_find);

    selectStarSearch();

    if (!_isCoincidence) showNoData();
}

void WildCard::selectStarSearch() {

    if (_words.first._needSearch && _words.second._needSearch)
        searchByTwoWord();

    else if(_words.first._needSearch || _words.second._needSearch)
        searchByOneWord();

    else{
        std::string search_str;
        if (_words.first._str_word.empty()) search_str=_words.second._str_word;
        else if (_words.second._str_word.empty()) search_str=_words.first._str_word;
        else{
            throw WildCardException(WildCardException::ErrorsType::INPUT_ERROR);
        }

        if(isPhoneNumber(search_str)){
            auto it_on_str=_container_forNumberSearch.find(search_str);
            if (it_on_str!=_container_forNumberSearch.end()){
                std::cout<<it_on_str->second<<std::endl;
                _isCoincidence= true;
            }
        }else{
            auto it_on_str=_container_forNameSearch.equal_range(search_str);
            for(auto it = it_on_str.first; it != it_on_str.second; it++)
            {
                std::cout<<it->second<<std::endl;
                _isCoincidence= true;
            }
        }
    }
}


void WildCard::searchByOneWord() {

    Word* analyzedWord;
    std::string* str_secondWord; // if is
    PositionOfStar positionOfStar;

    if (_words.first._needSearch) {
        analyzedWord=&_words.first;
        str_secondWord=&_words.second._str_word;
    }
    else {
        analyzedWord=&_words.second;
        str_secondWord=&_words.first._str_word;
    }

    if(analyzedWord->_areThereStars.first) positionOfStar=PositionOfStar::Right;
    else if(analyzedWord->_areThereStars.second) positionOfStar=PositionOfStar::Left;
    else positionOfStar=PositionOfStar::BothSide;

//    if( isPhoneNumber(analyzedWord->_str_word))
//        start_searchByOneWord(_container_forNumberSearch, &analyzedWord->_str_word, str_secondWord, positionOfStar);
//
//    else
//        start_searchByOneWord(_container_forNameSearch, &analyzedWord->_str_word,  str_secondWord, positionOfStar);
}

//void WildCard::start_searchByOneWord(const std::unordered_multimap<std::string, std::string> &container, std::string*  analyzedWord,
//                                      std::string* secondWord, PositionOfStar positionOfStar) {
//
//    int firstPosition;
//    int endOfSearchPosition; // location before star
//
//
//    if (positionOfStar==Left){
//        firstPosition=(int)analyzedWord->size()-1;
//        endOfSearchPosition=(int)analyzedWord->find('*')+1;
//    } else{ // Left and BothSide
//        firstPosition=0;
//        endOfSearchPosition=(int)analyzedWord->find('*')-1;
//    }
//
//    for (const auto& it:container) {
//
//        switch (positionOfStar) {
//        case Left:
//                for (int i = firstPosition; i !=analyzedWord->find('*'); --i) {
//                    if (analyzedWord->at(i)!=it.first.at(i)) break;
//                    else if (i==endOfSearchPosition) {
//                        if (secondWord->empty() || *secondWord == it.second) {
//                            std::cout << it.first << "  " << it.second << std::endl;
//                            _isCoincidence = true;
//                        }
//                    }
//                }
//            break;
//
//        case Right:
//            for (int i = firstPosition; i !=analyzedWord->find('*'); ++i) {
//                if (analyzedWord->at(i)!=it.first.at(i)) break;
//                else if (i==endOfSearchPosition) {
//                    if (secondWord->empty() || *secondWord == it.second) {
//                        std::cout << it.first << "  " << it.second << std::endl;
//                        _isCoincidence = true;
//                    }
//                }
//            }
//            break;
//
//        case BothSide:
//            for (int i = firstPosition; i !=analyzedWord->find('*'); --i) {
//                if (analyzedWord->at(i)!=it.first.at(i)) break;
//                else if (i==endOfSearchPosition) {
//                    if (secondWord->empty() || *secondWord == it.second) {
//                        std::cout << it.first << "  " << it.second << std::endl;
//                        _isCoincidence = true;
//                    }
//                }
//            }
//            break;
//
//        default:
//            break;
//        }
//
//    }
//
//}


void WildCard::searchByTwoWord() {
    if(isPhoneNumber(_words.first._str_word))
        start_searchByTwoWord(_container_forNumberSearch);
    else
        start_searchByTwoWord(_container_forNameSearch);
}

void WildCard::start_searchByTwoWord(const std::unordered_multimap<std::string, std::string> &container) {
    for (auto it:container) {

        int first_border=(int)_words.first._str_word.find('*')-1; // read until the star and go to the second key
        int second_border=(int)_words.second._str_word.find('*') - 1;

        for (int i = 0; i !=_words.first._str_word.find('*'); ++i) {
            if (_words.first._str_word[i] != it.first.at(i)) {
                break; // to search for another record in the container
            }
            if (i == first_border){ // If the first key found a coincidence
                for (int j = 0; j < _words.second._str_word.find('*'); ++j) {
                    if (_words.second._str_word[j] != it.second.at(j)) {
                        break;
                    }
                    if (j == second_border) { //If the second key found a coincidence
                        std::cout<< it.first << "  " << it.second << std::endl;
                        _isCoincidence = true;
                    }
                }
            }
        }
    }
}


