#include <iostream>
#include <ctime>
#include "Wildcard.h"

int main(){
    std::srand(time(0));

    Wildcard wildcard;

    wildcard.autoCreate_Directory(150);
    wildcard.load_Directory();

    std::string str_find;
    std::cout<<"Please enter data: ";
    std::cin>>str_find;
    wildcard.search(str_find);



    return 0;
}