/*
 * Copyright (c) 2009-2017 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: sub.h 91476 2017-02-10 20:11:42Z $
 */

#ifndef _INCLUDED_tib_sub_h
#define _INCLUDED_tib_sub_h

#include "tib/msg.h"
#include "tib/types.h"
#include "tib/tibexp.h"
#include "tib/conmatch.h"
#include "tib/inbox.h"
#include "tib/props.h"
#include "tib/tiblock.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file sub.h
 * 
 * @brief Subscribers express interest to receive inbound messages.
 *
 * This file defines subscriber objects and calls that manipulate them.
 */

/** @brief A subscriber object realizes the message-receiving
 *         functionality of an endpoint within a program.
 *
 * A subscriber object encapsulates all the information that a program
 * needs to subscribe to a stream of messages.  Subscriber objects use
 * the transport and format resources associated with an endpoint (as
 * defined in the realm).
 *
 * A subscriber does not receive messages
 * until you add it to an event queue.
 * 
 * \li A subscriber can also use an optional content matcher to filter
 * the message stream.  \li An \e inbox subscriber receives messages
 * sent to a specific inbox.
 */
typedef struct __tibSubscriberId        *tibSubscriber; 

typedef struct __tibDirectSubscriberId  *tibDirectSubscriber; 

/** @brief Static durable subscriber name property; string.
 *
 * To request messages from static durable in a persistence store, 
 * programs may supply a
 * subscriber name as the value of this property in a
 * @ref tibSubscriber_Create call.
 *
 * If @ref TIB_SUBSCRIBER_PROPERTY_STRING_DURABLE_NAME is present,
 * it overrides this property.
 *
 * If neither this property nor @ref TIB_SUBSCRIBER_PROPERTY_STRING_DURABLE_NAME
 * is present, then the create call uses the
 * default durable subscriber name (\c _default).
 */
#define TIB_SUBSCRIBER_PROPERTY_STRING_NAME     "com.tibco.ftl.client.subscriber.name"

/** @brief Durable name property; string.
 *
 * To request messages from a persistence store, supply a
 * durable name as the value of this property in a
 * @ref tibSubscriber_Create call.
 *
 * If a static or dynamic durable with this name already exists,
 * the new subscriber draws messages from that durable.
 *
 * Otherwise, the store creates a new dynamic durable with this name.
 *
 * If this property is present, it overrides the 
 * @ref TIB_SUBSCRIBER_PROPERTY_STRING_NAME property.
 */
#define TIB_SUBSCRIBER_PROPERTY_STRING_DURABLE_NAME     "com.tibco.ftl.client.durable.name"

/** @brief Explicit acknowledgement property; boolean.
 *
 * A durable subscriber can acknowledge message receipt to its durable
 * (in the persistence store) in either of two ways: <ul>
 *
 * <li> With \e automatic acknowledgement, the FTL library
 * automatically acknowledges the message when the application
 * callback returns.
 *
 * <li> With \e explicit acknowledgement, the application program must
 * call @ref tibMessage_Acknowledge to explicitly acknowledge each
 * message.    </ul>
 *
 * To enable explicit acknowledgement mode, pass this
 * property to @ref tibSubscriber_Create with value \c tibtrue.
 * Otherwise, the default behavior is automatic acknowledgement.
 */
#define TIB_SUBSCRIBER_PROPERTY_BOOL_EXPLICIT_ACK "com.tibco.ftl.client.subscriber.explicitack"

/** @brief Ownership of inbound messages; boolean.
 *
 * When \c tibfalse or absent, the FTL library retains ownership of inbound
 * messages that arrive through this subscriber.  The library
 * automatically destroys each message after the callback returns.
 *
 * When \c tibtrue, client program code accepts ownership of inbound
 * messages that arrive through this subscriber.  The FTL library
 * releases the message object when it dispatches the message to the
 * client's callback.  The callback can pass this message to another
 * program thread.  Client program code must explicitly destroy the
 * message object.
 */
#define TIB_SUBSCRIBER_PROPERTY_BOOL_RELEASE_MSGS_TO_CALLBACK "com.tibco.ftl.client.subscriber.release"

/** @brief Key field name for a dynamic last-value durable; string.
 *
 * When creating or subscribing to a dynamic last-value durable,
 * this property is required.
 * Supply the name of the last-value durable's key field
 * as the value of this property.
 *
 * The subscriber's content matcher must match the key field
 * against a specific value.
 */
#define TIB_SUBSCRIBER_PROPERTY_STRING_KEY_FIELD_NAME "com.tibco.ftl.client.subscriber.keyfieldname"


/** @brief Create a subscriber.
 *
 * Applications use subscriber objects to register interest
 * in a stream of messages (sent with one-to-many send calls).
 *
 * A subscriber does not receive messages
 * until you add it to an event queue.
 *
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration information
 *              about the application, endpoints, transports and formats.
 * @param endpointName The endpoint identifies the subscriber's data stream
 *                     within the realm.
 *                     To use the default endpoint, supply \c NULL.
 * @param matcher Optional.  \c NULL to omit.\n
 *                The matcher filters messages based on their content.
 * @param props Optional.  \c NULL to omit.\n
 *              Persistence properties:   <ul>
 *                  <li> @ref TIB_SUBSCRIBER_PROPERTY_STRING_DURABLE_NAME   
 *                  <li> @ref TIB_SUBSCRIBER_PROPERTY_STRING_NAME  
 *                  <li> @ref TIB_SUBSCRIBER_PROPERTY_STRING_KEY_FIELD_NAME
 *                  <li> @ref TIB_SUBSCRIBER_PROPERTY_BOOL_EXPLICIT_ACK  </ul>
 *              Message ownership property:   <ul>
 *                  <li> @ref TIB_SUBSCRIBER_PROPERTY_BOOL_RELEASE_MSGS_TO_CALLBACK
 *                    </ul>
 *
 * @return a new subscriber object
 */
TIB_API
tibSubscriber
tibSubscriber_Create(
    tibEx               e,
    tibRealm            realm,
    const char          *endpointName,
    tibContentMatcher   matcher,
    tibProperties       props
);

/** @brief Create an inbox subscriber.
 *
 * Applications use inbox subscriber objects to register interest
 * in a stream of inbox messages.
 *
 * A subscriber does not receive messages
 * until you add it to an event queue.
 *
 * Because inbox messages do not trigger the content matchers of
 * other subscribers on the endpoint, they are an efficient way to
 * target a message to a specific destination.  For example, you might
 * use an inbox message to query a specific data source, or to reply
 * to a query.
 *
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration information
 *              about the application, endpoints, transports and formats.
 * @param endpointName The endpoint identifies the subscriber's data stream
 *                     within the realm.
 *                     To use the default endpoint, supply \c NULL.
 * @param props Optional.  \c NULL to omit.\n
 *              Message ownership property:   <ul>
 *                  <li> @ref TIB_SUBSCRIBER_PROPERTY_BOOL_RELEASE_MSGS_TO_CALLBACK
 *                    </ul>
 *
 * @return a new subscriber object
 */
TIB_API
tibSubscriber
tibSubscriber_CreateOnInbox(
    tibEx               e,
    tibRealm            realm,
    const char          *endpointName,
    tibProperties       props
);

/** @brief Get the inbox object from an inbox subscriber.
 * 
 * Messages can carry this inbox object to other programs.
 * Programs that receive that inbox can send a stream of one-to-one messages to the
 * inbox subscriber.
 * See @ref tibMessage_SetInbox.
 * 
 * Your program must not destroy the inbox because the subscriber owns
 * it.
 * 
 * @param e The exception object captures information about failures.
 * @param subscriber The call copies the inbox from this inbox subscriber.
 *  
 * @return an inbox object
 */
TIB_API
tibInbox
tibSubscriber_GetInbox(
    tibEx               e,
    tibSubscriber       subscriber
);

/** @brief Close a subscriber.
 * 
 * Closing a subscriber frees all the resources associated with it
 * (for example, inbox objects; transport resources that are not used
 * in other endpoints).
 * 
 * @param e The exception object captures information about failures.
 * @param subscriber The call closes this subscriber.
 *  
 * @return void
 */
TIB_API
void
tibSubscriber_Close(
    tibEx               e,
    tibSubscriber       subscriber
);

/** @brief Create a direct subscriber.
 *
 * Applications use direct subscriber objects to receive
 * data buffers with low latency. 
 *
 * For a complete explanation, including best practices, see
 * "Direct Publishers and Subscribers"
 * in <i> TIBCO FTL Development</i>.
 *
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration information
 *              about the application, endpoints, and transports.
 * @param endpointName The endpoint identifies the subscriber's data stream
 *                     within the realm.
 * @param props Reserved for future use.\n
 *              To ensure forward compatibility,
 *              programmers must supply \c NULL.
 *
 * @return On success, this call returns a new direct subscriber object.\n
 *         On failure, this call returns \c NULL.
 */
TIB_API
tibDirectSubscriber
tibDirectSubscriber_Create(
    tibEx               e,
    tibRealm            realm,
    const char          *endpointName,
    tibProperties       props);

/** @brief Close a direct subscriber.
 *
 * Closing a direct subscriber invalidates all subsequent method calls,
 * and forces any blocked dispatch calls to return.
 *
 * However, closing a direct subscriber
 * does <i> not </i> free its memory.
 * For more information, see
 * "Programming with Direct Subscribers"
 * in <i> TIBCO FTL Development</i>.
 * 
 * @param e The exception object captures information about failures.
 * @param subscriber The call closes this direct subscriber.
 *  
 * @return void
 */
TIB_API
void
tibDirectSubscriber_Close(
    tibEx               e,
    tibDirectSubscriber subscriber);

/** Direct subscriber callback.
 *
 * Programs define callbacks of this type to process data buffers
 * inbound to direct subscribers.
 * @ref tibDirectSubscriber_Dispatch invokes the callback.
 *
 * The callback must unpack the data buffer.
 * When the callback returns,
 * the direct subscriber automatically acknowledges
 * that it has received the buffer.
 *
 * The data buffer and the size array belong to the library.
 * Applications must not write into these objects, nor free them.
 *
 * @note The buffer, its data, and the size array
 * are valid only until the callback returns.
 * To extend the lifetime of the data,
 * copy it to application memory.
 *
 * @param e The exception object captures information about failures.
 * @param count The number of data items in the data buffer.
 * @param totalSize The size of the data buffer (in bytes).
 * @param sizeArray An array of sizes corresponding to the
 *                 \p count data items in the buffer.
 * @param data The data buffer.
 * @param closure A closure object.
 *                The direct subscriber receives this object
 *                in the dispatch call, and passes it unchanged
 *                to the callback.  The callback may modify it.
 *  
 */
typedef void
(*tibDirectCallback)(
    tibEx       e,
    tibint64_t  count,
    tibint64_t  totalSize,
    tibint64_t  *sizeArray,
    tibint8_t   *data,
    void        *closure);

/** @brief Dispatch inbound data for a direct subscriber.
 * 
 * If the timeout elapses before an event arrives in the queue, then
 * the dispatch call returns normally.  The call does not indicate
 * whether or not it actually dispatched an event.
 *
 * @param e The exception object captures information about failures
 *          (during dispatch)
 *          and conveys it back to its caller.  The exception does not
 *          carry information from the callback.
 * @param subscriber Dispatch a data buffer for this direct subscriber.
 * @param timeout If a data buffer is not available,
 *                the call waits for inbound data.
 *                If a data buffer does not arrive before this timeout
 *                (in seconds) elapses, the call returns.\n
 *                The constants @ref TIB_TIMEOUT_WAIT_FOREVER and
 *                @ref TIB_TIMEOUT_NO_WAIT are special values.
 * @param callback Process inbound data using this callback method.
 * @param closure The callback method receives this
 *                data object as an argument.
 */
TIB_API
void
tibDirectSubscriber_Dispatch(
    tibEx                       e,
    tibDirectSubscriber         subscriber,
    double                      timeout,
    tibDirectCallback           callback,
    void                        *closure);

/** @brief Test a direct subscriber for inbound data.
 *
 * If the subscriber has data ready to dispatch, this call returns
 * @ref tibtrue.  Otherwise it returns @ref tibfalse.
 * 
 * When data is not ready to dispatch, this call is faster than
 * calling @ref tibDirectSubscriber_Dispatch 
 * with a timeout value of @ref TIB_TIMEOUT_NO_WAIT.
 */
TIB_API
tibbool_t
tibDirectSubscriber_HasData(
    tibEx               e,
    tibDirectSubscriber id);

#if defined(__cplusplus)
}
#endif


#endif /* _INCLUDED_tib_sub_h */
