#include <iostream>
#include <ctime>
#include "Wildcard.h"

int main(){
    std::srand(time(0));

    Wildcard wildcard;
    wildcard.autocreate_Directory(150);

    return 0;
}