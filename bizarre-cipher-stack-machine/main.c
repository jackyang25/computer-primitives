//
//  main.c
//  CSO_Project_1
//
//  Created by Jack Yang on 9/25/22.
//


#include <stdio.h>
#include <string.h>
#include "implement.h"
#include <math.h>

    



int main(void){
 
    
    StackNodePtr main_s = NULL;
    StackNodePtr helper_s = NULL;
  
    int num;
    int cont;
    
    while(cont != EOF){
        
        printf("Enter an integer number to push: ");
        
        cont = scanf("%d", &num);
        
        if(isPrime(num) == 1 && cont == 1){
            
            BizarreNumber_t main_next;
            BizarreNumber_t helper_next;
            
            main_next.num = num * num;
            main_next.nrOfpr = 0;
            strcpy(main_next.type, "PR");
            
            
            helper_next.num = -1;
            helper_next.nrOfpr = 0;
            strcpy(helper_next.type, "");
            
            
            push(&main_s, main_next);
            push(&helper_s, helper_next);
            
        }
        
    
        else if(isPerfectSquare(num) == 1 && cont == 1){
            BizarreNumber_t main_next;
            BizarreNumber_t helper_next;
            
            
            int number_operat = 0;
            
            while (isPerfectSquare(num)) {
                number_operat += 1;
                num = sqrt(num);
            }
            
            main_next.num = num;
            main_next.nrOfpr = number_operat;
            strcpy(main_next.type, "SQ");
            
            helper_next.num = -1;
            helper_next.nrOfpr = 0;
            strcpy(helper_next.type, "");
            
            
            
            push(&main_s, main_next);
            push(&helper_s, helper_next);
            
            
            
        }
        
        
        else if(isAbundantNumber(num) == 0 && cont == 1){
            
            
            BizarreNumber_t main_next;
            BizarreNumber_t helper_next;
            
            main_next.num = num;
            main_next.nrOfpr = 0;
            strcpy(main_next.type, "PF");
            
            
            helper_next.num = -1;
            helper_next.nrOfpr = 0;
            strcpy(helper_next.type, "");
            
            
            push(&main_s, main_next);
            push(&helper_s, helper_next);
            
        }
        
        
        
        else if(isAbundantNumber(num) < 0 && cont == 1){
            
            BizarreNumber_t main_next;
            BizarreNumber_t helper_next;
            
            main_next.num = -1;
            main_next.nrOfpr = 0;
            strcpy(main_next.type, "");
            
            helper_next.num = num;
            helper_next.nrOfpr = 0;
            strcpy(helper_next.type, "DF");
            
            push(&main_s, main_next);
            push(&helper_s, helper_next);
            
            
        }
        
        else if(isAbundantNumber(num) > 0 && cont == 1){
            
            BizarreNumber_t main_next;
            BizarreNumber_t helper_next;
            
            
            main_next.num = num - isAbundantNumber(num);
            main_next.nrOfpr = 0;
            strcpy(main_next.type, "AB");
            
            helper_next.num = isAbundantNumber(num);
            helper_next.nrOfpr = 0;
            strcpy(helper_next.type, "AB");
            
            push(&main_s, main_next);
            push(&helper_s, helper_next);
            
            
        }
        
    }
        

      printf("\n\n%s", "The main stack: ");
      printStack(main_s);
      printf("\n%s", "The helper stack: ");
      printStack(helper_s);
      printf("\n%s", "Recovered stack is: ");
    
      
    
      
      
    

      printStackDetailed(recoverCipher(main_s,helper_s));

    
}



