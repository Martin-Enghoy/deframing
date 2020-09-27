/*
 *
 *	Martin Adrian C. Enghoy - 11811463 - DIGDACM
 *	Deframing after Line Coding Decode Output
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 100000

//2nd DF

char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize);
int *getPayloadSize(char payOne[], char payTwo[], int *payloadSize);
//void *getPayloadSize(char payOne[], char payTwo[], int *payloadSize);


int main(){
	
	int maxLength =128;
	int currSize = 0;
	int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p;
	int *payloadSize;
	
	char *inputFromLC = malloc(MAX);
	currSize = maxLength;
	
	//Get input
	inputFromLC = storingLongStr(inputFromLC, maxLength, currSize);
	
	/* 
	 *
	 *	Start byte has 8 bits.
	 *	Payload size value (2 bytes MAX) = 16 bits
	 *	Payload with EDC (45 bytes + 1 MAX) = 405 bits
	 *	Stop byte has 8 bits.
	 *	
	 */
	int frameTotSize = 440; //437 total bits + 3;
	int LClen = strlen(inputFromLC);
	int groups = LClen / 405;

	char dframes[groups][frameTotSize];
	int indexCount = 0;
	int counter = 0;
	
	
	/* 
	 *
	 *	Checking for start bit and stop bit ahead.
	 *	
	 */
	char startByte[8], stopByte[8];
	strncpy(startByte,inputFromLC,8);
	startByte[8] = '\0';
	printf("%s~",startByte);
	strncpy(stopByte,&inputFromLC[LClen - 8],8);
	stopByte[8] = '\0';
	printf("%s",stopByte);
	if (!strcmp(startByte,"00000001")){
		indexCount = 8;
		//startByte == "00000001"
		printf("\nI found the start byte.");
		if (!strcmp(stopByte,"00000100")){		
			//stopByte == "00000100"
			printf("\nI found the stop byte.\n");
			/* 
			 *
			 *	Initialization of acquisition of header value
			 *	for Payload size.
			 *	
			 */
			
			char firstHeader[8]; //first number of payload size
			for(c=0; c<8; c++){
				
				//strncpy(firstHeader,inputFromLC,8);
				firstHeader[c] = inputFromLC[indexCount];
				//printf("%s\n",firstHeader);
				//printf("~");
				//printf("%s\n",inputFromLC);
				//printf("-%d",count);
				indexCount++;
				//printf("-%d",count);
			}
			firstHeader[c] = '\0';
			//printf("-%d",count);
			printf("%s\n",firstHeader);
			printf("-%d\n",indexCount);
			
			char secondHeader[8]; //second number of payload size
			for(d=0;d<8;d++){
				secondHeader[d] = inputFromLC[indexCount];
				//printf("%s~",secondHeader);
				//printf("%s\n",inputFromLC);
				//printf("-%d",count);
				indexCount++;
				//printf("-%d",count);
			}
			secondHeader[d]='\0';
			//printf("-%d",count);
			
			printf("%s\n",secondHeader);
			int payLoad;
			
			//getPayloadSize(firstHeader,secondHeader,payloadSize);
			
			//printf("\n%d",payloadSize);
			
			payloadSize = getPayloadSize(firstHeader,secondHeader,payloadSize);
			
			printf("\n%d", payLoad);
			
			printf("~%d", indexCount);
			/* 
			 *
			 *	Starting from first index of payload.
			 *	First index starts after 2nd value of header.
			 *
			 */
			 
			printf("%\n%d", payloadSize*8);
			 
			int maxLC = 405; // Maximum size of entire payload + parity byte + bits.
			int grpCount = 0; //init for number of groups in output payload
			for(e = 24; e < (indexCount+payloadSize*8); e+=8){
				for(f = 0; f < 8; f++){
					dframes[grpCount][f] = inputFromLC[e+f];
				}
				grpCount++;
			}
			printf("\n~%d\n",grpCount);
		
			for(k=0; k<grpCount; k++){
				for(l=0; l<8; l++){
				printf("%c", dframes[k][l]);
				}
				printf("\n");
			}
			printf("\n");
			
		
			
			/* 
			 *
			 *	Getting the parity byte and parity bits
			 *	Parity byte is the last of the payload size value.
			 *	Parity bits start after parity byte index.
			 *	Parity bits will be concatenated into initial payload
			 *
			 *	
			 *	Current tests have problems with concatenation.
			 */
			
			int indexOfParityBits = indexCount+payloadSize*8;
			printf("%d\n", indexOfParityBits);
			for (g = 0; g < grpCount; g++){
				
				dframes[g][8] = inputFromLC[indexOfParityBits];
				
				//for(h = indexOfParityBits; h < (LClen - 8); h++){
					//concat(dframes[g],inputFromLC[h]);
					//strcat(dframes[g],inputFromLC[h]);
					//dframes[g][9] = inputFromLC
				//}
				indexOfParityBits++;
				dframes[g][9] = '\0';
			}
			 			
			/* 
			 *
			 *	Outputting of 2D Array storing payload concatenated.
			 *
			 */

			for(i=0; i<grpCount; i++){
				for(j=0; j<9; j++){
					printf("%c", dframes[i][j]);
				}
				printf("~");
			}
			
			
		}
	}
	
	free(inputFromLC);
	inputFromLC = NULL;
	return 0;
}

/*
void *getPayloadSize(char payOne[], char payTwo[], int *payloadSize){
	int i,j,k,l,bin,sumOne=0,sumTwo=0;
	char buffer[3];
	
	//Tens value of Payload Size (1st number)
	bin = 128;
	for (i=0; i<8; i++){
		printf("%d~",sumOne);
		if(payOne[i]=='1'){
			sumOne += bin;
			printf("%d<",sumOne);
		}
		bin = bin/2;
	}	
	
	buffer[0] = sumOne; //'1' -> int 1;
	
	
	//Ones value of Payload Size (2nd number)
	bin = 128;
	for (k=0; k<8; k++){
		if(payTwo[k]=='1'){
			sumTwo += bin;
		}
		bin = bin/2;
	}
	
	buffer[1] = sumTwo;
	
	printf("%d", buffer[0]);
	printf("\n%d", buffer[1]);
	printf("\n~%s",buffer);
	payloadSize = atoi(buffer);
}
*/


int *getPayloadSize(char payOne[], char payTwo[], int *payloadSize){
	int i,j,k,l,bin,sumOne=0,sumTwo=0;
	char buffer[3];
	
	//Tens value of Payload Size (1st number)
	bin = 128;
	for (i=0; i<8; i++){
		if(payOne[i]=='1'){
			sumOne += bin;
		}
		bin = bin/2;
	}	
	
	buffer[0] = sumOne; //'1' -> int 1;
	
	
	//Ones value of Payload Size (2nd number)
	bin = 128;
	for (k=0; k<8; k++){
		if(payTwo[k]=='1'){
			sumTwo += bin;
		}
		bin = bin/2;
	}
	
	buffer[1] = sumTwo;
	buffer[2] = '\0';
	
	printf("%s",buffer);
	
	return payloadSize = atoi(buffer);
}


void concat(char p[], char q[]){
	int c=0, d=0;
	
	while (p[c] != '\0'){
		c++;
	}
	
	while (q[d] != '\0'){
		p[c] = q[d];
		d++;
		c++;
	}
	p[c] = '\0';
}


// Dynamic allocation for the inputted data
char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize) {
    if (str != NULL) {
        int c = EOF;
        unsigned int i = 0;

        // Accept user input until hit enter or end of file
        while ((c = getchar()) != '\n' && c != EOF) {
            str[i++] = (char) c;

            // If i reaches maximum size, realloc activated
            if (i == currSize) {
                currSize = i + maxLength;
                str = realloc(str, currSize);
            }
        }
        str[i] = '\0';
    }
    return str;
}


