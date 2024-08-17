/*Students: Mansoor Ali, Illya Gordyy
 *Date: 07/12/2024
Class: CSC 139 - 02
Description:  C program to copy contents of fileA to fileB using different buffer sizes, while also doubling 
the file size each time with respect to the time it took the program to execute. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

//Read and writes the contents of fileA into fileB while also storing the elapsed time inside of an array
void readWrite(size_t chunks, int *times, FILE *fileA, FILE *fileB, int i){
    clock_t timer = clock();
    char buffer[chunks];
    size_t bytesRead;
    while((bytesRead = fread(buffer, 1, chunks, fileA)) > 0)
        fwrite(buffer, 1, bytesRead, fileB);
    clock_t timerEnd = clock();
    long double elapsed = (long double) (timerEnd - timer) / (long double)(CLOCKS_PER_SEC); 
    *(times+i) = (int)((elapsed)*1000000);   //Microseconds   10^(-6) of a second

}



void data(size_t chunks){


 
    int  times[8];

    FILE *fileA, *fileB;


    //Initial file size
    size_t fileSize = 1024*1024;

    int fileNum = 0; // There are 8 file sizes in total


    while(fileNum < 8){




        // Open file fileB.txt
        fileB = fopen("file2.txt", "wb");
        if (fileB == NULL) {
            perror("Error opening file 2");
            fclose(fileA);
        }




        //Truncating file size to 0
        fileA = fopen("file1.txt", "wb");


        //Increasing the fileA size by fileSize
        fileA = fopen("file1.txt", "ab");
        if (fileA == NULL) {
            perror("Error opening file 1 in append mode");
        }
        char *buffer = (char *)calloc(fileSize, sizeof(char));
        if(fwrite(buffer, sizeof(char), fileSize, fileA) == -1)
            printf("Error");
        free(buffer);// Freeing the buffer




        // Open file fileA.txt
        fileA = fopen("file1.txt", "rb");
        if (fileA == NULL) {
            perror("Error opening file 1");
        }


        readWrite(chunks, times, fileA, fileB, fileNum);


        fclose(fileA);
        fclose(fileB);


        fileNum++; //File iteration
        fileSize*=2; //Doubling the file size
    }


    //Prints out elapsed time for each file size
    for(int i = 0; i < 8; i++){
        printf(" %i ", *(times+i));

    }

    printf("\n");
}



int main() {
    //Different buffer sizes
    size_t chunks[] = {1,8, 1024};

    //Prints out data for each buffer size
    for(int i = 0 ; i < 3; i++){
        printf("Chunk %i bytes: ", *(chunks+i));
        data(*(chunks+i));
    }


    return 0;
}


