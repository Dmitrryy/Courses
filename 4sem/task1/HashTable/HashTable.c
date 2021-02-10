
#include "HashTable.h"

#include <assert.h>
#include <inttypes.h>
#include <sys/types.h>


struct Hash_t
{
    size_t      m_size;
    size_t      m_maxInOne;
    uint32_t (* m_fHash )(void* data, size_t size);

    HS_VAL_TYPE* m_data;
};

