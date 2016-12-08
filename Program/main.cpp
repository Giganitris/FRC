/*
The main ideas of this particxulat version is that it would use plugins to
tell the corruptor how the file should be corrupted. This, so far, uses dll/so
files to do this, even though the dll is handling the entire process. You can
personally choose to do what ever you wish, including reverting back to the old
system until you understand and decide what's best. 
*/

#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<clocale>
#include<unistd.h>
#include<dlfcn.h>

#define VERSION "0.6.0.0"
#define LANG_MAX 5

bool (*method)(char*); //A currently undefined function

char msg[LANG_MAX][32] =  //A list of default messages
	{
	"Unknown switch: ",
	"\x1B[1;33mWarning:\x1B[0m ",
	"\x1B[1;31mError:\x1B[0m ",
	"Too few arguments",
	"",
	};

int corrupt(char* plugin,char* args) //dl a function and execute. 
	{
	chdir(getenv("HOME"));chdir(".corrupt");chdir("plugins");
	char *error;
	void *handle; //points to the opened DL file
	handle = dlopen(plugin,RTLD_LAZY);
	error = dlerror();
	if(error != NULL)
		{
		printf("%s%s\n",msg[2],error); //prints Error: 
		return 0;
		}
	method = (bool (*)(char*))dlsym(handle, "method"); //Defines the previously undefined function (global)
	error = dlerror();
	if(error != NULL)
		{
		printf("%s%s\n",msg[2],error);
		return 0;
		}
	return method(args);
	}

int main(int argc, char* argv[]) //detect language and parse arguments
	{
	int err;
	int loop;
	FILE* langfile;
	char language[3];
	strncpy(language,setlocale(LC_ALL, ""),2); //Copy language code into char language[3]
	language[2] = '\0'; //We need to manually add the terminator
	chdir(getenv("HOME"));chdir(".corrupt");chdir("lang"); //Change to ~/.corrupt/lang
	langfile = fopen(language,"r");//If English, it'll open en. 
	if(langfile != NULL)
		{
		for(loop = 0;loop != LANG_MAX;loop++)
			{
			fgets(msg[loop],32,langfile); //Copy all messages, overwrite the default.
			}
		}	//Begin parsing args
	if(argc == 1)	//Of there are no args, print a message.
		{
		printf(msg[4]);
		return 0;
		}
	for(loop = 1;loop != argc;loop++) //Go through all args, and run them.
		{
		if(
		(strcmp(argv[loop],"-h") == 0) or
		(strcmp(argv[loop],"/h") == 0) or
		(strcmp(argv[loop],"--help") == 0))
			(printf(msg[4]));
		else if(
		(strcmp(argv[loop],"-v") == 0) or
		(strcmp(argv[loop],"/v") == 0) or
		(strcmp(argv[loop],"--version") == 0))
			(printf("%s%s",VERSION,"\n"));
		else if(
		(strcmp(argv[loop],"-e") == 0) or
		(strcmp(argv[loop],"/e") == 0) or
		(strcmp(argv[loop],"--execute") == 0))
			{
			loop++;
			system(argv[loop]);
			}
		else if(
		(strcmp(argv[loop],"-c") == 0) or
		(strcmp(argv[loop],"/c") == 0) or
		(strcmp(argv[loop],"--corrupt") == 0))
			{
			if((loop +2) >= argc){printf("%s%s\n",msg[2],msg[3]);exit(0);}
			//If there aren't enough args, print "Error: Too few arguments" and exit.
			err = corrupt(argv[loop+1],argv[loop+2]);
			loop = loop +2;
			}
		else{printf("%s%s%s\n",msg[1],msg[0],argv[loop]);} //Warning: Unknown switch: 
		}
	}
