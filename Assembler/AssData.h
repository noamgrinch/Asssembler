#ifndef ASSDATA_H_
#define ASSDATA_H_
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
#define MATR 2
#define EXTE 3
#define ENTR 4
#define MAXI 511
#define MINI -512
#define SBD2 1000
#define ECC2 1001
#define NNM 1002
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
#define MSD2 -24
/*-------------------------Functions prototypes----------------------------*/
int addSymbol(SymbolPointer *Shptr,char * symbol,int adress ,  int action,CodePointer Chead); 
void addData(DataPointer *Dhptr,int n);
char * getWord(FILE *fp);
void intToBinary(int n,DataPointer Dhptr);
int StringLabel(FILE *fpr,char * LBL,DataPointer *Dhead);
char * getString(FILE *fpr);
int getData(FILE *fpr);
int DataLabel(FILE *fpr,char *LBL,DataPointer *Dhead);
int MatLabel(FILE *fpr,char *LBL,DataPointer *Dhead);
int getIntMat(FILE *fpr);
int getDC(DataPointer *Dhead);
int ValidOperand(char *c);
int EntLabel(FILE *fpr, char * LBL);
int ExtLabel(FILE *fpr,char *LBL,SymbolPointer *Shead);
void addSymbolEXTERN(SymbolPointer *Shptr,char * symbol);
/*------------------------------------------------------------------------*/
#endif
