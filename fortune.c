/*      This is a DOS version of the  classic unix fortune cookie program*/
/*      Copyright (C) 2000 Ian Scott (email:ianandjane@waitrose.com)*/

/*      This program is free software; you can redistribute it and/or modify */
/*      it under the terms of the GNU General Public License as published by */
/*      the Free Software Foundation; either version 2 of the License, or */
/*      (at your option) any later version. */

/*      This program is distributed in the hope that it will be useful, */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*      GNU General Public License for more details. */

/*      You should have received a copy of the GNU General Public License */
/*      along with this program; if not, write to the Free Software */
/*      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307  USA */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#ifndef FORTUNE_FILE
/*Default fortune file*/
#define FORTUNE_FILE "/Users/yane/fortune.txt"
#endif

/*Size of buffer*/
#define MAX_FORTUNE_SIZE 80*25

/*separator*/
#define SEPARATOR_CHAR '%'

/*function declarations*/

void dofortune(char *filename);
/*gets and displays a fortune from the supplied file*/

void readln(FILE *fp, char *buffer);
/*reads a line from file fp into buffer*/

long getfilesize(FILE *fp);
/*gets the size of the file*/

void seedrand();
/*seeds the random number generator*/

int getrand(int max);
/*gets a random number between 0 and max*/

int main(int argc,char *argv[]){
   char filename[256];
   switch(argc){
    case 1:
      /*use default fortune file*/
      strcpy(filename,FORTUNE_FILE);
      break;
    case 2:
      /*use supplied file*/
      strcpy(filename,argv[1]);
      break;
    default:
      /*print usage and exit*/
      fprintf(stderr,"Usage: fortune [fortune-file]\n");
      exit(1);
   }
   /*retrieve and print the fortune*/
   dofortune(filename);
   return(EXIT_SUCCESS);
}

void dofortune(char *filename){
   FILE *fp;
   long fsize;
   int rnd;
   char txt[MAX_FORTUNE_SIZE];
   
   /*open file*/
   fp=fopen(filename,"rt");
   /*check for failure*/
   if(fp==NULL){
      printf("%s cannot be opened\n",filename);
      exit(EXIT_FAILURE);
   }
   
   /*move to random position in file*/
   fsize=getfilesize(fp);
   seedrand();
   rnd=getrand(fsize);
   fseek(fp,rnd,SEEK_SET);
   
   /*look for next separator*/
   txt[0]='A';
   while(txt[0]!=SEPARATOR_CHAR){
      readln(fp, txt);
   }
   
   /*read and print fortune*/
   readln(fp,txt);
   while(txt[0]!='%'){
      printf("%s",txt);
      readln(fp, txt);
   }
   
   /*close the file*/
   fclose(fp);
}

void seedrand(){
   long seed;
   seed=time(0);
#ifdef __GNUC__
   srandom((int)seed);
#else
   srand((unsigned) time(NULL));
#endif
}

void readln(FILE *fp, char *buffer){
   char *ptr;
   ptr=fgets(buffer,MAX_FORTUNE_SIZE,fp);
  if(ptr==NULL){
      printf("Arrgh! An error!\n");
      exit(EXIT_FAILURE);
   }
   
}

long getfilesize(FILE *fp){
   long end;
   fseek(fp,0,SEEK_END);
   end= ftell(fp);
   fseek(fp,0,SEEK_SET);
   return end;
}


int getrand (int max){
   long sysrand;
   float randfloat,myrand;
#ifdef __GNUC__
   sysrand=random();
#else
   sysrand=rand();
#endif   
   randfloat=(float)sysrand/(float)RAND_MAX;
   myrand=randfloat*max;
   return floor(myrand);
}
