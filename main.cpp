#include <iostream>
#include <ctime>
#include "Wildcard.h"

int main(){
    std::srand(time(0));

    Wildcard wildcard;

    wildcard.autoCreate_Directory(150);
    wildcard.load_Directory();

    std::cout << "\nEnter \'e\' or \'E\' for exit\n\n";
    std::string str_find;
    bool search=true;
    while(1) {
        std::cout << "Please enter data: ";
        std::cin >> str_find;

        if (str_find=="e"||str_find=="E") break;

        wildcard.search(str_find);

        std::cout << "\n\n";
    }




    return 0;
}