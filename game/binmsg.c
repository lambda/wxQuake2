#ifdef IML_Q2_EXTENSIONS

#include <stddef.h>
#include "binmsg.h"


//=========================================================================
//  Message Building Functions
//=========================================================================

static binmsg_bool reserve_size(binmsg_message *msg, size_t needed,
                                binmsg_byte **out_iter)
{
    if (msg->buffer_size + needed <= msg->buffer_size_max) {
        *out_iter = msg->buffer + msg->buffer_size;
        msg->buffer_size += needed;
        return 1;
    } else {
        return 0;
    }
}

#define RESERVE_SIZE(MSG,NEEDED,ITER) \
    do { \
        if (!reserve_size((MSG), (NEEDED), (ITER))) \
            return 0; \
    } while (0)

#define RETURN_OK return 1

binmsg_bool binmsg_build(binmsg_message *message,
                         binmsg_byte *buffer,
                         size_t buffer_size,
                         binmsg_string name)
{
    binmsg_byte *out;

    message->buffer = buffer;
    message->buffer_size = 0;
    message->buffer_size_max = buffer_size;

    RESERVE_SIZE(message, 3 + strlen(name), &out);
    *out++ = BINMSG_MSG_TYPE;
    *out++ = BINMSG_STRING_TYPE;
    strcpy(out, name);

    message->args.message = message;
    message->args.size = 0;
    RESERVE_SIZE(message, 5, &out);
    *out++ = BINMSG_ARRAY_TYPE;
    message->args.size_backref = out;
    
    RETURN_OK;
}

binmsg_bool binmsg_build_done(binmsg_message *message)
{
    return binmsg_add_array_end(&message->args);
}

binmsg_bool binmsg_add_int(binmsg_array_info *array, binmsg_int value)
{
    binmsg_byte *out;
    ++array->size;
    RESERVE_SIZE(array->message, 5, &out);
    *out++ = BINMSG_INT_TYPE;
    *out++ = value >> 24;
    *out++ = (value >> 16) & 0xFF;
    *out++ = (value >> 8) & 0xFF;
    *out++ = value & 0xFF;
    RETURN_OK;
}

binmsg_bool binmsg_add_float(binmsg_array_info *array, binmsg_float value)
{
    binmsg_byte *out;
    union { binmsg_int i; binmsg_float f; } u;

    ++array->size;
    u.f = value;

    RESERVE_SIZE(array->message, 5, &out);
    *out++ = BINMSG_FLOAT_TYPE;
    *out++ = u.i >> 24;
    *out++ = (u.i >> 16) & 0xFF;
    *out++ = (u.i >> 8) & 0xFF;
    *out++ = u.i & 0xFF;
    RETURN_OK;
}

binmsg_bool binmsg_add_string(binmsg_array_info *array, binmsg_string value)
{
    binmsg_byte *out;
    ++array->size;
    RESERVE_SIZE(array->message, 2 + strlen(value), &out);
    *out++ = BINMSG_STRING_TYPE;
    strcpy(out, value);
    RETURN_OK;
}

binmsg_bool binmsg_add_bool(binmsg_array_info *array, binmsg_bool value)
{
    binmsg_byte *out;
    ++array->size;
    RESERVE_SIZE(array->message, 2, &out);
    *out++ = BINMSG_BOOL_TYPE;
    *out++ = value ? 1 : 0;
    RETURN_OK;    
}

binmsg_bool binmsg_add_array_begin(binmsg_array_info *array,
                                   binmsg_array_info *out_array)
{
    binmsg_byte *out;
   ++array->size;

    out_array->message = array->message;
    out_array->size = 0;
    RESERVE_SIZE(array->message, 5, &out);
    *out++ = BINMSG_ARRAY_TYPE;
    out_array->size_backref = out;
    
    RETURN_OK;    
}

binmsg_bool binmsg_add_array_end(binmsg_array_info *array)
{
    binmsg_byte *out;
    binmsg_int value = array->size;
    out = array->size_backref;
    *out++ = value >> 24;
    *out++ = (value >> 16) & 0xFF;
    *out++ = (value >> 8) & 0xFF;
    *out++ = value & 0xFF;
    RETURN_OK;
}


//=========================================================================
//  Message Parsing Functions
//=========================================================================


#endif // IML_Q2_EXTENSIONS

