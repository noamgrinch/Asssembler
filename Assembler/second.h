#ifndef SECOND_H_
#define SECOND_H_
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
/*----------------------------define prototypes----------------------------*/
#define MAXI 127
#define MINI -128
#define TRU 1
#define FLS 0
#define INL -1
#define BYTE 10
#define MAT2 2
#define REG2 3
/*-------------------------Functions prototypes---------------------------*/
int isThere(SymbolPointer *hpt,char * symbol);
char * getWord(FILE *fp);
int sortSRC(FILE **fpr,SymbolPointer *Shead);
int sortDES(FILE **fpr,SymbolPointer *Shead);
int isReg(char * word);
void MainByte(char Byte[],int opcode,int srcOperand,int desOperand,int ERA);
void addCode(CodePointer *Chead,char byte[]);
int DUALOPERANDS(FILE *fpr,CodePointer *Chead,SymbolPointer *Shead,char *c,int srcOperand,int desOperand,FILE* fpwEXT);
int SINGALOPERAND(FILE *fpr,CodePointer *Chead,SymbolPointer *Shead,char *c,int desOperand,FILE *fpwEXT);
void IntToByte(int num,char byte[]);
int getAdressByLabel(SymbolPointer *Shead,char *label);
void CodeRegs(char *reg1,char *reg2,char Byte[]);
int isRegFuture(FILE **fpr);
int isExternal(SymbolPointer *Shead,char *c);
void NOOPERANDS(CodePointer *Chead,int opcode);
void printCodeLineEXT(char * label,int ic,FILE * fpw);
int isMat(FILE **fpr);
int getICEXT(CodePointer * Chead);
int CheckOperand(char * action,int srcOperand,int desOperand);
/*-------------------------------------------------------------------------*/
#endif
