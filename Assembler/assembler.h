#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_
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
#define TRU 1
#define FLS 0
#define INL -1
#define DUAL 3
#define DUAL2 6
#define SIGNAL 13
#define MINARGC 2

/*--------------------------Functions prototypes----------------------------*/
char * getWord(FILE *fp);
int addSymbolEXTERN(SymbolPointer *Shptr,char * label);
void addCodeLine(CodePointer *Chead);
void printCodeLineOB(CodePointer Chead,DataPointer Dhead,FILE * FPW);
int actionSort(FILE *fpr , char * label,char * TYPE,CodePointer Chead,SymbolPointer *Shptr,int lines);
int printCodeLineENT(SymbolPointer Shead,FILE * fpw,char * label);
void printCodeLine(CodePointer Chead);
int checkSyntax(FILE *fpr);
int validLabel(char * LBL,int line,FILE * fpr);
void freeHeads(SymbolPointer Shead,CodePointer Chead,DataPointer Dhead);
int CODEONE(FILE * fpr,CodePointer * Chead,SymbolPointer *Shead,int opcode,FILE * fpwEXT,char * action);
int CODETWO(FILE * fpr,CodePointer * Chead,SymbolPointer *Shead,int opcode,FILE *fpwEXT,char * action);
void CODETHREE(FILE * fpr,CodePointer * Chead,SymbolPointer *Shead,int opcode);
char * toObject(char * argv);
char * toEntry(char * argv);
char * toExternal(char * argv);
int isAction(char * word);
int orderLineSort(FILE *fpr,char *action);
int dataSort(FILE *fpr,char * LBL,char * type,DataPointer *Dhead,SymbolPointer *Shead,CodePointer Chead);
void upDateSymbol(SymbolPointer *Shead,CodePointer * Chead);
int isAction(char * word);
int dataWithoutLabel(FILE *fpr,char *TYPE,DataPointer *Dhead);
int isData(char * c);
void CheckErrors(int num,int line);
int isThere(SymbolPointer * Shead,char *c);
int isExternal(SymbolPointer * Shead,char *c);
/*---------------------------------------------------*/


#endif
