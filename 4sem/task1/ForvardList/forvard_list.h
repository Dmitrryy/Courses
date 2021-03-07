#pragma once


#include <stdlib.h>


struct forward_list_t;
struct flIt_t;


typedef struct forward_list_t* forward_list;
typedef struct flIt_t* flIt;


typedef int fl_key_type;
typedef int fl_val_type;


forward_list flCreate();
void         flDestroy(forward_list fl);


fl_val_type flFront(forward_list fl);

int flEmpty(forward_list fl);

void flPushFront(forward_list fl, fl_key_type key, fl_val_type val);
void flPopFront (forward_list fl);

void erase_after (flIt it);
void insert_after(flIt it, fl_key_type key, fl_val_type val);


flIt flBegin(forward_list fl);

void        flItNext (flIt it);
fl_val_type flItKey  (flIt it);
fl_val_type flItVal  (flIt it);
int         flItIsEnd(flIt it);

void flItDestroy(flIt it);