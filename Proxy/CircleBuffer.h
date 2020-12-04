#ifndef MIPT_S3_LUNEV_CIRCLEBUFFER_H
#define MIPT_S3_LUNEV_CIRCLEBUFFER_H

#include <stdlib.h>

struct  CircleBuffer_t;
typedef struct CircleBuffer_t* CircleBuffer;


enum CB_RESULT {
    CB_SUCCESS = 0,
/*    CB_FALSE = 0,
    CB_TRUE = 1,*/
    CB_FAILURE = -1
};


int cbCreate(size_t size_, CircleBuffer *buffer_);
int cbDestroy(CircleBuffer buffer_);

int cbReadFromFD(CircleBuffer buffer_, int fd_, size_t num_, int *read_res_);
int cbWriteToFD(CircleBuffer buffer_, int fd_, size_t num_, int* write_res_);

size_t cbGetSize(CircleBuffer buffer_);
size_t cbGetEmpty(CircleBuffer buffer_);

int cbIsEmpty(CircleBuffer buffer_);
int cbIsFull(CircleBuffer buffer_);

void cbDump(CircleBuffer buffer_);

#endif //MIPT_S3_LUNEV_CIRCLEBUFFER_H
