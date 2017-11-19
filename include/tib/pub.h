/*
 * Copyright (c) 2009-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: pub.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_pub_h
#define _INCLUDED_tib_pub_h

#include "tib/msg.h"
#include "tib/types.h"
#include "tib/inbox.h"
#include "tib/props.h"
#include "tib/tibexp.h"
#include "tib/tiblock.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file pub.h
 * 
 * @brief Publisher objects send messages.
 *
 * This file defines publisher objects and calls that send messages.
 */


/** @brief A publisher object realizes the message-sending
 *  functionality of an endpoint within a program.
 *
 * A publisher object encapsulates all the information that a program
 * needs to send a stream of messages.  Publisher objects use the
 * transport and format resources associated with an endpoint (as
 * defined in the realm).
 *
 * Programs can use a publisher to send messages one-to-many, or
 * one-to-one.
 */
typedef struct __tibPublisherId     *tibPublisher;

typedef struct __tibDirectPublisherId   *tibDirectPublisher;

/** @brief Ownership of outbound messages; boolean.
 *
 * When \c tibfalse or absent, client program code retains ownership of
 * outbound messages that it sends through this publisher.
 *
 * When \c tibtrue, the FTL library accepts ownership of outbound messages
 * that the client sends through this publisher.  Client code releases
 * the message object when it invokes the send call; from that moment
 * onward, client code must \em not reference the message object,
 * because the FTL library can destroy the message object at any time.
 *
 * Client code can release a message only if client owns the message.
 * In particular, if a subscriber does <i>not</i> release its messages
 * to the callback,
 * then the callback must <i>not</i> send it
 * through a publisher that releases messages.
 * (See @ref TIB_SUBSCRIBER_PROPERTY_BOOL_RELEASE_MSGS_TO_CALLBACK .)
 */
#define TIB_PUBLISHER_PROPERTY_BOOL_RELEASE_MSGS_TO_SEND "com.tibco.ftl.client.publisher.release"

/** @brief Create a publisher.
 * 
 * Applications use publisher objects to send messages.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration information
 *              about the application, endpoints, transports and formats.
 * @param endpointName The endpoint identifies the publisher's data stream
 *                     within the realm.
 *                     To use the default endpoint, supply \c NULL.
 * @param props Optional.  \c NULL to omit.\n
 *              Message ownership property:   <ul>
 *                  <li> @ref TIB_PUBLISHER_PROPERTY_BOOL_RELEASE_MSGS_TO_SEND
 *                    </ul>
 *
 * @return On success, this call returns a new publisher object.\n
 *         On failure, this call returns \c NULL.
 */
TIB_API
tibPublisher
tibPublisher_Create(
    tibEx               e,
    tibRealm            realm,
    const char          *endpointName,
    tibProperties       props
);

/** @brief Send a message, one-to-many.
 * 
 * @param e The exception object captures information about failures.
 * @param publisher The call sends the message using this publisher.
 * @param msg The call sends this message.
 *  
 * @return void
 */
TIB_API
void
tibPublisher_Send(
    tibEx               e,
    tibPublisher        publisher,
    tibMessage          msg
);

/** @brief Send an array of messages, one-to-many.
 * 
 * The application sends each message in the array, in order.
 *
 * Sending an array of messages eliminates the overhead cost of a
 * separate call for each message.
 *
 * @param e The exception object captures information about failures.
 * @param publisher The call sends the messages using this publisher.
 * @param msgCount The number of messages provided in the array.
 * @param msgs The call sends msgCount messages from this array.
 *  
 * @return void
 */
TIB_API
void
tibPublisher_SendMessages(
    tibEx               e,
    tibPublisher        publisher,
    tibint32_t          msgCount,
    tibMessage          *msgs
);

/** @brief Send an inbox message.
 * 
 * Because inbox messages do not trigger the content matchers of
 * other subscribers on the endpoint, they are an efficient way to
 * target a message to a specific destination.  For example, you might
 * use an inbox message to query a specific data source, or to reply
 * to a query.
 * 
 * For details about getting an inbox, see @ref tibMessage_GetInbox
 * 
 * @param e The exception object captures information about failures.
 * @param publisher The call sends the message using this publisher.
 * @param inbox The call sends the message to this inbox.
 * @param msg The call sends this message.
 *  
 * @return void
 */
TIB_API
void
tibPublisher_SendToInbox(
    tibEx               e,
    tibPublisher        publisher,
    tibInbox            inbox,
    tibMessage          msg
);

/** @brief Close a publisher.
 *
 * Closing a publisher releases all the resources associated with it
 * (for example, transport resources that are not used in other
 * endpoints).
 * 
 * @param e The exception object captures information about failures.
 * @param publisher The call closes this publisher.
 *  
 * @return void
 */
TIB_API
void
tibPublisher_Close(
    tibEx               e,
    tibPublisher        publisher
);

/** @brief Create a direct publisher.
 * 
 * Applications use direct publisher objects to send 
 * data buffers with low latency. 
 *
 * For a complete explanation, including best practices,
 * see "Direct Publishers and Subscribers"
 * in <i> TIBCO FTL Development</i>.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration information
 *              about the application, endpoints, and transports.
 * @param endpointName The endpoint identifies the publisher's data stream within the realm.
 * @param props Reserved for future use.\n
 *              To ensure forward compatibility,
 *              programmers must supply \c NULL.
 *
 * @return On success, this call returns a new direct publisher object.\n
 *         On failure, this call returns \c NULL.
 */
TIB_API
tibDirectPublisher
tibDirectPublisher_Create(
    tibEx               e,
    tibRealm            realm,
    const char          *endpointName,
    tibProperties       props);

/** @brief Close a direct publisher.
 * 
 * Closing a direct publisher invalidates all subsequent method calls,
 * and forces any blocked reserve calls to return.
 *
 * However, closing a direct publisher
 * does <i> not </i> free its memory.
 * For more information, see
 * "Programming with Direct Publishers"
 * in <i> TIBCO FTL Development</i>.
 * 
 * @param e The exception object captures information about failures.
 * @param publisher The call closes this direct publisher.
 */
TIB_API
void
tibDirectPublisher_Close(
    tibEx               e,
    tibDirectPublisher  publisher);

/** @brief Reserve a memory buffer for direct publishing.
 *
 * This call reserves two blocks of memory:
 * a data buffer and a size array.
 * The application can fill the memory buffer
 * with one or more data values,
 * and fill the size array with the corresponding sizes
 * (in bytes) of the data values.
 * (In the special case that the application indicates
 * only one data value,
 * this call does not reserve a size array.)
 *
 * A direct publisher can reserve only one buffer at a time.
 * Subsequent reserve calls in other threads block until
 * the application sends the reserved buffer.
 *
 * After calling reserve, the application must
 * send the reserved buffer before calling reserve again
 * within the same thread.
 *
 * To avoid blocking, which increases latency,
 * send the buffer soon after reserving it.
 *
 * The data buffer and the size array belong to the library.
 * 
 * @param e The exception object captures information about failures.
 * @param publisher The call reserves memory in this publisher.
 * @param count The application will write this number of data items
 *              into the data buffer.
 *              The call reserves a size array of this length.
 *              Supply a positive integer.
 * @param totalSize The call reserves a data buffer of this size (in bytes).
 * @param sizeArray The call also reserves a size array,
 *                 and stores the address of that array
 *                 in this pointer argument.
 *                 The application will fill the array with
 *                 the sizes of the data values.
 *                 If \p count is 1, indicating a single data item,
 *                 supply \a NULL.
 *  
 * @return A pointer to the data buffer.
 */
TIB_API
tibint8_t *
tibDirectPublisher_Reserve(
    tibEx               e,
    tibDirectPublisher  publisher,
    tibint64_t          count,
    tibint64_t          totalSize,
    tibint64_t          **sizeArray);

/** @brief Send the data in the reserved buffer.
 * 
 * This call makes the data available to direct subscribers.
 *
 * After this call returns, the publisher can reserve another buffer.
 * 
 * @param e The exception object captures information about failures.
 * @param publisher The call sends the reserved buffer of this publisher.
 */
TIB_API
void
tibDirectPublisher_SendReserved(
    tibEx               e,
    tibDirectPublisher  publisher);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_pub_h */
