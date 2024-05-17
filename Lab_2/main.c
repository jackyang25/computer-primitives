//
//  main.c
//  Lab_2
//
//  Created by Jack Yang on 3/28/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct pageTableEntry {
    int pageNumber;
    int referenced;
    int modified;
    int inMemory;
    int operation;
    int memoryLocation;
    int class;
    int evicted;
};

struct BufferCollection{
    struct pageTableEntry* buffer0;
    struct pageTableEntry* buffer1;
    struct pageTableEntry* buffer2;
    struct pageTableEntry* buffer3;
    struct pageTableEntry* buffer4;
    int bufferSize0;
    int bufferSize1;
    int bufferSize2;
    int bufferSize3;
    int bufferSize4;
};

#define READ 0
#define WRITE 1

int initializeSize(const char *filename, int pageSize);
int initializePageTable(struct pageTableEntry* pageTable, const char *filename);
int allocateMemory(struct pageTableEntry* physicalMemory,  struct pageTableEntry* pageTable, struct pageTableEntry* loadPagePtr);
int insertPageIntoMemory(struct pageTableEntry* physicalMemory, int freeFrameIndex, struct pageTableEntry* loadPagePtr, int swapFrame, struct pageTableEntry* pageTable);
struct pageTableEntry* getPage(struct pageTableEntry* pageTable);
int clearRBit(struct pageTableEntry* pageTable);
struct BufferCollection classBuffers(struct pageTableEntry* pageTable, struct pageTableEntry* loadPagePtr);
int replacePage(struct pageTableEntry* pageTable, struct BufferCollection buffers);



int pageSize;
int pageTableSize;
int timeUntilClear;
int physicalMemorySize;
int num_reads = 0;
int num_writes = 0;
int nextFreeFrame;
int timestep;
int rbitcounter;
int swapFrame = -1;
int totalTime = 0;
int pageFaults = 0;
float percentPageFaults = 0.0;
int memFull = 0;
int duplicate = 0;


struct pageTableEntry* loadPagePtr;
struct BufferCollection buffers;


int main(int argc, const char * argv[]) {

    pageSize = atoi(argv[2]);
    timeUntilClear = atoi(argv[3]);

    initializeSize(argv[1], pageSize);
    
    struct pageTableEntry* physicalMemory = (struct pageTableEntry*)calloc(physicalMemorySize, sizeof(struct pageTableEntry));
    struct pageTableEntry* pageTable = (struct pageTableEntry*)calloc(pageTableSize, sizeof(struct pageTableEntry));
    

//    printf("---- Input Stats ----\n");
//    printf("Page Size: %d\n", pageSize);
//    printf("Physical Memory Size: %d\n", physicalMemorySize);
//    printf("Page Table Size: %d\n", pageTableSize);
//    printf("Memory Accesses Until R-bits Cleared: %d\n", timeUntilClear);
    printf("num reads = %d\n", num_reads);
    printf("num writes = %d\n", num_writes);

    initializePageTable(pageTable, argv[1]);
        
    // Mapping
    
    
    for (int i = 0; i < pageTableSize; i++) {
        totalTime ++;
        timestep = i;
        rbitcounter ++;
        
        for (int i = 0; i < physicalMemorySize; i++) {
            if(physicalMemory[i].pageNumber == 0)
                memFull = 0;
            else{
                memFull =1;
            }
        }

        
        
//        printf("~~~~~~~~~~~~ Timestep %d ~~~~~~~~~~~~ \n\n", timestep);
//
//        printf("---- Physical Memory ----\n");
        

        loadPagePtr = getPage(pageTable);

        duplicate = 0;
        nextFreeFrame = allocateMemory(physicalMemory, pageTable, loadPagePtr);
        
//        printf("---- Page Table ----\n");
        
        
                
        if (nextFreeFrame != -1){
//            printf("---- Physical Memory (Mapped) ----\n");
            insertPageIntoMemory(physicalMemory, nextFreeFrame, loadPagePtr, swapFrame, pageTable);
        }
        
        
        // No space in memory -> apply NRU
        
        else if (nextFreeFrame == -1){
//            printf("---- Class Buffers ----\n");
            buffers = classBuffers(pageTable, loadPagePtr);
//            printf("---- Physical Memory (Swapped) ----\n");
            swapFrame = replacePage(pageTable, buffers);
            
            insertPageIntoMemory(physicalMemory, nextFreeFrame, loadPagePtr, swapFrame, pageTable);

        
        }
        
        if (rbitcounter == timeUntilClear){
            clearRBit(pageTable);
            rbitcounter = 0;
            
        }
        
        
    }
    
//    printf("\n");
//    for (int i = 0; i < pageTableSize; i++) {
//        printf("pageNumber: %x\n", pageTable[i].pageNumber);
//        printf("referenced: %d\n", pageTable[i].referenced);
//        printf("modified: %d\n", pageTable[i].modified);
//        printf("inMemory: %d\n", pageTable[i].inMemory);
//        printf("operation: %d\n", pageTable[i].operation);
//        printf("memoryLocation: %d\n", pageTable[i].memoryLocation);
//        printf("class: %d\n", pageTable[i].class);
//        printf("evicted: %d\n", pageTable[i].evicted);
//        printf("\n");
//        }

    
    percentPageFaults = (float)pageFaults/(float)totalTime;
    
//    printf("%d\n", pageFaults);
//    printf("%d\n", totalTime);

    
//    if (percentPageFaults < 0) {
//        percentPageFaults = 0; 
//    }
    
    printf("percentage of page faults %.2f\n", percentPageFaults);


    for (int i = 0; i < physicalMemorySize; i++) {
        if(physicalMemory[i].pageNumber != 0){
            printf("mem[%d]: %x\n", i, physicalMemory[i].pageNumber);
        }
        else{
            printf("mem[%d]: ffffffff\n", i);
            
        }
    }
    
    
    return 0;
}




int initializeSize(const char *filename, int pageSize) {
    
    int v_address;
    int operation;
    int scanResult;
    int offset = (int)log2(pageSize);
    
    FILE *filePointer = fopen(filename, "r");
    
    if (filePointer == NULL) {
        perror("Error");
        return 1;
    }

    while ((scanResult = fscanf(filePointer, "%x %d", &v_address, &operation)) != EOF) {
        if(operation == 0){
            num_reads += 1;
        }
        else{
            num_writes += 1;
        }
                    
    }
    
    physicalMemorySize = (int)(pow(2, 10) / pow(2, offset));
    
    pageTableSize = num_reads + num_writes;
    
    fclose(filePointer);
    
    
    return 0;
}



int initializePageTable(struct pageTableEntry* pageTable, const char *filename){
    
    int v_address;
    int v_pageNumber;
    int operation;
    int scanResult;
    int offset = (int)log2(pageSize);
    int curr_element = 0;

    
    

    FILE *filePointer = fopen(filename, "r");
    

    while ((scanResult = fscanf(filePointer, "%x %d", &v_address, &operation)) != EOF) {
                
//        printf("%x %d\n", v_address, operation);
        
        v_pageNumber = v_address >> offset;
        
        // Initialize struct values
        pageTable[curr_element].pageNumber = v_pageNumber;
        pageTable[curr_element].operation = operation;
        pageTable[curr_element].memoryLocation = -1;
        pageTable[curr_element].class = -1;
        
        if(operation == 0){
            pageTable[curr_element].referenced = 1;
        }
        else{
            pageTable[curr_element].referenced = 1;
            pageTable[curr_element].modified = 1;

        }

//        printf("Offset: %d bits, Virtual Page Number: %x\n\n", offset, v_pageNumber);
        
        curr_element += 1;

    }
    
    fclose(filePointer);
        
    
//    for(int i = 0; i < pageTableSize; i++){
//        pageTableEntryPtr = &pageTable[i];
//        printf("VPN: %x, Operation: %d\n", pageTableEntryPtr -> pageNumber, pageTableEntryPtr -> operation);

//    }
//    printf("\n");
    
    return 1;
}



int allocateMemory(struct pageTableEntry* physicalMemory,  struct pageTableEntry* pageTable,  struct pageTableEntry* loadPagePtr){
    int freeFrame = -1;
    
    
    if (memFull == 0){
        for (int i = 0; i < physicalMemorySize; i++){
            
            
//            printf("HELLO %x, %x\n", loadPagePtr->pageNumber, physicalMemory[i].pageNumber);

            
            if(loadPagePtr->pageNumber == physicalMemory[i].pageNumber){
                freeFrame = i;
//                printf("FREE %d\n", freeFrame);
                
   

                
                for(int i = 0; i < pageTableSize; i++){
                    if(pageTable[i].pageNumber == loadPagePtr->pageNumber && pageTable[i].inMemory == 1){
                        duplicate = 1;
                        
                        
                        pageTable[i].inMemory = 0;
                        pageTable[i].memoryLocation = -1;
                        pageTable[i].evicted = 1;
                        pageTable[i].referenced = 0;
                        pageTable[i].modified = 0;
                        pageTable[i].class = -1;
                        
                        
                        break;
                    }
                }
                physicalMemory[i].inMemory = 0;
                physicalMemory[i].pageNumber = 0;
                physicalMemory[i].memoryLocation = -1;
            }
        }
    }
    
    


        for (int i = 0; i < physicalMemorySize; i++) {
            if (physicalMemory[i].pageNumber == 0){
                freeFrame = i;
                break;
            }
        }
    

    
    
    
//    printf(" \n");
    return freeFrame;
    
    
    }



struct pageTableEntry* getPage(struct pageTableEntry* pageTable){
    struct pageTableEntry* loadPagePtr = NULL;
    
    
    for (int i = 0; i < pageTableSize; i++) {
        if (pageTable[i].inMemory == 0 && pageTable[i].evicted == 0){
            loadPagePtr = &pageTable[i];
//            printf("Page to load into memory: %x\n", loadPagePtr -> pageNumber);

            break;
        }
    }
    
    
//    for (int i = 0; i < pageTableSize; i++) {
//        printf("pageNumber: %x\n", pageTable[i].pageNumber);
//        printf("referenced: %d\n", pageTable[i].referenced);
//        printf("modified: %d\n", pageTable[i].modified);
//        printf("inMemory: %d\n", pageTable[i].inMemory);
//        printf("operation: %d\n", pageTable[i].operation);
//        printf("memoryLocation: %d\n", pageTable[i].memoryLocation);
//        printf("class: %d\n", pageTable[i].class);
//        printf("evicted: %d\n", pageTable[i].evicted);
//        printf("\n");
//        }
    
    
    
    return loadPagePtr;

}



int insertPageIntoMemory(struct pageTableEntry* physicalMemory, int nextFreeFrame, struct pageTableEntry* loadPagePtr, int swapFrame, struct pageTableEntry* pageTable){
    
    
    if (swapFrame != -1){
        pageFaults++;
        
        int evictedPageNumber = physicalMemory[swapFrame].pageNumber;
//        int evictedPageIndex = physicalMemory[swapFrame].memoryLocation;

        physicalMemory[swapFrame] = *loadPagePtr;
        
        loadPagePtr -> inMemory = 1;
        
        for(int i = 0; i < pageTableSize; i++){
            if(pageTable[i].pageNumber == evictedPageNumber && pageTable[i].inMemory == 1){
//                printf("xxxxxxxxxxxxxxxxxxxxxxxx");
                
                loadPagePtr -> memoryLocation = pageTable[i].memoryLocation;

                pageTable[i].inMemory = 0;
                pageTable[i].memoryLocation = -1;
                pageTable[i].evicted = 1;
                pageTable[i].referenced = 0;
                pageTable[i].modified = 0;
                pageTable[i].class = -1;

                break;
            }
        }
    }
    
    else{
        

        if(duplicate == 0){
            pageFaults++;

        }

        physicalMemory[nextFreeFrame] = *loadPagePtr;
        loadPagePtr -> inMemory = 1;
        loadPagePtr -> memoryLocation = nextFreeFrame;
            
        }
    
//    for (int i = 0; i < physicalMemorySize; i++) {
//            printf("mem[%d]: %x\n", i, physicalMemory[i].pageNumber);
//        }

    
    return 1;
}


int clearRBit(struct pageTableEntry* pageTable){
    
    for (int i = 0; i < pageTableSize; i++) {
        if(pageTable[i].inMemory){
            pageTable[i].referenced = 0;
        }
    }
//    printf("R-Bits Cleared!\n\n");

    return 1;
}



struct BufferCollection classBuffers(struct pageTableEntry* pageTable, struct pageTableEntry* loadPagePtr){
//    printf("No space in memory to load page: %x\n", loadPagePtr -> pageNumber);
    
    int class0 = 0;
    int class1 = 0;
    int class2 = 0;
    int class3 = 0;
    int class4 = 0;
    
    for(int i = 0; i < pageTableSize; i++){
        
        // Determine duplicate

        if ((pageTable[i].inMemory == 1 && pageTable[i].evicted == 0) && (loadPagePtr -> pageNumber == pageTable[i].pageNumber)){
            pageTable[i].class = 0;
            class0 ++;
        }
        
        else if(pageTable[i].inMemory == 1 && pageTable[i].evicted == 0){
            // Determine class
            
            if ((pageTable[i].referenced == 0) && (pageTable[i].modified == 0)){
                pageTable[i].class = 1;
                class1 ++;
            }
            if ((pageTable[i].referenced == 0) && (pageTable[i].modified == 1)){
                pageTable[i].class = 2;
                class2 ++;
            }
            if ((pageTable[i].referenced == 1) && (pageTable[i].modified == 0)){
                pageTable[i].class = 3;
                class3 ++;
            }
            if ((pageTable[i].referenced == 1) && (pageTable[i].modified == 1)){
                pageTable[i].class = 4;
                class4++;
            }
        }
        
    }
    struct pageTableEntry* classBuffer0 = (struct pageTableEntry*)calloc(class0, sizeof(struct pageTableEntry));
    struct pageTableEntry* classBuffer1 = (struct pageTableEntry*)calloc(class1, sizeof(struct pageTableEntry));
    struct pageTableEntry* classBuffer2 = (struct pageTableEntry*)calloc(class2, sizeof(struct pageTableEntry));
    struct pageTableEntry* classBuffer3 = (struct pageTableEntry*)calloc(class3, sizeof(struct pageTableEntry));
    struct pageTableEntry* classBuffer4 = (struct pageTableEntry*)calloc(class4, sizeof(struct pageTableEntry));
    
    struct BufferCollection BufferCollection;
    BufferCollection.buffer0 = classBuffer0;
    BufferCollection.buffer1 = classBuffer1;
    BufferCollection.buffer2 = classBuffer2;
    BufferCollection.buffer3 = classBuffer3;
    BufferCollection.buffer4 = classBuffer4;
    
    BufferCollection.bufferSize0 = class0;
    BufferCollection.bufferSize1 = class1;
    BufferCollection.bufferSize2 = class2;
    BufferCollection.bufferSize3 = class3;
    BufferCollection.bufferSize4 = class4;


    // Add to buffers
    int counts[5] = {0, 0, 0, 0, 0};

//    struct pageTableEntry* classBuffers[5] = {classBuffer0, classBuffer1, classBuffer2, classBuffer3, classBuffer4};

    

//    printf("ADD TO BUFFERS\n");

    for(int i = 0; i < pageTableSize; i++) {
        int class = pageTable[i].class;

        if(class != -1 && pageTable[i].evicted == 0 && pageTable[i].inMemory == 1) {
//            printf("PN: %x, CL: %d, EV: %d, IM: %d, ML: %d\n", pageTable[i].pageNumber, class, pageTable[i].evicted, pageTable[i].inMemory, pageTable[i].memoryLocation);
            
//            classBuffers[class][counts[class]] = pageTable[i];

            if(class == 0){
                pageFaults -= 1;
                classBuffer0[counts[class]].pageNumber = pageTable[i].pageNumber;
                classBuffer0[counts[class]].memoryLocation = pageTable[i].memoryLocation;
//                printf("append page %x to index %d of buffer 0, actual: %d\n", pageTable[i].pageNumber, counts[class],  BufferCollection.buffer0[counts[class]].pageNumber);

//                printf("PN: %x, CL: %d, EV: %d, IM: %d, ML: %d\n", pageTable[i].pageNumber, class, pageTable[i].evicted, pageTable[i].inMemory, pageTable[i].memoryLocation);

                
            }
            else if (class == 1){
                classBuffer1[counts[class]].pageNumber = pageTable[i].pageNumber;
                classBuffer1[counts[class]].memoryLocation = pageTable[i].memoryLocation;
//                printf("append page %x to index %d of buffer 1, actual: %x\n", pageTable[i].pageNumber, counts[class],  BufferCollection.buffer1[counts[class]].pageNumber);

//                printf("PN: %x, CL: %d, EV: %d, IM: %d, ML: %d\n", pageTable[i].pageNumber, class, pageTable[i].evicted, pageTable[i].inMemory, pageTable[i].memoryLocation);


                
            }
            else if (class == 2){
                classBuffer2[counts[class]].pageNumber = pageTable[i].pageNumber;
                classBuffer2[counts[class]].memoryLocation = pageTable[i].memoryLocation;
                
//                printf("append page %x to index %d of buffer 2, actual: %x\n", pageTable[i].pageNumber, counts[class],  BufferCollection.buffer2[counts[class]].pageNumber);

//                printf("PN: %x, CL: %d, EV: %d, IM: %d, ML: %d\n", pageTable[i].pageNumber, class, pageTable[i].evicted, pageTable[i].inMemory, pageTable[i].memoryLocation);


                
            }
            else if (class == 3){
                classBuffer3[counts[class]].pageNumber = pageTable[i].pageNumber;
                classBuffer3[counts[class]].memoryLocation = pageTable[i].memoryLocation;
//                printf("append page %x to index %d of buffer 3, actual: %x\n", pageTable[i].pageNumber, counts[class],  BufferCollection.buffer3[counts[class]].pageNumber);
                
//                printf("PN: %x, CL: %d, EV: %d, IM: %d, ML: %d\n", pageTable[i].pageNumber, class, pageTable[i].evicted, pageTable[i].inMemory, pageTable[i].memoryLocation);


                
            }
            else if (class == 4){
                classBuffer4[counts[class]].pageNumber = pageTable[i].pageNumber;
                classBuffer4[counts[class]].memoryLocation = pageTable[i].memoryLocation;
//                printf("append page %x to index %d of buffer 4, actual: %x\n", pageTable[i].pageNumber, counts[class],
//                       BufferCollection.buffer4[counts[class]].pageNumber);

//                printf("PN: %x, CL: %d, EV: %d, IM: %d, ML: %d\n", pageTable[i].pageNumber, class, pageTable[i].evicted, pageTable[i].inMemory, pageTable[i].memoryLocation);
                
            }
 
            counts[class]++;
            
            
//            printf("CURRENT ARRAY COUNT: %d, CLASS: %d \n", class, counts[class]);
//            printf("CURRENT PAGE NUMBER %x \n", pageTable[i].pageNumber);


//            printf("PN: %x, FRAME: %d\n", classBuffers[class][counts[class]].pageNumber, classBuffers[class][counts[class]].memoryLocation);
        }
        
        
    }

    
//
//    printf("\nClass 0 (Duplicates) %d\n",  class0);
//    for (int i = 0; i < class0; i++) {
//            printf("Frame: %d, VPN: %x \n", BufferCollection.buffer0[i].memoryLocation, BufferCollection.buffer0[i].pageNumber);
//    }
//
//    printf("\nClass 1 %d \n", class1);
//    for (int i = 0; i < class1; i++) {
//        printf("Frame: %d, VPN: %x \n", BufferCollection.buffer1[i].memoryLocation, BufferCollection.buffer1[i].pageNumber);
//    }
//
//    printf("\nClass 2 %d \n",  class2);
//    for (int i = 0; i < class2; i++) {
//            printf("Frame: %d, VPN: %x \n", BufferCollection.buffer2[i].memoryLocation, BufferCollection.buffer2[i].pageNumber);
//    }
//
//    printf("\nClass 3 %d \n", class3);
//    for (int i = 0; i < class3; i++) {
//        printf("Frame: %d, VPN: %x \n", BufferCollection.buffer3[i].memoryLocation, BufferCollection.buffer3[i].pageNumber);
//    }
//
//    printf("\nClass 4 %d \n", class4);
//    for (int i = 0; i < class4; i++) {
//        printf("Frame: %d, VPN: %x \n", BufferCollection.buffer4[i].memoryLocation, BufferCollection.buffer4[i].pageNumber);
//    }
//
//    printf("\n");


    
    return BufferCollection;
}


int replacePage(struct pageTableEntry* pageTable, struct BufferCollection buffers){
    int lowestIndex = physicalMemorySize + 1;

    if (buffers.bufferSize0 > 0){
        for(int i = 0; i < buffers.bufferSize0; i++){
            if(buffers.buffer0[i].memoryLocation < lowestIndex){
                lowestIndex = buffers.buffer0[i].memoryLocation;
            }
        }
    }
    
    else if (buffers.bufferSize1 > 0){
        for(int i = 0; i < buffers.bufferSize1; i++){
            if(buffers.buffer1[i].memoryLocation < lowestIndex){
                lowestIndex = buffers.buffer1[i].memoryLocation;
            }
        }
    }
    
    else if (buffers.bufferSize2 > 0){
        for(int i = 0; i < buffers.bufferSize2; i++){
            if(buffers.buffer2[i].memoryLocation < lowestIndex){
                lowestIndex = buffers.buffer2[i].memoryLocation;
            }
        }
    }
    else if (buffers.bufferSize3 > 0){
        for(int i = 0; i < buffers.bufferSize3; i++){
            if(buffers.buffer3[i].memoryLocation < lowestIndex){
                lowestIndex = buffers.buffer3[i].memoryLocation;
            }
        }
    }
    else if (buffers.bufferSize4 > 0){
        for(int i = 0; i < buffers.bufferSize4; i++){
            if(buffers.buffer4[i].memoryLocation < lowestIndex){
                lowestIndex = buffers.buffer4[i].memoryLocation;
            }
        }
    }
    
    
    free(buffers.buffer0);
    free(buffers.buffer1);
    free(buffers.buffer2);
    free(buffers.buffer3);
    free(buffers.buffer4);
    
    
//    printf("Frame index to swap: %d\n", lowestIndex);

    return lowestIndex;

}
