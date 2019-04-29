#include "misc.h"
/*------------------------------------------------------------------------*/
char * decToQuad(int dec){/*transforms 10-bit to 4-bit*/

    char * Quad = NULL;
    int i=FLS,buf;

    Quad=(char *)malloc(sizeof(char)); /*allocation first memory*/
/*----------------------------*/
    if(!(Quad)) /*executes if memory allocation fails*/
    {
	printf("memory allocation failed.\n");
	exit(TRU);
    }
/*----------------------------*/
    while(dec!=FLS) {
	i++;
	buf = dec%4;
        Quad=(char*)realloc(Quad,i*sizeof(char));
     
        if(!Quad){/*executes if memory allocation fails*/
	      printf("memory allocation failed.\n");
	      exit(TRU);
         }
	if(buf==FLS) {
	  Quad[i-1]='a';
	  }
	else  if(buf==1) {
		Quad[i-1]='b';
		}
	      else  if(buf==2) {
			Quad[i-1]='c';
			}
		    else Quad[i-1]='d';
        dec = dec/4;
   
     }/*end while*/
/*----------------------------*/
    reverseString(Quad);/*reverses the string*/
    return Quad;

}/*decrToQuad*/

/*------------------------------------------------------------------------*/

void reverseString(char * stri) {/*reverse the order of the characters*/

   char *start = stri;
   char *end = start + strlen(stri)-1;
   char tmp;
/*----------------------------*/
   if(stri==FLS) {
	return;
   }
/*----------------------------*/
   if(*stri==FLS) {
	return;
   }
/*----------------------------*/
   while(end>start) {
	tmp=*start;
	*start=*end;
	*end=tmp;

	start++;
	end--;
   }/*while*/
/*----------------------------*/
}/*reverseString*/

/*-------------------------------------------------------------------------*/

char * getWord(FILE *fp){/*reads a word from the file and returns it.*/

   static int flag;/*if special character appears*/
   char * word=NULL;
   int t=FLS;
   int i=FLS;
   char * Spec[] = {":","+",",","#","-","[","]","\n","\""};/*special chars*/
   int j;
   int arrlen = 9;

  word=(char *)malloc(sizeof(int));
   if(!(word)){/*executes if memory allocation fails*/
   
		printf("memory allocation failed.\n");
		exit(TRU);
	}
/*----------------------------*/
  if(flag==FLS) {
		while((t=fgetc(fp))!=EOF) {

			if(t==';') {/*comment sentence*/
				free(word);
				return ";";
			}
/*----------------------------*/
			if(isalpha(t)||isdigit(t)|| t=='.' ){/*builds a word*/
				i++;
				word=(char *)realloc(word,i*sizeof(char));
				word[i-1] = t;
			}
			else {
				if(t==':' || t==',' || t=='+' || t==']' || t=='\n' || t=='#'|| t=='[' || t=='"'|| t=='-') {/*gather special chars*/
					if(*word!=FLS) {
						myGetch(t,flag++);
						return word;
					}
					else 
						for(j=0;j<9;j++){/*returns spec chars. saves memory*/
							if(t==*Spec[j]){
							free(word);
							return Spec[j];
						}/*if*/
					}/*for*/

				}/*special characters if*/
				else if(*word==FLS){
							continue;
						}
					else {
							break;
						}/*break else*/
				}/*BIG else*/

		}/*fgetc while*/

	}/*end if flag == 0*/
   else {/*gets myGetch if the flag is on*/
	   *word = myGetch(t,flag--);
	   }
/*----------------------------*/
	for(j=0;j<arrlen;j++){/*returns spec chars*/
		if(strcmp(word,Spec[j])==FLS){
		free(word);
		return Spec[j];
		}
	}/*for*/
/*----------------------------*/
  return word;

}/*getWord*/

/*-------------------------------------------------------------------------*/

int myGetch(int  ch,int flag) {/*the buffer for special characters*/

   static int BUFFER;
   static char buf[TRU];
/*----------------------------*/
   if(flag==FLS) {/*if buffer is empty enters the character*/
		buf[BUFFER++] = ch;
    }
   else {
		return buf[--BUFFER];/*returns the character if buffer is full*/
	}
/*----------------------------*/
 return 0;
}/*mygetch*/

/*-------------------------------------------------------------------------*/

int getIC(CodePointer Chead) {/*returns the current free IC line*/
 CodePointer tmp = Chead;
/*----------------------------*/
 if(!tmp) {/*if IC is empty*/
   return 100;
 }
 while(tmp->next) {
   tmp=tmp->next;
 }/*while*/
/*----------------------------*/
return tmp->IC;/*returns the IC*/
}/*getIC*/

/*--------------------------------------------------------------------------*/

char * getString(FILE *fpr){/*returns the string inside "'s*/
  char * word = NULL;
  int i = FLS;
  char t;
  word=(char*)malloc(sizeof(int));
/*----------------------------*/
  if(!word){
	printf("Memory allocation failed.\n");
	exit(TRU);
  }
/*----------------------------*/
  while((t=fgetc(fpr))!='"'&&(t)!='\n'){
		if(t=='"' || t=='\n'){
			break;
		}
		i++;
		word=(char *)realloc(word,i*sizeof(char));
		word[i-1] = t;
	}/*while*/
/*----------------------------*/
  if((t)!='"'){
		printf("String must end with \".Program Terminates.\n");
		exit(TRU);
	}
/*----------------------------*/
 return word;

}/*getString*/

/*--------------------------------------------------------------------------*/

int getData(FILE *fpr){/*for .DATA labels. checking commas and syntax validation*/
  char * word = NULL;
  int i = FLS;
  char t;/*buffer*/
  static int COMMAFLAG=FLS;/*flag for commas excceivness*/
  int COMMAEXPECT =FLS;/*flag is on if comma is excpected*/
  static int NEWLINE = FLS;/*on if a new line char is excpected*/
  static int SIGN = FLS;/*on if a '-' or '+' appears*/
  int result = FLS;/*returns the number*/
/*----------------------------*/
  word=(char*)malloc(sizeof(int));
  if(!word){
	printf("Memory allocation failed.\n");
	exit(TRU);
  }
/*----------------------------*/
  if(NEWLINE!=FLS){/*ends session*/
    word[i]='\n';
    NEWLINE=FLS;
    SIGN = FLS;
    free(word);
    return INL;
   }
/*----------------------------*/
  while((t=fgetc(fpr))!='\n'){
		if(isdigit(t)||t=='-'||t=='+'){
			if(COMMAEXPECT!=FLS){
				NEWLINE=FLS;
				SIGN = FLS;
				return SBD2;
			}/*space between digits*/
			if(t=='-'||t=='+'){
				SIGN++;
					if(SIGN>TRU){
						SIGN = FLS;
						return NNV;
					}
			}
			COMMAFLAG=FLS;
			NEWLINE = FLS;
			i++;
			word=(char *)realloc(word,i*sizeof(char));
			word[i-1] = t;}
			else if(t==' ' || t=='\t'){
						if(*word!=FLS){/*check if there is space between digits.*/
							COMMAEXPECT++;
						}/*end if there's space between digits.*/
						continue;
					}
					else if(t==','){
								COMMAFLAG++;
								if(COMMAFLAG>1){
									NEWLINE=FLS;
									SIGN = FLS;
									return ECC2;
								}/*exccessive commas*/
								break;
							}
						else {
								NEWLINE=FLS;
								SIGN = FLS;
								return NNV;
							}/*non-numeric value*/
  }/*while*/
/*----------------------------*/
 if(COMMAFLAG!=FLS&&t=='\n') {
	 SIGN = FLS;
	 return MSD2;/*Missing data*/
  }
/*----------------------------*/
 if(t=='\n') {
   NEWLINE++;
   }
/*----------------------------*/
 result = atoi(word);
 free(word);
 if((result>MAXI||result<MINI)&&t!='\n'){/*finished checking line*/
	NEWLINE = FLS;
  }
 SIGN = FLS;
 return result;
/*----------------------------*/
}/*getData*/

/*--------------------------------------------------------------------------*/

int getIntMat(FILE *fpr){/*gets the dimentions of the matrix*/
/*----------------------------*/
  char * word = NULL;
  int i = 0;
  char t;
  int result = FLS;
  word=(char*)malloc(sizeof(int));
/*----------------------------*/
  if(!word){
		printf("Memory allocation failed.\n");
		exit(TRU);
	}
/*----------------------------*/
  while((t=fgetc(fpr))!=']'&&t!='\n'){
		if(!(isdigit(t))){
			return INL;
		}
		i++;
		word=(char *)realloc(word,i*sizeof(char));
		word[i-1] = t;
	}/*while*/
/*----------------------------*/
 result = atoi(word);
 free(word);
 return result;
}/*getIntMat*/
/*--------------------------------------------------------------------------*/
int isMat(FILE **fpr) {/*checks if next label is matrix. returns the fpr back*/
  fpos_t tmp;
  char t;
  char * c;
  FILE * fp;
  fp=*fpr;
  fgetpos(fp,&tmp);
/*----------------------------*/
  if((t=fgetc(fp))!='r'){
   fsetpos(fp,&tmp);
   return FLS;}
/*----------------------------*/
  if(((!isdigit(t=fgetc(fp))))||(t<'0'||t>'7')){/* 7 and 0 are the minimum and maximum for the register number*/
   fsetpos(fp,&tmp);
   return IRD;}
/*----------------------------*/
  if((t=fgetc(fp))!=']'){
   fsetpos(fp,&tmp);
   return MRD;}
/*----------------------------*/
   if((t=fgetc(fp))!='['){
   fsetpos(fp,&tmp);
   return MCD;}
/*----------------------------*/
  if((t=fgetc(fp))!='r'){
   fsetpos(fp,&tmp);
   return ICD;}
/*----------------------------*/
  if(((!isdigit(t=fgetc(fp))))||(t<'0'||t>'7')){/* 7 and 0 are the minimum and maximum for the register number*/
   fsetpos(fp,&tmp);
   return ICD;}
/*----------------------------*/
  if((t=fgetc(fp))!=']'){
   fsetpos(fp,&tmp);
   return MCD;}
/*----------------------------*/
   fsetpos(fp,&tmp);
   while((*(c=getWord(fp)))!=']'){;}
/*----------------------------*/
 return TRU;
}/*isMat*/

/*--------------------------------------------------------------------------*/

int isAction(char * word) {/*checks if a word is an action. returns the dec value*/
 char * ACTIONS[] = {"mov","cmp","add","sub","not",
		      "clr","lea","inc","dec","jmp","bne","red","prn","jsr",
		       "rts","stop"};
 int i;
 int arrlen = 16;
/*----------------------------*/
 for(i=0;i<arrlen;i++){
   if(strcmp(word,ACTIONS[i])==0){
	return i;
    }
  }/*for*/
/*----------------------------*/
 return INL;
}/*isAction*/

/*--------------------------------------------------------------------------*/

int isReg(char * word){/*checks if a word is a register. returns 1 if it is*/
 char * regs[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
 int i;
 int arrlen = 8;
/*----------------------------*/
 for(i=0;i<arrlen;i++){
   if(strcmp(word,regs[i])==0){
	return TRU;
   }
  }
/*----------------------------*/
return FLS;
}/*isReg*/

/*--------------------------------------------------------------------------*/ /*returns the adress of a label*/

int getAdressByLabel(SymbolPointer *Shead,char *label){/*returns the adress of the label*/

 SymbolPointer tmp = *Shead;
/*----------------------------*/
 while(tmp){
	if(strcmp(label,tmp->Label)==0){
	  return tmp->Adress;
	}
   tmp=tmp->next;
 }
/*----------------------------*/
return INL;/*returns -1 if failed*/
}/*getAdressByLabel*/

/*--------------------------------------------------------------------------*/

int isRegFuture(FILE **fpr){/*checking if there 2 regs in an order(then only 1 extra byte is needed)*/
 FILE *fp = *fpr;
 char *c;
 int i;
 char * regs[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
 int arrlen = 8;
/*----------------------------*/
 while((*(c=getWord(fp)))!=','){;}/*gets the comma*/
 c=getWord(fp);
 for(i=0;i<arrlen;i++){
	if(strcmp(c,regs[i])==0){
	 return TRU;
	}
  }/*for*/
/*----------------------------*/
return FLS;
}/*isRegFuture*/

/*------------------------------------------------------------------------*/

int isExternal(SymbolPointer *Shead,char *c){/*checks if a symbol is external*/

 SymbolPointer tmp=*Shead;
/*----------------------------*/
 while(tmp){
	if(strcmp(c,tmp->Label)==0){
	  if(tmp->External==TRU){
		return TRU;
	  }
	}
    tmp=tmp->next;
 }
/*----------------------------*/
return FLS;
}/*isExternal*/

/*------------------------------------------------------------------------*/

char * toObject(char * argv){/*prepares the output object file*/

 char * result;
 int i = 0;
 result=(char *)malloc(strlen(argv)+5);
 memset(result,'\0',sizeof(result));
/*----------------------------*/
 while((*(argv+i))!='.'){
	(*(result+i))=(*(argv+i));
  i++;
  }/*while*/
/*----------------------------*/
 (*(result+i))='.';
i++;
 (*(result+i))='o';
i++;
 (*(result+i))='b';
 return result;
}/*toObject*/

/*------------------------------------------------------------------------*/

char * toEntry(char * argv){/*prepares the entry output file*/

 char * result;
 int i = 0;
 result=(char *)malloc(strlen(argv)+5);
 memset(result,'\0',sizeof(result));
/*----------------------------*/
 while((*(argv+i))!='.'){
	(*(result+i))=(*(argv+i));
  i++;
  }/*while*/
/*----------------------------*/
 (*(result+i))='.';
i++;
 (*(result+i))='e';
i++;
 (*(result+i))='n';
i++;
 (*(result+i))='t';
 return result;
}/*toEntry*/

/*------------------------------------------------------------------------*/

char * toExternal(char * argv){/*prepares the external output file*/
/*----------------------------*/
 char * result;
 int i = 0;
 result=(char *)malloc(strlen(argv)+5);
 memset(result,'\0',sizeof(result));
/*----------------------------*/
 while((*(argv+i))!='.'){
	(*(result+i))=(*(argv+i));
  i++;
  }/*while*/
/*----------------------------*/
 (*(result+i))='.';
i++;
 (*(result+i))='e';
i++;
 (*(result+i))='x';
i++;
 (*(result+i))='t';
 return result;
}/*toExternal*/

/*------------------------------------------------------------------------*/


int printCodeLineENT(SymbolPointer Shead,FILE * fpw,char * label) {/*prints out the ENTRY file*/

   SymbolPointer tmp = Shead;
   char * adress;
/*----------------------------*/
   while(tmp!=NULL) {
		if(strcmp(tmp->Label,label)==0){
			adress=decToQuad(tmp->Adress);
			break;
		}
		tmp = tmp->next;
   }/*while*/

   if(tmp==NULL){
		printf("Error: Uknown Label '%s'",label);
		return INL;
	}
    fprintf(fpw,"%s\t%s\n",label,adress);

 return TRU;
/*----------------------------*/
}/*printCodeLineENT*/

/*------------------------------------------------------------------------*/


void printCodeLineEXT(char * label,int ic,FILE * fpw) {/*prints out the external file*/

	fprintf(fpw,"%s\t%s\n",label,decToQuad(ic));
	
}/*printCodeLineEXT*/

/*------------------------------------------------------------------------*/

int getICEXT(CodePointer * Chead){/*returns the the relevant IC*/

 CodePointer tmp = *Chead;
/*----------------------------*/
 while(tmp->Bits!=NULL){
	tmp=tmp->next;
 }/*while*/
/*----------------------------*/
 return tmp->IC;
}/*getICEXT*/

/*------------------------------------------------------------------------*/


void printCodeLineOB(CodePointer Chead,DataPointer Dhead,FILE * fpw) {/*prints out the object output*/

   CodePointer Ctmp = Chead;
   DataPointer Dtmp = Dhead;
   int factor=0;/*gets the diffirence between the data structure and the code structure*/
   fprintf(fpw,"bcc     dd\n");
/*----------------------------*/
 if(Ctmp==NULL){
	factor=100;
  }
   while(Ctmp!=NULL) {
		fprintf(fpw,"%s\t%s\n",decToQuad(Ctmp->IC),BinaryToQuad(Ctmp->Bits));
		factor=Ctmp->IC+1; 
		Ctmp = Ctmp->next;
	}/*while*/
/*----------------------------*/
   while(Dtmp!=NULL) {
		fprintf(fpw,"%s\t%s\n",decToQuad((Dtmp->DC)+factor),BinaryToQuad(Dtmp->Bits));
		Dtmp = Dtmp->next;
	}/*while*/
/*----------------------------*/
}/*printCodeLineOB*/

/*------------------------------------------------------------------------*/

int checkCommas(FILE * fpr){/*checks for commas in a line*/

 char t;
 char * c;
 int ctr=0;
 long int loc = 0;
 loc = ftell(fpr);
/*----------------------------*/
 while((t=fgetc(fpr))!='\n'){
	if(t==','){
	ctr++;
	  if((t=fgetc(fpr))==','){
	  ctr++;
	  break;
	  }
 	else ctr=0;}
   }/*while*/
/*----------------------------*/
 if(ctr>1){
    printf("Excessive commas ");
    while((*(c=getWord(fpr)))!='\n'){;}
    return INL;
   }
/*----------------------------*/
 fseek(fpr,loc,SEEK_SET);/*returns the pointer*/
 return TRU;
}/*checkCommas*/

/*------------------------------------------------------------------------*/

int checkSyntax(FILE *fpr){/*checks for invalid characters inside the text*/
  char t;
  int line = 1;
  int errors=0;
/*----------------------------*/
  while((t=fgetc(fpr))!=EOF){
	if(t=='\n'){
	 line++;
	}
	if(t==';'){/*comment*/
	   while((t=fgetc(fpr))!=EOF){
			if(t=='\n'){
				line++;
				break;
			}
		}
	}
	if(t=='"'){
		while((t=fgetc(fpr))!='"'){
			if(t=='\n'){
				printf("Missing \" in line %d\n",line);
				line++;
				errors++;
				break;
			}/*if*/
		}/*while*/
		t=fgetc(fpr);
	}/*if '"'*/
	if(((t=='-')||(t=='+'))&&isdigit(t=fgetc(fpr))){
		continue;
	}
    if(isdigit(t)||isalpha(t)||t=='.'||t==','||t=='['||t==']'||t==':'||t==';'||t=='#'||t==' '||t=='\t'||t=='\n'){
		continue;
	}
	else 
	    errors++;
	    printf("Invalid character type.t=='%c' int line:%d\n",t,line);
   }/*while*/
/*----------------------------*/
 if(errors!=0){
	rewind(fpr);
	return TRU;
 }
/*----------------------------*/
 rewind(fpr);
 return FLS;
}/*checkSyntax*/

/*------------------------------------------------------------------------*/

int isData(char *c){/*returns true of a word is data container*/

 char * data[]={".data",".string",".mat",".extern",".entry"};
 int arrlen=5;
 int i;
/*----------------------------*/
 for(i=FLS;i<arrlen;i++){
	if(strcmp(c,data[i])==FLS){
		return TRU;
	}
 }
/*----------------------------*/
return FLS;
}

/*------------------------------------------------------------------------*/

int validLabel(char * LBL,int line,FILE * fpr){/*checks if a label is valid*/

 char * c;
 char t;
/*----------------------------*/
 if(isdigit(*(LBL))){/*checks if lable starts with a digit*/
 printf("Label:'%s'",LBL);
 while((*(c=getWord(fpr)))!='\n'){;}
 return DGW;
  }
/*----------------------------*/
 if(isAction(LBL)==TRU){/*checks if lable is a reserved word*/
 printf("Label:'%s'",LBL);
 while((*(c=getWord(fpr)))!='\n'){;}
 return RSW2;
  }
/*----------------------------*/
 if(isReg(LBL)==TRU){/*checks if lable is a reserved word*/
 printf("Label:'%s' cannot be a register ",LBL);
 while((*(c=getWord(fpr)))!='\n'){;}
 return INL;
  }
/*----------------------------*/
 if(strlen(LBL)>MAXLABEL){/*checks if label is more than 30 characters*/
  printf("Label:'%s' musn't be longer than 30 characters ",LBL);
  while((*(c=getWord(fpr)))!='\n'){;}
  return INL;
 }
/*----------------------------*/	
 if(*(c=getWord(fpr))!=':') {/*IF ':' IS MISSING*/
 printf("Error: missing ':' after label '%s' ",LBL);
 while((*(c=getWord(fpr)))!='\n'){;}
 return INL;
 }
/*----------------------------*/
 if((t=fgetc(fpr))!=' '&& t!='\t'){/*checks for space or tabe after ':'*/
 printf("Missing space or tab after label:%s ",LBL);
 while((*(c=getWord(fpr)))!='\n'){;}
 return INL;
 }
/*----------------------------*/
 return TRU;
}

/*------------------------------------------------------------------------*/

/*freeing the lists*/
void freeHeads(SymbolPointer Shead,CodePointer Chead,DataPointer Dhead){

 SymbolPointer SP = Shead;
 CodePointer   CP = Chead;
 DataPointer   DP = Dhead;
/*----------------------------*/
 while(Chead!=NULL){
   CP=Chead;
   Chead=Chead->next;
   free(CP->Bits);
   free(CP);
  }
/*----------------------------*/
 while(Dhead!=NULL){
   DP=Dhead;
   Dhead=Dhead->next;
   memset(DP->Bits,FLS,10);
   free(DP);
  }
/*----------------------------*/
 while(Shead!=NULL){
   SP=Shead;
   Shead=Shead->next;
   free(SP->Label);
   free(SP);
  }
/*-------------freeheads---------------*/
}

/*------------------------------------------------------------------------*/

void CheckErrors(int num,int line) {/*Errors container*/
/*----------------------------*/
 if(num==INL){
  printf("in line %d.\n",line);
  }
 if(num==ECT){
  printf("Exccessive text in line %d.\n",line);
  }
 if(num==MSO){
  printf("Missing operand in line %d.\n",line);
  }
 if(num==IVO){
  printf("Invalid operand in line %d.\n",line);
  }
 if(num==RSW){
  printf("Invalid label name in line %d.Label cannot be a reserved word.\n",line);
  }
 if(num==IMP){
  printf("Invalid matrix parameter in line %d.\n",line);
  }
 if(num==STD){
  printf("String data in line %d must start with ' \" '.\n",line);
  }
 if(num==IOT){
  printf("Invalid command/data type in line: %d.\n",line);
  }
 if(num==MRD){
  printf("Missing row dimension in line %d.\n",line);
  }
 if(num==IRD){
  printf("Invalid row length in line %d.\n",line);
  }
 if(num==MCD){
  printf("Missing column dimension in line %d.\n",line);
  }
 if(num==ICD){
  printf("Invalid culumn length in line %d.\n",line);
  }
 if(num==TPM){
  printf("Too many paramterers for matrix in line %d.\n",line);
  }
 if(num==-SBD){
  printf("Space between parameters in data array. line : %d.\n",line);
  }
 if(num==ECC){
  printf("Exccessive commas in data array. line : %d.\n",line);
  }
 if(num==NNV){
  printf("Non-Numeric value in data array. line : %d.\n",line);
  }
 if(num==DGW){
  printf(" in line %d mustn't start with a digit.\n",line);
  }
 if(num==RSW2){
  printf(" in line %d cannot be a reseverd word.\n",line);
  }
 if(num==RCB){
  printf("Row/culumn dimension for matrix in line %d is too big.\n",line);
  }
 if(num==IST){
  printf("in line %d is already exsits.\n",line);
 }
 if(num==SBD){
  printf("Space between digits in line %d.\n",line);
 }
 if(num==WCT){
  printf("cannot have an immedaite number as destination operand in line %d.\n",line);
 }
 if(num==IVOL){
  printf("cannot have an immedaite number as source operand in line %d.\n",line);
 }
 if(num==IVOL2){
  printf("cannot have a register as source operand in line %d.\n",line);
 }
 if(num==MSD2){
  printf("Missing data after comma in line %d.\n",line);
 }

/*----------------------------*/
}

/*------------------------------------------------------------------------*/

int CheckOperand(char * action,int srcOperand,int desOperand){/*logic for operands*/

  char *Dual[]={"mov","add","sub","lea","not","clr","inc","dec","jmp","bne","red","jsr"};
  int i;

  for(i=0;i<12;i++){
		if(strcmp(action,Dual[i])==FLS&&desOperand==FLS){
			printf("Command '%s' has an invalid destination operand ",action);
			return INL;
		}
	}/*for*/
  if(strcmp(action,Dual[3])==0){
		if(srcOperand==FLS||srcOperand==REG){
		printf("Command '%s' has an invalid source operand ",action);
		return INL;
		}
	}

  return TRU;

}/*CheckOperand*/

/*------------------------------------------------------------------------*/

int CannotHaveImd(char * action){/*actions that cannot have Imd operand*/

 char *Dual[]={"mov","add","sub","lea","not","clr","inc","dec","jmp","bne","red","jsr"};
 int i;

  for(i=0;i<12;i++){
	if(strcmp(action,Dual[i])==FLS){
	  return TRU;
	}
  }/*for*/

 return FLS;

}/*CannotHaveImd*/
