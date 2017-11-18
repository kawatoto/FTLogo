/*
 * Copyright (c) 2009-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: mapiter.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_mapiter_h
#define _INCLUDED_tib_mapiter_h

#include "tib/ftl.h"
#include "tib/types.h"
#include "tib/except.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file mapiter.h
 * 
 * @brief Map iterators iterate over the key/value pairs in a map.
 *
 * An iterator can produce values added to a map after the iterator is created.
 *
 * To create a map iterator object, see @ref tibMap_CreateIterator.
 *
 * This file defines map iterator calls.
 */
typedef struct __tibMapIteratorId        *tibMapIterator; 


/** @brief Iterate to the next key/value pair. 
 *  
 * If the iterator has an associated lock
 * and the process does not hold that lock,
 * the call throws an exception and does not
 * advance to the next pair.
 * (See also @ref tibMap_CreateIteratorWithLock.)
 *
 * To get the key and value from each pair, see
 * @ref tibMapIterator_CurrentKey and @ref tibMapIterator_CurrentValue.
 *
 * @param e The exception object captures information about failures.
 * @param mapIterator The call advances the focus of this iterator to the next pair.
 *
 * @return <ul>
 * <li> True indicates that the next pair is ready. </li>
 * <li> False indicates that the iteration is complete. </li>
 * </ul>
 */
TIB_API
tibbool_t
tibMapIterator_Next(
    tibEx           e,
    tibMapIterator  mapIterator);


/** @brief Get the key from the current key/value pair.
 *
 * The library owns the key string, and may destroy it after the program
 * calls @ref tibMapIterator_Next or @ref tibMapIterator_Destroy.
 * 
 * @param e The exception object captures information about failures.
 * @param mapIterator The call gets the key from the current pair of this iterator.
 *
 * @return The key string.
 */
TIB_API
const char*
tibMapIterator_CurrentKey(
    tibEx              e,
    tibMapIterator     mapIterator);

/** @brief Get the message value from the current key/value pair.
 *
 * The library owns the message object, and may destroy it after the program
 * calls @ref tibMapIterator_Next or @ref tibMapIterator_Destroy.
 * 
 * @param e The exception object captures information about failures.
 * @param mapIterator The call gets the value from the current pair of this iterator.
 *
 * @return The value (which is a message object).
 */
TIB_API
tibMessage
tibMapIterator_CurrentValue(
    tibEx              e,
    tibMapIterator     mapIterator);

/** @brief Destroy a map iterator object, reclaiming its resources.
 *
 * @param e The exception object captures information about failures.
 * @param mapIterator The call destroys this iterator object.
 */
TIB_API
void
tibMapIterator_Destroy(
    tibEx              e,
    tibMapIterator     mapIterator);

#if defined(__cplusplus)
}
#endif


#endif /* _INCLUDED_tib_mapiter_h */
