//
//  main.c
//  CSO_Project_2
//
//  Created by Jack Yang on 10/14/22.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "implement.h"


int main() {
    
    DynamicArray array;
    init(&array);
    
    
    printf("Initialization - count: %d, capacity: %d\n", array.count, array.capacity);
    
    array.pushBackFP(&array, "Gizem");
    array.pushBackFP(&array, "NYU");
    array.pushBackFP(&array, "CSCI");
    array.pushBackFP(&array, "201");
    array.pushBackFP(&array, "Fall");
    array.pushBackFP(&array, "2022");
    array.pushBackFP(&array, "Project201");

    
    printf("\nStep 2 - count: %d, capacity: %d\n\n", array.count, array.capacity);

  
//    int x = 5;
//    array.pushBackFP(&array,&x);
//
//    for (int i = 0; i <= array.count;i++){
//        printf("%d\n", ((int*)array.data)[i]);
//    }
    
    
    
    int i = 0;
    for(i = 0; i < array.count; i++){
        printf("%s\n", array.data[i]);
    }
        
    array.removeAtFP(&array, 2);
    
    
    printf("\nStep 3 - count: %d, capacity: %d\n", array.count, array.capacity);
    printf("\nDynamic Array data:\n\n");
    
    for(i = 0; i < array.count; i++){
        printf("%s\n", array.data[i]);
    }
    
    array.clearArrayFP(&array);

    
    if(array.isEmptyFP(&array)){
        
        printf("\n%s\n", "The array is empty now");
    }
    
    return 0;

}


