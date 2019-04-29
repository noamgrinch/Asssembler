#include "first.h"
/*-------------------------------------------------------------------------*/
int ValidOperand(char * op){/*checks if Operand is valid*/

  if(((*(op)))=='#'){
	return TRU;/*ok*/
  }
/*------------------------*/
  if(((*(op)))==','){
	return IVO;/*invalid*/
  }
/*------------------------*/
  if(!isalpha((*(op)))){
	return IVO;/*invalid*/
  }
/*------------------------*/
  if(isAction(op)==TRU){
	return RSW;/*revserved word*/
  }
/*------------------------*/
return TRU;
}

/*-------------------------------------------------------------------------*/

void addCodeLine(CodePointer *Chead){/*INCREASING THE IC. not coding*/

   
   CodePointer t;
   CodePointer p1 = *Chead;
   CodePointer p2;
   
   t=(CodePointer)malloc(sizeof(CodeWriter)); /*memory allocation for a new item*/
   t->Bits=NULL;
/*------------------------*/
   if(!t){
		printf("\n memory allocation failed \n");
		exit(FLS);
	}

/*------------------------*/
	if(*Chead==NULL){  /*executes the node is the first to the list*/
		*Chead=t;
		t->IC = BEG;
		return;
    }
/*------------------------*/
   while(p1!=NULL){
         p2=p1;
         p1=p1->next;
    }/*while*/
/*------------------------*/  
   t->IC=p2->IC+TRU;
   p2->next = t;
 }/*addCodeLine*/
 
/*------------------------------------------------------------------------*/

int actionSort(FILE *fpr , char * label,char * TYPE,CodePointer Chead,SymbolPointer *Shptr,int lines) {/*sotring method for command lines*/
 char * ActionOne[] = {"mov","cmp","add","sub","lea"};
 char * ActionTwo[] = {"not","clr","inc","dec","jmp","bne","red","prn","jsr"};
 char * ActionThree[] = {"rts","stop"};
 int i = 0;
 int ic;
 char * c;/*word to be held*/
 int arrone = 5;/*lengths of the arrays*/
 int arrtwo = 8;
 int arrthr = 2;
 int error = FLS;
 ic = getIC(Chead)+TRU;
/*------------------------*/
   for(i=FLS;i<arrone;i++) {/*dual operands method*/
		if(strcmp(ActionOne[i],TYPE)==FLS) {
			lines = ActionOneMethod(fpr,TYPE,label,Chead,lines);
			error = addSymbol(Shptr,label,ic,TRU,Chead);
			if(error<FLS){
				return error;
			}
        }
    }/*for*/
/*------------------------*/
   for(i=FLS;i<arrtwo;i++) {/*signal operand method*/
		if(strcmp(ActionTwo[i],TYPE)==FLS) {
		lines=ActionTwoMethod(fpr,TYPE,label,Chead,lines);
		error = addSymbol(Shptr,label,ic,TRU,Chead);
			if(error<FLS){
				return error;
			}
        }
    }/*for*/
/*------------------------*/
   for(i=FLS;i<arrthr;i++) {/*non operand method*/
		if(strcmp(ActionThree[i],TYPE)==FLS) {
			error = addSymbol(Shptr,label,ic,TRU,Chead);
			if(error<FLS){
				return error;
			}
			if((*(c=getWord(fpr)))!='\n'){
				while((*(c=getWord(fpr)))!='\n'){;}
				return ECT;/*excesseive text*/
			}
			return TRU;/*non operand method worth is 1 byte*/
        }
    }/*for*/
/*------------------------*/
  return lines;/*lines to be coded*/
}

/*-------------------------------------------------------------------------*/

int ActionOneMethod(FILE *fpr,char * action,char * label,CodePointer Chead,int lines){/*dual operands(LABLED) calculating method.*/
  char * RGSTS[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
  char * c;/*word to be held*/
  char * buf;/*backup*/
  char lea[] = {"lea"};/*action that have specific operands*/
  int i;
  int matval;
  int error;
  int counter = 0;/*registers counter*/
  int commactr = 0;/*comma counter*/
  int arrlen = 8;
  lines = 3;

/*-------------*/
 if((commactr=checkCommas(fpr))==INL){/*commas check*/
	return INL;
  }
/*-------------*/
 if((error=ValidOperand(c=getWord(fpr)))<=FLS){/*c gets the operand*/	
	while((*(c))!='\n'){
		c=getWord(fpr);
	}
	return error;/*invalid operand*/
  }
/*-------------*/
 if((*(c))=='#'){/*Immed number case*/
	if(strcmp(lea,action)==0){
	   while((*(c=getWord(fpr)))!='\n'){;}
	   printf("Action:'%s' ",action);
	   return IVOL;
	}

	if((*(c=getWord(fpr)))=='-'||(*(c))=='+'){
		if(!isdigit(*(buf=getWord(fpr)))){
			while((*(c=getWord(fpr)))!='\n'){;}
			return IVO;/*invalid operand*/
		}
		if((*c)=='-'){
			if((error=INL*atoi(buf))<MINI){
				while((*(c=getWord(fpr)))!='\n'){;}
				printf("number %d is too big to represent in 8 bits ",error);
				return INL;
			}
		}
		else if((error=atoi(c))>MAXI){
				while((*(c=getWord(fpr)))!='\n'){;}
				printf("number %d is too big to represent in 8 bits ",error);
				return INL;
				}

	}
	else if(!isdigit(*c)){
				while((*(c=getWord(fpr)))!='\n'){;}
				return IVO;/*invalid operand*/
			}
		else if((error=atoi(c))>MAXI){
				while((*(c=getWord(fpr)))!='\n'){;}
				printf("number %d is too big to represent in 8 bits ",error);
				return INL;
			}
	}/*Immed number*/
/*-------------*/
   for(i=FLS;i<arrlen;i++){/*register check*/
		if(strcmp(c,RGSTS[i])==FLS) {
			if(strcmp(lea,action)==0){
				while((*(c=getWord(fpr)))!='\n'){;}
				printf("Action:'%s' ",action);
				return IVOL2;
			}
			counter++;
		}
    }/*for*/
/*-------------*/
    if(*(buf=getWord(fpr))=='['){/*matrix case*/
		if((matval=isMat(&fpr))==TRU){
			lines++;
			while((*(c=getWord(fpr)))!=']'){;}
			buf=getWord(fpr);
		}
		if(matval==INL){
			while((*(c=getWord(fpr)))!='\n'){;}
			return IMP;/*invalid matrix parameter*/
		}
    }/*matrix case*/
/*-------------*/
  if(*(buf)=='\n'){
	return MSO;/*Missing operand*/
   }
  if(*(buf)!=','){
	while((*(c))!='\n'){
		c=getWord(fpr);
	}
	return MSO;/*Missing operand*/
  }
/*-------------*/
    c=getWord(fpr);/*gets 2nd label*/
/*-------------*/
   if((*(c))=='#'){/*Immed number case*/
		if(CannotHaveImd(action)==TRU){/*checks if valid operand*/
			while((*(c))!='\n'){
				c=getWord(fpr);
			}
			printf("Commnad:'%s' ",action);
			return WCT;/*wrong catagoriziation*/
		}

		if((*(c=getWord(fpr)))=='-'||(*(c))=='+'){
			if(!isdigit(*(buf=getWord(fpr)))){
				return IVO;/*invalid operand*/
			}
			if((*c)=='-'){
				if((error=INL*atoi(buf))<MINI){
					while((*(c=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
				}
			}
			else if((error=atoi(c))>MAXI){
					while((*(c=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
		     		}
		}
		else if(!isdigit(*c)){
				return IVO;/*invalid operand*/
			}
		     if((error=atoi(c))>MAXI){
				while((*(c=getWord(fpr)))!='\n'){;}
				printf("number %d is too big to represent in 8 bits ",error);
				return INL;
				}
	}/*Immed number case*/
/*-------------*/
    if(*(buf=getWord(fpr))=='['){/*matrix case*/
		if((matval=isMat(&fpr))==TRU){
			lines++;
			while((*(c=getWord(fpr)))!=']'){;}
			buf=getWord(fpr);
		}
		if(matval==INL){
			while((*(c=getWord(fpr)))!='\n'){;}
			return IMP;/*invalid matrix parameter*/
		}
    }/*matrix case*/
/*-------------*/
    for(i=FLS;i<arrlen;i++) {/*regs check*/
	    if(strcmp(c,RGSTS[i])==FLS){
	      counter++;
	    }
        i++;
    }/*for*/
/*-------------*/
   if(*(buf)!='\n'){
        while((*(c=getWord(fpr)))!='\n'){;}
		return ECT;/*Exccessve text*/
	}
/*-------------*/
   if(counter==2){/*executes only of 2 registers are the two operands*/
		lines=2;
	}

return lines;/*amount of lines to be added to the IC*/
}

/*--------------------------------------------------------------------------*/

int ActionTwoMethod(FILE *fpr,char * action,char * label,CodePointer Chead,int lines){/*Singal operand action sortig method(LABELD)*/
  char * RGSTS[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
  char * c;/*main word*/
  char * buf;/*buffer*/
  int matval;/*value of matrix*/
  int i;
  int arrlen = 8;
  int error = 0;
  lines=TRU;
/*-------------*/
  c=getWord(fpr);/*gets operand*/
/*-------------*/
  if(ValidOperand(c)<FLS){
		while((*(c))!='\n'){c=getWord(fpr);}
		return IVO;/*invalid operand*/
	}
/*-------------*/
  if((*(c))=='\n'){
		return MSO;/*Missing operand*/
	}
/*-------------*/
  if((*(buf=getWord(fpr)))=='['){/*matrix case*/
		if((matval=isMat(&fpr))==TRU){/*matrix case*/
			lines++;
			while((*(c=getWord(fpr)))!=']'){;}
			buf=getWord(fpr);
		}
		if(matval==INL){
			while((*(c=getWord(fpr)))!='\n'){;}
			return IMP;/*invalid MAtrix paramter*/
		}
    }/*matrix case*/
/*-------------*/
  for(i=FLS;i<arrlen;i++) {/*checks for registers*/
		if(strcmp(c,RGSTS[i])==FLS) {
			return lines;
		}
    }
/*-------------*/
  if((*(c))=='#'){/*Immed number case*/
		if(CannotHaveImd(action)==TRU){/*checks if valid operand*/
			while((*(c))!='\n'){
				c=getWord(fpr);
			}
			printf("Commnad:'%s' ",action);
			return WCT;/*wrong catagoriziation*/
		}
		if((*(buf))=='-'||(*(buf))=='+'){
			if(!isdigit(*(c=getWord(fpr)))){
				return IVO;/*invalid operand*/
			}
			if((*buf)=='-'){
				if((error=INL*atoi(c))<MINI){
					while((*(c=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
				}
		     }
			else if((error=atoi(c))>MAXI){
						while((*(c=getWord(fpr)))!='\n'){;}
						printf("number %d is too big to represent in 8 bits ",error);
						return INL;
		    		}
		  }/*bif if buf='-' or '+'*/
		else if(!isdigit((*buf))){
					return IVO;/*invalid operand*/
				}
			if((error=atoi(buf))>MAXI){
				while((*(c=getWord(fpr)))!='\n'){;}
				printf("number %d is too big to represent in 8 bits ",error);
				return INL;
		     }
			 buf=getWord(fpr);
  }/*Immed number case*/
/*-------------*/
 if((*(buf))!='\n'){
	while((*(c=getWord(fpr)))!='\n'){;}
	return ECT;/*exccesive text*/
  }
 
return ++lines;/*returns the amount of lines to be added to the IC*/
}

/*--------------------------------------------------------------------------*/

int orderLineSort(FILE *fpr,char *action){/*main sotring method for command lines(NOT LABELD)*/
 char * ActionOne[] = {"mov","cmp","add","sub","lea"};
 char * ActionTwo[] = {"not","clr","inc","dec","jmp","bne","red","prn","jsr"};
 char * ActionThree[] = {"rts","stop"};
 int i = 0;
 char * c;
 int Lines = 0;
 int lenone = 5;/*lengths of the arrays*/
 int lentwo = 8;
 int lenthr = 2;
/*-------------*/
   for(i=FLS;i<lenone;i++) {/*sends to dual operands sotring method*/
		if(strcmp(ActionOne[i],action)==FLS) {
			Lines = actionOneOrder(fpr,action);
		}
    }
/*-------------*/
   for(i=FLS;i<lentwo;i++) {/*sends to signal operand sorting method*/
		if(strcmp(ActionTwo[i],action)==FLS) {
	          Lines = actionTwoOrder(fpr,action);
        }
    }
/*-------------*/
   for(i=FLS;i<lenthr;i++) {/*returns 1 line to be coded if executes*/
		if(strcmp(ActionThree[i],action)==FLS) {
			if((*(c=getWord(fpr)))!='\n'){
				while((*(c=getWord(fpr)))!='\n'){;}
				return ECT;/*exccesive text*/
			}
			return TRU;
        }
    }
/*-------------*/
 return Lines;/*returns the amount of lines to be added to the IC*/
}

/*--------------------------------------------------------------------------*/

int actionOneOrder(FILE *fpr,char * action){/*calculates how many lines to be added to the IC. DUAL operands commands(NOT LABELD)*/
 char * regs[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
 char lea[] = {"lea"};
 int arrlen=8;
 char * tmp;
 char * buf;/*gets the '[' for the matrix in the buffer*/
 int Lines=3;
 int errors = FLS;/*error number*/
 int counter = FLS;/*counting regs. when hits 2 it means  2 regs to be coded*/
 int i;
 int commactr = FLS;/*checks of commas*/
 int error = 0;
/*-------------*/
 if((commactr=checkCommas(fpr))==-1){
		return INL;/*missing commas*/
  }
/*-------------*/
 tmp = getWord(fpr);/*Gets the operand*/
/*-------------*/
 if((*tmp)=='\n'){/*checks if opernad is missing*/
		return MSO;
 }
/*-------------*/
 if((*tmp)==':'){
		while((*(tmp=getWord(fpr)))!='\n'){;}
		return RSW;/*invalid operand reserved word*/
  }
/*-------------*/
 if(ValidOperand(tmp)==FLS){
		while((*(tmp=getWord(fpr)))!='\n'){;}
		return IVO;/*invalid operand*/
	}
/*-------------*/
  for(i=FLS;i<arrlen;i++) {/*checks for registers*/
		if(strcmp(tmp,regs[i])==FLS) {
			if(strcmp(lea,action)==0){
	   		    while((*(tmp=getWord(fpr)))!='\n'){;}
	   		    printf("Action:'%s' ",action);
	   		    return IVOL2;
			}
			counter++;
	    }
    }/*for*/
/*-------------*/
  if((*(buf=getWord(fpr)))=='['){/*matrix case*/
      if((errors = isMat(&fpr))==TRU){
			Lines++;
			while((*(tmp=getWord(fpr)))!=']'){;}
			buf=getWord(fpr);
		}
      if(errors<FLS){
			while((*(tmp=getWord(fpr)))!='\n'){;}
			return errors;
		}
    }/*matrix if*/
/*-------------*/
  if((*tmp)=='#'){/*Immed number case*/
		if(strcmp(lea,action)==0){/*Invalid operands for lea command*/
			while((*(tmp=getWord(fpr)))!='\n'){;}
			printf("Action:'%s' ",action);
			return IVOL;/*Invalid operand*/
		}
		if((*buf)=='-'||(*(buf))=='+'){/*deals with # and negative numbers*/
			tmp=getWord(fpr);
		 	if((*buf)=='-'){
				if((error=INL*atoi(tmp))<MINI){
					while((*(tmp=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
		     		}
			}/*small if buf =='-'*/
			else if((error=atoi(tmp))>MAXI){
					while((*(tmp=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
		     		}
		}/*negative buf*/
		else if((error=atoi(buf))>MAXI){
				while((*(tmp=getWord(fpr)))!='\n'){;}
				printf("number %d is too big to represent in 8 bits ",error);
				return INL;
		     	}
		if(!isdigit(*(buf))&&!isdigit(*(tmp))){/*checks if operand is valid*/
			while((*(tmp))!='\n'){
				tmp=getWord(fpr);
			}
			return IVO;/*Invalid operand*/
		}
		tmp=getWord(fpr);/*gets the comma*/
	}/*Immed number case*/
/*-------------*/
  while((*(buf))!=','&&(*(tmp))!=','){/*gets comma*/
		if((*(tmp))=='\n'){
			Lines = MSO;
			return Lines;
		}/*missing operand*/
	}/*while*/
/*-------------*/
  if((*(tmp=getWord(fpr)))=='\n'){/*Gets the label*/
		Lines = MSO;/*missing operand*/
			return Lines;
	}
/*-------------*/
  if(ValidOperand(tmp)==FLS){
		while((*(tmp=getWord(fpr)))!='\n'){;}
		return IVO;/*invalid operand*/
	}
/*-------------*/
 if((*(buf=getWord(fpr)))=='['){/*matrix case*/	
	if((errors = isMat(&fpr))==TRU){/*matrix case*/
		Lines++;
	}
	if(errors<FLS){/*executes if error is made*/
		while((*(tmp=getWord(fpr)))!='\n'){;}
		return errors;
	}
 }
/*-------------*/
  for(i=FLS;i<arrlen;i++) {/*checks for registers*/
		if(strcmp(tmp,regs[i])==FLS) {
			counter++;
	    }
	}/*for*/
/*-------------*/
  if((*(tmp))=='#'){/*Immed numbers case*/
		if(CannotHaveImd(action)==TRU){/*checks if valid operand*/
			while((*(tmp))!='\n'){
				tmp=getWord(fpr);
			}
			printf("Commnad:'%s' ",action);
			return WCT;/*wrong catagoriziation*/
		}
		if((*(buf))=='-'||(*(buf))=='+'){
			tmp=getWord(fpr);
			if((*buf)=='-'){
				if((error=INL*atoi(tmp))<MINI){
					while((*(tmp=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
		     	}
			}
			else if((error=atoi(tmp))>MAXI){
						while((*(tmp=getWord(fpr)))!='\n'){;}
						printf("number %d is too big to represent in 8 bits ",error);
						return INL;
		     		}
		}
		else if((error=atoi(buf))>MAXI){
					while((*(tmp=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
		     	}
		if(!isdigit(*(buf))&&!isdigit(*tmp)){
			while((*(tmp))!='\n'){
				tmp=getWord(fpr);
			}
			return IVO;
		}
		tmp=getWord(fpr);

	}/*Immed number base*/
/*-------------*/
  if((*buf)!='\n'&&(*tmp)!='\n'){/*checks for errors*/
		if((*(buf))=='['){
			while((*(tmp=getWord(fpr)))!=']'){;}
			if((*(tmp=getWord(fpr)))=='\n'){;}
	        else {
				while((*(tmp=getWord(fpr)))!='\n'){;}
				Lines = ECT;
				}  /*exccesive text*/
		}/*buf ='[' if*/
		else {
				Lines = ECT;/*exccesive text*/
				while((*(tmp=getWord(fpr)))!='\n'){;}
			}/*else*/
	}/*if*/
/*-------------*/
 if(counter==2&&Lines<=3&&Lines>FLS){Lines = 2;}/* two regs case*/
/*-------------*/

 return Lines;/*lines to be coded*/
}

/*-------------------------------------------------------------------------*/

int actionTwoOrder(FILE *fpr,char * action){/*Singal operand actions.calculates how many lines to be added to the IC(without label)*/
  char * RGSTS[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
  char * c;/*main word*/
  char * buf;/*buffer*/
  int matval;/*value of matrix*/
  int i;
  int arrlen = 8;
  int error = 0;
  int lines=TRU;
/*-------------*/
  c=getWord(fpr);/*gets operand*/
/*-------------*/
  if(ValidOperand(c)<FLS){
		while((*(c))!='\n'){c=getWord(fpr);}
		return IVO;/*invalid operand*/
	}
/*-------------*/
  if((*(c))=='\n'){
		return MSO;/*Missing operand*/
	}
/*-------------*/
  if((*(buf=getWord(fpr)))=='['){/*matrix case*/
		if((matval=isMat(&fpr))==TRU){/*matrix case*/
			lines++;
			while((*(c=getWord(fpr)))!=']'){;}
			buf=getWord(fpr);
		}
		if(matval==INL){
			while((*(c=getWord(fpr)))!='\n'){;}
			return IMP;/*invalid MAtrix paramter*/
		}
    }/*matrix case*/
/*-------------*/
  for(i=FLS;i<arrlen;i++) {/*checks for registers*/
		if(strcmp(c,RGSTS[i])==FLS) {
			lines = lines;
		}
    }
/*-------------*/
  if((*(c))=='#'){/*Immed number case*/
		if(CannotHaveImd(action)==TRU){/*checks if valid operand*/
			while((*(c))!='\n'){
				c=getWord(fpr);
			}
			printf("Commnad:'%s' ",action);
			return WCT;/*wrong catagoriziation*/
		}
		if((*(buf))=='-'||(*(buf))=='+'){
			if(!isdigit(*(c=getWord(fpr)))){
				return IVO;/*invalid operand*/
			}
			if((*buf)=='-'){
				if((error=INL*atoi(c))<MINI){
					while((*(c=getWord(fpr)))!='\n'){;}
					printf("number %d is too big to represent in 8 bits ",error);
					return INL;
				}
		     }
			else if((error=atoi(c))>MAXI){
						while((*(c=getWord(fpr)))!='\n'){;}
						printf("number %d is too big to represent in 8 bits ",error);
						return INL;
		    		}
		  }/*bif if buf='-' or '+'*/
		else if(!isdigit((*buf))){
					return IVO;/*invalid operand*/
				}
			if((error=atoi(buf))>MAXI){
				while((*(c=getWord(fpr)))!='\n'){;}
				printf("number %d is too big to represent in 8 bits ",error);
				return INL;
		     }
			 buf=getWord(fpr);
  }/*Immed number case*/
/*-------------*/
 if((*(buf))!='\n'){
	while((*(c=getWord(fpr)))!='\n'){;}
	return ECT;/*exccesive text*/
  }
 
return ++lines;/*returns the amount of lines to be added to the IC*/
}
/*------------------------------------------------------------------------*/
