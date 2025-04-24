//
//  implement.c
//  CSO_Project_2
//
//  Created by Jack Yang on 10/14/22.
//

#include "implement.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void init(DynamicArray* array){
    
    
    array -> capacity = 4;
    array -> count = 0;
    array -> data = malloc(4 * sizeof(void*));
    
    int total_length = sizeof(array->data);
    
    
//    array -> curr = 0;
    
    
    
    array -> pushBackFP = pushBack;
    array -> removeAtFP = removeAt;
    array -> clearArrayFP = clearArray;
    array -> isEmptyFP = isEmpty;
    

}


void pushBack(DynamicArray* array, void* val){
    int test = sizeof(val);
    if(array -> count == array -> capacity){
        array -> capacity = array -> capacity * 3;
    }
    

//    printf("%d\n", &((unsigned char*)array -> data)[array -> curr]);
    
    memcpy(&array->data[array->count],&val, sizeof(val));
//    int *ptr = &((array -> data)[array -> curr]);
    
    
//    for(int i = 0; i <= array->capacity; i++){
//        if(ptr == NULL){
//            strcpy(ptr, '\0');
//            break;
//
//        }
//        array -> curr += i;
//        ptr = &((array -> data)[array -> curr]);
//
//    }
    
//    strcpy(ptr, val);

//    printf("%s\n", ptr);

    array -> count += 1;
}
            
    


void clearArray(DynamicArray* array){
    
    int i = 0;

    for(i = 0; i < array -> capacity; i++){
        if(((unsigned char*)array->data)[i] != NULL){
            ((unsigned char*)array->data)[i] = NULL;

        }
                
    }
    array -> count = 0;


}
    


int isEmpty(DynamicArray* array){

    if (array -> count == 0){
        return 1;
    }
    
    else{
        return 0;
    }
    
}
    


void removeAt(DynamicArray* array, int index){
    
    void* new_val = NULL;
    
    memcpy(&array->data[index], &array->data[array->count-1], sizeof(void*));
    memcpy(&array->data[array->capacity-1], &new_val, sizeof(void*));
    
        
//
//        memcpy(array->data + i, &((char*)array->data)[sizeof(void*) * (array -> count -1)], sizeof(void*));
//
//
//        ((char*)array->data)[sizeof(void*) * (array -> count -1)] = NULL;
        
        
    array -> count -= 1;

    
}
    
    



