#ifndef FIRST_H_
#define FIRST_H_
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
#define BEG 100
#define TRU 1
#define FLS 0
#define INL -1
#define ECT -2
#define MSO -3
#define IVO -4
#define RSW -5
#define IMP -6
#define STD -7
#define IOT -8
#define MRD -9
#define IRD -10
#define MCD -11
#define ICD -12
#define TPM -13
#define SBD -14
#define ECC -15
#define NNV -16
#define DGW -17
#define RSW2 -18
#define RCB -19
#define WCT -21
#define IVOL -22
#define IVOL2 -23
#define MAXI 127
#define MINI -128
/*-------------------------Functions prototypes----------------------------*/
int addSymbol(SymbolPointer *Shptr,char * symbol,int adress,int action,CodePointer Chead); 
char * getWord(FILE *fp);
int ActionOneMethod(FILE *fpr,char * action,char * label,CodePointer Chead,int lines);
int ActionTwoMethod(FILE *fpr,char * action,char * label,CodePointer Chead,int lines);
int getIC(CodePointer Chead);
int isMat(FILE **fpr);
int actionOneOrder(FILE *fpr,char * action);
int actionTwoOrder(FILE *fpr,char * action);
int checkCommas(FILE * fpr);
int getData(FILE *fpr);
int isAction(char * c);
int CannotHaveImd(char * action);
/*-------------------------------------------------------------------------*/
#endif
