#include<cstdint>
#include<cstdio>
#include<cstdlib>

/*
Ahaha there's no way I'll be able to comment on all this. Just a collection of
really old utility functions that I use. I'd recommend avoiding these as much
as possible. 

*/
char IntToStrStr[256];
const char* IntToString(int64_t IntToStrInt){
sprintf(IntToStrStr,"%d",IntToStrInt);
return IntToStrStr;
}

int32_t GetFileSize(FILE* GetFileSizeFile){
int32_t getfilesizesize = 0;
fseek(GetFileSizeFile, 0L, SEEK_SET);
while(fgetc(GetFileSizeFile) != EOF){getfilesizesize = getfilesizesize +1;}
fseek(GetFileSizeFile, 0L, SEEK_SET);
return getfilesizesize;
}

int32_t FormattedStringToInt(const char* FStoIiS,int32_t FStoIEOF = 0){
char FStoIti[16];
strcpy(FStoIti,FStoIiS);
if(strcmp(FStoIti,"EOF") == 0){strcpy(FStoIti,IntToString(FStoIEOF));}
return strtol(FStoIti, NULL, 0);
}

char randm8(char randm8seed, char randm8seed2)
{
randm8seed = 192 * (randm8seed & 255) + (randm8seed >> 8);
randm8seed2 = 134 * (randm8seed2 & 255) + (randm8seed2 >> 8);
return(randm8seed << 8) +randm8seed2;
}

int file_exists(const char* file_exists_filename)
{
FILE* file_exists_fptr = fopen(file_exists_filename, "r");
if(file_exists_fptr != NULL)
	{
	fclose(file_exists_fptr);
	return 1;
	}
return 0;
}

int RNG(int RNGseed)
{
RNGseed = RNGseed*0x343fd+0x269EC3;
RNGseed = (RNGseed >> 0x10) & 0x7FFF;
if (RNGseed < 0){RNGseed = RNGseed *-1;}
return RNGseed;
}
