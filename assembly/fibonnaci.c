//
//  main.c
//  CSO_Project_3
//
//  Created by Jack Yang on 11/6/22.
//

#include <stdio.h>


int fibonnaci(int n) {
    
    if(n == 0 || n == 1){
        return n;
        
    }
    else{
        return fibonnaci(n - 1) + fibonnaci(n - 2);
        
    }
    
}
