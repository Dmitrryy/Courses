
#include "CircleBuffer.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#define MIN(a, b) ((a > b) ? b : a)
#define MAX(a, b) ((a < b) ? b : a)

struct CircleBuffer_t {
    char *pBuffer;
    char *tpForWrite;
    char *tpForRead;

    size_t size;
    size_t empty;
};


int cbCreate(size_t size_, CircleBuffer *buffer_)
{
    *buffer_ = (CircleBuffer)calloc(1, sizeof (struct CircleBuffer_t));
    if (*buffer_ == NULL) {
        return CB_FAILURE;
    }

    CircleBuffer tmp = *buffer_;
    tmp->pBuffer = (char*) calloc (size_, sizeof(char));
    if (tmp->pBuffer == NULL) {
        return CB_FAILURE;
    }

    tmp->tpForWrite = tmp->tpForRead = tmp->pBuffer;
    tmp->size = tmp->empty = size_;

    return CB_SUCCESS;
}

int cbDestroy(CircleBuffer buffer_)
{
    if (buffer_ == NULL) {
        return CB_FAILURE;
    }

    free(buffer_->pBuffer);
    buffer_->pBuffer = buffer_->tpForRead = buffer_->tpForWrite = NULL;
    buffer_->size = buffer_->empty = 0;

    free(buffer_);

    return CB_SUCCESS;
}


int cbReadFromFD(CircleBuffer buffer_, int fd_, size_t num_, int *read_res_)
{
    if (buffer_ == NULL || buffer_->pBuffer == NULL) {
        return CB_FAILURE;
    }

    int res = 0;
    if (buffer_->tpForWrite <= buffer_->tpForRead)
    {
        /*
         * buffer:
         *  pBuffer    tpForWrite      tpForRead
         * /          /                /
         * |x|x|x|x|x| | | | | | | | | |x|x|x|x|
         *           |<-----empty----->|
         * |<---------------size-------------->|
         */
        res = read(fd_, buffer_->tpForWrite, MIN(buffer_->empty, num_));
    }
    else
    {
        /*
         * buffer:
         *  pBuffer   tpForRead     tpForWrite
         * /         /             /
         * | | | | | |x|x|x|x|x|x|x| | | | | | |
         *                         |<-- num -->|
         */
        res = read(fd_, buffer_->tpForWrite, MIN(buffer_->pBuffer - buffer_->tpForWrite + buffer_->size, num_));
    }

    if (res > 0) {
        buffer_->empty -= res;
        buffer_->tpForWrite += res;
    }

    assert(buffer_->tpForWrite - buffer_->pBuffer <= buffer_->size);
    if (buffer_->tpForWrite - buffer_->pBuffer == buffer_->size)
    {
        buffer_->tpForWrite = buffer_->pBuffer;
    }

    if (read_res_ != NULL) {
        *read_res_ = res;
    }

    return CB_SUCCESS;
}

int cbWriteToFD(CircleBuffer buffer_, int fd_, size_t num_, int* write_res_)
{
    if (buffer_ == NULL || buffer_->pBuffer == NULL) {
        return CB_FAILURE;
    }

    int res = 0;
    if (buffer_->tpForRead <= buffer_->tpForWrite)
    {
        //see pictures above
        res = write(fd_, buffer_->tpForRead, buffer_->size - buffer_->empty);
    }
    else
    {
        res = write(fd_, buffer_->tpForRead, buffer_->pBuffer + buffer_->size - buffer_->tpForRead);
    }

    if (res > 0) {
        buffer_->tpForRead += res;
        buffer_->empty += res;
    }

    assert(buffer_->tpForRead <= buffer_->pBuffer + buffer_->size);
    if (buffer_->tpForRead - buffer_->pBuffer == buffer_->size)
    {
        buffer_->tpForRead = buffer_->pBuffer;
    }


    if (write_res_ != NULL) {
        *write_res_ = res;
    }

    return CB_SUCCESS;
}


size_t cbGetSize(CircleBuffer buffer_)
{
    return buffer_->size;
}
size_t cbGetEmpty(CircleBuffer buffer_)
{
    return buffer_->empty;
}

int cbIsEmpty(CircleBuffer buffer_)
{
    return buffer_->empty == buffer_->size;
}
int cbIsFull(CircleBuffer buffer_)
{
    return buffer_->empty == 0;
}

void cbDump(CircleBuffer buffer_);