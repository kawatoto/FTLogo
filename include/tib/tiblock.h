/*
 * Copyright (c) 2009-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: tiblock.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_tiblock_h
#define _INCLUDED_tib_tiblock_h

#include "tib/types.h"
#include "tib/tibexp.h"

/**
 * @file tiblock.h
 * 
 * @brief Cooperating application programs can use locks
 * to implement exclusive access to a map within a persistence store.
 * 
 * To create a lock object, see @ref tibRealm_CreateLock.
 *
 * If a client process exits or disconnects from the persistence server,
 * the library automatically releases all the locks that the process holds.
 *
 * This file defines the calls that manipulate locks.
 * In addition, map objects and map iterator objects have methods
 * that operate while holding a lock.
 */
 
 
#if defined(__cplusplus)
extern "C" {
#endif

typedef struct __tibLockId  *tibLock;

/** @brief  No operation.
 *
 * In releases earlier than 5.0,
 * this call requested a lock from its persistence store.
 *
 * In Release 5.0 and later,
 * this call is no longer necessary;
 * it returns \c tibTrue without any other effect.
 *
 * Calls that require a lock automatically request it; see:
 * \li @ref tibMap_SetWithLock
 * \li @ref tibMap_GetWithLock
 * \li @ref tibMap_RemoveWithLock
 * \li @ref tibMap_CreateIteratorWithLock
 *
 * Programs that correctly used earlier versions of lock calls
 * continue to operate properly.
 *
 * @param e Unused.
 * @param lock Unused.
 * 
 * @return This call returns the boolean value \c tibTrue.
 */
TIB_API
tibbool_t
tibLock_Request(
    tibEx            e,
    tibLock          lock);

/** @brief Forcibly acquire a lock, even if another process holds it. 
 *
 * The library delays the effect of this call (stealing the lock)
 * until the next time the process calls a map method
 * that requires the lock.
 *  
 * @param e The exception object captures information about 
 *          failures.
 * @param lock The call steals the lock represented by this lock object. 
 *  
 * @return void
 */
TIB_API
void
tibLock_Steal(
    tibEx         e,
    tibLock       lock);

/** @brief Release a lock. 
 *
 * If the process does not hold the lock, this call has no effect.
 *  
 * @param e The exception object captures information about failures.
 * @param lock The call releases the lock represented by this lock object. 
 * 
 * @return void 
 */
TIB_API
void
tibLock_Return(
    tibEx         e,
    tibLock       lock);

/** @brief Destroy a local lock object and reclaim its resources.
 *
 * If the process holds the lock, this call returns the lock before destroying the object.
 *  
 * @param e The exception object captures information about failures.
 * @param lock The call destroys the local lock object.
 * 
 * @return void 
 */
TIB_API
void
tibLock_Destroy(
    tibEx       e,
    tibLock     lock);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_tiblock_h */
