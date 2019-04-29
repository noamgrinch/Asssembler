#include "second.h"
/*------------------------------------------------------------------------*/
char * BinaryToQuad(char * bits){/*coding into weird-base representation*/

 char * result= NULL;
 int i=9;
 int j =4;
/*----------------------*/
 result=(char *)malloc(5*sizeof(char));/*for a new word*/
 if(!result){
		printf("Memory allocation failed.\n");
		exit(FLS);
	}
/*----------------------*/
 while(i>INL){
		if((*(bits+i))=='0'){
			i--;
			if((*(bits+i))=='0'){
				(*(result+j))='a';
				i--;
				j--;
			}
			else {
					(*(result+j))='c'; i--; j--;
				}
		}/*if*/

		if((*(bits+i))=='1'){
			i--;
			if((*(bits+i))=='0'){
				(*(result+j))='b';
				i--;
				j--;}
			else {
					(*(result+j))='d'; i--; j--;
				}
		}/*if*/
	}/*while*/
/*----------------------*/
 return result;
}/*BinaryToQuad*/

/*------------------------------------------------------------------------*/

void CodeRegs(char *reg1,char *reg2,char Byte[]){/*coding registers to a byte*/
 char * regs[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
 char * regsnum[] = {"0000","0001","0010","0011","0100","0101","0110","0111"};
 int i;
 int j;
 int k;
 int regslen = 8;/*length of the array*/
 int op = 4;
/*----------------------*/
 for(i=FLS;i<regslen;i++){/*Destination operand*/
		if(strcmp(reg1,regs[i])==FLS){
			for(j=FLS;j<op;j++){
				Byte[j]=*(regsnum[i]+j);
			}
		}
	}/*for*/
/*----------------------*/
 for(i=FLS;i<regslen;i++){/*Source operand*/
		if(strcmp(reg2,regs[i])==FLS){
			for(j=op,k=FLS;j<regslen;j++,k++){
				Byte[j]=*(regsnum[i]+k);
			}/*for*/
		}
	}/*for*/
/*----------------------*/
}/*CodeRegs*/

/*------------------------------------------------------------------------*/

void addCode(CodePointer *Chead,char byte[]){/*coding into the list*/

 int bitlen = 10;
 CodePointer tmp = *Chead;
/*----------------------*/
 while(tmp->Bits!=FLS){
   tmp=tmp->next;
 }/*while*/
/*----------------------*/
 tmp->Bits=(char *)malloc(bitlen*sizeof(char));
 if(!tmp->Bits){
	printf("Memory allocation failed.\n");
	exit(FLS);
 }
 memset(tmp->Bits,'\0',(bitlen*sizeof(char)));
 strcpy(tmp->Bits,byte);
}/*addCode*/

/*------------------------------------------------------------------------*/

void IntToByte(int n,char byte[]){/*coding integers to the right byte form*/
 int i =INL;
 int dup = n;
 int bitlen = 9;
 int postERA = 7;
/*----------------------*/
   while(i!=bitlen) {
	  byte[++i]='0';
    }
   i=postERA;
/*----------------------*/
   while(n!=FLS) {
		if(n%2==FLS) {
			;
		}
		else {
			byte[i]='1';
		}
		i--;
		n=n/2;
    }
/*----------------------*/
  if(dup<FLS) {/*if number is negative*/
		for(i=postERA;i<bitlen+TRU;i--){
			if(byte[i]=='1'){
				i--;
				break;
			}
		}
		while(i>INL){
			if(byte[i]=='1'){
				byte[i]='0';
			}
			else if(byte[i]=='0'){
						byte[i]='1';
					}
				i--;
			}
	}/*negative number*/
/*----------------------*/
}/*IntToByte*/

/*--------------------------------------------------------------------------*/

int CODEONE(FILE * fpr,CodePointer * Chead,SymbolPointer *Shead,int opcode,FILE * fpw,char * action) {/*Preparing to code a dual operand line*/
 char Byte[BYTE];/*the main byte that contains the instructions*/
 int srcOperand;/*represents type of coding*/
 int desOperand;/*represents type of coding*/
 int ERA=FLS;/*ERA bits*/
 long int loc;/*used to save location in the disk*/
 char * c;
 int error=0;
 loc = ftell(fpr);
 srcOperand = sortSRC(&fpr,Shead);/*finds out what kind of operand*/
 if(srcOperand<FLS){/*checks if valid*/
	return srcOperand;
 }
 desOperand = sortDES(&fpr,Shead);/*finds out what kind of operand*/
 if(desOperand<FLS){/*checks if valid*/
	return desOperand;
 }
/*checks operand logic*/
 if((error=CheckOperand(action,srcOperand,desOperand))<FLS){
	c=getWord(fpr);/*gets \n*/
	printf("Invalid operand for '%s' command ",action);
	return INL;
 }
 fseek(fpr,loc,SEEK_SET); 
 MainByte(Byte,opcode,srcOperand,desOperand,ERA);/*coding into Byte*/
 addCode(Chead,Byte);/*adds the main order byte*/
 c=getWord(fpr);/*gets MyGetch \n*/
 if((DUALOPERANDS(fpr,Chead,Shead,c,srcOperand,desOperand,fpw))<FLS){/*coding*/
	return INL;
  }  
 return TRU;
}

/*--------------------------------------------------------------------------*/

int CODETWO(FILE * fpr,CodePointer * Chead,SymbolPointer *Shead,int opcode,FILE * fpw,char * action) {/*preparing to code in signal operand line*/
 char Byte[BYTE];/*the main byte that contains the instructions*/
 int desOperand;/*represents type of coding into the operand*/
 int ERA=FLS;/*ERA bits*/
 long int loc;/*location in the disk*/
 char * c;
 int error = 0;
 loc = ftell(fpr);
 desOperand = sortSRC(&fpr,Shead);/*finds out what kind of operand*/
 if(desOperand<FLS){/*checks if valid*/
	return desOperand;
 }
 if((error=CheckOperand(action,BYTE,desOperand))<FLS){/*checks operand logic*/
	c=getWord(fpr);/*gets the \n*/
	printf("Invalid operand for command '%s' ",action);
	return INL;
 }
 fseek(fpr,loc,SEEK_SET); 
 MainByte(Byte,opcode,FLS,desOperand,ERA);/*coding into Byte*/ 
 addCode(Chead,Byte);/*adds the main order byte*/
 c=getWord(fpr);/*gets MyGetch \n*/
 if((*c)=='\n'){/*if actually \nis returnd*/
	c=getWord(fpr);
 }/*gets source operand*/
 if((SINGALOPERAND(fpr,Chead,Shead,c,desOperand,fpw))<FLS){/*coding method*/
	return INL;
 }
 return TRU;
}

/*--------------------------------------------------------------------------*/

void CODETHREE(FILE * fpr,CodePointer * Chead,SymbolPointer *Shead,int opcode) {/*coding a method that has no operands*/

 NOOPERANDS(Chead,opcode);/*rts and stop commands*/
 
}

/*--------------------------------------------------------------------------*/

int sortSRC(FILE **fpr,SymbolPointer *Shead){/*finds out what kind of operand to be coded*/
 FILE *fp = *fpr;
 char *c;
 char * buf;
/*----------------------*/
 if((*(c=getWord(fp)))=='#'){
		if((*(c=getWord(fp)))=='-'){
			c=getWord(fp);
		}
		return FLS;
	}
/*----------------------*/
 if(isThere(Shead,c)==TRU){
		if((*(buf=getWord(fp)))=='['){
			if(isMat(&fp)){
				return MAT2;
			}
		}
		return TRU;
	}
/*----------------------*/
 if(isReg(c)==TRU){
	return REG2;
  }
/*----------------------*/
 if((*(c))=='\n'){
    printf("Missing operand in a single operand order.\n");
    exit(FLS);
  }
/*----------------------*/
printf("Error: Unknown Variable: '%s' ",c);/*Errors*/
return INL;
}

/*-------------------------------------------------------------------------*/

int sortDES(FILE **fpr,SymbolPointer *Shead){/*finds out what kind of operand to be coded*/
 FILE *fp = *fpr;
 char *c;
/*----------------------*/
 while((*(c))!=','){
	c=getWord(fp);}/*gets the the comma*/
/*----------------------*/
 if((*(c=getWord(fp)))=='#'){/*gets the operand*/
	return FLS;
  }
/*----------------------*/
 if(isThere(Shead,c)==TRU){
		if(isMat(&fp)){
			return MAT2;
		}
		return TRU;
	}
/*----------------------*/
 if(isReg(c)==TRU){
	return REG2;
  }
/*----------------------*/
printf("Error: Unknown Variable: '%s' ",c);/*Error*/
return INL;
}

/*--------------------------------------------------------------------------*/

void MainByte(char Byte[],int opcode,int srcOperand,int desOperand,int ERA){/*codes the mainbyte*/
 int i;
 int j;
 char *opc[]={"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
 char *opr[]={"00","01","10","11"};
 char * era[]={"00","01","10"};
/*----------------------*/
 for(i=FLS,j=FLS;i<4;i++,j++){
  Byte[i]=*(opc[opcode]+i);
 }
/*----------------------*/
 for(i=4,j=FLS;i<6;i++,j++){
  Byte[i]=*(opr[srcOperand]+j);
 }
/*----------------------*/
 for(i=6,j=FLS;i<8;i++,j++){
  Byte[i]=*(opr[desOperand]+j);
 }
/*----------------------*/
 for(i=8,j=FLS;i<BYTE;i++,j++){
  Byte[i]=*(era[ERA]+j);
 }
/*----------------------*/
}

/*--------------------------------------------------------------------------*/

int DUALOPERANDS(FILE *fpr,CodePointer *Chead,SymbolPointer *Shead,char *c,int srcOperand,int desOperand,FILE *fpw){/*coding dual operands actions*/
 enum type {IMDT = 0, STRT = 1,MAT = 2,REG = 3};
 int tmp=0; /*tmp represents the adress of the label*/
 char * reg1=NULL;
 char * reg2=NULL;
 char Byte[BYTE];/*the byte string to coded*/
 int doubleregflag=FLS;/*turns on only if 2 operands are regs*/
 int loc;/*for fseek ftell commands*/
 switch (srcOperand){
/*----------------------*/
  case IMDT:/*case of #. immediate numbers*/
    c=getWord(fpr);/*gets the number*/
    if(*(c)=='#'){
		c=getWord(fpr);
	}
    tmp=atoi(c);
    if((*(c))=='-'){
		tmp = (FLS-atoi(c=getWord(fpr)));
	}
    if((*(c))=='+'){/*negative number*/
		tmp = atoi(c=getWord(fpr));
    }
    if(tmp>MAXI||tmp<MINI){/*checks for size*/
		printf("Error. Number %d is too big to represent in 8 bits ",tmp);
		return INL;
	}
    IntToByte(tmp,Byte);/*transforms the number into a byte*/
    addCode(Chead,Byte);/*coding into the list*/
    c=getWord(fpr);/*gets the comma*/
    break;
/*----------------------*/
 case STRT:/*Variable case*/
    if((*(c))=='\n'){
		c=getWord(fpr);
    }/*gets the operand*/
    if(isExternal(Shead,c)==TRU){/*ouput*/
		printCodeLineEXT(c,getICEXT(Chead),fpw);
	}
    tmp=getAdressByLabel(Shead,c);/*gets the adress*/
    IntToByte(tmp,Byte);/*coding the adress to byte*/
    if(isExternal(Shead,c)==TRU){
		Byte[8]='0'; Byte[9]='1';
	}/*codes ERA*/
    else{
		Byte[8]='1'; Byte[9]='0';
	}/*codes ERA*/
    addCode(Chead,Byte);/*adding code the the list*/
    c=getWord(fpr);/*gets the comma*/
    break;
/*----------------------*/
 case MAT:
    if((*c)=='\n'){/*gets the operand*/
		c=getWord(fpr);
	}
    tmp=getAdressByLabel(Shead,c);/*gets the adress of the matrix*/
    IntToByte(tmp,Byte);/*transforms the  adress to byte*/
    Byte[8]='1'; Byte[9]='0';/*ERA*/
    addCode(Chead,Byte);/*adding code to the list*/
    c=getWord(fpr);/*gets '['*/
    c=getWord(fpr);/*gets first register*/
    reg1=c;/*the register in the row*/
    while((*(c=getWord(fpr)))!='['){;}
    c=getWord(fpr);/*gets the culumn register*/
    reg2=c;/*culumn register*/
    CodeRegs(reg1,reg2,Byte);/*coding regs into a byte*/
    Byte[8]='0'; Byte[9]='0';/*ERA*/
    addCode(Chead,Byte);/*coding into the list*/
    c=getWord(fpr);/*gets the ']'*/
    c=getWord(fpr);/*gets the comma*/
    break;
/*----------------------*/
 case REG:
	if((*(c))=='\n'){
		c=getWord(fpr);
	}/*gets the register*/
    Byte[8]='0'; Byte[9]='0';/*ERA*/
    reg1 = c;/*source register*/
    loc=ftell(fpr);/*saves place in the disk*/
    if(isRegFuture(&fpr)==TRU){/*checks if 2nd operand is register*/
    	fseek(fpr,loc,SEEK_SET);
        c=getWord(fpr);/*get the comma*/
		if(isReg(c)==FLS){
			c=getWord(fpr);
		}/*gets 2nd reggister*/
    	reg2=c;
    	CodeRegs(reg1,reg2,Byte);
    	addCode(Chead,Byte);
		doubleregflag++;/*saves a byte*/
    	break;
	}/*end if*/
    else/*in case the second operand is not a register*/
        fseek(fpr,loc,SEEK_SET);/*returns to the 2nd operand location*/
		c=getWord(fpr);
		if((*(c))=='\n'){
			c=getWord(fpr);
		}
		reg2="r0";
    	CodeRegs(reg1,reg2,Byte);/*coding into the list*/
    	addCode(Chead,Byte);
		break;
 }/*switch*/
/*----------------------*/
if(doubleregflag!=FLS){
	return TRU;
}
/*----------------------*/
 switch (desOperand){/*Destination operand*/
/*----------------------*/
  case IMDT:/*Imidiate number*/
    if((*c)!='#'){/*gets the #*/
		c=getWord(fpr);
    }
    c=getWord(fpr);/*gets the number*/
    if((*(c))=='-'){/*if negative*/
		tmp = (FLS-atoi(c=getWord(fpr)));
	}
    else{
		tmp=atoi(c);
	}
    if((*(c))=='+'){/*negative number*/
		tmp = atoi(c=getWord(fpr));
    }
    
    if(tmp>MAXI||tmp<MINI){/*checks for maximum and minimum numbers to be coded*/
		printf("Error. Number %d is too big to represent in 8 bits ",tmp);
		return INL;
	}
    IntToByte(tmp,Byte);/*coding the number into a byte*/
    addCode(Chead,Byte);/*coding into the list*/
    break;
/*----------------------*/
 case STRT:
    if((*(c))==','){
		c=getWord(fpr);
	}/*gets the label*/
    if(isExternal(Shead,c)==TRU){/*Checks of external to write in output*/
		printCodeLineEXT(c,getICEXT(Chead),fpw);
	}
    tmp=getAdressByLabel(Shead,c);/*get the adress*/
    IntToByte(tmp,Byte);/*coding the adress into a byte*/
    if(isExternal(Shead,c)==TRU){/*ERA*/
		Byte[8]='0';
		Byte[9]='1';
    }
    else{
		Byte[8]='1';
		Byte[9]='0';
	}
    addCode(Chead,Byte);/*coding into the list*/
    break;
/*----------------------*/
 case MAT: /*matrix case*/
    c=getWord(fpr);/*gets the label*/
    tmp=getAdressByLabel(Shead,c);/*gets the adress*/
    IntToByte(tmp,Byte);/*coding the adress into a byte*/
    Byte[8]='1'; Byte[9]='0';/*ERA*/
    addCode(Chead,Byte);/*coding into the list*/
    c=getWord(fpr);/*gets '[' from myGetch*/
    c=getWord(fpr);/*gets first register*/
    reg1=c;/*gets the row register*/
    while((*(c=getWord(fpr)))!='['){;}
    c=getWord(fpr);/*gets column register*/
    reg2=c;
    CodeRegs(reg1,reg2,Byte);/*coding regs into a byte*/
    Byte[8]='0'; Byte[9]='0';/*ERA*/
    addCode(Chead,Byte);/*coding into the list*/
    c=getWord(fpr);/*gets the ']'*/
    break;
/*----------------------*/
 case REG:/*register case*/
    c=getWord(fpr);/*gets the register*/
    Byte[8]='0'; Byte[9]='0';/*ERA*/
    reg2=c;
    reg1 = "r0";/*the non-relevaent bits*/
    CodeRegs(reg1,reg2,Byte);/*coding regiter into the byte*/
    addCode(Chead,Byte);/*coding into the list*/
    break;
 } 
/*----------------------*/
 return TRU;
}

/*--------------------------------------------------------------------------*/

int SINGALOPERAND(FILE *fpr,CodePointer *Chead,SymbolPointer *Shead,char *c,int desOperand,FILE * fpw){/*singal operand coding method*/

 enum type {IMDT = 0, STRT = 1,MAT = 2,REG = 3};
 int tmp;
 char * reg1=NULL;
 char * reg2=NULL;
 char Byte[BYTE];

 switch (desOperand){
/*----------------------*/
  case IMDT:/*Immidiate number*/
    if((*c)=='#'){
		c=getWord(fpr);
    }
    tmp=atoi(c);
    if((*(c))=='-'){/*negative number*/
		tmp = (FLS-atoi(c=getWord(fpr)));
    }
    if((*(c))=='+'){/*negative number*/
		tmp = atoi(c=getWord(fpr));
    }
    if(tmp>MAXI||tmp<MINI){/*checks for maximum and minimum numbers to be coded*/
		printf("Error. Number %d is too big to represent in 8 bits ",tmp);
		return INL;
	}
    IntToByte(tmp,Byte);/*coding the number into the byte*/
    addCode(Chead,Byte);/*coding into the list*/
    break;
/*----------------------*/
 case STRT:/*label case*/
    if(isExternal(Shead,c)==TRU){/*Checks if external to write in output*/
		printCodeLineEXT(c,getICEXT(Chead),fpw);
	}
    tmp=getAdressByLabel(Shead,c);/*gets the label's adress*/
    IntToByte(tmp,Byte);/*coding the adress into the byte*/
    if(isExternal(Shead,c)==TRU){/*ERA*/
		Byte[8]='0';
		Byte[9]='1';
     }
    else{
		Byte[8]='1';
		Byte[9]='0';
	}
    addCode(Chead,Byte);/*coding into the list*/
    break;
/*----------------------*/
 case MAT:/*case matrix*/
    if((*c)=='\n'){
       c=getWord(fpr);
    }/*gets the label*/
    tmp=getAdressByLabel(Shead,c);/*gets the adress*/
    IntToByte(tmp,Byte);/*coding the adress into the byte*/
    Byte[8]='1'; Byte[9]='0';/*ERA*/
    addCode(Chead,Byte);/*coding into the list*/
    c=getWord(fpr);/*gets '['*/
    c=getWord(fpr);/*gets first register*/
    reg1=c;/*row dimendion register*/
    while((*(c=getWord(fpr)))!='['){;}
    c=getWord(fpr);/*gets second register*/
    reg2=c;/*culumn dimention register*/
    CodeRegs(reg1,reg2,Byte);/*coding the register into the byte*/
    Byte[8]='0'; Byte[9]='0';/*ERA*/
    addCode(Chead,Byte);/*coding into the list*/
    c=getWord(fpr);/*gets the ']'*/
    break;
/*----------------------*/
 case REG:/*register case*/
    Byte[8]='0'; Byte[9]='0';/*ERA*/
    reg2 = c;/*gets the register*/
    reg1="r0";/*Non-relevant register*/
    CodeRegs(reg1,reg2,Byte);/*coding register into the byte*/
    addCode(Chead,Byte);/*coding into the list*/
    break;
 }
/*----------------------*/
 return TRU;
}

/*-------------------------------------------------------------------------*/

void NOOPERANDS(CodePointer *Chead,int opcode){/*codes rts and stop commands*/
 int rts = 15;
 if(opcode==rts){/*RTS*/
		addCode(Chead,"1111000000");
		return;
	}
 else /*STOP*/
   addCode(Chead,"1110000000");
 return;
}
/*-------------------------------------------------------------------------*/