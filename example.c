/*
This is not a working example. It just shows off the possibilites in one single file!
Therefore it is commented and with short code examples.

Feel free to use this project for everything you want. 

Author: Felix Froehling <felix.froehling1@gmail.com>
*/


#include <stdio.h>
#include <stdlib.h>
#include "json/json.h"


void parseJson(char* string){
    JSON_OBJECT* json_obj;
    JSON_ARRAY* json_arr;
   
    //Parse JSON
    if(!parse(string, &json_obj, &json_arr)){
        printf("Json is not valid!\n");
        return;
    }

    //Do sth with the json...

    //If you don't know if u have an object or array, just check for NULL:
    if(json_obj != NULL){
        //You have an Object
    }
    else if(json_arr != NULL){
        //You have an Array
    }
    else{
        //This shouldn't happen with valid json. If it does, please send bug report!
    }

    //For accessing the contents of json, please have a look at the structs in json.h
    /*
       In general a JSON_OBJECT* holds multiple KeyValue-Pairs. 
       A Key-Value Pair consists of a key (obvious) and a value either being a 
        - string (stringvalue) 
        - int (intvalue)
        - null (is_null) (0 or 1)
        - JSON_OBJECT* (json_obj)
        - JSON_ARRAY* (json_arr)

        whereas the names in brackets corresponds to the names in JSON_OBJECT. Check for datatype by checking anything against NULL. 
        You might simplify this by using functions or typecastings, etc. or by just knowing each datatype for a given key.

        Unfortunately, no true/false parsing is supported yet, but will come in the future as this isn't that much work :)
        As a workaroung right know you can replace every true/false pattern either as string or int.
    */

    //You could iterate an json_obj like this, or write an iterator, etc....
    for(int i = 0; i < json_obj->len_entries; i++){
        printf("Key : %s\n", json_obj->entries[i]->key); //Get the key of an attribute in json_object

        //Get Corresponding Value
        if(json_obj->entries[i]->is_null){
            //Null-value
        }
        else if(json_obj->entries[i]->stringvalue != NULL){
            //it is string value
        }
        else if(json_obj->entries[i]->intvalue != NULL){
            //it is int value
        }
        //...Same for JSON_OBJECT* and JSON_ARRAY*
        //Obviously, exactly one the possibilities is != NULL
    }


    //A JSON_ARRAY* constists of a list of JSON_OBJECTS
    //You can iterate it the same way
    for(int i = 0; i < json_arr->len_entries; i++){
        JSON_OBJECT* json_obj = json_arr->entries[i];
        //Do sth with your json-object...
    }


    //For testing purposes there are two really basic output functions.
    //They are just writing out each key and value without any formatting or indents on subobjects. 
    outputJsonObject(json_obj);
    outputJsonArray(json_arr);

    //When you are done, release resources
    freeJsonArray(json_arr); //For JSON-Array
    freeJsonObject(json_obj); //For JSON-Object

    //Thats it.
}




int main(int argc, char** argv){
    char* json = "{\"key\" : \"value\"}";
    parseJson(json);
}
