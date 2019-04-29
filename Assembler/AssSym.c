#include "AssSym.h"
/*--------------------------------------------------------------------------*/
int addSymbol(SymbolPointer *Shptr,char * symbol,int adress , int action,CodePointer Chead) {/*adding symbol(label) to the list*/

   SymbolPointer t;
   SymbolPointer p1 = *Shptr;
   SymbolPointer p2;
   int ctr = FLS;
/*----------------------*/
   if(isThere(Shptr,symbol)==TRU) {
		printf("Label:'%s' ",symbol);
		return IST;
    } /*checks if number exsits in the list already*/
/*----------------------*/
   t=(SymbolPointer)malloc(sizeof(SymbolList)); /*memory allocation for a new item*/

   if(!t) {
		printf("\n memory allocation failed \n");
		exit(FLS);
    }
/*----------------------*/
   t->Label=symbol;
   t->External = FLS;
   t->Action = action;
/*----------------------*/
   if(*Shptr==NULL){  /*executes the node is the first to the list*/
		*Shptr=t;
		if(action==TRU){
			t->Adress = getIC(Chead);
		}
		else{
			t->Adress=FLS;
			}
		return TRU;
    }
/*----------------------*/
	t->Adress = adress;
		while(p1!=NULL){
			if(p1->Action==TRU){
				ctr++;
			}/*checks if only extern are in the list*/
			p2=p1;
			p1=p1->next;
		}/*while*/
		if(ctr!=FLS){  
			t->Adress = adress;
		}
		else{
			t->Adress=getIC(Chead);
			}
		p2->next = t;
   return TRU;
 }/*addSymbol*/  
 
/*--------------------------------------------------------------------*/

int isThere(SymbolPointer *hpt,char * symbol) {/*returns true of a label exsits in the list*/

  SymbolPointer tmp;
  tmp = *hpt;
/*----------------------*/
  while(tmp) {
	 if(strcmp(tmp->Label,symbol) == FLS) {
		return TRU;
	  }
	else
		tmp = tmp->next;
    }/*while*/
/*----------------------*/
return FLS;
} /*isThere*/

/*---------------------------------------------------------------------*/

int addSymbolEXTERN(SymbolPointer *Shptr,char * label) {/*external labels*/

   SymbolPointer t;
   SymbolPointer p1 = *Shptr;
   SymbolPointer p2;

   if(isThere(Shptr,label)==TRU&&isExternal(Shptr,label)==0) {/*checks if number exsits in the list already*/
		printf("Label:'%s ",label);
		return IST;
	} 

   t=(SymbolPointer)malloc(sizeof(SymbolList)); /*memory allocation for a new item*/

   if(!t){
		printf("\n memory allocation failed \n");
		exit(FLS);
	 }

   t->Label=label;
   t->Adress = FLS;
   t->External = TRU;
   t->Action = FLS;

   if(*Shptr==NULL){  /*executes the node is the first to the list*/
		*Shptr=t;
		return TRU;
    }
    while(p1!=NULL){
         p2=p1;
         p1=p1->next;
    }/*while*/
	p2->next = t;
 return TRU;
 }/*addSymbolEXTERNAL*/
 
/*---------------------------------------------------------------------*/

void upDateSymbol(SymbolPointer *Shead,CodePointer * Chead){/*updates adress of symbols after first wave*/

  SymbolPointer Stmp = *Shead;
  CodePointer Ctmp = *Chead;
  int FACTOR;
/*----------------------------*/
 if(Ctmp!=NULL){
		while(Ctmp->next!=NULL){
			Ctmp=Ctmp->next;
		}/*while*/
	}
/*----------------------------*/
  if(Ctmp!=NULL){/*the diffirence*/
		FACTOR = Ctmp->IC+TRU;
	}
  else{
		FACTOR=100;/*if no commands have been made*/
    }
/*----------------------------*/
 while(Stmp!=NULL){/*updates*/
	if((Stmp->Action)==FLS&&(Stmp->External)==FLS){
	   Stmp->Adress+=FACTOR;
	}
  Stmp=Stmp->next;
 }/*while*/

}/*upDateSymbol*/
/*-------------------------------------------------------------------------*/
