//
//  implement.h
//  CSO_Project_1
//
//  Created by Jack Yang on 9/26/22.
//

#ifndef implement_h
#define implement_h

#include <stdio.h>

#endif /* implement_h */


typedef struct {
    char type[3];
    int nrOfpr;
    int num;
} BizarreNumber_t;

struct stackNode {
    BizarreNumber_t data;
    struct stackNode *nextPtr;
    
};

typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;



// Stack

void push(StackNodePtr *topPtr,BizarreNumber_t info);
BizarreNumber_t pop(StackNodePtr *topPtr);
int isEmpty(StackNodePtr topPtr);
void printStack(StackNodePtr currentPtr);
void printStackDetailed(StackNodePtr currentPtr);


// Math

int isAbundantNumber(int num);
int isPrime(int num);
int isPerfectSquare(int num);

// Recovery

StackNodePtr recoverCipher(StackNodePtr mainStack, StackNodePtr helperStack);




