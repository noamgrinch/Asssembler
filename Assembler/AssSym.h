#ifndef ASSSYM_H_
#define ASSSYM_H_
/*-------------------------------libraries---------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/*----------------------------structs prototypes---------------------------*/
typedef struct SymbolList * SymbolPointer;
typedef struct SymbolList {
  char * Label;
  int Adress;
  int External;
  int Action;               
  SymbolPointer next;
  } SymbolList;

typedef struct CodeWriter * CodePointer;
typedef struct CodeWriter {
  char * Bits;
  int IC;              
  CodePointer next;
  } CodeWriter;

typedef struct DataList * DataPointer;
typedef struct DataList {
  char Bits[10];
  int DC;              
  DataPointer next;
  } DataList;

#define TRU 1
#define FLS 0
#define IST -20

/*--------------------------Functions prototypes----------------------------*/
int isThere(SymbolPointer *hpt,char * symbol);
int addSymbol(SymbolPointer *Shptr,char * symbol,int adress ,  int action,CodePointer Chead);
int addSymbolEXTERN(SymbolPointer *Shptr,char * label);
int getIC(CodePointer Chead);
int isAction(char * word);
int isExternal(SymbolPointer *Shead,char * label);
/*---------------------------------------------------*/


#endif
