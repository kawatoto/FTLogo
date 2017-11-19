/*
 * Copyright (c) 2009-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: map.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_map_h
#define _INCLUDED_tib_map_h

#include "tib/ftl.h"
#include "tib/tiblock.h"
#include "tib/msg.h"
#include "tib/mapiter.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file map.h
 * 
 * @brief Programs can use maps to store key/value pairs in a persistence store.
 * 
 * To create a map object, see @ref tibRealm_CreateMap.\n
 * To delete a map from a store, see @ref tibRealm_RemoveMap.
 *
 * Prerequisite: Administrators must enable dynamic
 * last-value durables in a separate persistence store.
 * For more information, see <i> TIBCO FTL Administration</i>.
 * 
 * This file defines map calls.
 */

typedef struct __tibMapId        *tibMap; 

/** @brief Set a key/value pair in a map.
 *
 * If a value has already been set for the key,
 * this call overwrites the existing value.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call stores the key/value pair in this map.
 * @param key The call sets a value for this key.
 * @param value The call sets this value for the key.
 */
 TIB_API
void
tibMap_Set(
    tibEx          e,
    tibMap         tibmap,
    const char     *key,
    tibMessage     value);

/** @brief Set a key/value pair in a map as a locked operation.
 *
 * Before its map operation,
 * this call ensures that the process holds the lock.
 * If the process does not already hold the lock,
 * this call acquires the lock for the process.
 * If it cannot acquire the lock,
 * this call throws an exception,
 * and does not change the key's value.
 *
 * If the process acquires the lock through this call,
 * it retains the lock after the call completes.
 * The process holds the lock until it explicitly returns it.
 *
 * If a value has already been set for the key,
 * this call overwrites the existing value.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call stores the key/value pair in this map.
 * @param key The call sets a value for this key.
 * @param value The call sets this value for the key.
 * @param lock The call requires the lock represented by this lock object. 
 */
TIB_API
void
tibMap_SetWithLock(
    tibEx          e,
    tibMap         tibmap,
    const char     *key,
    tibMessage     value,
    tibLock        lock);


/** @brief Get the value of a key in a map.
 *
 * This call returns a copy of the stored message.
 * Client program code accepts ownership of the message object.
 * Client program code may pass this message to another
 * program thread. 
 * Client program code must explicitly destroy the message object.
 *
 * If the key is not set in the map,
 * this call returns null.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call gets the key's value in this map.
 * @param key The call gets a value for this key.
 * 
 * @return A message.
 */
TIB_API
tibMessage
tibMap_Get(
    tibEx          e,
    tibMap         tibmap,
    const char     *key);

/** @brief Get the value of a key in a map as a locked operation.
 *
 * This call returns a copy of the stored message.
 * Client program code accepts ownership of the message object.
 * Client program code may pass this message to another
 * program thread. 
 * Client program code must explicitly destroy the message object.
 *
 * Before its map operation,
 * this call ensures that the process holds the lock.
 * If the process does not already hold the lock,
 * this call acquires the lock for the process.
 * If it cannot acquire the lock,
 * this call throws an exception,
 * and does not return the key's value.
 *
 * If the process acquires the lock through this call,
 * it retains the lock after the call completes.
 * The process holds the lock until it explicitly returns it.
 *
 * If the key is not set in the map,
 * this call returns null.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call gets the key's value in this map.
 * @param key The call gets a value for this key.
 * @param lock The call requires the lock represented by this lock object. 
 * 
 * @return A message.
 */
 TIB_API
tibMessage
tibMap_GetWithLock(
    tibEx          e,
    tibMap         tibmap,
    const char     *key,
    tibLock        lock);

/** @brief Remove a key/value pair from a map.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call removes the key from this map.
 * @param key The call removes this key.
 */
TIB_API
void
tibMap_Remove(
    tibEx          e,
    tibMap         tibmap,
    const char     *key);

/** @brief Remove a key/value pair from a map as a locked operation.
 *
 * Before its map operation,
 * this call ensures that the process holds the lock.
 * If the process does not already hold the lock,
 * this call acquires the lock for the process.
 * If it cannot acquire the lock,
 * this call throws an exception,
 * and does not remove the key's value.
 *
 * If the process acquires the lock through this call,
 * it retains the lock after the call completes.
 * The process holds the lock until it explicitly returns it.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call removes the key from this map.
 * @param key The call removes this key.
 * @param lock The call requires the lock represented by this lock object. 
 */
TIB_API
void
tibMap_RemoveWithLock(
    tibEx          e,
    tibMap         tibmap,
    const char     *key,
    tibLock        lock);

/** @brief Destroy a map object.
 *
 * This call destroys only the local map object in the client process.
 * To delete the map from the persistence store, see @ref tibRealm_RemoveMap.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call destroys this map object.
 */
TIB_API
void
tibMap_Close(
    tibEx      e,
    tibMap     tibmap);

/** @brief Create an iterator over the keys in a map.
 *
 * The iterator produces every key/value pair in the map.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call destroys this map object.
 * @param props Reserved for future use.\n
 *              To ensure forward compatibility, programmers must supply \c NULL.
 * 
 * @return A map iterator object. 
 */
TIB_API
tibMapIterator
tibMap_CreateIterator(
    tibEx          e,
    tibMap         tibmap,
    tibProperties  props);

/** @brief Create an iterator over the keys in a map
 *         and associate it with a lock.
 *
 * The iterator produces every key/value pair in the map.
 *
 * Before creating an iterator,
 * this call ensures that the process holds the lock.
 * If the process does not already hold the lock,
 * this call acquires the lock for the process.
 * If it cannot acquire the lock,
 * this call throws an exception,
 * and does not create an iterator.
 *
 * Subsequent calls to advance the iterator
 * require that the process hold the lock.
 *
 * If the process acquires the lock through this call,
 * it retains the lock after the call completes.
 * The process holds the lock until it explicitly returns it.
 *
 * @param e The exception object captures information about failures.
 * @param tibmap The call destroys this map object.
 * @param lock The iterator uses the lock represented by this lock object. 
 * @param props Reserved for future use.\n
 *              To ensure forward compatibility, programmers must supply \c NULL.
 * 
 * @return A map iterator object. 
 */
TIB_API
tibMapIterator
tibMap_CreateIteratorWithLock(
    tibEx          e,
    tibMap         tibmap,
    tibLock        lock,
    tibProperties  props);

#if defined(__cplusplus)
}
#endif


#endif /* _INCLUDED_tib_map_h */
