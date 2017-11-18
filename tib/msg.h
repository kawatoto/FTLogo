/*
 * Copyright (c) 2009-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: msg.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_msg_h
#define _INCLUDED_tib_msg_h

#include "tib/except.h"
#include "tib/types.h"
#include "tib/inbox.h"
#include "tib/tibexp.h"
#include "tib/fldref.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file msg.h
 * 
 * @brief Message objects contain fields with values.
 *
 * This file defines message objects, message fields, message
 * iterators, and calls that manipulate them.
 *
 * @section accessors Accessors by Name and Field Reference
 * @ref tibFieldRef
 *
 * @section msg_iterator Message Iterators
 * @ref tibMessageIterator
 */

/** @brief Built-in format name for opaque messages.
 *
 * Opaque messages contain only a byte-string.
 *
 * This format name is valid as an argument to @ref tibMessage_Create.
 */
#define TIB_BUILTIN_MSG_FMT_OPAQUE              "_default_opaque"

/** @brief Built-in format name for keyed opaque messages.
 *
 * Keyed opaque messages contain only a character string and a byte-string.
 * Content matchers can match against the key field (character string).
 *
 * This format name is valid as an argument to @ref tibMessage_Create.
 */
#define TIB_BUILTIN_MSG_FMT_KEYED_OPAQUE        "_keyed_opaque"

/** @brief Maximum efficient length (in bytes) of a built-in opaque message.
 *
 * Larger payloads are less efficient.
 *
 * This threshold also applies to the total payload of a keyed opaque message
 * (including the key string, its null terminator, and the opaque data).
 */
#define TIB_BUILTIN_MSG_FMT_OPAQUE_MAXSIZE      (12000)

/** @brief Maximum length (in bytes) of the key field of a
 * keyed opaque message.
 */
#define TIB_BUILTIN_MSG_FMT_KEY_MAXLEN          (255)

/** @brief Built-in field name of the data field in an opaque
 * message or a keyed opaque message.
 */
#define TIB_BUILTIN_MSG_FMT_OPAQUE_FIELDNAME    "_data"

/** @brief Built-in field name of the key field in a
 * keyed opaque message.
 */
#define TIB_BUILTIN_MSG_FMT_KEY_FIELDNAME       "_key"

/** @brief Datatypes of FTL message fields.
 */
typedef enum {

    /** @brief Opaque byte-string.
     */
    TIB_FIELD_TYPE_OPAQUE        = 0,

    /** @brief Long integer.
     */
    TIB_FIELD_TYPE_LONG          = 1,

    /** @brief Array of long integers.
     */
    TIB_FIELD_TYPE_LONG_ARRAY    = 2,

    /** @brief Double floating-point number.
     */
    TIB_FIELD_TYPE_DOUBLE        = 3,

    /** @brief Array of double floating-point numbers.
     */
    TIB_FIELD_TYPE_DOUBLE_ARRAY  = 4,

    /** @brief Character string.
     */
    TIB_FIELD_TYPE_STRING        = 5,

    /** @brief Array of character strings.
     */
    TIB_FIELD_TYPE_STRING_ARRAY  = 6,

    /** @brief Sub-message (that is, a nested message).
     */
    TIB_FIELD_TYPE_MESSAGE       = 7,

    /** @brief Array of sub-messages.
     */
    TIB_FIELD_TYPE_MESSAGE_ARRAY = 8,

    /** @brief Inbox.
     */
    TIB_FIELD_TYPE_INBOX         = 9,

    /** @brief DateTime.
     */
    TIB_FIELD_TYPE_DATETIME      = 10,

    /** @brief Array of DateTimes.
     */
    TIB_FIELD_TYPE_DATETIME_ARRAY = 11,

    /** @brief Unrecognized type.
     *
     * @ref tibMessage_GetFieldType and @ref
     * tibMessage_GetFieldTypeByRef return \c TIB_FIELD_TYPE_UNKNOWN
     * to indicate that the field's type is not defined.
     *
     * For example, getting the field type for a NULL message returns
     * this value.
     *
     * It is not legal for programs to supply \c TIB_FIELD_TYPE_UNKNOWN
     * to any API call except @ref tibFieldType_GetAsString.
     */
     TIB_FIELD_TYPE_UNKNOWN       = 2048

} tibFieldType;

/** @brief Message object type.
 *
 * Message objects are not thread-safe.  Programs must not access a
 * message from several threads simultaneously.
 */
typedef struct __tibMessage         *tibMessage;

/** @brief Message iterator object type.
 *
 * When you do \e not know the format of a message at the time you
 * design a program, your program can use a message iterator to
 * traverse the fields of a message.  For example, you can use this
 * technique to output the complete content of an erroneous or
 * unexpected message to a log file.  (However, when you \e do know
 * the format of an inbound message, then you can code the program to
 * get each field in the most efficient manner.)
 *
 * A message iterator presents all the fields 
 * that are \e actually \e set in a message object.
 * The message format determines the order in which an iterator
 * presents the fields of a message.
 *
 * Message iterator objects are not thread-safe.  Programs must not
 * access a message iterator from several threads simultaneously.
 *
 * An iterator becomes invalid when its message becomes invalid.
 */
typedef struct __tibMessageIterator         *tibMessageIterator;

/** @brief Return a string that represents a field type.
 * 
 * @param e The exception object captures information about failures.
 * @param fieldType The call converts this type to a string.
 * 
 * @return a string corresponding the fieldType argument
 *
 * @see tibFieldType
 */
TIB_API
const char*
tibFieldType_GetAsString(
    tibEx        e,
    tibFieldType fieldType);

/** @brief Create a mutable message object.
 * 
 * Programs can use this call to create messages for sending.
 * 
 * Message objects are not thread-safe.  Programs must not access a
 * message from several threads simultaneously.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The call searches this realm for the format definition.
 * @param formatName The call creates a message with this format.\n
 *                   \c NULL is a special value, indicating an unnamed dynamic
 *                   format (for use only in one specific message).
 * 
 * @return a new message object
 *
 * @see TIB_BUILTIN_MSG_FMT_OPAQUE (as \e formatName argument)
 */
TIB_API
tibMessage
tibMessage_Create(
    tibEx               e,
    tibRealm            realm,
    const char*         formatName);

/** @brief Create a mutable copy of a message.
 * 
 * Programs cannot modify inbound messages that subscribers receive.
 * However, programs can use this call to create mutable copies (which
 * they may modify).
 * 
 * Programs must destroy mutable copies to reclaim resources.
 *
 * @param e The exception object captures information about failures.
 * @param message The call copies this source message.
 * 
 * @return a mutable message object
 */
TIB_API
tibMessage
tibMessage_MutableCopy(
    tibEx               e,
    tibMessage          message
);

/** @brief Get the value of a string field from a message.
 * 
 * A \c NULL character terminates the string.
 * The string pointer is valid only for the lifetime of the message.
 * The string is part of the message object;
 * the program must neither modify nor free it.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param name The call gets the value of the field with this name.
 *  
 * @return a pointer to the string field
 *
 * @see tibMessage_GetStringByRef
 */
TIB_API
const char*
tibMessage_GetString(
    tibEx               e,
    tibMessage          message,
    const char*         name
);

/** @brief Get the value of a string field from a message.
 * 
 * A \c NULL character terminates the string.
 * The string pointer is valid only for the lifetime of the message.
 * The string is part of the message object;
 * the program must neither modify nor free it.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param fieldRef The call gets the value of this field.
 *  
 * @return a pointer to the string field
 *
 * @see tibMessage_GetString
*/
TIB_API
const char*
tibMessage_GetStringByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef
);


/** @brief Get the value of a long field from a message.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param name The call gets the value of the field with this name.
 * 
 * @return a 64-bit integer value
 *
 * @see tibMessage_GetLongByRef
 */
TIB_API
tibint64_t
tibMessage_GetLong(
    tibEx               e,
    tibMessage          message,
    const char*         name
);


/** @brief Get the value of a long field from a message.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param fieldRef The call gets the value of this field.
 * 
 * @return a 64-bit integer value
 *
 * @see tibMessage_GetLong
 */
TIB_API
tibint64_t
tibMessage_GetLongByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef
);

/** @brief Get the value of a double floating-point field from a message.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param name The call gets the value of the field with this name.
 * 
 * @return a double floating-point value
 *
 * @see tibMessage_GetDoubleByRef
 */
TIB_API
tibdouble_t
tibMessage_GetDouble(
    tibEx               e,
    tibMessage          message,
    const char*         name
);

/** @brief Get the value of a double floating-point field from a message.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param fieldRef The call gets the value of this field.
 * 
 * @return a double floating-point value
 *
 * @see tibMessage_GetDouble
 */
TIB_API
tibdouble_t
tibMessage_GetDoubleByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef
);

/** @brief Get the content of an opaque field from a message.
 *
 * The data pointer is valid only for the lifetime of the message.
 * The data bytes are part of the message object;
 * the program must neither modify nor free them.
 *
 * The data pointer is not necessarily aligned to a word boundary.
 * Before casting the opaque data to a struct, you must either verify
 * that it is properly aligned (that is, the data pointer is divisible
 * by 8), or copy the data to an aligned location.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param name The call gets the content of the field with this name.
 * @param size The call returns the length (in bytes) of the opaque field.
 *  
 * @return a read-only pointer to opaque data
 *
 * @see tibMessage_GetOpaqueByRef
 */
TIB_API
const void*
tibMessage_GetOpaque(
    tibEx               e,
    tibMessage          message,
    const char*         name,
    tibint32_t          *size);


/** @brief Get the content of an opaque field from a message.
 *
 * The data pointer is valid only for the lifetime of the message.
 * The data bytes are part of the message object;
 * the program must neither modify nor free them.
 *
 * The data pointer is not necessarily aligned to a word boundary.
 * Before casting the opaque data to a struct, you must either verify
 * that it is properly aligned (that is, the data pointer is divisible
 * by 8), or copy the data to an aligned location.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param fieldRef The call gets the content of this field.
 * @param size The call returns the length (in bytes) of the opaque field.
 *  
 * @return a read-only pointer to opaque data
 *
 * @see tibMessage_GetOpaque
 */
TIB_API
const void*
tibMessage_GetOpaqueByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    tibint32_t          *size);

/** @brief Destroy a message object.
 * 
 * A program that creates a \e mutable message must explicitly 
 * destroy it to reclaim its resources.
 * (However, it is often more efficient to \e reuse a mutable message.
 * For further details, see the book <b> TIBCO FTL Development</b>.)
 *
 * A program may destroy only \e mutable messages -- that is, those
 * messages that the program creates using @ref tibMessage_Create,
 * @ref tibMessage_MutableCopy or @ref tibMessage_ReadFromByteArray.
 * 
 * Inbound messages (and sub-messages) in callbacks belong to the FTL
 * library; programs must not destroy them.
 *
 * Do not destroy a message if the program needs a reference
 * into that message -- for example, a string pointer (from @ref
 * tibMessage_GetString) or an opaque pointer (from @ref
 * tibMessage_GetOpaque).
 *
 * Destroying a message frees all resources associated with it.
 *
 * However, destroying a message that contains sub-messages does not
 * automatically destroy the sub-messages.
 * Before destroying an enclosing message, you must first explicitly
 * destroy any sub-messages.
 *
 * @param e The exception object captures information about failures.
 * @param message The call destroys this message object.
 *
 * @return void
 */
TIB_API
void
tibMessage_Destroy(
    tibEx               e,
    tibMessage          message
);

/** @brief Set a string field in a mutable message.
 *
 * This call copies the string value into the message.
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param value The call sets this string as the field value.
 *  
 * @return void
 *
 * @see tibMessage_SetStringByRef
 */
TIB_API
void
tibMessage_SetString(
    tibEx               e,
    tibMessage          message,
    const char*         name,
    const char          *value);

/** @brief Set a string field in a mutable message.
 *
 * This call copies the string value into the message.
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param value The call sets this string as the field value.
 *  
 * @return void
 *
 * @see tibMessage_SetString
 */
TIB_API
void
tibMessage_SetStringByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    const char          *value);

/** @brief Set an opaque (byte-string) field in a mutable message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param value The call copies this byte-string into the message field.
 * @param size Supply the length (in bytes) of the opaque value.
 *  
 * @return void
 *
 * @see tibMessage_SetOpaqueByRef
 */
TIB_API
void
tibMessage_SetOpaque(
    tibEx               e,
    tibMessage          message,
    const char*         name,
    const void          *value,
    tibint32_t          size);

/** @brief Set an opaque (byte-string) field in a mutable message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param value The call copies this byte-string into the message field.
 * @param size Supply the length (in bytes) of the opaque value.
 *  
 * @return void
 *
 * @see tibMessage_SetOpaque
 */
TIB_API
void
tibMessage_SetOpaqueByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    const void          *value,
    tibint32_t          size);

/** @brief Set an opaque (byte-string) field in a mutable message.
 * 
 * For efficiency, programs can set an opaque field without the
 * expense of copying the byte-string.
 *
 * @ref tibMessage_SetOpaque copies the opaque value from your
 * buffer into the mutable message object.  The program may then
 * modify the buffer without affecting the message.
 * 
 * In contrast, @ref tibMessage_SetOpaqueDirect stores only a \e pointer
 * to the data in the message object.  The send call serializes the
 * opaque data.  If you change the data in the buffer before the send
 * call, you must call @ref tibMessage_SetOpaqueDirect \e again to ensure
 * correct serialization.  You must not deallocate the buffer until
 * after destroying the message (that is, until after @ref
 * tibMessage_Destroy returns).
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param value The call sets the field value in the message to be a pointer to this byte-string.
 * @param size Supply the length (in bytes) of the opaque value.
 * 
 * @return void
 *
 * @see tibMessage_SetOpaqueDirectByRef
 */
TIB_API
void
tibMessage_SetOpaqueDirect(
    tibEx               e,
    tibMessage          message,
    const char*         name,
    const void          *value,
    tibint32_t          size);

/** @brief Set an opaque (byte-string) field by reference in a mutable message.
 * 
 * For efficiency, programs can set an opaque field without the
 * expense of copying the byte-string.
 *
 * @ref tibMessage_SetOpaque copies the opaque value from your
 * buffer into the mutable message object.  The program may then
 * modify the buffer without affecting the message.
 * 
 * In contrast, @ref tibMessage_SetOpaqueDirect stores only a \e pointer
 * to the data in the message object.  The send call serializes the
 * opaque data.  If you change the data in the buffer before the send
 * call, you must call @ref tibMessage_SetOpaqueDirect \e again to ensure
 * correct serialization.  You must not deallocate the buffer until
 * after destroying the message (that is, until after @ref
 * tibMessage_Destroy returns).
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param value The call sets the field value in the message to be a pointer to this byte-string.
 * @param size Supply the length (in bytes) of the opaque value.
 * 
 * @return void
 *
 * @see tibMessage_SetOpaqueDirect
 */
TIB_API
void
tibMessage_SetOpaqueDirectByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    const void          *value,
    tibint32_t          size);

/** @brief Set a long integer field in a mutable message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param value The call sets this long integer as the field value.
 * 
 * @return void
 *
 * @see tibMessage_SetLongByRef
 */
TIB_API
void
tibMessage_SetLong(
    tibEx               e,
    tibMessage          message,
    const char*         name,
    tibint64_t          value);

/** @brief Set a long integer field in a mutable message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param value The call sets this long integer as the field value.
 * 
 * @return void
 *
 * @see tibMessage_SetLong
 */
TIB_API
void
tibMessage_SetLongByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    tibint64_t          value);

/** @brief Set a double floating-point field in a mutable message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param value The call sets this double as the field value.
 *  
 * @return void
 *
 * @see tibMessage_SetDoubleByRef
 */
TIB_API
void
tibMessage_SetDouble(
    tibEx               e,
    tibMessage          message,
    const char*         name,
    tibdouble_t         value
);

/** @brief Set a double floating-point field in a mutable message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param value The call sets this double as the field value.
 *  
 * @return void
 *
 * @see tibMessage_SetDouble
 */
TIB_API
void
tibMessage_SetDoubleByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    tibdouble_t         value
);

/** @brief Clear a field in a mutable message.
 * 
 * Clearing a field clears the data from a field in the message
 * object, and flags the field so a subsequent send call does not
 * transmit it.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call clears the field in this message.
 * @param name The call clears the field with this name.
 *
 * @return void
 *
 * @see tibMessage_ClearFieldByRef
 */
TIB_API
void
tibMessage_ClearField(
    tibEx               e,
    tibMessage          message,
    const char*         name
);

/** @brief Clear a field in a mutable message.
 * 
 * Clearing a field clears the data from a field in the message
 * object, and flags the field so a subsequent send call does not
 * transmit it.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call clears the field in this message.
 * @param fieldRef The call clears the value of this field.
 *
 * @return void
 *
 * @see tibMessage_ClearField
 */
TIB_API
void
tibMessage_ClearFieldByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef
);

/** @brief Clear all fields in a mutable message.
 * 
 * After clearing all fields, you can re-use the message.  The message
 * format does not change.
 *
 * This call is more efficient than creating a new empty message of
 * the same format.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call clears all fields in this message.
 *  
 * @return void
 */
TIB_API
void
tibMessage_ClearAllFields(
    tibEx               e,
    tibMessage          message
);

/** @brief Set an array field in a mutable message.
 * 
 * When the array contains strings or message objects, this call \b
 * copies the data into the message field.  The program may modify (or
 * free) the values array, and the strings or messages in it, after
 * this call returns.
 * 
 * If a program sets a field to contain an array of message objects,
 * destroying the outer message does not automatically destroy the
 * sub-messages.
 * Before destroying the enclosing message, you must first explicitly
 * destroy sub-messages within arrays.
 * 
 * This table lists the valid array types.
 *        <TABLE>
 *         <TR>
 *          <TH> Array Type Constant </TH> <TH> Values Array Pointer Type </TH>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_LONG_ARRAY </TD> <TD> tibint64_t* </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DOUBLE_ARRAY </TD> <TD> double*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_STRING_ARRAY  </TD> <TD> char** </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_MESSAGE_ARRAY </TD> <TD> tibMessage*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DATETIME_ARRAY </TD> <TD> tibDateTime*  </TD>
 *         </TR>
 *        </TABLE>
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param arrayType The program supplies a values array of this type.
 * @param name The call sets the value of the field with this name.
 * @param values The call sets the field value to the array at this location.
 *              (Null is not a legal value within a string array.)
 * @param arraySize The program supplies the number of elements in the array.
 *  
 * @return void
 *
 * @see tibMessage_SetArrayByRef
 */
TIB_API
void
tibMessage_SetArray(
    tibEx               e,
    tibMessage          message,
    tibFieldType        arrayType,
    const char*         name,
    const void* const   values,
    tibint32_t          arraySize
);

/** @brief Set an array field in a mutable message.
 * 
 * When the array contains strings or message objects, this call \b
 * copies the data into the message field.  The program may modify (or
 * free) the values array, and the strings or messages in it, after
 * this call returns.
 * 
 * If a program sets a field to contain an array of message objects,
 * destroying the outer message does not automatically destroy the
 * sub-messages.
 * Before destroying the enclosing message, you must first explicitly
 * destroy sub-messages within arrays.
 * 
 * This table lists the valid array types.
 *        <TABLE>
 *         <TR>
 *          <TH> Array Type Constant </TH> <TH> Values Array Pointer Type </TH>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_LONG_ARRAY </TD> <TD> tibint64_t* </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DOUBLE_ARRAY </TD> <TD> double*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_STRING_ARRAY  </TD> <TD> char** </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_MESSAGE_ARRAY </TD> <TD> tibMessage*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DATETIME_ARRAY </TD> <TD> tibDateTime*  </TD>
 *         </TR>
 *        </TABLE>
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param arrayType The program supplies a values array of this type.
 * @param fieldRef The call sets the value of this field.
 * @param values The call sets the field value to the array at this location.
 *              (Null is not a legal value within a string array.)
 * @param arraySize The program supplies the number of elements in the array.
 *  
 * @return void
 *
 * @see tibMessage_SetArray
 */
TIB_API
void
tibMessage_SetArrayByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldType        arrayType,
    tibFieldRef         fieldRef,
    const void* const   values,
    tibint32_t          arraySize
);

/** @brief Get the value of an array field from a message.
 * 
 * The program must cast the return value appropriately.  The values
 * array is valid only for the lifetime of the message.  Programs must
 * not modify or free the array.
 * 
 * This table lists the valid array types.
 *        <TABLE>
 *         <TR>
 *          <TH> Array Type Constant </TH> <TH> Values Array Pointer Type </TH>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_LONG_ARRAY </TD> <TD> tibint64_t* </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DOUBLE_ARRAY </TD> <TD> double*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_STRING_ARRAY  </TD> <TD> char** </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_MESSAGE_ARRAY </TD> <TD> tibMessage*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DATETIME_ARRAY </TD> <TD> tibDateTime*  </TD>
 *         </TR>
 *        </TABLE>
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param arrayType The program supplies the array type of the field.
 * @param name The call gets the value of the field with this name.
 * @param arraySize The call stores the number of array elements in this location.
 *  
 * @return void*  
 * The program must cast this return value to the appropriate pointer
 * type (see the table above).
 *
 * @see tibMessage_GetArrayByRef
 */
TIB_API
void*
tibMessage_GetArray(
    tibEx            e,
    tibMessage       message,
    tibFieldType     arrayType,
    const char*      name,
    tibint32_t       *arraySize
);


/** @brief Get the value of an array field from a message.
 * 
 * The program must cast the return value appropriately.  The values
 * array is valid only for the lifetime of the message.  Programs must
 * not modify or free the array.
 * 
 * This table lists the valid array types.
 *        <TABLE>
 *         <TR>
 *          <TH> Array Type Constant </TH> <TH> Values Array Pointer Type </TH>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_LONG_ARRAY </TD> <TD> tibint64_t* </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DOUBLE_ARRAY </TD> <TD> double*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_STRING_ARRAY  </TD> <TD> char** </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_MESSAGE_ARRAY </TD> <TD> tibMessage*  </TD>
 *         </TR>
 *         <TR>
 *          <TD> TIB_FIELD_TYPE_DATETIME_ARRAY </TD> <TD> tibDateTime*  </TD>
 *         </TR>
 *        </TABLE>
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param arrayType The program supplies the array type of the field.
 * @param fieldRef The call gets the value of this field.
 * @param arraySize The call stores the number of array elements in this location.
 *  
 * @return void* 
 * The program must cast this return value to the appropriate pointer
 * type (see the table above).
 *
 * @see tibMessage_GetArray
 */
TIB_API
void*
tibMessage_GetArrayByRef(
    tibEx            e,
    tibMessage       message,
    tibFieldType     arrayType,
    tibFieldRef      fieldRef,
    tibint32_t       *arraySize
);

/** @brief Determine whether a field is set in a message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call checks in this message.
 * @param name The call checks for a field with this name.
 *  
 * @return tibbool_t
 *
 * @see tibMessage_IsFieldSetByRef
 */
TIB_API
tibbool_t
tibMessage_IsFieldSet(
    tibEx            e,
    tibMessage       message,
    const char*      name
);

/** @brief Determine whether a field is set in a message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call checks in this message.
 * @param fieldRef The call checks for this field.
 *  
 * @return tibbool_t
 *
 * @see tibMessage_IsFieldSet
 */
TIB_API
tibbool_t
tibMessage_IsFieldSetByRef(
    tibEx            e,
    tibMessage       message,
    tibFieldRef      fieldRef
);

/** @brief Set an inbox field in a mutable message.
 * 
 * This call copies an inbox into a message field.  Programs that
 * receive the message can get the inbox, and send messages to it
 * (using @ref tibPublisher_SendToInbox).
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param inbox The call sets this inbox as the field value.
 *  
 * @return void
 *
 * @see tibSubscriber_CreateOnInbox
 * @see tibSubscriber_GetInbox
 *
 * @see tibMessage_SetInboxByRef
 */
TIB_API
void
tibMessage_SetInbox(
    tibEx               e,
    tibMessage          message,
    const char          *name,
    tibInbox            inbox);

/** @brief Set an inbox field in a mutable message.
 * 
 * This call copies an inbox into a message field.  Programs that
 * receive the message can copy the inbox, and send messages to it
 * (using @ref tibPublisher_SendToInbox).
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param inbox The call sets this inbox as the field value.
 *  
 * @return void
 *
 * @see tibSubscriber_CreateOnInbox
 * @see tibSubscriber_GetInbox
 *
 * @see tibMessage_SetInbox
 */
TIB_API
void
tibMessage_SetInboxByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    tibInbox            inbox);

/** @brief Get the value of an inbox field from a message.
 * 
 * This call deserializes the inbox value, caches the result with
 * the message object, and returns that cached inbox object.  The
 * inbox object is valid only for the lifetime of the message
 * (though programs may copy it).  Your
 * program must not modify nor destroy the inbox object.
 *
 * Calling this method repeatedly returns the same cached
 * inbox; it does not repeat the deserialization.
 *
 * Programs can send messages to the inbox
 * using @ref tibPublisher_SendToInbox.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param name The call gets the value of the field with this name.
 * 
 * @return tibInbox
 *
 * @see tibMessage_GetInboxByRef
 */
TIB_API
tibInbox
tibMessage_GetInbox(
    tibEx               e,
    tibMessage          message,
    const char          *name);

/** @brief Get the value of an inbox field from a message.
 * 
 * This call deserializes the inbox value, caches the result with
 * the message object, and returns that cached inbox object.  The
 * inbox object is valid only for the lifetime of the message
 * (though programs may copy it).  Your
 * program must not modify nor destroy the inbox object.
 *
 * Calling this method repeatedly returns the same cached
 * inbox; it does not repeat the deserialization.
 *
 * Programs can send messages to the inbox
 * using @ref tibPublisher_SendToInbox.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param fieldRef The call gets the value of this field.
 * 
 * @return tibInbox
 *
 * @see tibMessage_GetInbox
 */
TIB_API
tibInbox
tibMessage_GetInboxByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef);

/** @brief Set a DateTime field in a mutable message.
 * 
 * This call copies a DateTime value into
 * a message field of type \c TIB_FIELD_TYPE_DATETIME.
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param dateTime The call sets this DateTime as the field value.
 *  
 * @return void
 *
 * @see tibMessage_SetDateTimeByRef
 */
TIB_API
void
tibMessage_SetDateTime(
    tibEx               e,
    tibMessage          message,
    const char          *name,
    const tibDateTime   *dateTime);

/** @brief Set a DateTime field in a mutable message.
 * 
 * This call copies a DateTime value into
 * a message field of type \c TIB_FIELD_TYPE_DATETIME.
 *
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param dateTime The call sets this DateTime as the field value.
 *  
 * @return void
 *
 * @see tibMessage_SetDateTime
 */
TIB_API
void
tibMessage_SetDateTimeByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    const tibDateTime   *dateTime);

/** @brief Get the value of a DateTime field from a message.
 * 
 * The program must not destroy the \c tibDateTime struct because the message owns it.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param name The call gets the value of the field with this name.
 * 
 * @return tibDateTime
 *
 * @see tibMessage_GetDateTimeByRef
 */
TIB_API
tibDateTime*
tibMessage_GetDateTime(
    tibEx               e,
    tibMessage          message,
    const char          *name);

/** @brief Get the value of a DateTime field from a message.
 * 
 * The program must not destroy the \c tibDateTime struct because the message owns it.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field from this message.
 * @param fieldRef The call gets the value of this field.
 * 
 * @return tibDateTime
 *
 * @see tibMessage_GetDateTime
 */
TIB_API
tibDateTime*
tibMessage_GetDateTimeByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef);

/** @brief Set a sub-message field in a mutable message.
 *
 * This call copies the sub-message into the enclosing message field.
 *
 * Do not set a message as a sub-message of itself (at any level of nesting).
 *
 * Before destroying the enclosing message, you must first explicitly
 * destroy the inner sub-message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param name The call sets the value of the field with this name.
 * @param value The call sets this sub-message as the field value.
 *
 * @return void
 *
 * @see tibMessage_SetMessageByRef
 */
TIB_API
void
tibMessage_SetMessage(
    tibEx               e,
    tibMessage          message,
    const char*         name,
    tibMessage          value);

/** @brief Set a sub-message field in a mutable message.
 * 
 * This call copies the sub-message into the enclosing message field.
 *
 * Do not set a message as a sub-message of itself (at any level of nesting).
 * 
 * Before destroying the enclosing message, you must first explicitly
 * destroy the inner sub-message.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call sets the field in this message.
 * @param fieldRef The call sets the value of this field.
 * @param value The call sets this sub-message as the field value.
 *
 * @return void
 *
 * @see tibMessage_SetMessage
 */
TIB_API
void
tibMessage_SetMessageByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef,
    tibMessage          value);

/** @brief Get the value of a message field from a message.
 * 
 * This call deserializes the sub-message value, caches the result
 * with the message object, and returns a pointer to that sub-message.
 * The sub-message is valid only for the lifetime of the parent
 * message.

 * The program must not modify nor destroy the sub-message.  The FTL library
 * destroys the sub-message when it destroys the parent message
 * (usually when the callback returns).
 *
 * Calling this method repeatedly returns the same cached
 * sub-message; it does not repeat the deserialization.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field in this message.
 * @param name The call gets the value of the field with this name.
 *  
 * @return tibMessage
 *
 * @see tibMessage_GetMessageByRef
 */
TIB_API
tibMessage
tibMessage_GetMessage(
    tibEx               e,
    tibMessage          message,
    const char*         name
);

/** @brief Get the value of a message field from a message.
 * 
 * This call deserializes the sub-message value, caches the result
 * with the message object, and returns a pointer to that sub-message.
 * The sub-message is valid only for the lifetime of the parent
 * message.
 * The program must not modify nor destroy the sub-message.  The FTL library
 * destroys the sub-message when it destroys the parent message
 * (usually when the callback returns).
 *
 * Calling this method repeatedly returns the same cached
 * sub-message; it does not repeat the deserialization.
 *
 * @param e The exception object captures information about failures.
 * @param message The call gets the field in this message.
 * @param fieldRef The call gets the value of this field.
 *  
 * @return tibMessage
 *
 * @see tibMessage_GetMessage
 */
TIB_API
tibMessage
tibMessage_GetMessageByRef(
    tibEx               e,
    tibMessage          message,
    tibFieldRef         fieldRef
);

/** @brief Create a message from a byte-array representation.
 * 
 * This call reconstitutes a message object from its serialized
 * byte-array representation.
 * 
 * After this call returns, you may re-use or destroy the byte-array.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The call gets format information from this realm.
 * @param byteArray The call reads this byte-array representation of the message.
 * @param byteArraySize The program supplies the size of the byte-array (in bytes).
 *  
 * @return tibMessage a new message, equivalent to the byte-array representation
 *
 * @see tibMessage_WriteToByteArray
 * @see tibMessage_WriteToPortableByteArray
 */
TIB_API
tibMessage
tibMessage_ReadFromByteArray(
    tibEx              e,
    tibRealm           realm,
    const tibuint8_t*  byteArray,
    tibint32_t         byteArraySize);

/** @brief Write a serialized representation of a message to a byte-array.
 * 
 * Programmers estimate the required size of the byte-array buffer.
 * If the buffer is too small, the call fails, but returns the actual
 * required size.  The program can use this information to supply a
 * buffer of the required size in the second call.
 * 
 * Programming Strategy:<ol>
 * <li> Create an array you think is big enough. </li>
 * <li> Call tibMessage_WriteToByteArray. </li>
 * <li> If the returned size is bigger than your array, then call
 *      tibMessage_WriteToByteArray again with a larger array (at least the
 *      returned size). </li>
 * </ol>
 *
 * @param e The exception object captures information about failures.
 * @param message The call serializes this message.
 * @param byteArray The call writes the serialized representation into this byte-array buffer.
 * @param byteArraySize The program supplies the size of the buffer (in bytes).
 *  
 * @return the actual size of the serialized representation (in bytes)
 * 
 * @see tibMessage_WriteToPortableByteArray
 * @see tibMessage_ReadFromByteArray
 */
TIB_API
tibint32_t
tibMessage_WriteToByteArray(
    tibEx              e,
    tibMessage         message,
    tibuint8_t*        byteArray,
    tibint32_t         byteArraySize);

/** @brief Write a self-describing serialized representation of a message to
 * a byte-array.
 * 
 * This call writes a portable form of the message, which you can use
 * outside the originating realm.  The portable form is larger than
 * optimized forms, because it includes all format metadata (as with a
 * dynamic format).
 *
 * Programmers estimate the required size of the byte-array buffer.
 * If the buffer is too small, the call fails, but returns the actual
 * required size.  The program can use this information to supply a
 * buffer of the required size in the second call.
 * 
 * Programming Strategy:<ol>
 * <li> Create an array you think is big enough. </li>
 * <li> Call tibMessage_WriteToPortableByteArray. </li>
 * <li> If the returned size is bigger than your array, then call
 *      tibMessage_WriteToPortableByteArray again with a larger array (at least the
 *      returned size). </li>
 * </ol>
 *
 * @param e The exception object captures information about failures.
 * @param message The call serializes this message.
 * @param byteArray The call writes the serialized representation into this byte-array buffer.
 * @param byteArraySize The program supplies the size of the buffer (in bytes).
 *  
 * @return the actual size of the serialized representation (in bytes)
 * 
 * @see tibMessage_WriteToByteArray
 * @see tibMessage_ReadFromByteArray
 */
TIB_API
tibint32_t
tibMessage_WriteToPortableByteArray(
    tibEx              e,
    tibMessage         message,
    tibuint8_t         *byteArray,
    tibint32_t         byteArraySize);

/** @brief Get a printable string that describes the contents of a
 * message.
 * 
 * Programmers estimate the required size of the string buffer.  If
 * the buffer is too small (or \c NULL), the call fails, but returns
 * the actual required size.  The program can use this information to
 * supply a buffer of the required size in the second call.
 *
 * @param e The exception object captures information about failures.
 * @param message The call produces a string description of this message.
 * @param buffer The call stores the description in this string buffer.
 * @param size The program supplies the length of buffer (in bytes).
 *  
 * @return the actual length of the description string, including
 * terminating \c NULL character (in btyes)
 */
TIB_API
tibint32_t
tibMessage_ToString(
    tibEx              e,
    tibMessage         message,
    char               *buffer,
    tibint32_t         size);

/** @brief Get the type of a field within a message.
 *
 *  @param e The exception object captures information about failures.
 *  @param message The call finds the field within this message.
 *  @param fieldRef The call gets the type of this field.
 *
 * @return the type of the field
 */
TIB_API
tibFieldType
tibMessage_GetFieldTypeByRef(
    tibEx       e,
    tibMessage  message,
    tibFieldRef fieldRef);

/** @brief Get the type of a field within a message.
 *
 *  @param e The exception object captures information about failures.
 *  @param message The call finds the field within this message.
 *  @param name The call gets the type of the field with this name.
 *
 * @return the type of the field
 */
TIB_API
tibFieldType
tibMessage_GetFieldType(
    tibEx       e,
    tibMessage  message,
    const char* name);

/** @brief Create a message iterator object.
 * 
 * @param e The exception object captures information about failures.
 * @param message The call initializes an iterator for the fields of this message.
 *  
 * @return a new message iterator object
 * 
 * @see tibMessageIterator
 */
TIB_API
tibMessageIterator
tibMessageIterator_Create(
    tibEx               e,
    tibMessage          message);

/** @brief Reset a message iterator object.
 * 
 * This call resets a message iterator to the beginning of its message
 * (that is, the first field of the message).
 *
 * @param e The exception object captures information about failures.
 * @param messageIterator The call resets this message iterator.
 *  
 * @return void
 */
TIB_API
void
tibMessageIterator_Reset(
    tibEx               e,
    tibMessageIterator  messageIterator);


/** @brief Determine whether a message iterator object can present another field.
 * 
 * \li This call returns \c true if a message iterator is ready to present
 * another field.
 * \li This call returns \c false if the iterator has presented the last
 * field (and has not been reset).
 *
 * @param e The exception object captures information about failures.
 * @param messageIterator The call checks this iterator.
 *
 * @return @ref tibbool_t
 */
TIB_API
tibbool_t
tibMessageIterator_HasNext(
    tibEx              e,
    tibMessageIterator messageIterator);

/** @brief Destroy a message iterator object.
 * 
 * Destroying a message iterator object frees all the resources
 * associated with it, but does not affect the message object.
 * 
 * @param e The exception object captures information about failures.
 * @param messageIterator The call destroys this message iterator object.

 * @return @ref tibMessageIterator
 */
TIB_API
void
tibMessageIterator_Destroy(
    tibEx               e,
    tibMessageIterator  messageIterator);

/** @brief Get the next field of a message iterator object.
 *
 * This call advances the iterator to the next field of its message,
 * and returns a field reference object, which refers to that next
 * field.  Programs can use that field reference object  to get the
 * field's name, type and value.
 *
 * \b Warning: For efficiency, an iterator object \e reuses a single
 * field reference object.  Iterator calls \e overwrite that field
 * reference object.  Programs can use the field reference object with
 * correct results \e only until the next call that changes the state
 * of the iterator.  That is, when you get the next field, reset the
 * iterator, or destroy the iterator, then the field reference object
 * changes too (which also invalidates any field name string you might
 * have previously extracted from the field reference).  Although
 * field reference objects are usually thread-safe, this one is \e not
 * thread-safe.  Your program must \e not destroy this field reference
 * object.
 *
 * If the iterator has already presented the last field of the
 * message, then this call throws the exception @ref TIB_NOT_FOUND.
 *
 * @param e The exception object captures information about failures.
 * @param messageIterator The call advances this iterator to the next field.
 *
 * @return a field reference to the next field
 */
TIB_API
tibFieldRef
tibMessageIterator_GetNext(
    tibEx              e,
    tibMessageIterator messageIterator);

/** @brief Explicitly acknowledge a message.
 *
 * When a durable subscriber object specifies explicit
 * acknowledgement, the application program must acknowledge each
 * message to the durable by calling this method.
 *
 * When a durable subscriber object does \e not specify explicit
 * acknowledgement, the FTL library automatically acknowledges the
 * message when the application callback method returns.
 *
 * For any message that arrives through a non-durable subscriber,
 * this call returns without error (and without action).
 *
 *
 * @param e The exception object captures information about failures.
 * @param message The call acknowledges receiving this message.
 *
 * @return void
 *
 * @see TIB_SUBSCRIBER_PROPERTY_BOOL_EXPLICIT_ACK
 */
TIB_API
void
tibMessage_Acknowledge(
    tibEx              e,
    tibMessage         message);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_msg_h */
