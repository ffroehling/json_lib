/*
Please send Bug-Reports to my given mail address.
Feel free to use this project for everything you want. 

Author: Felix Froehling <felix.froehling1@gmail.com>
*/

#include "helper.h"

int flip(int i){
    return i == 0;
}

//Returns next Object or Array to parse
//Therefore looks for next comma not in any bracket, array or "
char* getCommaSeperatedString(char* string, char** following){
    int i = 0, quote = 0, brackets = 0, arrays = 0;
    do{
        if(string[i] == '"'){
            quote = flip(quote);
        }
        else if(string[i] == '{'){
            brackets++;
        }
        else if(string[i] == '}'){
            brackets--;
        }
        else if(string[i] == '['){
            arrays++;
        }
        else if(string[i] == ']'){
            arrays--;
        }
        else if(string[i] == ',' && !quote && (brackets == 0) && (arrays == 0)){
            char* found = malloc((i+1) * sizeof(char));
            if(found == NULL){
                //Malloc failed. 
                exit(1);
            }

            found[i] = '\0';
            strncpy(found, string, i);
            if(i == (strlen(string) - 1)){
                *following = NULL;
            }
            else{
                *following = string + i + 1;
            }
            return found;
        }

        i++;
    }while(i < strlen(string));


    *following = NULL;
    char* result = malloc(strlen(string));
    if(result == NULL){
        exit(1);
    }

    strncpy(result, string, strlen(string));

    return result;
}
