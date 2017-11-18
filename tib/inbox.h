/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: inbox.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_inbox_h
#define _INCLUDED_tib_inbox_h

#if defined(__cplusplus)
extern "C" {
#endif

#include "tib/types.h"
#include "tib/except.h"
#include "tib/tibexp.h"

/**
 * @file inbox.h
 * 
 * @brief Inbox objects address inbox subscribers.
 *
 * Programs can insert inbox objects into message fields.
 *
 * This file defines inbox objects and calls to copy and destroy them.
 *
 * @see For more calls that use inbox objects, see @ref sub.h and @ref msg.h.
 */

/** @brief Inbox object type. */
typedef struct __tibInboxId     *tibInbox; 

/** @brief Copy an inbox object.
 * 
 * When a callback gets an inbox from a field of an inbound message,
 * the message owns that inbox object.  FTL implicitly destroys that
 * embedded inbox object when the callback returns.  Programs can use
 * this call to copy the inbox object.
 *
 * Similarly, when a program gets an inbox from a subscriber object,
 * the subscriber owns that inbox.  FTL implicitly destroys that
 * embedded inbox object when the program closes the subscriber.
 * Programs can use this call to copy the inbox object.
 *
 * Programs must explicitly destroy copies (with @ref
 * tibInbox_Destroy).
 * 
 * @param e The exception object captures information about failures.
 * @param inbox The call copies this source inbox.
 *  
 * @return a copy of the source inbox
 *
 * @see tibMessage_GetInbox
 * @see tibMessage_GetInboxByRef 
 * @see tibSubscriber_GetInbox 
 */
TIB_API
tibInbox
tibInbox_Copy(
    tibEx               e,
    tibInbox            inbox);

/** @brief Destroy an inbox object.
 * 
 * A program that copies an inbox object must explicitly destroy
 * it to reclaim its resources.
 *
 * @note @ref tibMessage_SetInbox and @ref tibMessage_SetInboxByRef
 * both copy their inbox argument into a message field.  After those
 * calls return, a program may destroy the inbox without affecting the
 * message.
 * 
 * @param e The exception object captures information about failures.
 * @param inbox The call destroys this inbox.
 *  
 * @return void
 */
TIB_API
void
tibInbox_Destroy(
    tibEx               e,
    tibInbox            inbox);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_inbox_h */
