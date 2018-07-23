/*
Please send Bug-Reports to my given mail address.
Feel free to use this project for everything you want. 

Author: Felix Froehling <felix.froehling1@gmail.com>
*/

#include "json.h"

#define OBJECT_OPEN '{'
#define OBJECT_CLOSE '}'
#define ARRAY_OPEN '['
#define ARRAY_CLOSE ']'
#define STRING_OPEN '"'

#define SIZE_JSON_VALUES 10

//Forward definitions
JSON_OBJECT* parseObject(char* string);
JSON_ARRAY* parseArray(char* string);

KEYVALUE* getEntry(){
    KEYVALUE* entry = (KEYVALUE*) malloc(1 * sizeof(KEYVALUE));
    if(entry == NULL){
        //TODO: Maybe Log failed mallocs or handle it another way
        exit(1);
        return NULL;
    }

    //Everything else is NULL
    entry->key = NULL;
    entry->stringvalue = NULL;
    entry->intvalue = NULL;
    entry->json_obj = NULL;
    entry->json_arr = NULL;
    entry->is_null = 0;
    return entry;
}

JSON_OBJECT* getEmptyJsonObject(){
    JSON_OBJECT* result = malloc(sizeof(JSON_OBJECT));
    if(result == NULL){
        exit(1);
    }

    result->entries = (KEYVALUE**) malloc(SIZE_JSON_VALUES * sizeof(KEYVALUE*));
    if(result->entries == NULL){
        exit(1);
    }
    

    result->len_entries = 0;
    return result;
}

JSON_ARRAY* getEmptyJsonArray(){
    JSON_ARRAY* result = malloc(sizeof(JSON_OBJECT));

    if(result == NULL){
        //ggf. Loggen
        exit(1);
    }
    result->entries = (JSON_OBJECT**) malloc(SIZE_JSON_VALUES * sizeof(JSON_OBJECT*));
    if(result->entries == NULL){
        exit(1);
    }

    result->len_entries = 0;
    return result;
}

char* skipspaces(char* str){
    for(int i = 0; str[i] < 33; str++){
        if(!(i < strlen(str))){
            return NULL;
        }
    }
    return str;
}

char* removetrailingspaces(char* str){
    for(int i = (strlen(str) - 1); str[i] < 33; i--){
        str[i] = '\0';
    }

    return str;
}

char* extractstring(char* str, char** offset){
    if (str[0] != STRING_OPEN){
        return NULL; //Invalid
    }
    for(int i = 1; i <  strlen(str); i++){
        if(str[i] == STRING_OPEN) {// HERE Closing
            int len = ((str + i - 1) - (str));
            char* result = malloc((len+1) * sizeof(char)); 
            //TODO: Check
            strncpy(result, str+1, len);
            result[len] = '\0';

            if(offset != NULL){
                (*offset) = str + i + 1;
            }
            return result;
        }
    }

    return NULL;
}

void pushKvToJSON(JSON_OBJECT* object, KEYVALUE* kv){
    object->entries[object->len_entries] = kv; 
    object->len_entries++;
    
    if((object->len_entries % SIZE_JSON_VALUES) == 0){
        KEYVALUE** newEntries = (KEYVALUE**) malloc((object->len_entries + SIZE_JSON_VALUES) * sizeof(KEYVALUE*));
        if(newEntries == NULL){
            exit(1);
        }

        memcpy(newEntries, object->entries, object->len_entries);
        free(object->entries);
        object->entries = newEntries;
    }
}


void pushObjToArray(JSON_ARRAY* array, JSON_OBJECT* obj){
    array->entries[array->len_entries] = obj;
    array->len_entries++;

    if((array->len_entries % SIZE_JSON_VALUES) == 0){
        JSON_OBJECT** newObjects = (JSON_OBJECT**) malloc((array->len_entries + SIZE_JSON_VALUES) * sizeof(JSON_OBJECT*));
        if(newObjects == NULL){
            exit(1);
        }

        memcpy(newObjects, array->entries, array->len_entries);
        free(array->entries);
        array->entries = newObjects;
    }
}

char* readKey(char* string, char** following){
    string = skipspaces(string);
    char* key = extractstring(string, following);
    return key;
}

KEYVALUE* parseKV(char* string){ 
    char* following, *key, *value;

    //Create KV
    KEYVALUE* kv = getEntry();

    //Read Key
    key = readKey(string, &following);
    kv->key = key;

    //Check for :     
    following = skipspaces(following);
    if(following[0] != ':'){
        //TODO: INVALID JSON
        free(key);
        return NULL;
    }

    //Read Value
    following = skipspaces(following + 1);
    if(strlen(following) == 4 && (strncmp(following, "null", 4) == 0)){
        kv->is_null = 1;
    }
    else if(following[0] == STRING_OPEN){
        //String value
        value = extractstring(following, NULL);
        if(value != NULL){
            kv->stringvalue = value;
        }
        else{
            //Sth went wrong here
            free(key);
            free(kv);
            return NULL;
        }
    }
    else if((following[0] <= 57 && following[0] >= 48) || following[0] == 45){
        int* value = (int*) malloc(sizeof(int));
        if(value == NULL){
            exit(1);
        }

        *value = strtoimax(following, NULL, 10);
        if ((*value) == UINTMAX_MAX && errno == ERANGE){
            //TODO: INVALID JSON
            free(key);
            free(value);
            free(kv);
            return NULL;
        }

        kv->intvalue = value;
    }
    else if(following[0] == OBJECT_OPEN){
        JSON_OBJECT* json_obj = parseObject(following);
        if(json_obj == NULL){
            //TODO: INVALID JSON
            free(key);
            free(kv);
            return NULL;
        }

        kv->json_obj = json_obj;
    }
    else if(following[0] == ARRAY_OPEN){
        JSON_ARRAY* json_arr = parseArray(following);
        if(json_arr == NULL){
            //TODO: INVALID JSON
            free(key);
            free(kv);
            return NULL;
        }

        kv->json_arr = json_arr;
    }
    else{
        //TODO: Invalid JSON
        free(key);
        return NULL;
    }

    return kv;
}

void outputJsonObject(JSON_OBJECT* obj){
    if(obj == NULL){
        return;
    }
    for(int i = 0; i < obj->len_entries; i++){
        KEYVALUE* kv = obj->entries[i];
        printf("Key: %s\nValue: ", kv->key);
        if(kv->is_null == 1){
            printf("NULL\n");
        }
        if(kv->stringvalue != NULL){
            printf("%s\n", kv->stringvalue);
        }
        else if(kv->intvalue != NULL){
            printf("%d\n", *(kv->intvalue));
        }
        else if(kv->json_obj != NULL){
            outputJsonObject(kv->json_obj);
        }
        else if(kv->json_arr != NULL){
            outputJsonArray(kv->json_arr);
        }
    }
}

void freeJsonObject(JSON_OBJECT* obj){
    if(obj == NULL){
        return;
    }

    for(int i = 0; i < obj->len_entries; i++){
        KEYVALUE* kv = obj->entries[i];
        free(kv->key);
        if(kv->stringvalue != NULL){
            free(kv->stringvalue);
        }
        else if(kv->intvalue != NULL){
            free(kv->intvalue);
        }
        else if(kv->json_obj != NULL){
            freeJsonObject(kv->json_obj);
        }
        else if(kv->json_arr != NULL){
            freeJsonArray(kv->json_arr);
        }
    }

    free(obj);
}

void outputJsonArray(JSON_ARRAY* arr){
    if(arr == NULL){
        return;
    }

    for(int i = 0; i < arr->len_entries; i++){
        JSON_OBJECT* obj = arr->entries[i];
        outputJsonObject(obj);
    }
}

void freeJsonArray(JSON_ARRAY* arr){
    if(arr == NULL){
        return;
    }

    for(int i = 0; i < arr->len_entries; i++){
        JSON_OBJECT* obj = arr->entries[i];
        freeJsonObject(obj);
    }

    free(arr);
}

JSON_OBJECT* parseObject(char* string){
    string = skipspaces(string);
    string = removetrailingspaces(string);

    if(string[0] != OBJECT_OPEN || string[strlen(string) - 1] != OBJECT_CLOSE){
        //INVALID OBJECT
        return NULL;
    }

    //Ignore first and last character of string
    string[strlen(string) -1] = '\0';
    string++;

    char* following;
    char* entry;
    JSON_OBJECT* obj = getEmptyJsonObject();

    do{
        entry = getCommaSeperatedString(string, &following);

        KEYVALUE* kv = parseKV(entry);
        if(kv == NULL){
            //TODO: INVALID JSON
            free(obj);
            return NULL;
        }

        pushKvToJSON(obj, kv);

        string = following;
        free(entry);
        
           
    }while(following != NULL);

    return obj;
}

JSON_ARRAY* parseArray(char* string){
    JSON_ARRAY* json_array;
    char* following;
    char* entry;

    string = skipspaces(string);
    string = removetrailingspaces(string);

    if(string[0] != ARRAY_OPEN || string[strlen(string) - 1] != ARRAY_CLOSE){
        //INVALID ARRAY
        return NULL;
    }

    //Ignore first and last character of string
    string[strlen(string) -1] = '\0';
    string++;

    //init
    json_array = getEmptyJsonArray();

    do{
        entry = getCommaSeperatedString(string, &following);

        JSON_OBJECT* json_obj = parseObject(entry);
        if(json_obj == NULL){
            //TODO: INVALID JSON
            freeJsonArray(json_array);
            return NULL;
        }

        pushObjToArray(json_array, json_obj);

        string = following;
        free(entry);
        
           
    }while(following != NULL);

    return json_array;
}

int parse(char* string, JSON_OBJECT** result_obj, JSON_ARRAY** result_arr){
    //Init
    (*result_obj) = NULL;
    (*result_arr) = NULL;

    int pos;
    JSON_OBJECT* obj;
    JSON_ARRAY* arr;
    
    if((obj = parseObject(string)) != NULL){
        (*result_obj) = obj;
        return 1;
    }
    else if((arr = parseArray(string)) != NULL){
        (*result_arr) = arr;
        return 1;
    }
    else{
        return 0;
    }
}
