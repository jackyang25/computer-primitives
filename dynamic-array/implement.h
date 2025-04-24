//
//  implement.h
//  CSO_Project_2
//
//  Created by Jack Yang on 10/14/22.
//

#ifndef implement_h
#define implement_h
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif /* implement_h */


typedef struct DynamicArray DynamicArray;
struct DynamicArray{
    
    void** data;
    int capacity;
    int count;
    int curr;
    
    
    void (*pushBackFP)(DynamicArray*, void* val);
    int (*isEmptyFP)(DynamicArray*);
    void (*removeAtFP)(DynamicArray*, int index);
    void (*clearArrayFP)(DynamicArray*);
    
};


void pushBack(DynamicArray* array, void* val);
int isEmpty(DynamicArray* array);
void removeAt(DynamicArray* array, int index);
void clearArray(DynamicArray* array);
void init(DynamicArray* array);







