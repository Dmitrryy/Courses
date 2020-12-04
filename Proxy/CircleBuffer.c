
#include "CircleBuffer.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#define MIN(a, b) ((a > b) ? b : a)
#define MAX(a, b) ((a < b) ? b : a)

struct CircleBuffer_t {
    char *pBuffer;

    size_t ri, wi;
    size_t size;
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

    tmp->size = size_;
    tmp->ri = tmp->wi = 0;

    return CB_SUCCESS;
}

int cbDestroy(CircleBuffer buffer_)
{
    if (buffer_ == NULL) {
        return CB_FAILURE;
    }

    free(buffer_->pBuffer);
    buffer_->pBuffer = NULL;
    buffer_->size = buffer_->ri = buffer_->wi = 0;

    free(buffer_);

    return CB_SUCCESS;
}


int cbReadFromFD(CircleBuffer buffer_, int fd_, size_t num_, int *read_res_)
{
    if (buffer_ == NULL || buffer_->pBuffer == NULL) {
        return CB_FAILURE;
    }
    size_t bSize = buffer_->size;
    int res = 0;
    res = read(fd_, buffer_->pBuffer + (buffer_->wi % bSize), MIN(bSize - (buffer_->wi % bSize), bSize - (buffer_->wi - buffer_->ri)));

    if (res > 0) {
        buffer_->wi += res;
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

    size_t bSize = buffer_->size;
    int res = 0;
    res = write(fd_, buffer_->pBuffer + (buffer_->ri % bSize), MIN(buffer_->wi - buffer_->ri, bSize - (buffer_->ri % bSize)));

    if (res > 0) {
        buffer_->ri += res;
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

int cbIsEmpty(CircleBuffer buffer_)
{
    return buffer_->ri == buffer_->wi;
}
int cbIsFull(CircleBuffer buffer_)
{
    return buffer_->wi - buffer_->ri == buffer_->size;
}

void cbDump(CircleBuffer buffer_);