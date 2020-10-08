#include <iostream>
#include "WildCard.h"
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <cctype>

// #define PlusPlus ++i
// ifdef c++


WildCard::WildCard():_path("Phone_Directory.txt"),_isCoincidence(false) {}

bool WildCard::isPhoneNumber(std::string &str)const {
    int i=0;
    while (str[i]){ // while it's not end (of the string)
        if (str[i]=='*'||str[i]=='&'){
            ++i;
            continue;
        }
        else if (isalpha(str[i])) return false;
        else return true;
    }
    throw WildCardException(WildCardException::ErrorsType::INPUT_ERROR);
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
        if (isPhoneNumber(word)){

            _words.phone._str_word = word;
            findStars(_words.phone);

            if (_words.phone._str_word.find('&')!=std::string::npos || _words.phone._str_word.find('*') != std::string::npos)
            {_words.phone._needSearch= true;}

        } else{

            _words.l_name._str_word=word;
            findStars(_words.l_name);

            if (_words.l_name._str_word.find('&')!=std::string::npos||_words.l_name._str_word.find('*')!=std::string::npos)
            { _words.l_name._needSearch = true; }
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

    if (_words.phone._needSearch && _words.l_name._needSearch)
        searchByTwoWord();
    else
        searchByOneWord();

    if (!_isCoincidence) showNoData();
}

void WildCard::start_SimpleSearch() {

    std::string search_str;

    if (_words.phone._str_word.empty()) search_str=_words.l_name._str_word;
    else if (_words.l_name._str_word.empty()) search_str=_words.phone._str_word;
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


void WildCard::searchByOneWord() {

    Word* analyzedWord;
    std::string* str_secondWord; // for to match
    PositionOfStar positionOfStar;

    if (_words.phone._needSearch) {
        analyzedWord=&_words.phone;
        str_secondWord=&_words.l_name._str_word;
    }
    else if (_words.l_name._needSearch) {
        analyzedWord=&_words.l_name;
        str_secondWord=&_words.phone._str_word;
    } else{
        start_SimpleSearch();
    }


    if(analyzedWord->_areThereStars.first) positionOfStar=PositionOfStar::Left;
    else if(analyzedWord->_areThereStars.second) positionOfStar=PositionOfStar::Right;
    else if(analyzedWord->_areThereStars.first&&analyzedWord->_areThereStars.second) positionOfStar=PositionOfStar::BothSide;
    else positionOfStar=PositionOfStar::NoOne;

    if( isPhoneNumber(analyzedWord->_str_word)) // Два раза используем isPhoneNumber()
        start_searchByOneWord(_container_forNumberSearch, &analyzedWord->_str_word, str_secondWord, positionOfStar);

    else
        start_searchByOneWord(_container_forNameSearch, &analyzedWord->_str_word,  str_secondWord, positionOfStar);
}

void WildCard::start_searchByOneWord(const std::unordered_multimap<std::string, std::string> &container, std::string*  analyzedWord,
                                      std::string* secondWord, PositionOfStar positionOfStar) {

    int firstPosition;
    int endOfSearchPosition; // location before star


    if (positionOfStar==PositionOfStar::Left){
        firstPosition=analyzedWord->size()-1;
        endOfSearchPosition=(int)analyzedWord->find('*')+1; // find() return -> 0
    }else if(positionOfStar==PositionOfStar::Right) {
        firstPosition=0;
        endOfSearchPosition=(int)analyzedWord->find('*')-1;
    }else if (positionOfStar==PositionOfStar::BothSide){
        firstPosition=1; // because 0 is *
        endOfSearchPosition=(int)analyzedWord->find('*')-1;
    }
    else{ // None
        firstPosition=0;
        endOfSearchPosition=(int)analyzedWord->size()-1;
    }


    switch (positionOfStar) {
        case PositionOfStar::Left:
            for (const auto& it:container) {
                for (int j_str = firstPosition,j_container=it.first.size()-1; j_str !=analyzedWord->find('*')&&j_container!=-1; --j_str,--j_container) {//  find return 0
                    if (analyzedWord->at(j_str) == '&') {} // *ова   *&ва    *о&а
                    else if (analyzedWord->at(j_str) != it.first.at(j_container))
                        break;
                    if (j_str == endOfSearchPosition) {
                        if (secondWord->empty() || *secondWord == it.second) {
                            std::cout << it.first << "  " << it.second << std::endl;
                            _isCoincidence = true;
                        }
                    }
                }
            }
        break;
    case PositionOfStar::Right:

        for (const auto& it:container) {

            for (int j = firstPosition; j != analyzedWord->find('*'); ++j) {

                if (it.first.at(j) == '&') {} // *&ва   *ова    *&&а

                for (int j_container=0;j_container!=it.first.size(); ++j_container) {

                    if (analyzedWord->at(j)!=it.first.at(j_container)) continue;
                    else //

//                    else if (analyzedWord->at(j) != it.first.at(j))
//                        break;

                    if (j == endOfSearchPosition) {
                        if (secondWord->empty() || *secondWord == it.second) {
                            std::cout << it.first << "  " << it.second << std::endl;
                            _isCoincidence = true;
                        }
                    }
                }
            }
        }
        break;

    case PositionOfStar::BothSide:
        for (const auto& it:container) {
            for (int j = firstPosition; j != analyzedWord->size(); ++j) {
                if (it.first.at(j) == '&') {}
                else if (analyzedWord->at(j) != it.first.at(j)) break;

                if (j == endOfSearchPosition) {
                    if (secondWord->empty() || *secondWord == it.second) {
                        std::cout << it.first << "  " << it.second << std::endl;
                        _isCoincidence = true;
                    }
                }
            }
        }
        break;

        case PositionOfStar::NoOne:
            for (const auto& it:container) {
                for (int j = 0; j != analyzedWord->size(); ++j) {

                    if (it.first.at(j) == '&') {} // &ова    Вов&    &&ва    В&ва
                    else if (analyzedWord->at(j) != it.first.at(j)) break;

                    if (j == endOfSearchPosition) {
                        if (secondWord->empty() || *secondWord == it.second) {
                            std::cout << it.first << "  " << it.second << std::endl;
                            _isCoincidence = true;
                        }
                    }

                }
            }
            break;
    default:
        break;
    }
}


void WildCard::searchByTwoWord() {
    if(isPhoneNumber(_words.phone._str_word))
        start_searchByTwoWord(_container_forNumberSearch);
    else
        start_searchByTwoWord(_container_forNameSearch);
}

void WildCard::start_searchByTwoWord(const std::unordered_multimap<std::string, std::string> &container) {
    for (auto it:container) {

        int first_border=(int)_words.phone._str_word.find('*')-1; // read until the star and go to the second key
        int second_border=(int)_words.l_name._str_word.find('*') - 1;

        for (int i = 0; i !=_words.phone._str_word.find('*'); ++i) {
            if (_words.phone._str_word[i] != it.first.at(i)) {
                break; // to search for another record in the container
            }
            if (i == first_border){ // If the first key found a coincidence
                for (int j = 0; j < _words.l_name._str_word.find('*'); ++j) {
                    if (_words.l_name._str_word[j] != it.second.at(j)) {
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




