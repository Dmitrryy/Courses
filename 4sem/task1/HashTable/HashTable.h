/*************************************************************************
 *
 *   HashTable.h
 *
 *   Created by dmitry
 *   10.02.2021
 *
 ***/


#pragma once


#include <stdlib.h>


struct Hash_t;
struct htIt_t;


//Hash table type
typedef struct Hash_t* Hash;
//Hash Table iterator type
typedef struct htIt_t* hsIt;

//type of key in table
typedef int hs_key_type;
//type of value in table
typedef int hs_val_type;



//create hash table
Hash hsCreate (size_t size);
//destroy hash table
void hsDestroy(Hash ht);


size_t hsSize(Hash ht);

/*
 * add or set new value with key
 *
 * params:
 *  ht - hash table
 *  key - key in table
 *  val - new value
 */
void hsInsert    (Hash ht, hs_key_type key, hs_val_type val);//todo
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
hs_val_type hsGet(Hash ht, hs_key_type key);//todo


void hsSetHashFunc       (Hash ht, uint(*hashCompute)(void* key, size_t size, uint max));
void hsSetDefaultHashFunc(Hash ht);


hsIt hsBegin(Hash ht); //todo

int hsItIsEnd  (hsIt it); //todo

void hsItNext(hsIt it); //todo

hs_key_type hsItKey(hsIt it); //todo
hs_val_type hsItVal(hsIt it); //todo

void hsItDestroy(hsIt it);