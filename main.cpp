#include <iostream>
#include <ctime>
#include "WildCard.h"

enum WorkStatus{
    Good,
    Bad
};

bool TryAgain(){
    while (true){
        std::cout<<"Try again? (Yes or No):";
        std::string choice;
        std::cin>>choice;
        if (choice=="No"||choice=="no") return false;
        else if (choice!="Yes"||choice!="yes"){
            std::cout<<"Incorrect input !"<<std::endl;
            continue;
        }
        return true; // choice=="Yes"(yes)
    }
}

int main(){
    std::srand(time(0));

    WildCard wildcard;
    std::string str_find{};
    bool exit =false;
    WorkStatus workStatus=WorkStatus::Bad;

    while (workStatus==WorkStatus::Bad){
        try {
            wildcard.autoCreate_Directory(150);
            wildcard.load_Directory();
            workStatus=WorkStatus::Good;
        }
        catch (WildCardException& ex) {
            std::cout<<"In WildCardException occurred ("<<ex.what()<<")"<<std::endl;
        }
        catch (const std::exception& ex) {
            std::cout<<"In Other std::exception occurred ("<<ex.what()<<")"<<std::endl;
        }
        if (workStatus==WorkStatus::Good||!TryAgain()) break;
    }

    if (workStatus==WorkStatus::Good){
        std::cout << "\nEnter \'e\' or \'E\' for exit\n\n";
        while(true) {
            std::cout << "Please enter data:";
            std::getline(std::cin, str_find);
            if(str_find=="e"||str_find=="E") break;

            try {
                wildcard.startSearch(str_find);
            }
            catch (WildCardException& ex) {
                std::cout<<"In WildCardException occurred ("<<ex.what()<<")"<<std::endl;
            }
            catch (std::bad_alloc &ex)
            {
                std::cerr << "Out of memory!" << '\n';
            }
            catch (const std::exception& ex) {
                std::cout<<"In Other std::exception occurred ("<<ex.what()<<")"<<std::endl;
            }
            std::cout<<std::endl;
        }
    }
    return 0;
}