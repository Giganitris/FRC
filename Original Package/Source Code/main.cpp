#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<cstdint>
#include<cstring>
#include<time.h>
#include<cerrno>
#include"Portable.h"
char version[16] = "[A.1.5.8]";
unsigned int intversion = 1158;
//These strings allow easy translation to other languages
const char* error1 = "Unknown switch.\n";
const char* error2 = "Missing arguements.\n";
const char* error3 = "Unknown operation or function:";
const char* warning1 = "This file appears to be from a new version of this corruptor. Go to https://sourceforge.net/projects/corrupter-for-linux/ to download.\n";

char* Settings[8];	//Stores a POINTER to the user settings
char iputCa[8][32];
FILE* iputROM;
uint32_t iputROMsize;
FILE* oputROM;
FILE* iputC;
FILE* oputC;
uint32_t Start;
uint32_t Stop;
uint32_t Period;
char Additional;

void load_from_settings(){
if((iputROM = fopen(Settings[1],"rb")) == NULL){perror(Settings[1]);exit(1);}	//Load the victim ROM file
if((oputROM = fopen(Settings[2],"wb")) == NULL){perror(Settings[2]);exit(1);}	//Load the new ROM file
iputROMsize = GetFileSize(iputROM);			//Get the ROM file size for percentage calcs (not implemented)
Start = FormattedStringToInt(Settings[3]);		//Where to start
Stop =  FormattedStringToInt(Settings[4],iputROMsize);	//Where to stop
Period =  FormattedStringToInt(Settings[5]);		//Period (Reciprocal of the frequency)
//op doesn't need processing, that's why 6 is skipped
Additional =  FormattedStringToInt(Settings[7]);	//Additional value for op
}

char op(char opc,uint32_t opCdimX){
char opTempPtr[8];
strncpy(opTempPtr,Settings[6],3);
if(strcmp(opTempPtr, "add") == 0){return opc+Additional;}
else if(strcmp(opTempPtr, "sub") == 0){return opc-Additional;}
else if(strcmp(opTempPtr, "mul") == 0){return opc*Additional;}
else if(strcmp(opTempPtr, "div") == 0){return opc/Additional;}
else if(strcmp(opTempPtr, "ran") == 0){return RNG(iputROMsize*Additional*opc*opCdimX) % 255 -128;}
else if(strcmp(opTempPtr, "xor") == 0){return opc xor Additional;}
else if(strcmp(opTempPtr, "and") == 0){return opc and Additional;}
else if(strcmp(opTempPtr,  "or") == 0){return opc or Additional;}
else if(strcmp(opTempPtr,"or\n") == 0){return opc or Additional;}
else if(strcmp(opTempPtr, "not") == 0){return not(opc);}
else if(strcmp(opTempPtr, "bsl") == 0){return opc << Additional;}
else if(strcmp(opTempPtr, "bsr") == 0){return opc >> Additional;}
else if(strcmp(opTempPtr, "mod") == 0){return opc % Additional;}
else if(strcmp(opTempPtr, "rep") == 0){return Additional;}
else{printf("%s %s\n",error3,opTempPtr);exit(1);}
}

void corrupt(){
uint32_t CdimX = 0;
uint16_t CFreqCounter = 0;
int Cregbyte;
while((Cregbyte = fgetc(iputROM)) != EOF){				//While we haven't reached the end of the file...
	if((CFreqCounter >= Period) and ((CdimX >= Start) and (CdimX <= Stop))){//And the conditions are juuust right...
		Cregbyte = op(Cregbyte,CdimX);putc(Cregbyte, oputROM);CFreqCounter = 0;}	//corrupt dat byte!
	else{putc(Cregbyte, oputROM);}							//Or don't...
	CFreqCounter = CFreqCounter +1;CdimX = CdimX +1;}				//counting
perror("");
}

int main(int argc, char* argv[])
{
char maintempsetting[256];	//Extra container, non pointer
uint8_t MainArgIndex = 1; 	//which argument to check
uint8_t MainArgSI;		//An index variable for the settings
while(MainArgIndex < argc){	//Begin parsing CMD
	MainArgSI = 1; 			//Restore Settings Indexer (so it can be reused)
	if(strcmp(argv[MainArgIndex],"-v") == 0){printf("%s\n", version);}		//Output version
	else if(strcmp(argv[MainArgIndex],"-s") == 0){					//Create corruption file
		if((argc-(MainArgIndex+1))<0){printf(error2);exit(1);}			//Avoid seg fault
		while(MainArgSI != 2){Settings[MainArgSI] = argv[MainArgIndex +MainArgSI];MainArgSI = MainArgSI +1;}	//Load settings
		MainArgIndex = MainArgIndex +1; 								//Restore the 	cursor
		sprintf(maintempsetting,Settings[1]);
		while(file_exists(maintempsetting)){sprintf(maintempsetting,"%s%i",maintempsetting,time(NULL));}//don't erase existing C's
		if((oputC = fopen(maintempsetting,"wb")) == NULL){perror(maintempsetting);exit(1);}	//Load the new file
		fputs(IntToString(intversion),oputC);
		fputs("\n",oputC);
		fputs(IntToString(Start),oputC);
		fputs("\n",oputC);
		fputs(IntToString(Stop),oputC);
		fputs("\n",oputC);
		fputs(IntToString(Period),oputC);
		fputs("\n",oputC);
		fputs(Settings[6],oputC);
		fputs("\n",oputC);
		fputs(IntToString(Additional),oputC);
		fputs("\n",oputC);
		errno = 0;
		perror("");
		while(fclose(oputC) == EOF){}	//Close the file as needed
		}	
	else if(strcmp(argv[MainArgIndex],"-l") == 0){					//Corrupt using a file
		if((argc-(MainArgIndex+3))<0){printf(error2);exit(1);}	//Avoid seg fault
		while(MainArgSI != 4){Settings[MainArgSI] = argv[MainArgIndex +MainArgSI];MainArgSI = MainArgSI +1;} //load
		MainArgIndex = MainArgIndex +3; 						//Restore the 	cursor
		if((iputC = fopen(Settings[1],"rb")) == NULL){perror(Settings[1]);exit(1);}	//Load the instructions
		Settings[1] = Settings[2];
		Settings[2] = Settings[3];
		fgets(iputCa[1],256,iputC);if(FormattedStringToInt(iputCa[1]) > intversion){printf(warning1);}
		fgets(iputCa[2],256,iputC);Settings[3] = iputCa[2];
		fgets(iputCa[3],256,iputC);Settings[4] = iputCa[3];
		fgets(iputCa[4],256,iputC);Settings[5] = iputCa[4];
		fgets(iputCa[5],256,iputC);Settings[6] = iputCa[5];
		fgets(iputCa[6],256,iputC);Settings[7] = iputCa[6];
		load_from_settings();
		corrupt();
		while(fclose(iputROM) == EOF){}	//Close the files as needed
		while(fclose(oputROM) == EOF){}	//If you can't close the file, try again
		}
	else if(strcmp(argv[MainArgIndex],"-c") == 0){					//Corrupt using settings
		if((argc-(MainArgIndex+7))<0){printf(error2);exit(1);}					//Avoid seg fault
		while(MainArgSI != 8){Settings[MainArgSI] = argv[MainArgIndex +MainArgSI];MainArgSI = MainArgSI +1;} //load
		MainArgIndex = MainArgIndex +7; 	//Restore the cursor
		load_from_settings();			//Prepare for corruption
		corrupt();				//Corrupt
		while(fclose(iputROM) == EOF){}	//Close the files as needed
		while(fclose(oputROM) == EOF){}	//If you can't close the file, try again
		}
	else if(strcmp(argv[MainArgIndex],"-e") == 0){					//Corrupt using settings
		if((argc-(MainArgIndex+1))<0){printf(error2);exit(1);}			//Avoid seg fault
		while(MainArgSI != 2){Settings[MainArgSI] = argv[MainArgIndex +MainArgSI];MainArgSI = MainArgSI +1;} //load
		MainArgIndex = MainArgIndex +1; //Restore the cursor
		system(Settings[1]);
		}
	else{printf("%s",error1);exit(1);}
	MainArgIndex = MainArgIndex +1;
	}
}
