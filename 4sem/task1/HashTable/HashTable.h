#pragma once


#include <stdlib.h>

//type of key in table
#define HS_KEY_TYPE int
//type of value in table
#define HS_VAL_TYPE int


struct Hash_t;
struct htIt_t;

//Hash table type
typedef struct Hash_t* Hash;
//Hash Table iterator type
typedef struct htIt_t* hsIt;


//create hash table
Hash hsCreate (size_t size);
//destroy hash table
void hsDestroy(Hash ht);


/*
 * add or set new value with key
 *
 * params:
 *  ht - hash table
 *  key - key in table
 *  val - new value
 */
void insert    (Hash ht, HS_KEY_TYPE key, HS_VAL_TYPE val);
/*
 * get value hidden under the key.
 *
 * params:
 *  ht - hash table;
 *  key - key in table
 *
 * return:
 *  value with 'key' id
 */
HS_VAL_TYPE get(Hash ht, HS_KEY_TYPE key);


void hsSetHashFunc(Hash ht, uint(*hashCompute)(HS_KEY_TYPE key)); //todo
void hsSetDefaultHashFunc(Hash ht); //todo


hsIt hsBegin(Hash ht); //todo
hsIt hsEnd  (Hash ht); //todo

int hsItIsEnd  (hsIt it); //todo
int hsItIsBegin(hsIt it); //todo

void hsItNext(hsIt it); //todo
void hsItPrev(hsIt it); //todo

HS_KEY_TYPE hsItKey(hsIt it); //todo
HS_VAL_TYPE hsItVal(hsIt it); //todo