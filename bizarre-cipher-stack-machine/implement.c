//
//  implement.c
//  CSO_Project_1
//
//  Created by Jack Yang on 9/26/22.
//

#include "implement.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>




// Math

int isAbundantNumber(int num){
    int total = 0;
    for(int i = 1; i <= num - 1; i++){
        if(num % i == 0){
            total = total + i;
        }
        
    }
    int val = total - num;
    return val;
}
    


int isPrime(int num){
    
    if(num < 0){
        num = num * -1;
    }
    
    
    int factor = 0;
    for(int i = 2; i <= num - 1; i++){
        if(num % i == 0){
            factor = factor + i;
        }
    }
    
    if(factor > 0){
        return 0;
    }
    else{
        return 1;
    }
}



int isPerfectSquare(int num){
    
    int x = sqrt(num);
    
    if((x*x) == num){
        return 1;
    }
    else{
        return 0;
    }
}

// Main Stack

void push(StackNodePtr *topPtr, BizarreNumber_t info){
    StackNodePtr newPtr = malloc(sizeof(StackNode));
    
    // insert node at top
    
    if (newPtr != NULL) {
        
        newPtr -> data = info;
        newPtr -> nextPtr = *topPtr;
        *topPtr = newPtr;
        
    }
    
    else {
        // no space in memory
        printf("%d not inserted. No memory available.\n", info.num);
    }
}

BizarreNumber_t pop(StackNodePtr *topPtr){
  
    StackNodePtr temp = *topPtr;
    BizarreNumber_t val = (*topPtr) -> data;
    *topPtr = (*topPtr) -> nextPtr;
    free(temp);
    return val;
    
}

int isEmpty(StackNodePtr topPtr){
    return topPtr == NULL;
}

void printStack(StackNodePtr currentPtr){
    
    // if empty
    
    if (currentPtr == NULL) {
        
        puts("The stack is empty.\n");
        
    }
    
    else {
        printf("%s", "\nTOP --> ");
    }
    
    // while stack is not empty
    
    while (currentPtr != NULL) {
        
        
        if (currentPtr -> data.num == -1){
            printf("%s --> ", "NULL");
        }
            
        else{
            printf("%d --> ", currentPtr -> data.num);
            
        }
            

        currentPtr = currentPtr -> nextPtr;
    }
    
    
    puts("BOTTOM");
    

}

void printStackDetailed(StackNodePtr currentPtr){
    
    printf("\n%s\n", "**TOP**");
    
    while (currentPtr != NULL){
        printf("%d, %d, %s\n", currentPtr -> data.num, currentPtr -> data.nrOfpr, currentPtr -> data.type);
        currentPtr = currentPtr -> nextPtr;
               }
    
    printf("%s\n\n", "**BOTTOM**");
}

StackNodePtr reverseStack(StackNodePtr currentPtr){
    
    StackNodePtr temp = NULL;
    
    while (currentPtr != NULL){
        BizarreNumber_t temp_data = pop(&currentPtr);
        push(&temp, temp_data);
    }
    
    return temp;
}


// Recovery

StackNodePtr recoverCipher(StackNodePtr mainStack, StackNodePtr helperStack){
    StackNodePtr newStackPtr = NULL;
    
    
    while(isEmpty(mainStack) != 1){
        
        if(strcmp(mainStack -> data.type, "PR") == 0){
            
            BizarreNumber_t new;
    
            
            new.num = sqrt(mainStack -> data.num);
            new.nrOfpr = 0;
            strcpy(new.type, "PR");
            
            
     
            pop(&mainStack);
            pop(&helperStack);
            push(&newStackPtr, new);
        }
            
        else if(strcmp(mainStack -> data.type, "PF") == 0){
            
            BizarreNumber_t new;
            
            new.num = mainStack -> data.num;
            new.nrOfpr = 0;
            strcpy(new.type, "PF");
                        
            

            pop(&mainStack);
            pop(&helperStack);
            push(&newStackPtr, new);
            
        }
        
        else if(strcmp(mainStack -> data.type, "SQ") == 0){
            
            BizarreNumber_t new;
            
            int start = mainStack -> data.num;
            int times = mainStack->data.nrOfpr;
            
            new.nrOfpr = mainStack->data.nrOfpr;
            strcpy(new.type, "SQ");
                        
            for (int i = 1; i <= times; i++){
                start = start * start;
            }
            
            new.num = start;
            
    
            pop(&mainStack);
            pop(&helperStack);
            push(&newStackPtr, new);
            
        }
        
        
        else if(strcmp(mainStack -> data.type, "AB") == 0){
            
            
            BizarreNumber_t new;
            
            new.num = (mainStack -> data.num) + (helperStack -> data.num);
            new.nrOfpr = 0;
            strcpy(new.type, "AB");
            
            

            pop(&mainStack);
            pop(&helperStack);
            push(&newStackPtr, new);
            
            
            
        }
        
        else if(strcmp(helperStack -> data.type, "DF") == 0){
            
            
            BizarreNumber_t new;
            
            new.num = helperStack -> data.num;
            new.nrOfpr = 0;
            strcpy(new.type, "DF");
            
            
     
            pop(&mainStack);
            pop(&helperStack);
            push(&newStackPtr, new);
            
            
            
        }
        
        
        
    }
    

    return reverseStack(newStackPtr);;
    
}








