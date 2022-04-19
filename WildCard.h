#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

enum class PositionOfStar{
    Left,
    Right,
    BothSide,
    NoOne
};

class WildCardException:public std::exception{
public:

    enum class ErrorsType{
        LOAD_ERROR,
        INPUT_ERROR
    };

    WildCardException(ErrorsType error):_error(error){}

    const char* what() const noexcept override{
        switch (getError()){
            case ErrorsType::LOAD_ERROR:
                return "Problem loading the file!";
            case ErrorsType::INPUT_ERROR:
                return "Incorrectly entered data!";
            default:
                break;
        }
    }
private:
    ErrorsType _error{};
    ErrorsType getError() const{
        return _error;
    }
};

struct Word{
    bool _needSearch=false;
    std::string _str_word{};
    std::pair<bool,bool> _areThereStars={false, false};
};

struct Words{
    Word phone;
    Word l_name;
};

class WildCard {
public:

    WildCard();

    void autoCreate_Directory(int number_of_record);
    void load_Directory();
    void startSearch(std::string& str_find);

private:
    std::string _path;
    bool _isCoincidence;

    std::unordered_multimap<std::string,std::string> _container_forNameSearch{};
    std::unordered_multimap<std::string,std::string>  _container_forNumberSearch{};


    Words _words{};

    void start_SimpleSearch();

    void searchByOneWord();
    void start_searchByOneWord(const std::unordered_multimap<std::string, std::string> &container, std::string* analyzedWord,
                               std::string* secondWord, PositionOfStar positionOfStar);

    void searchByTwoWord();
    void start_searchByTwoWord(const std::unordered_multimap<std::string, std::string> &container);

    bool isPhoneNumber(std::string &str) const;
    void showNoData() const;

    void prepareWords(std::string& str_find);
    void findStars(Word& word);
};
