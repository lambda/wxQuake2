#ifndef BINMSG_H
#define BINMSG_H

#ifdef IML_Q2_EXTENSIONS

//=========================================================================
//  Binary Serialized Message System
//=========================================================================
//  This is roughly based on my Binmode RPC design, which was in turn
//  inspired by XML-RPC.  The basic idea: Serialize a modest number of
//  data types--including nested arrays--quickly and compactly onto the
//  wire.  The following types are supported:
//
//    32-bit signed integers (I)
//    floating point numbers (D)
//    strings (S)
//    booleans (B)
//    arrays (A)
//
//  The following types have been omitted:
//
//    dates
//    maps
//    raw binary data
//
//  I've also left out any notion of return values and compression of
//  repeated strings, and changed around the wire representations.

#define BINMSG_MAX_SIZE (1024)

#define BINMSG_MSG_TYPE ('M')    // 'M' name:STRING args:ARRAY
#define BINMSG_INT_TYPE ('I')    // 'I' value:RAW_INT32
#define BINMSG_FLOAT_TYPE ('F')  // 'F' value:RAW_FLOAT32
#define BINMSG_STRING_TYPE ('S') // 'S' data:CHAR*n '\0'
#define BINMSG_BOOL_TYPE ('B')   // 'B' value:BYTE (0 or 1)
#define BINMSG_ARRAY_TYPE ('A')  // 'A' length:RAW_INT32 data:ANY*length

typedef char binmsg_type;
typedef unsigned char binmsg_byte;

typedef int binmsg_int;
typedef float binmsg_float;
typedef char *binmsg_string;
typedef int binmsg_bool;

struct binmsg_message_tag;

typedef struct {
    struct binmsg_message_tag *message;
    size_t size;

    // Only defined when building.
    binmsg_byte *size_backref;
} binmsg_array;

typedef struct binmsg_message_tag {
    binmsg_byte *buffer;
    size_t buffer_size;
    binmsg_array args;
    binmsg_byte *next, *limit;

    // Only defined when building.
    binmsg_byte *limit_max;

    // Only defined when parsing.
    binmsg_string name;
} binmsg_message;

binmsg_bool binmsg_build(binmsg_message *message,
                         binmsg_byte *buffer,
                         size_t buffer_size,
                         binmsg_string name);
binmsg_bool binmsg_build_done(binmsg_message *message);
binmsg_bool binmsg_add_int(binmsg_array *array, binmsg_int value);
binmsg_bool binmsg_add_float(binmsg_array *array, binmsg_float value);
binmsg_bool binmsg_add_string(binmsg_array *array, binmsg_string value);
binmsg_bool binmsg_add_bool(binmsg_array *array, binmsg_bool value);
binmsg_bool binmsg_add_array_begin(binmsg_array *array,
                                   binmsg_array *out_array);
binmsg_bool binmsg_add_array_end(binmsg_array *array);

binmsg_bool binmsg_parse(binmsg_message *message,
                         binmsg_byte *data,
                         size_t size);
binmsg_bool binmsg_get_next_type(binmsg_array *array, binmsg_type *out_type);
binmsg_bool binmsg_get_int(binmsg_array *array, binmsg_int *out_value);
binmsg_bool binmsg_get_float(binmsg_array *array, binmsg_float *out_value);
binmsg_bool binmsg_get_string(binmsg_array *array, binmsg_string *out_value);
binmsg_bool binmsg_get_bool(binmsg_array *array, binmsg_bool *out_value);
binmsg_bool binmsg_get_array(binmsg_array *array, binmsg_array *out_array);

#endif // IML_Q2_EXTENSIONS
#endif // BINMSG_H
