#include <iostream>
#include "Wildcard.h"
#include <ctime>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <cstdlib>

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

void Wildcard::autocreate_Directory(int number_of_record) {

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
            std::string Number =  generatePhoneNumber(); // generatePhoneNumber +73389177162
            phoneNumbers.push_back(atoll(Number.c_str()));
            for (int i = 1; i < number_of_record; ++i) {
                phoneNumbers.push_back(atoll(generatePhoneNumber().c_str()));
                while (phoneNumbers[i]==phoneNumbers[i-1]){
                    phoneNumbers[i]=atoll(generatePhoneNumber().c_str());
                }
            }

            // writing to file
            for (int i = 0; i < number_of_record; ++i) {
                fout<<phoneNumbers[i]<<" "<<l_names[rand()%20]<<std::endl;
            }

        }
    }
    else{
        std::cout<<"It's impossible to create "<<number_of_record<<" records!"<<std::endl;
    }

}
