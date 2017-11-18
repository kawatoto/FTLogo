/*
 * Copyright (c) 2009-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: types.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_types_h
#define _INCLUDED_tib_types_h

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file types.h
 * 
 * @brief FTL datatype definitions.
 */

#define tibint32_t      int
#if defined(_WIN32) || defined(__APPLE__)
 #define tibint64_t     long long int
#else
 #define tibint64_t     long int
#endif

#define tibint8_t       signed char
#define tibuint8_t      unsigned char

#define tibbool_t       unsigned char
#define tibtrue         (tibbool_t) 1
#define tibfalse        (tibbool_t) 0

#define tibdouble_t     double

/** @brief DateTime struct.
 * Application programs can use this data structure
 * to represent date and time values
 * with large range and nanosecond precision.
 *
 * \c tibDateTime values combine two numeric components.
 * \li The \c sec component represents whole seconds with up to 64 bits.
 *     Zero denotes the UNIX epoch -- midnight entering January 1, 1970.
 * \li The \c nsec component represents nanoseconds
 *     \e after the time that the \c sec component denotes.
 *     Although the struct stores this component in a signed 64-bit integer,
 *     this component is always non-negative, between zero
 *     and 999999999.
 *
 * For example,
 * the value -1 seconds plus 999,999,998 nanoseconds
 * represents December 31, 1969, 2 nanoseconds before midnight
 * (that is, 2 nanoseconds before the epoch).
 */
typedef struct tibDateTime
{
    tibint64_t          sec;
    tibint64_t          nsec;
} tibDateTime;

/** The realm server defines information about endpoints, transports
 * and formats, which lets applications communicate within a realm.
 *
 * A realm object is a local copy (within your program) of the
 * subset of the realm information that pertains to your application.
 * See @ref realm.h
 */
typedef struct __tibRealmId     *tibRealm;

/** @brief An event queue object is a FIFO queue for message and timer
 * events.  Programs can add and remove subscribers on a queue; create
 * and destroy timers on a queue; dispatch events from a queue; and
 * stop a queue in preparation to destroy it.
 */
typedef struct __tibEventQueueId    *tibEventQueue;

#ifdef  __cplusplus
}
#endif

#endif /* _INCLUDED_tib_types_h */
