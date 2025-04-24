#include<stdio.h>
#include<stddef.h>

char mymemory[8000];

struct block
{
    size_t size;
    int free;
    struct block *next;
};

typedef struct block block;

block *list=(void*)mymemory;


void initmemory()
{
    //Initialize the list size, free and next
    
    list -> size = 8000 - sizeof(*list);
    list -> free = 1;
    list -> next = NULL;
    
}


void split(block *slotToBeSplitted, size_t size)
{
    //Split the slot to be splitted and create a new slot
    
    block *newslot = (void*)slotToBeSplitted + size + sizeof(*slotToBeSplitted);

    newslot -> size = slotToBeSplitted -> size - size - sizeof(*slotToBeSplitted);
    newslot -> next = slotToBeSplitted -> next;
    newslot -> free = 1;
    
    slotToBeSplitted -> size = size;
    slotToBeSplitted -> next = newslot;
    slotToBeSplitted -> free = 0;

    
}


void *NewMalloc(size_t allocsSize)
{
    block *curr;
    void *result;
    curr=list;
    

    //Find a chunk with enough free space

    
    while((curr -> free == 0 || curr -> size < allocsSize) && curr -> next != NULL){
        curr = curr -> next;
    }
    
    printf("curr size is: %lu\n\n", curr -> size);
    
    if(curr -> size < allocsSize)
    {
        printf("No sufficient memory to allocate %lu bytes \n", allocsSize);

    }
    
    else if(curr -> size == allocsSize){
        printf("Allocate block for %lu bytes \n", allocsSize);
    }
    
    else{
        printf("Allocate block with splitting for %lu bytes \n", allocsSize);

    }

    //Check cases and allocate with/without splitting
    
    if(curr -> size == allocsSize)
    {
        curr -> free = 0;
        result = (void*)curr;
    }
    
    else if(curr -> size > allocsSize)
    {
        split(curr, allocsSize);
        result = (void*)curr;
    }
    
    else
    {
        result = NULL;
        
    }
    
    return result;
    
}


void coalesce()
{
 
    struct block *curr;
    curr=list;


    //Coalesce consequtive free blocks
    

    while(curr -> next != NULL)
    {
        block *next = curr -> next;
        
        if(curr -> free == 1 && next -> free == 1)
        {
            
            curr -> next = next -> next;
            curr -> size += next -> size + sizeof(*next);

        }
        else{
            curr = curr -> next;

        }
   
                    
    }
    
    
}

void NewFree(void* ptr){
    
  //Check if the pointer is in a valid range
  //If yes, free the block and call coalesce
    
    if(ptr != NULL)
    {
        ((block*)ptr) -> free = 1;
        coalesce();
    
        
    }

}

int main(){


    if(!(list->size))
    {
        initmemory();
        printf("\n\n--Memory Initialization Completed--\n");
        printf("Initial size is: %lu\n\n", list->size);
    }
    
    
    int *p=(int*)NewMalloc(800);
    printf("\n");
    char *q=(char*)NewMalloc(250*sizeof(char));
    printf("\n");
    int *r=(int*)NewMalloc(1000*sizeof(int));
    printf("\n");
    NewFree(p);
    char *w=(char*)NewMalloc(1000);
    printf("\n");
    NewFree(r);
    int *k=(int*)NewMalloc(1500*sizeof(int));
    printf("\n");
    

}




