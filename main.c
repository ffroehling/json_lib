#include "stdio.h"
#include "stdlib.h"
#include "json.h"


int getRooms(int id){
    char command[100];
    snprintf(command, 100, "curl -X GET http://localhost:8000/api/room/%d/access/ > rooms.json", id);
    int resp = system(command);
    if(resp){
        return 0;
    }
    return 1;
}


char* loadRoomFile(){
    int size, read_bytes, objects;
    FILE* fp;
  
    //Open file
    fp = fopen("rooms.json", "rb");
    if(fp){
        //Get Size
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);

        char* data = malloc(size * sizeof(char));
        //TODO: Check malloc

        //Reset pointer
        rewind(fp);

        //Read file
        read_bytes = fread(data, size, 1, fp);
        if(read_bytes == 1){
            fclose(fp);
            printf("%s\n", data);
            return data;
        }
        else{
            //Sth wrong here
            fclose(fp);
            return NULL;
        }
    }

    return NULL;
}

JSON_ARRAY* parseRoomFile(char* data){
    JSON_OBJECT* obj;
    JSON_ARRAY* arr;

    if(parse(data, &obj, &arr)){
        if(arr != NULL){
            printf("Valid JSON\n");
            return arr;
        }
        else{
            printf("Invalid JSON\n");
            return NULL;
        }
    }

    printf("Invalid JSON\n");
    return NULL;
}

int main(int argc, char** argv){
    char* data;
    JSON_ARRAY* arr;

    if(getRooms(5)){
        printf("Pulled list\n");
        data = loadRoomFile();
        arr = parseRoomFile(data);
        outputJsonArray(arr);
    }
/*    
    int size, read_bytes, objects;
    if(fp){

        //Buffer
        //TODO: Check..


    }

    fclose(fp);*/

//    getRooms(5);
    return 0;
}
