#ifdef IML_Q2_EXTENSIONS

#include <stddef.h>
#include <string.h>
#include "binmsg.h"

// Optimize the living daylights out of local functions--most are very
// short, and all them alias variables with each other.  I haven't done
// any profiling, though, so I may be making embarassingly bogus
// assumptions.
#define LOCAL static __inline


//=========================================================================
//  Error Handling Support
//=========================================================================

#define RETURN_OK return 1

#define ABORT_ON_FAILURE(EXPR) \
    do { \
        if (!(EXPR)) \
            return 0; \
    } while (0)


//=========================================================================
//  Message Building Functions
//=========================================================================

#define RESERVE_SIZE(MSG,NEEDED) \
    ABORT_ON_FAILURE(reserve_size((MSG), (NEEDED)))
#define WRITE_TYPE(MSG,TYPE) \
    ABORT_ON_FAILURE(write_type((MSG), (TYPE)))
#define WRITE_STRING(MSG,STRING) \
    ABORT_ON_FAILURE(write_string((MSG), (STRING)))
#define WRITE_RAW_INT(MSG,I) \
    ABORT_ON_FAILURE(write_raw_int((MSG), (I)))
#define WRITE_ARRAY(MSG,OUT_ARRAY) \
    ABORT_ON_FAILURE(write_array((MSG), (OUT_ARRAY)))

LOCAL binmsg_bool reserve_size(binmsg_message *msg, size_t needed)
{
    msg->limit += needed;
    return (msg->limit <= msg->limit_max);
}

LOCAL binmsg_bool write_type(binmsg_message *msg, binmsg_type type)
{
    RESERVE_SIZE(msg, 1);
    *msg->next++ = type;
    RETURN_OK;
}

LOCAL binmsg_bool write_string(binmsg_message *msg, binmsg_string string)
{
    size_t bytes;
    WRITE_TYPE(msg, BINMSG_STRING_TYPE);
    bytes = strlen(string) + 1;
    RESERVE_SIZE(msg, bytes);
    memcpy(msg->next, string, bytes);
    msg->next += bytes;
    RETURN_OK;
}

LOCAL binmsg_bool write_raw_int(binmsg_message *msg, binmsg_int i)
{
    RESERVE_SIZE(msg, 4);
    *msg->next++ = i >> 24;
    *msg->next++ = (i >> 16) & 0xFF;
    *msg->next++ = (i >> 8) & 0xFF;
    *msg->next++ = i & 0xFF;
    RETURN_OK;
}

LOCAL binmsg_bool write_array(binmsg_message *msg, binmsg_array *out_array)
{
    out_array->message = msg;
    out_array->size = 0;

    WRITE_TYPE(msg, BINMSG_ARRAY_TYPE);
    RESERVE_SIZE(msg, 4);
    out_array->size_backref = msg->next;
    msg->next += 4;
    RETURN_OK;
}

binmsg_bool binmsg_build(binmsg_message *message,
                         binmsg_byte *buffer,
                         size_t buffer_size,
                         binmsg_string name)
{
    message->buffer    = buffer;
    message->next      = buffer;
    message->limit     = buffer;
    message->limit_max = buffer + buffer_size;

    WRITE_TYPE(message, BINMSG_MSG_TYPE);
    WRITE_STRING(message, name);
    WRITE_ARRAY(message, &message->args);
    
    RETURN_OK;
}

binmsg_bool binmsg_build_done(binmsg_message *message)
{
    message->buffer_size = message->limit - message->buffer;
    return binmsg_add_array_end(&message->args);
}

binmsg_bool binmsg_add_int(binmsg_array *array, binmsg_int value)
{
    WRITE_TYPE(array->message, BINMSG_INT_TYPE);
    WRITE_RAW_INT(array->message, value);
    ++array->size;
    RETURN_OK;
}

binmsg_bool binmsg_add_float(binmsg_array *array, binmsg_float value)
{
    union { binmsg_int i; binmsg_float f; } u;
    u.f = value;
    WRITE_TYPE(array->message, BINMSG_FLOAT_TYPE);
    WRITE_RAW_INT(array->message, u.i);
    ++array->size;
    RETURN_OK;
}

binmsg_bool binmsg_add_string(binmsg_array *array, binmsg_string value)
{
    WRITE_STRING(array->message, value);
    ++array->size;
    RETURN_OK;
}

binmsg_bool binmsg_add_bool(binmsg_array *array, binmsg_bool value)
{
    WRITE_TYPE(array->message, BINMSG_BOOL_TYPE);
    RESERVE_SIZE(array->message, 1);
    *array->message->next++ = value ? 1 : 0;
    ++array->size;
    RETURN_OK;    
}

binmsg_bool binmsg_add_array_begin(binmsg_array *array,
                                   binmsg_array *out_array)
{
    WRITE_ARRAY(array->message, out_array);
    ++array->size;
    RETURN_OK;    
}

binmsg_bool binmsg_add_array_end(binmsg_array *array)
{
    binmsg_byte *out = array->size_backref;
    binmsg_int size = array->size;
    
    *out++ = size >> 24;
    *out++ = (size >> 16) & 0xFF;
    *out++ = (size >> 8) & 0xFF;
    *out++ = size & 0xFF;
    
    RETURN_OK;
}


//=========================================================================
//  Message Parsing Functions
//=========================================================================

#define REQUIRE_PARSE_BYTES(MSG,BYTES) \
    ABORT_ON_FAILURE(require_parse_bytes((MSG), (BYTES)))
#define CHECK_NEXT_TYPE(MSG,TYPE) \
    ABORT_ON_FAILURE(check_next_type((MSG), (TYPE)))
#define PARSE_STRING(MSG,OUT_STR) \
    ABORT_ON_FAILURE(parse_string((MSG), (OUT_STR)))
#define PARSE_RAW_INT(MSG,OUT_INT) \
    ABORT_ON_FAILURE(parse_raw_int((MSG), (OUT_INT)))
#define PARSE_ARRAY(MSG,OUT_ARRAY) \
    ABORT_ON_FAILURE(parse_array((MSG), (OUT_ARRAY)))

LOCAL binmsg_bool require_parse_bytes(binmsg_message *message, size_t bytes)
{
    return (message->next + bytes <= message->limit);
}

LOCAL binmsg_bool check_next_type(binmsg_message *message, binmsg_type type)
{
    REQUIRE_PARSE_BYTES(message, 1);
    return (*message->next++ == type);
}

LOCAL binmsg_bool parse_string(binmsg_message *message,
                               binmsg_string *out_string)
{
    CHECK_NEXT_TYPE(message, BINMSG_STRING_TYPE);
    REQUIRE_PARSE_BYTES(message, 1);
    *out_string = message->next;
    while (*message->next)
    {
        message->next++;
        REQUIRE_PARSE_BYTES(message, 1);
    }
    message->next++;
    RETURN_OK;
}

LOCAL binmsg_bool parse_raw_int(binmsg_message *message, binmsg_int *out_int)
{
    REQUIRE_PARSE_BYTES(message, 4);
    *out_int =  *message->next++ << 24;
    *out_int |= *message->next++ << 16;
    *out_int |= *message->next++ << 8;
    *out_int |= *message->next++;
    RETURN_OK;
}

LOCAL binmsg_bool parse_array(binmsg_message *message, binmsg_array *out_array)
{
    CHECK_NEXT_TYPE(message, BINMSG_ARRAY_TYPE);
    PARSE_RAW_INT(message, &out_array->size);
    out_array->message = message;
    RETURN_OK;
}

binmsg_bool binmsg_parse(binmsg_message *message,
                         binmsg_byte *data, size_t size)
{
    message->buffer      = data;
    message->buffer_size = size;
    message->next        = data;
    message->limit       = data + size;

    CHECK_NEXT_TYPE(message, BINMSG_MSG_TYPE);
    PARSE_STRING(message, &message->name);
    PARSE_ARRAY(message, &message->args);

    RETURN_OK;
}

binmsg_bool binmsg_get_next_type(binmsg_array *array, binmsg_type *out_type)
{
    REQUIRE_PARSE_BYTES(array->message, 1);
    *out_type = *array->message->next;
    RETURN_OK;
}

binmsg_bool binmsg_get_int(binmsg_array *array, binmsg_int *out_value)
{
    CHECK_NEXT_TYPE(array->message, BINMSG_INT_TYPE);
    PARSE_RAW_INT(array->message, out_value);
    RETURN_OK;
}

binmsg_bool binmsg_get_float(binmsg_array *array, binmsg_float *out_value)
{
    union { binmsg_int i; binmsg_float f; } u;
    CHECK_NEXT_TYPE(array->message, BINMSG_FLOAT_TYPE);
    PARSE_RAW_INT(array->message, &u.i);
    *out_value = u.f;
    RETURN_OK;
}

binmsg_bool binmsg_get_string(binmsg_array *array, binmsg_string *out_value)
{
    PARSE_STRING(array->message, out_value);
    RETURN_OK;
}

binmsg_bool binmsg_get_bool(binmsg_array *array, binmsg_bool *out_value)
{
    CHECK_NEXT_TYPE(array->message, BINMSG_BOOL_TYPE);
    REQUIRE_PARSE_BYTES(array->message, 1);
    *out_value = *array->message->next++;
    RETURN_OK;
}

binmsg_bool binmsg_get_array(binmsg_array *array, binmsg_array *out_array)
{
    PARSE_ARRAY(array->message, out_array);
    RETURN_OK;
}

#endif // IML_Q2_EXTENSIONS

