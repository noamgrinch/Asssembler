#ifndef MISC_H_
#define MISC_H_
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
#define MAXI 511
#define MINI -512
#define SBD2 1000
#define ECC2 1001
#define NNM 1002
#define TRU 1
#define FLS 0
#define REG 3
#define MAXREGNUM 7
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
#define IST -20
#define WCT -21
#define IVOL -22
#define IVOL2 -23
#define MSD2 -24
#define MAXLABEL 30
/*--------------------------Functions prototypes----------------------------*/
void reverseString(char * str);
char * decToQuad(int dec);
int myGetch(int  ch,int flag);
char * getWord(FILE *fp);
char * BinaryToQuad(char * bits);
/*---------------------------------------------------*/


#endif
