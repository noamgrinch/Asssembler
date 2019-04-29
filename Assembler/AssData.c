#include "AssData.h"
/*------------------------------------------------------------------------------------------------------------*/
void addData(DataPointer *Dhptr,int n) {/*Adding data to the list*/

   DataPointer t;
   DataPointer p1 = *Dhptr;
   DataPointer p2;
   
   t=(DataPointer)malloc(sizeof(DataList)); /*memory allocation for a new item*/
/*----------------------*/
   if(!t) {
	printf("\n memory allocation failed \n");
	exit(FLS);
	  }
/*----------------------*/
   if(*Dhptr==NULL){  /*executes the node is the first to the list*/
      *Dhptr=t;
       t->DC  = FLS;
       intToBinary(n,*Dhptr);
    }
/*----------------------*/
   else {/*adds data to the list and codes it to a byte*/
      while(p1!=NULL){
         p2=p1;
         p1=p1->next;
        }

      t->DC=p2->DC+TRU;
      p2->next = t;
      intToBinary(n,*Dhptr);
    }/*else*/
 }/*addData*/
 
/*------------------------------------------------------------------------------------------------------------*/

void intToBinary(int n,DataPointer hptr) {/*transfering int to binary rep*/

 DataPointer tmp = hptr;
 int i=INL;
 int dup;/*used to check if number is negative*/
 dup = n;
/*----------------------*/
 while(tmp->next!=NULL) {
	tmp = tmp->next;
 }
/*----------------------*/
 while(i!=9) {/*initialize the string to 0's*/
	tmp->Bits[++i]='0';
 }
/*----------------------*/
 while(n!=FLS) {/*check for 1's*/
	if(n%2==FLS) {
	  ;}
	else {
	  tmp->Bits[i]='1';
	}
  i--;
  n=n/2;
 }
/*----------------------*/
 i=9;
 if(dup<FLS){/*if number is negative*/
  while(i>INL){
	if(tmp->Bits[i]=='1'){
		i--;
		break;
	}
    i--;
  }

 while(i>INL){
	if(tmp->Bits[i]=='1'){
	   tmp->Bits[i]='0';
	}
	else {tmp->Bits[i]='1';}
	     
    i--;
  }
}
  
}/*int to binary*/

/*------------------------------------------------------------------------------------------------------------*/

int dataSort(FILE *fpr,char * LBL,char * type,DataPointer *Dhead,SymbolPointer *Shead,CodePointer Chead){/*main data sotring method*/

 char *DATA[] = {".data",".string",".mat",".extern",".entry"};
 int status;/*positive if line is ok*/
 int error;
/*----------------------*/
 if(strcmp(DATA[FLS],type)==FLS) {/*deals with Data arrays*/
   error=addSymbol(Shead,LBL,getDC(Dhead)+TRU,FLS,Chead);
   status=DataLabel(fpr,LBL,Dhead);
   if(error<FLS){
	return error;
   }
   return status;
  }
/*----------------------*/
 if(strcmp(DATA[TRU],type)==FLS) {/*deals with Strings*/
   error=addSymbol(Shead,LBL,getDC(Dhead)+TRU,FLS,Chead);
   status=StringLabel(fpr,LBL,Dhead);
   if(error<FLS){
	return error;
   }
   return status;
  }
/*----------------------*/
 if(strcmp(DATA[MATR],type)==FLS) {/*deals with Matrixs*/
   error=addSymbol(Shead,LBL,getDC(Dhead)+TRU,FLS,Chead);
   status=MatLabel(fpr,LBL,Dhead);
   if(error<FLS){
	return error;
   }
   return status;
  }
/*----------------------*/
 if(strcmp(DATA[EXTE],type)==FLS) {/*deals with External labels*/
   status=ExtLabel(fpr,LBL,Shead);
   return status;
  }
/*----------------------*/
 if(strcmp(DATA[ENTR],type)==FLS) {/*deals with Entry labels*/
   status=EntLabel(fpr,LBL);
   return status;
  }
/*----------------------*/
 printf("Invalid command/data '%s' type after label: %s.\n",type,LBL);
 return IOT; /*Error*/
}/*dataSort*/

/*------------------------------------------------------------------------------------------------------------*/

int StringLabel(FILE *fpr,char * LBL,DataPointer *Dhead){/*coding string*/
  char * c =NULL;
  int i=FLS;
/*----------------------*/
  if(*(c=getWord(fpr))!='"'){/*checks for "'s*/
	while((*(c=getWord(fpr)))!='\n'){;}
	return STD;
  }
/*----------------------*/
  c=getString(fpr);/*gets the string as a whole. this is NOT getWord*/
  while(*(c+i)!=FLS){/*coding the data*/
    addData(Dhead,(*(c+i)));
    i++;
  }/*while*/
/*----------------------*/
  addData(Dhead,FLS);/*end of string*/
  while((*(c=getWord(fpr)))!='\n'){
	if((*(c))!='\n'){
		while((*(c=getWord(fpr)))!='\n'){;}
		return ECT;}
  }
/*----------------------*/
 return TRU;

}/*StringLabel*/

/*------------------------------------------------------------------------------------------------------------*/

int DataLabel(FILE *fpr,char *LBL,DataPointer *Dhead){/*coding data*/

  int tmp;
  int tmp2;
  char * c;
/*----------------------*/
  while(((tmp=getData(fpr)))!=INL){
 	tmp2=tmp;
/*----------------------*/
	if(tmp==SBD2){/*Errors waterfall*/
	   while((*(c=getWord(fpr)))!='\n'){;}
	   return SBD;/*space between digits*/
	}
	if(tmp==ECC2){
	   while((*(c=getWord(fpr)))!='\n'){;}
	   return ECC;/*exccesive commas*/
	}
	if(tmp==NNM||tmp==NNV){
	   while((*(c=getWord(fpr)))!='\n'){;}
	   return NNV;/*non-numeric */
	}
	if(tmp==MSD2){/*missing data*/
	   return MSD2;
	}
	if(tmp>MAXI||tmp<MINI){/*if number is too big*/
	 if((tmp2=getData(fpr))==INL){
	 printf("Error. Number %d is too big to represent in 10 bits ",tmp);
	 return INL;
	 }
	 while((*(c=getWord(fpr)))!='\n'){;}
	 printf("Error. Number %d is too big to represent in 10 bits ",tmp);
	 return INL;
	}

	addData(Dhead,tmp);/*coding if succesful*/
   }/*while*/
/*----------------------*/
 return FLS;
}/*DataLabel*/

/*------------------------------------------------------------------------------------------------------------*/

int MatLabel(FILE *fpr,char *LBL,DataPointer *Dhead){/*matrix coding*/
  
  char * c; /*buffer*/
  int row = FLS; /*row dimension*/
  int column = FLS;/*column dimension*/
  int total = FLS;/*sum of the dimentions*/
  int tmp = FLS;/*the number to be added INTO the matrix*/
/*----------------------*/
  if((*(c=getWord(fpr)))!='['){/*error*/
    while((*(c))!='\n'){c=getWord(fpr);}
    return MRD;/*missing row dimension*/
   }
/*----------------------*/
  row=getIntMat(fpr);/*gets row dimention*/
/*----------------------*/
  if(row>MAXI){/*errors waterfall*/
	while((*(c=getWord(fpr)))!='\n'){;}
	return RCB;/*number is too big*/
  }
  if(row<FLS){
	while((*(c=getWord(fpr)))!='\n'){;}
	return IRD;/*error code invalid number*/
  }
/*----------------------*/
  if((*(c=getWord(fpr)))!='['){
	while((*(c))!='\n'){c=getWord(fpr);}
	return MCD;/*missing culumn dimension*/
    }
/*----------------------*/
  column=getIntMat(fpr);/*gets culumn dimention*/
/*----------------------*/
 if(column>MAXI){/*errors waterfall*/
	while((*(c=getWord(fpr)))!='\n'){;}
	return RCB;
  }/*number is too big*/
  if(column<FLS){
	while((*(c=getWord(fpr)))!='\n'){;}
	return ICD;/*error code invalid number*/
  }
/*----------------------*/
  total = (row*column);
/*----------------------*/
  while(((tmp=getData(fpr)))!=INL){/*coding data*/
/*----------------------*/
	if(tmp==SBD2){/*errors waterfall*/
	   while((*(c=getWord(fpr)))!='\n'){;}
	   return SBD;/*space between digits*/
	}
	if(tmp==ECC2){
	   while((*(c=getWord(fpr)))!='\n'){;}
	   return ECC;/*exccesive commas*/
	}
	if(tmp==NNV){
	   while((*(c=getWord(fpr)))!='\n'){;}
	   return NNV;/*Non-numeric value*/
	}
	if(tmp==MSD2){
	   return MSD2;/*missing data*/
	}
	if(tmp>MAXI||tmp<MINI){
	 while((*(c=getWord(fpr)))!='\n'){;}
	 printf("Error. Number %d is too big to represent in 10 bits ",tmp);
	 return INL;/*invalid*/
	}
/*----------------------*/
	addData(Dhead,tmp);/*coding if succeful*/
	total--;
   }/*while*/
/*----------------------*/
  if(total<FLS) {/*error*/
   return TPM;/*too many parameters*/
   }
/*----------------------*/
  if(total>FLS){
     while(total!=FLS) {
	addData(Dhead,FLS);/*puts zero in uninitiallized cells*/
	total--;
     }
   }
/*----------------------*/
 return TRU;
}/*matLabel*/

/*------------------------------------------------------------------------------------------------------------*/

int getDC(DataPointer *Dhead) {/*returns the current last USED DC*/
  DataPointer tmp = *Dhead;
  if(tmp==NULL) return INL;
  while(tmp->next!=NULL){
	tmp=tmp->next;
   }/*while*/

 return tmp->DC;
}/*getDC*/

/*------------------------------------------------------------------------------------------------------------*/

int dataWithoutLabel(FILE *fpr,char *TYPE,DataPointer *Dhead){/*coding data that has no label*/

 char *DATA[] = {".data",".string",".mat"};
 int MAT = 2;
 int status;
/*----------------------*/
 if(strcmp(DATA[FLS],TYPE)==FLS) {/*coding data*/
   status = DataLabel(fpr,TYPE,Dhead);
   return status;
  }
/*----------------------*/
 if(strcmp(DATA[TRU],TYPE)==FLS) {/*coding strings*/
   status = StringLabel(fpr,TYPE,Dhead);
   return status;
  }
/*----------------------*/
 if(strcmp(DATA[MAT],TYPE)==FLS) {/*codig matrixs*/
   status = MatLabel(fpr,TYPE,Dhead);
   return status;
  }
/*----------------------*/
 return IOT;
}/*dataWithoutLabel*/

/*------------------------------------------------------------------------------------------------------------*/

int EntLabel(FILE *fpr , char * LBL){/*deals with entry labels, also checking if the label is valid*/

 char * c;
 int error = 0;

 c=getWord(fpr);/*gets the entry label*/
 if((error=ValidOperand(c))<FLS){/*checks if operand is valid*/
	while((*(c=getWord(fpr)))!='\n'){;}
	return error;
 }
 if((*(c))=='\n'){/*checks if lable is missing*/
	return MSO;
 }
 if((*(c=getWord(fpr)))!='\n'){/*checks if there is exccesive text*/
	while((*(c=getWord(fpr)))!='\n'){;}
	return ECT;
 }
 printf("Warning: label '%s' containing entry command.\n",LBL);/*prints out a warning*/
 return TRU;
}/*EntLabel*/

/*------------------------------------------------------------------------------------------------------------*/

int ExtLabel(FILE *fpr,char *LBL,SymbolPointer *Shead){/*deals with external labels, also checks if label is valid*/

 char * c;
 int error;

 c=getWord(fpr);/*gets the extern label*/
 if((error=ValidOperand(c))<FLS){/*checks if operand is valid*/
	while((*(c=getWord(fpr)))!='\n'){;}
	return error;
 }
 addSymbolEXTERN(Shead,c);
 if((*(c))=='\n'){/*checks if lable is missing*/
	return MSO;
 }
 if((*(c=getWord(fpr)))!='\n'){/*checks if there is exccesive text*/
	while((*(c=getWord(fpr)))!='\n'){;}
	return ECT;
 }

 printf("Warning: label '%s' containing extern command.\n",LBL);
 return TRU;
}/*ExtLabel*/
/*------------------------------------------------------------------------------------------------------------*/
