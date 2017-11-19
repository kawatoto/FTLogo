/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: except.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_except_h
#define _INCLUDED_tib_except_h

#include "tib/tibexp.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file except.h
 * 
 * @brief Exceptions capture error information during FTL calls.
 *
 * This file defines FTL exception objects, calls that manipulate
 * them, and the error codes that can appear within them.
 */

/**********************
 * Public header file *
 **********************/
typedef int                     tibErrorCode;

/*
 * Common error codes.
 */

/** @name Error Codes
 *
 * FTL exceptions can report these error and status codes.
 */
//@{

/** @brief The program supplied \c NULL instead of an exception object.
 *
 * Some calls require an actual exception object.\n
 * Other calls allow \c NULL as a special value.
 */
#define TIB_NULL_EXCEPTION              ((tibErrorCode) -1)

/** @brief The FTL call completed correctly (success) -- that is, no
 * errors occurred in executing the FTL call.  The return value is
 * reliable.
 */
#define TIB_OK                          ((tibErrorCode)  0)

/** @brief Invalid argument (or invalid value embedded in an argument).

    Common Diagnoses:

    \li A \c NULL argument (where \c NULL is not acceptable).

    \li A nonsensical value.

    \li A field or property value does not match the required data type.

    \li A value is outside the range of legal values.

    \li An object that is invalid (that is, it has already been
        destroyed or closed).

    \li The realm object is closed.
 */
#define TIB_INVALID_ARG                 ((tibErrorCode)  1)

/** @brief Memory allocation failed.
 */
#define TIB_NO_MEMORY                   ((tibErrorCode)  2)

/** @brief Internal timeout elapsed.

    Common Diagnoses:

    \li Cannot connect to the realm server within a reasonable time
        period.

    \li A transport failed to start within a reasonable time period.
 */
#define TIB_TIMEOUT                     ((tibErrorCode)  4)

/** @brief The program has not yet called @ref tib_Open to start FTL.
 */
#define TIB_NOT_INITIALIZED             ((tibErrorCode)  5)

/** @brief An operating system call failed.

    Common Diagnoses:

    \li File access failed.

    \li Socket access failed.
 */
#define TIB_OS_ERROR                    ((tibErrorCode)  6)

/** @brief Thread internal interrupt.
 */
#define TIB_INTR                        ((tibErrorCode)  7)

/** @brief Invalid operation (on a valid object).

    Common Diagnoses:

    \li A program attempted to destroy a message that the program did
    not create (that is, the FTL library created it).

    \li A program attempted to unsubscribe a static durable.
    (Unsubscribe applies only to dynamic durables.)

    \li A program attempted to unsubscribe a dynamic durable,
    but the endpoint does not support dynamic durables.
 */
#define TIB_NOT_PERMITTED               ((tibErrorCode)  8)

/** @brief Cannot find a requested object.

    Common Diagnoses:

    \li A property doesn't exist in a properties object.

    \li A transport doesn't exist in the realm.

    \li A field is not set in the message, or not defined in the
        format.

    \li A message iterator has already presented the last field of its
        message.

    \li Cannot connect to the realm server at the URL supplied.

    \li Cannot remove a subscriber from a queue because it is not
        associated with that queue.

    \li Cannot destroy a timer because it is not associated with the
        queue.
 */
#define TIB_NOT_FOUND                   ((tibErrorCode)  9)

/** @brief An action is inconsistent with internal state.

    Common Diagnoses:

    \li When a \e transport is associated with an inline event queue, it
        cannot be associated with any other event queue.
        Adding a subscriber to a queue can violate this restriction,
        triggering this exception.
        For a detailed explanation, see the book <b> TIBCO FTL Development</b>.

    \li The action would violate a low-level transport constraint.

 */
#define TIB_ILLEGAL_STATE               ((tibErrorCode) 10)

/** @brief Unsupported datatype or feature.

    Common Diagnoses:

    \li A message from a later release of FTL contains a field datatype
        that this version does not support (that is, it cannot
        manipulate the value, nor resend it).
 */

#define TIB_NOT_SUPPORTED               ((tibErrorCode) 11)

/** @brief End of buffer while parsing an inbound message.

    Common Diagnoses:

    \li Message data is corrupted.
 */
#define TIB_END_OF_BUFFER               ((tibErrorCode) 12)

/** @brief Incompatible versions of FTL components.

    Common Diagnoses:

    \li Your program compiled and linked different compatibility
        versions of FTL; see @ref tib_Open and @ref
        TIB_COMPATIBILITY_VERSION.

    \li The FTL client library is not compatible with the realm server.
 */
#define TIB_VERSION_MISMATCH            ((tibErrorCode) 13)

/** @brief Conflict with an existing object, value or definition.

    Common Diagnoses:

    \li Adding or setting a value conflicts with an existing value.

    \li Cannot add a subscriber to an event queue,
        because that subscriber is already associated with another queue.

    \li Erroneous realm configuration.  Ask your administrator for assistance.

    \li Cannot create a subscriber on a non-shared durable because
        another subscriber on the same durable already exists within the
        same client process.

 */
#define TIB_ALREADY_EXISTS              ((tibErrorCode) 14)

/** @brief I/O error.

    Common Diagnoses:

    \li Error accessing shared memory.

    \li Error accessing file system.
 */
#define TIB_FILE_IO_ERROR               ((tibErrorCode) 15)

/** @brief FTL internal error.

    Common Diagnoses:

    \li FTL client cannot deserialize a malformed message.

    \li FTL internal I/O received a malformed packet.

    \li FTL client received a malformed realm definition.
 */
#define TIB_INVALID_VALUE               ((tibErrorCode) 16)

/** @brief Field datatype mismatch.

    Common Diagnoses:

    \li A message field access call received a datatype indicator that
        does not match the actual datatype of the field's value.

    \li A message field access call received a datatype indicator that
        does not represent an FTL field datatype.
 */
#define TIB_INVALID_TYPE                ((tibErrorCode) 17)

/** @brief Invalid or contradictory property values.

    Common Diagnoses:

    \li Configuration mismatch among event queue discard policy
        properties.

    \li A program attempted to create a dynamic durable
        but the endpoint is not associated with a store.

    \li A program attempted to unsubscribe a dynamic durable
        but the endpoint is not associated with a store.

    \li An argument to an API call does not support the action of that call.
 */
#define TIB_INVALID_CONFIG              ((tibErrorCode) 18)

/** @brief The FTL client library encountered a message with an invalid format.

    Common Diagnoses:

    \li The library received a malformed message.
    \li The program supplied a malformed message to tibMessage_ReadFromByteArray.
 */
#define TIB_INVALID_FORMAT              ((tibErrorCode) 19)

/** @brief The program attempted an operation on an unusable realm object.

    Common Diagnoses:

    \li The program has closed the realm object.
    \li The administrator has disabled the application.
 */
#define TIB_CLIENT_SHUTDOWN             ((tibErrorCode) 20)

/** @brief A resource required by the client program is unavailable.

    Common Diagnoses:

    \li A persistent application cannot access its store.
 */
#define TIB_RESOURCE_UNAVAILABLE        ((tibErrorCode) 21)

/** @brief A resource cannot accept data because it has reached a limit.

    Common Diagnoses:

    \li A persistent store is full.
 */
#define TIB_LIMIT_REACHED               ((tibErrorCode) 22)

/** @brief A format required by the client program is unavailable.

    Common Diagnoses:

    \li The format has not been made available to an application as a preload format.
 */
#define TIB_FORMAT_UNAVAILABLE          ((tibErrorCode) 23)

/**
    TODO:  Document
*/
#define TIB_EXCEPTION           ((tibErrorCode) 100)

/**
    TODO: Document
*/
#define TIB_UNKNOWN_SYSPROP     ((tibErrorCode) 101)

//@}


/** @name Realm Server Error Codes
 *
 * These exceptions occur primarily within the realm server. \n
 * The client library may throw these exceptions.
 */
//@{

/** @brief Internal error.
 */
#define TIB_RS_UNKNOWN_ERROR             ((tibErrorCode) 1000)

/** @brief A client program or administrative utility supplied invalid
    authentication credentials.
 */
#define TIB_RS_INVALID_SESSION_USER      ((tibErrorCode) 1001)

/** @brief An administrative utility sent an invalid command.

    Common Diagnoses:

    \li A newer version of the administrative utility sent a command
        that an older version of the realm server does not recogize.
 */
#define TIB_RS_UNKNOWN_OPERATION_CODE    ((tibErrorCode) 1002)

/** @brief Internal error.

    Review the server log for details.
 */
#define TIB_RS_INTERNAL_SERVER_ERROR     ((tibErrorCode) 1003)

/** @brief The communication protocol between the realm server and its
           clients has changed.

    Common Diagnoses:

    \li A significant version difference between the realm server and
        the FTL client library prevents communcation.
 */
#define TIB_RS_PROTOCOL_MISMATCH         ((tibErrorCode) 1004)

/** @brief A client or utility omitted the protocol version from its
           intial handshake message.
 */
#define TIB_RS_MISSING_PROTOCOL          ((tibErrorCode) 1005)

/** @brief A client or utility omitted its client identifier from a
           protocol message.
 */
#define TIB_RS_MISSING_CLIENT_ID         ((tibErrorCode) 1006)

/** @brief A client requested a value that is not in the database.
 */
#define TIB_RS_KEY_NOT_FOUND             ((tibErrorCode) 1007)

/** @brief The server must be paused before attempting this operation.
 */
#define TIB_RS_SERVER_ISNT_PAUSED        ((tibErrorCode) 1008)

/** @brief The server is in a read-only state, so it cannot accept
    configuration changes.

    Read-only states include paused and secondary.
 */
#define TIB_RS_SERVER_IS_READONLY        ((tibErrorCode) 1009)

/** @brief The server does not accept requests during its exit
    sequence.
 */
#define TIB_RS_SERVER_IS_SHUTTING_DOWN   ((tibErrorCode) 1010)

/** @brief The server cannot accept requests until it completes its
    initialization sequence.
 */
#define TIB_RS_SERVER_IS_STARTING_UP     ((tibErrorCode) 1011)

/** @brief Fault-tolerant servers have different generations of the
    realm configuration.
 */
#define TIB_RS_DB_GENERATION_MISMATCH    ((tibErrorCode) 1012)

/** @brief The server is in the wrong state for the requested operation.
 */
#define TIB_RS_INVALID_STATE             ((tibErrorCode) 1013)

/** @brief The server cannot resume in the requested mode.
 */
#define TIB_RS_INVALID_MODE              ((tibErrorCode) 1014)

/** @brief The requested administrative operation failed.
 */
#define TIB_RS_ADMIN_OPERATION_FAILED    ((tibErrorCode) 1015)

/** @brief Clients cannot connect while an update is in progress.
 *         Try to connect again after the update is complete.
 */
#define TIB_RS_UPDATE_IN_PROGRESS        ((tibErrorCode) 1016)

/** @brief Client version is not compatible with configuration.
 */
#define TIB_RS_INCOMPATIBLE_CLIENT       ((tibErrorCode) 1018)

//@}



/* @brief FTL exception object.
 */
typedef struct tibExStruct *tibEx;

/** @brief Create an exception object.
 * 
 * An exception object captures information about failures within FTL
 * API calls.
 * <ol>
 *  <li> A program creates an exception object. </li>
 *
 *  <li> The program passes a cleared exception object as the first
 *       parameter to an FTL call. </li>
 *
 *  <li> The FTL call stores a return code in the exception object
 *       before the call returns. </li>
 *
 *  <li> The program \e must check the return code before using any
 *       return value or output parameter from the call.  If the code
 *       represents an error (that is, any code other than @ref
 *       TIB_OK), then values from the call are unreliable. </li>
 *
 * </ol>
 *
 * When an FTL call receives an exception argument that already
 * indicates an error, the call returns immediately.  That is, the
 * call has no effect, and the exception's error code remains
 * unchanged.
 *
 * To clear an exception's error code (so that calls that receive it
 * can operate correctly), use @ref tibEx_Clear.
 *
 * Exception objects are not thread-safe.  Programs must not access an
 * exception object simultaneously from different threads.
 *  
 * @return a new exception object
 */
TIB_IMPORT
tibEx
tibEx_Create(
    void);

/** @brief Destroy an exception object.
 * 
 * @param e The call destroys this exception object.
 *  
 * @return void
 */
TIB_IMPORT
void
tibEx_Destroy(
    tibEx e);

/** @brief Clear an exception object.
 * 
 * When an FTL call receives an exception argument that already
 * indicates an error, the call returns immediately.  That is, the
 * call has no effect, and the exception's error code remains
 * unchanged.
 *
 * Your code can clear an exception before passing it to another FTL
 * call, so that call can operate correctly.
 *
 * @param e The call clears this exception object.
 *  
 * @return void
 */
TIB_IMPORT
void
tibEx_Clear(
    tibEx e);

/** @brief Get the error code from an exception object.
 *
 * @ref TIB_OK indicates that the call returned correctly.
 * 
 * If the exception is \c NULL, this call returns \c
 * TIB_NULL_EXCEPTION, indicating that your program erroneously passed
 * \c NULL as the exception argument (instead of an actual exception
 * object).
 * 
 * @param e The call returns the error code from this exception object.
 *  
 * @return the error code
 */
TIB_IMPORT
tibErrorCode
tibEx_GetErrorCode(
    tibEx e);

/** @brief Get the summary string from an exception object.
 * 
 * This call returns a brief NULL-terminated string that describes the
 * exception object's error code.
 * 
 * @param e The call gets the summary string from this exception object.
 *  
 * @return a pointer to the summary string
 */
TIB_IMPORT
const char*
tibEx_GetSummaryString(
    tibEx e);

/** @brief Get the detailed description of an exception object.
 * 
 * Programs can output this description to end users, administrators
 * or log files.
 *
 * The description string includes this information:
 * \li Error code number
 * \li Summary string (see @ref tibEx_GetSummaryString)
 * \li Detail about the error (could be empty)
 * \li Name of the thread in which the error occurred
 * \li Stack trace
 * 
 * If a program passes \c NULL as the buffer argument, this call
 * returns the length of the description string, without actually
 * writing it.
 *
 * If the non-null buffer is too short for the entire description
 * string, the call writes to the end of the buffer, truncating the
 * string, and returns the actual number of bytes in the buffer.
 *
 * @param e The call returns a string corresponding to this exception object.
 * @param buffer The program supplies a string location,
 *               into which the call stores the description.\n
 *               We recommend a buffer of at least 2 kilobytes.
 * @param buflen The program supplies the length of the buffer (in bytes).
 * 
 * @return \li On success, this call returns the actual length of the
 *             description string (that is, the actual number of bytes
 *             the call wrote into the buffer), including the terminating
 *             \c NULL character.
 *         \li On failure, this
 *             call returns zero, indicating failure (and the buffer
 *             remains unchanged).
 */
TIB_IMPORT
int
tibEx_ToString(
    tibEx       e,
    char        *buffer,
    int         buflen);

#ifdef  __cplusplus
}
#endif

#endif /* _INCLUDED_tib_except_h */
