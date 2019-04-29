#include "assembler.h"
/*------------------------------------------------------------------------*/
int main(int argc, char * argv[]) {
 SymbolPointer Shead = NULL;
 DataPointer Dhead = NULL;
 CodePointer Chead = NULL;
 FILE *fpr;
 FILE *fpwOB;
 FILE *fpwEXT;
 FILE *fpwENT;
 char * Object=NULL;
 char * External=NULL;
 char * Entry = NULL;
 int i=1;
 int Lines = 0;/*Lines to put inside IC*/
 int line = 1;/*current line being checked*/
 int Errors1 = 0;
 int Errors2 = 0;
 char * c;
 char * LBL;
 int OPCode = 0;
/*----------------------------------------*/
 if(argc<MINARGC) {
 printf("Not enough arguments.Program terminates.\n");
 exit(FLS);
 }
/*----------------------------------------*/
 while(i<=argc-TRU) {/*The big loop. moves on to the next file if needed*/
 Errors1=0;/*errors on the first wave*/
 Errors2=0;/*errors on the second wave*/

  fpr=fopen(argv[i],"r");/*opens file*/

  if(!fpr) {/*check if pointer is null*/
   printf("Couldn't open file.Program terminates.\n");
   exit(FLS);
   }
/*----------------------------------------*/
Errors1 = FLS;
 if(checkSyntax(fpr)){/*checks for invalid characters*/
	Errors1++;
  }
line = TRU;
/******************************FIRST WAVE**********************************/
   Shead = NULL;
   Dhead = NULL;
   Chead = NULL;
/*----------------------------------------*/
  while((feof(fpr))==FLS){/*c returns \n*/
  Lines = FLS;/*sets Lines to zero*/
/*----------------------------------------*/
   while(*(c=getWord(fpr))!=FLS&&(*c)!='\t'){/*gets the first word*/
	/*printf("c == %s\tline ==%d\n",c,line);*/
/*----------------------------------------*/
	/*printf("line==%d\tword==%s\n",line,c);*/
	if((*c)=='\n'){/*Empty line*/
	  line++;
	  break;
	}
/*-----------------------------COMMENT----------------------------------*/
   if((*c)==';'){/*comment*/
	while((*(c=getWord(fpr)))!='\n'){;}
	line++;
	break;
   }
/*-------------------------------ENTRY----------------------------------*/
   if(strcmp(c,".entry")==FLS){/*comment/entry else num' 1*/
	  if((*(c=getWord(fpr)))=='\n'){
		 printf("Missing label after entry in line %d\n",line);
		 Errors1++;
		 line++;
		 break;
	    }
	    if(isThere(&Shead,c)==TRU&&isExternal(&Shead,c)==TRU){/*checks if the variable is valid*/
		 printf("Variable '%s' in line %d is already declared as external.\n",c,line);
		 Errors1++;
	    }
		if((*(c=getWord(fpr)))!='\n'){
		 printf("Exccessive text in line %d.\n",line);
		 Errors1++;
		 while((*(c=getWord(fpr)))!='\n'){;}
	    }
		
	  line++;
	  break;
    }/*if*/
/*-------------------------------EXTERN---------------------------------*/
    else if(strcmp(c,".extern")==FLS) { /*deals with .EXTERN else number 2*/
	        if((*(c=getWord(fpr)))=='\n'){
		        printf("Missing label after extern in line %d\n",line);
		        Errors1++;
		        break;
			}
	        Lines = addSymbolEXTERN(&Shead,c);
	        if(Lines<FLS){/*check for errors*/
		        CheckErrors(Lines,line);
		        while((*(c))!='\n'){c=getWord(fpr);}
		        Errors1++;
		        line++;
		        break;
	        }
	        if((*(c=getWord(fpr)))!='\n'){
		        printf("Exccessive text in line %d.\n",line);
		        Errors1++;
		        while((*(c=getWord(fpr)))!='\n'){;}
		    }
	      line++;
	      break;
	     }/*.EXTERN if*/
/*-----------------------------ACTION-----------------------------------*/
	    else 
               if(isAction(c)!=INL){/*checks if order line*/
					Lines = orderLineSort(fpr,c);
					if(Lines<FLS){/*check for errors*/
						CheckErrors(Lines,line);
						Errors1++;
						line++;
						break;
					}
					line++;
					while(Lines!=FLS) {
						addCodeLine(&Chead);
						Lines--;
					}
					break;
				}/*isAction if*/
/*--------------GETS HERE ONLY IF IT'S A DATA WITHOUT A LABEL -------------*/
				if(isData(c)!=FLS){/*deals with data instructions that do not have a label*/
					Lines = dataWithoutLabel(fpr,c,&Dhead);
					if(Lines<FLS){/*check for errors*/
						CheckErrors(Lines,line);
						Errors1++;
						line++;
						break;
					}
					line++;
					break;
				}
/*-----------------------GETS HERE ONLY IF IT'S A LABEL -------------------*/
				LBL = c;
				if((Lines=validLabel(LBL,line,fpr))<FLS){/*checks if label is valid*/
					CheckErrors(Lines,line);
					Errors1++;
					line++;
					break;
				}
				else
					c=getWord(fpr);/*ACTION/DATANAME*/
					if(isAction(c)!=INL){/*Deals with action scenario*/
						Lines=actionSort(fpr,LBL,c,Chead,&Shead,Lines);
						if(Lines<FLS){/*checks for errors*/
							CheckErrors(Lines,line);
							Errors1++;
							line++;
							break;
						}
						line++;
						while(Lines!=FLS) {
							addCodeLine(&Chead);
							Lines--;
						}
						break;
					}/*isAction if*/
/*----------------------------DATA label status----------------------------*/
						else {
								Lines=dataSort(fpr,LBL,c,&Dhead,&Shead,Chead);
								if(Lines<FLS){
									CheckErrors(Lines,line);
									Errors1++;
									line++;
									break;
								}
								line++;
								break;
							}/*final else*/

		}/*general getWord while*/
	}/*feof(fpr) while*/ 
/****************************FIRST WAVE*******************************/
/*--------------------------------------------------------------------------*/
/****************************SYMBOL UPDATE*******************************/
/*~~~~~~~~~~~~~~~~~~~~*/upDateSymbol(&Shead,&Chead);/*~~~~~~~~~~~~~~~~~~~~~*/
/****************************SYMBOL UPDATE*******************************/
/*--------------------------------------------------------------------------*/
/*****************************SECOND WAVE*********************************/
/*------------------------------------------------------------------------*/
if(Errors1==FLS){/*Does not continue to the second wave if errors occurred in the first phase*/
rewind(fpr);
Entry=toEntry(argv[i]);/*getting entry file pointer*/
fpwENT=fopen(Entry,"w");
  if(!fpwENT){
	printf("Couldn't write entry output file.\n");
	exit(TRU);
   }
External=toExternal(argv[i]);/*getting external file pointer*/
fpwEXT=fopen(External,"w");
  if(!fpwEXT){
	printf("Couldn't write external output file.\n");
	exit(TRU);
   }
 Errors2 = 0;/*preparing for next phase*/
 Lines = 0;
 line = 1;
/*----------------------------------------*/
 while((feof(fpr))==FLS){/*c returns \n*/
   if((OPCode=isAction((c=getWord(fpr))))!=INL){/*gets the action*/
/*----------------------------------------*/
    if(OPCode<=DUAL || OPCode == DUAL2){
	Lines=CODEONE(fpr,&Chead,&Shead,OPCode,fpwEXT,c);/*Coding method*/
        if(Lines<FLS){/*checks for errors*/
			printf(" in line %d\n",line);
			Errors2++;
		}
	}
/*----------------------------------------*/
    else if(OPCode<=SIGNAL){
	   Lines=CODETWO(fpr,&Chead,&Shead,OPCode,fpwEXT,c);/*Coding method*/
     	   if(Lines<FLS){/*checks for errors*/
				printf(" in line %d\n",line);
				Errors2++;
			}
	
		}
/*----------------------------------------*/
		else CODETHREE(fpr,&Chead,&Shead,OPCode);}/*Coding method*/
/*----------------------------------------*/
	else if(strcmp(c,".entry")==FLS){/*entry output*/
			c=getWord(fpr);/*gets the label*/
			Lines=printCodeLineENT(Shead,fpwENT,c);
			if(Lines<FLS){/*checks for errors*/
				printf(" in line %d\n",line);
				Errors2++;
			}
	    }
/*----------------------------------------*/
         else {
				if((*c)==';'){
					while((*(c=getWord(fpr)))!='\n'){;}
				}
				if((*c)=='\n'){
					line++;
				}
			}

 }/*second wave while*/
/****************************SECOND WAVE******************************/
   Object=toObject(argv[i]);/*Getting object file pointer*/
   fpwOB=fopen(Object,"w");
   if(!fpwOB){
	printf("Couldn't write object output file.\n");
	exit(TRU);
   }
/*----------------------------------------*/
  if(Errors2==FLS){
   printCodeLineOB(Chead,Dhead,fpwOB);
   }/*prints the output*/
}/*if Errors!=0 if*/
/*---------------------------preparing for next file-----------------------*/
 i++;/*next file*/

}/*main while*/
freeHeads(Shead,Chead,Dhead);/*freeing heads*/
return TRU;
}
