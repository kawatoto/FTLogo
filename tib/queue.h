/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: queue.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_queue_h
#define _INCLUDED_tib_queue_h

#include "tib/except.h"
#include "tib/types.h"
#include "tib/tibexp.h"
#include "tib/sub.h"
#include "tib/props.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file queue.h
 * 
 * @brief Event queue objects hold message and timer events until
 * callbacks can process them.
 *
 * This file defines queues, callback types, calls that funnel events to queues,
 * and a call that dispatches events to callbacks.
 *
 * This file also defines timer events, and calls that manipulate them.
 */


/** @brief A timer object queues an event when its interval elapses.
 * 
 * The interval repeats indefinitely; to stop it, the program must
 * explicitly destroy the timer.
 */
typedef struct __tibTimerId         *tibTimer;



  /** @name Dispatch Timeout Constants
   *
   * These constants are special timeout values for dispatching a queue.
   */

  //@{

  /** @brief Wait indefinitely for an event.
  *
  * When the queue is empty, the dispatch call waits for an event.
  */
#define TIB_TIMEOUT_WAIT_FOREVER    ((tibdouble_t) -1.0)

  /** @brief Do not wait for an event
   *
   * When the queue is empty, the dispatch call returns immediately.
   */
#define TIB_TIMEOUT_NO_WAIT         ((tibdouble_t)  0.0)

  //@}

/// @brief Instructions for discarding events when a queue overflows.
/// @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY
typedef enum
{
  /// Do not discard events (default behavior).
  TIB_EVENTQUEUE_DISCARD_NONE = 0,

  /// Discard old events from the head of the queue.
  TIB_EVENTQUEUE_DISCARD_OLD  = 1,

  /// Discard new events, instead of adding them to the queue.
  TIB_EVENTQUEUE_DISCARD_NEW  = 2

} tibEventQueue_DiscardPolicy;

  /** @name Queue Properties
    @brief These string constants are the valid property names for creating a queue.
  */
  //@{

  /** @brief Inline mode (low-latency); boolean.
   *
   * Programs that receive time-sensitive messages can use inline mode
   * to favor low latency over high throughput.  Inline mode reduces
   * inbound message latency using inline transport I/O in the same
   * thread as the message callback.
   *
   * Inline mode requires that callbacks always return quickly;
   * otherwise, long callbacks can delay message I/O (defeating the
   * purpose of inline mode).
   *
   * Inline mode could reduce the average number of messages in the
   * vectors that the callback receives.
   *
   * We recommend dispatching inline queues from only one thread.
   * Dispatching an inline-mode queue from
   * several threads could result in actual wait times that are longer
   * than the dispatch timeout arguments.\n For example, if thread A dispatches
   * with timeout 10 seconds, and thread B dispatches with timeout 15
   * seconds, then the timer for thread B does not start until after the
   * dispatch call returns in thread A.  The apparent timeout for thread
   * B could be as long as 25 seconds.
   *
   * When specifying inline mode, programmers must coordinate with
   * administrators to avoid illegal state exceptions.
   *
   * To enable inline mode, pass this property to
   * @ref tibEventQueue_Create with value \c tibtrue. \n
   * Otherwise, the default behavior disables inline mode.
   */
#define TIB_EVENTQUEUE_PROPERTY_BOOL_INLINE_MODE                   "com.tibco.ftl.client.inline"

  /** @brief Discard policy; integer.
   *
   *  This property determines the behavior of the queue on overflow (too many events).
   *  For legal values, see @ref tibEventQueue_DiscardPolicy.
   *
   * To enable discard on overflow, pass this property to
   * @ref tibEventQueue_Create with a value from
   * @ref tibEventQueue_DiscardPolicy.\n
   * Otherwise, the default behavior disables discard.
   *
   * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_MAX_EVENTS
   * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_DISCARD_AMOUNT
   */
#define TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY                 "com.tibco.ftl.client.discard.policy"

  /** @brief Max events; integer.
   *
   *  When distributing an event to the queue would overflow this
   *  limit, the queue discards events.
   *
   *  If you specify a discard policy that could actually discard
   *  events, then you must also specify a value for this maximum.
   *
   * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY
   * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_DISCARD_AMOUNT
  */
#define TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_MAX_EVENTS      "com.tibco.ftl.client.discard.max_events"

  /** @brief Discard amount; integer.
   *
   * When a queue overflows, this property determines the number of
   * events to discard.
   *
   * If you specify @ref TIB_EVENTQUEUE_DISCARD_OLD, you may also
   * specify this value.  The value must be less than @ref
   * TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_MAX_EVENTS.  When
   * absent, the default value is 1.
   *
   * If you specify @ref TIB_EVENTQUEUE_DISCARD_NEW, then
   * @ref tibEventQueue_Create ignores this value.  Discarding new events
   * always discards exactly enough events so that the rest fit on the
   * queue.
   *
   * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY
   * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_MAX_EVENTS
   */
#define TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_DISCARD_AMOUNT  "com.tibco.ftl.client.discard.amount"

  /** @brief Queue name; string.
   *
   * We recommend assigning a unique name to each event queue (that
   * is, unique within the program).  If the queue discards events,
   * the advisory message identifies the queue using this name, which
   * can help diagnose the issue.
   */
#define TIB_EVENTQUEUE_PROPERTY_STRING_NAME "com.tibco.ftl.client.queue.name"

  //@}

/** @brief Message dispatch callback.
 * 
 * Programs define callbacks of this type to process inbound messages.
 * @ref tibEventQueue_Dispatch invokes the callback, passing an array
 * of messages paired with a parallel array of closure objects.
 *
 * The messages can arrive through different subscribers.  The only
 * aspect they necessarily share is that they require the same
 * callback to process them.
 *
 * Programs must not dispatch any event queue from within any callback.
 *
 * For more information about closure objects,
 * see @ref tibEventQueue_AddSubscriber.
 *
 * Inbound messages in callbacks belong to the FTL library; programs
 * must not destroy them.
 *
 * @note The message objects are valid only until the callback
 * returns.  To extend the lifetime of a message, make a copy using
 * @ref tibMessage_MutableCopy.
 * 
 * @param e FTL supplies callbacks with a clear exception object;
 *          your callback code may use it in its FTL API calls.\n
 *          When the completion callback returns, FTL does not examine
 *          the exception object for errors, nor does FTL return that
 *          exception to your program code.
 * @param queue The callback is processing messages from this queue.
 * @param msgCount The number of messages in the array.
 * @param msgs The array of messages to process.
 * @param closures The array of subscriber-specific data.
 *                 Each item corresponds to one message
 *                 (with the same array index).
 *
 * @note FTL can destroy the messages after your callback returns.  If
 *       you require a message to exist beyond the return of the
 *       callback, you must copy it using @ref tibMessage_MutableCopy.
 *
 * @see TIB_EVENTQUEUE_PROPERTY_BOOL_INLINE_MODE for requirements on
 *      callbacks.
 *  
 * @return void
 */
typedef void
(*tibMsgCallback) (
    tibEx                       e,
    tibEventQueue               queue,
    tibint32_t                  msgCount,
    tibMessage                  *msgs,
    void                        **closures
);

/** @brief Event timer callback.
 * 
 * Programs define callbacks of this type to process timer events.
 * @ref tibEventQueue_Dispatch invokes the callback, passing the timer
 * object and a closure object (see @ref tibEventQueue_CreateTimer).
 *
 * Programs must not dispatch any event queue from within any callback.
 *
 * @param e FTL supplies callbacks with a clear exception object;
 *          your callback code may use it in its FTL API calls.\n
 *          When the completion callback returns, FTL does not examine
 *          the exception object for errors, nor does FTL return that
 *          exception to your program code.
 * @param queue The callback is processing a timer event from this queue.
 * @param timer The timer object that triggered the event.
 * @param closure Timer-specific data.
 *  
 * @return void
 */
typedef void
(*tibTimerCallback) (
    tibEx            e,
    tibEventQueue    queue,
    tibTimer         timer,
    void*            closure);

/** @brief Subscriber completion callback.
 * 
 * When @ref tibEventQueue_RemoveSubscriber removes a subscriber from
 * an event queue, the subscriber object might still be in use within
 * a callback or an FTL library call.  Similarly, a closure object might
 * still be in use within a callback.  Programs can define a
 * completion callback of this type for cleanup operations that must
 * wait until after a subscriber and closure are no longer needed (for
 * example, closing a subscriber or freeing a closure).
 *
 * The FTL library calls this completion callback when the
 * subscriber and closure are no longer needed.  The completion
 * callback could run in the thread that removes the subscriber, or
 * asynchronously in another (FTL internal) thread.
 * 
 * Programs must not dispatch any event queue from within any callback.
 *
 * @note If you need to \e re-add a subscriber to a queue after removing
 * it, you \e must do so within a completion callback in order to prevent
 * a race condition that could cause @ref tibEventQueue_AddSubscriber
 * to throw an exception.
 * 
 * @param e FTL supplies callbacks with a clear exception object;
 *          your callback code may use it in its FTL API calls.\n
 *          When the completion callback returns, FTL does not examine
 *          the exception object for errors, nor does FTL return that
 *          exception to your program code.
 *
 * @param subscriber The subscriber that was removed.
 * @param closure The closure associated with the subscriber.
 *  
 * @return void
 */
typedef void
(*tibSubscriberComplete) (
    tibEx                       e,
    tibSubscriber               subscriber,
    void*                       closure
);

/** Timer completion callback.
 * 
 * When @ref tibEventQueue_DestroyTimer removes a timer from
 * an event queue, the timer object might still be in use within
 * a callback or an FTL library call.  Similarly, a closure object might
 * still be in use within a callback.  Programs can define a
 * completion callback of this type for cleanup operations that must
 * wait until after a timer and closure are no longer needed (for
 * example, freeing a closure).
 *
 * The FTL library calls this completion callback when the timer
 * and closure are no longer needed.  The completion callback could
 * run in the thread that stops the timer, or asynchronously in
 * another (FTL internal) thread.
 * 
 * Programs must not dispatch any event queue from within any callback.
 *
 * @param e FTL supplies callbacks with a clear exception object;
 *          your callback code may use it in its FTL API calls.\n
 *          When the completion callback returns, FTL does not examine
 *          the exception object for errors, nor does FTL return that
 *          exception to your program code.
 *          
 * @param timer The timer that was stopped.
 * @param closure The closure associated with the timer.
 *  
 * @return void
 */
typedef void
(*tibTimerComplete) (
    tibEx                       e,
    tibTimer                    timer,
    void*                       closure
);

/** @brief Event queue completion callback.
 * 
 * When @ref tibEventQueue_Destroy destroys a queue, the queue and the
 * events in it might still be in use within callbacks or FTL library
 * calls.  Programs can define a completion callback of this type for
 * cleanup operations that must wait until after all callbacks have
 * completed.
 *
 * The FTL library calls this completion callback when the queue
 * is no longer needed.  The completion callback could run in the
 * thread that destroys the queue, or asynchronously in another (FTL
 * internal) thread.
 * 
 * Programs must not dispatch any event queue from within any callback.
 *
 * @param e FTL supplies callbacks with a clear exception object;
 *          your callback code may use it in its FTL API calls.\n
 *          When the completion callback returns, FTL does not examine
 *          the exception object for errors, nor does FTL return that
 *          exception to your program code.
 *
 * @param queue The event queue that was destroyed.
 *  
 * @return void
 */
typedef void
(*tibEventQueueComplete) (
    tibEx                       e,
    tibEventQueue               queue
);

/** @brief Event queue completion callback.
 *
 * When @ref tibEventQueue_Destroy destroys a queue, the queue and the
 * events in it might still be in use within callbacks or FTL library
 * calls.  Programs can define a completion callback of this type for
 * cleanup operations that must wait until after all callbacks have
 * completed.
 *
 * The FTL library calls this completion callback when the queue
 * is no longer needed.  The completion callback could run in the
 * thread that destroys the queue, or asynchronously in another (FTL
 * internal) thread.
 *
 * Programs must not dispatch any event queue from within any callback.
 *
 * @param e FTL supplies callbacks with a clear exception object;
 *          your callback code may use it in its FTL API calls.\n
 *          When the completion callback returns, FTL does not examine
 *          the exception object for errors, nor does FTL return that
 *          exception to your program code.
 *
 * @param queue The event queue that was destroyed.
 * @param closure The closure supplied to @ref tibEventQueue_DestroyEx.
 *
 * @return void
 */
typedef void
(*tibEventQueueCompleteEx) (
    tibEx                       e,
    tibEventQueue               queue,
    void*                       closure
);

/** @brief Create an event queue.
 *
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration information
 *              about the application, endpoints, transports and formats.
 * @param props Optional.  To omit, supply \c NULL.\n
 *              Properties configure event queue behavior.
 *  
 * @return a new event queue object
 *
 * @see TIB_EVENTQUEUE_PROPERTY_BOOL_INLINE_MODE
 * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY
 * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_DISCARD_AMOUNT
 * @see TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY_MAX_EVENTS
 */
TIB_API
tibEventQueue
tibEventQueue_Create(
    tibEx                       e,
    tibRealm                    realm,
    tibProperties               props
);

/** @brief Destroy an event queue.
 * 
 * Destroying a queue object frees all the resources associated with
 * the queue.  (However, this call does not implicitly close
 * subscribers associated with the queue.)
 *
 * Best practice is to remove all subscribers \em before destroying
 * the event queue. However, when this practice is not possible, you
 * can still destroy the subscriber's closure in the event queue
 * destroy completion callback.
 *
 * @param e The exception object captures information about failures.
 * @param queue The call destroys this queue.
 * @param completeCb Optional.\n
 *                   This completion callback
 *                   runs after all running event callbacks have returned.\n
 *                   \c NULL indicates no completion callback.
 *  
 * @return void
 *
 * @see tibEventQueue_RemoveSubscriber
 */
TIB_API
void
tibEventQueue_Destroy(
    tibEx                       e,
    tibEventQueue               queue,
    tibEventQueueComplete       completeCb
);

/** @brief Destroy an event queue.
 *
 * Destroying a queue object frees all the resources associated with
 * the queue.  (However, this call does not implicitly close
 * subscribers associated with the queue.)
 *
 * Best practice is to remove all subscribers \em before destroying
 * the event queue. However, when this practice is not possible, you
 * can still destroy the subscriber's closure in the event queue
 * destroy completion callback.
 *
 * @param e The exception object captures information about failures.
 * @param queue The call destroys this queue.
 * @param completeCb Optional.\n
 *                   This completion callback
 *                   runs after all running event callbacks have returned.\n
 *                   \c NULL indicates no completion callback.
 * @param closure The library passes this closure to the callback.
 *
 * @return void
 *
 * @see tibEventQueue_RemoveSubscriber
 */
TIB_API
void
tibEventQueue_DestroyEx(
    tibEx                       e,
    tibEventQueue               queue,
    tibEventQueueCompleteEx     completeCb,
    void*                       closure
);

/** @brief Add a subscriber to a queue.
 * 
 * Adding a subscriber to a queue associates the two objects, which
 * yields the following behavior: Each time the subscriber receives a
 * message, it distributes an event to the queue.  The event includes
 * the inbound message, the callback, and the closure.
 *
 * You can add a subscriber to at most one queue.  If you have already
 * added a subscriber to a queue, and you attempt to add it to another
 * queue, this call throws an exception.
 *
 * If you add several subscribers to the same queue, the queue merges
 * their message streams.
 * 
 * @param e The exception object captures information about failures.
 * @param queue The call adds the subscriber to this queue.
 * @param subscriber The call adds this subscriber to the queue.
 * @param callback Dispatching a message event invokes this callback.
 * @param closure Dispatch passes this closure to the callback.
 *  
 * @return void
 */
TIB_API
void
tibEventQueue_AddSubscriber(
    tibEx                       e,
    tibEventQueue               queue,
    tibSubscriber               subscriber,
    tibMsgCallback              callback,
    void*                       closure
);

/** @brief Remove a subscriber from a queue.
 * 
 * Removing a subscriber from a queue dissociates the two objects.
 * The subscriber no longer distributes message events to the queue.
 * Message events that the subscriber has already distributed to the
 * queue remain in the queue.
 *
 * The completion callback runs asynchronously when the subscriber and
 * closure are no longer needed -- that is, after all the subscriber's
 * events have been processed, and the callbacks have returned. It is
 * safe to destroy the subscriber's closure within the completion
 * callback, but not sooner.
 *
 * Best practice is to remove all subscribers \em before destroying
 * the event queue. However, when this practice is not possible, you
 * can still destroy the subscriber's closure in the event queue
 * destroy completion callback.
 *
 * Associations between subscribers and queues are independent of one
 * another; that is, removing one subscriber from a queue does not affect
 * the association of other subscribers with that queue.
 * 
 * @param e The exception object captures information about failures.
 * @param queue The call removes the subscriber from this queue.
 * @param subscriber The call removes this subscriber from the queue.
 * @param completeCb Optional.\n
 *                   This completion callback runs after all events
 *                   from the subscriber have been processed, and
 *                   their callbacks have returned.\n
 *                   \c NULL indicates no completion callback.
 *  
 * @return void
 */
TIB_API
void
tibEventQueue_RemoveSubscriber(
    tibEx                       e,
    tibEventQueue               queue,
    tibSubscriber               subscriber,
    tibSubscriberComplete       completeCb
);

/** @brief Dispatch events.
 * 
 * If the queue is not empty, this call scans events from the head of
 * the queue to obtain a sequence of events that all contain the same
 * callback.  Scanning produces an array of messages, and a parallel
 * array of closures.  The dispatch call passes both arrays to the
 * callback for processing.
 *
 * If the queue is empty, the call can wait for events to arrive.  The
 * \e timeout parameter determines the maximum time it can wait.  Note that
 * this parameter does not guarantee a minimum wait time.
 * \li To wait indefinitely for an event, supply the timeout constant
 * @ref TIB_TIMEOUT_WAIT_FOREVER.
 * \li To prohibit waiting and return immediately, supply the timeout
 * constant @ref TIB_TIMEOUT_NO_WAIT.
 *
 * If the timeout elapses before an event arrives in the queue, then
 * the dispatch call returns normally.  The call does not indicate
 * whether or not it actually dispatched an event.
 *
 * @param e The exception object captures information about failures
 *          (during dispatch)
 *          and conveys it back to its caller.  The exception does not
 *          carry information from the callback.
 * @param queue The call dispatches events from this queue.
 *
 * @param timeout If the queue is empty, the call waits for an event.
 *                If an event does not arrive before this timeout (in
 *                seconds) elapses, the call returns.\n
 *                The constants @ref TIB_TIMEOUT_WAIT_FOREVER and
 *                @ref TIB_TIMEOUT_NO_WAIT are special values.
 *  
 * @return void
 */
TIB_API
void
tibEventQueue_Dispatch(
    tibEx                       e,
    tibEventQueue               queue,
    tibdouble_t                 timeout
);

/** @brief Create and start a timer.
 * 
 * This call creates a timer object and associates it with a queue.
 * The timer places a timer event on the queue every \e interval seconds.
 *
 * The interval repeats indefinitely; to stop it, the program must
 * explicitly destroy the timer object. 
 *
 * Each time @ref tibEventQueue_Dispatch dispatches a timer event,
 * the callback processes the event.
 * 
 * @param e The exception object captures information about failures.
 * @param queue The call associates the new timer with this queue.
 * @param interval The timer places events on the queue at this repeating interval (in seconds).
 * @param callback Dispatching a timer event invokes this callback.
 * @param closure Dispatch passes this closure to the callback.
 *  
 * @return a new timer object
 */
TIB_API
tibTimer
tibEventQueue_CreateTimer(
    tibEx               e,
    tibEventQueue       queue,
    tibdouble_t         interval,
    tibTimerCallback    callback,
    void*               closure);

/** @brief Stop and destroy a timer.
 * 
 * This call stops a timer so it does not place additional timer
 * events on the queue.  It also attempts to remove from the queue any
 * timer events (associated with the stopped timer) that have fired
 * but are not yet processed.  After the completion callback returns,
 * the FTL library destroys the timer asynchronously.
 * 
 * @param e The exception object captures information about failures.
 * @param queue The call disassociates the timer from this event queue.
 * @param timer The call stops this timer.
 * @param completeCb Optional.\n
 *                   This completion callback
 *                   runs after all timer event callbacks are complete.
 *                   It is safe to destroy the timer's closure object
 *                   within this callback.\n
 *                   \c NULL indicates no completion callback.
 * 
 * @return void
 */
TIB_API
void
tibEventQueue_DestroyTimer(
    tibEx                   e,
    tibEventQueue           queue,
    tibTimer                timer,
    tibTimerComplete        completeCb);

/** @brief Get the number of events in the queue.
 *
 * The count includes both message events and timer events.
 *
 * @param e The exception object captures information about failures.
 * @param id The call gets the number of events in this queue.
 */
TIB_API
tibint64_t
tibEventQueue_GetCount(
    tibEx               e,
    tibEventQueue       id);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_queue_h */
