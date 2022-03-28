#include <bits/stdc++.h>
#include <dirent.h>
#include "../HeaderFile/PorterStemming.h"


#define TRUE 1
#define FALSE 0

char * s;         /* a char * (=string) pointer; passed into b above */

#define INC 50           /* size units in which s is increased */
static int i_max = INC;  /* maximum offset in s */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

char *targetdir;    /* target directory*/
char *outputdir;    /* output directory*/

#pragma GCC diagnostic pop

void increase_s()
{  i_max += INC;
   {  char * new_s = (char *) malloc(i_max+1);
      { int i; for (i = 0; i < i_max; i++) new_s[i] = s[i]; } /* copy across */
      free(s); s = new_s;
   }
}

#define UC(ch) (ch <= 'Z' && ch >= 'A')
#define LC(ch) (ch <= 'z' && ch >= 'a')
#define LETTER(ch) (UC(ch) || LC(ch))
#define FORCELC(ch) (ch-('A'-'a'))

void stemfile(FILE * f, char* name){

	char outputfile[100] = "";
	strcpy(outputfile, outputdir);
	strcat(outputfile , name);
	//printf("%s", outputfile);
	FILE * op = fopen(outputfile,"w");
	while(TRUE){  
		int ch = getc(f);
		PorterStemming stm;
      	if (ch == EOF){
      		break;
      	}
      	if (LETTER(ch)){  
      		int i = 0;
         	while(TRUE){  
         		if (i == i_max) increase_s();

            	if UC(ch) ch = FORCELC(ch);
            	/* forces lower case. Remove this line to make the program work
               exactly like the Muscat stemtext command. */

            	s[i] = ch; i++;
            	ch = getc(f);
            	if (!LETTER(ch)) { ungetc(ch,f); break; }
         	}
		s[stm.stem(s,0,i-1)+1] = 0;
        /* the pevious line calls the stemmer and uses its result to
            zero-terminate the string in s */
        fputs(s,op);
        //printf("%s", s);
      }
      else putc(ch ,op);
      //putchar(ch);
   }
   fclose(op);
}


int main(int argc, char * argv[]){  
	int i;
	DIR *dir;
	struct dirent *ent;
	targetdir = argv[1];
	outputdir = argv[2];
	std::cout << "Stemming files from " << targetdir << " to " << outputdir << std::endl;
	if ((dir = opendir (targetdir)) != NULL) {
  	/* stem all the files and directories within directory */
	  	while ((ent = readdir (dir)) != NULL) {
	  		s = (char *) malloc(i_max+1);
	  		char targetfile[100] = "";
	  		strcpy(targetfile, targetdir);
	  		strcat(targetfile , ent->d_name);
	  		if(ent->d_name[0] == '.') continue;
	  		//std::cout << ent-> d_name << std::endl;
	  		FILE * f = fopen(targetfile,"r");
	  		if (f == NULL) { 
				fprintf(stderr, "File %s not found\n", ent->d_name);
			}else{
				printf("File %s is stemming... ", ent->d_name);
				stemfile(f, ent->d_name);
				printf("done\n");
			}
			free(s);
			fclose(f);
	  	}
	  	closedir (dir);
	}else{
	  	/* could not open directory */
	  	fprintf(stderr,"No such directory\n");
	  	return EXIT_FAILURE;
	}

	return 0;
}


