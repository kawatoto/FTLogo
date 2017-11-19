/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: ftl.h.config 90137 2016-12-13 19:04:42Z $
 */


/**
 * @mainpage TIBCO FTL&reg; 5.2.1  
 * 
 * @section intro Introduction
 *
 * This documentation describes the C API for TIBCO FTL&reg;. 
 * 
 * @section usage User Examples
 *
 * TIBCO FTL&reg; includes sample application programs; see the directory
 * \c samples.
 *
 * @section Copyrights Copyright Notice
 * 
 * Copyright &copy; 2009-2017 TIBCO Software Inc. All Rights Reserved. Confidential & Proprietary. 
 *
 * This documentation contains proprietary and confidential information 
 * of TIBCO Software Inc. Use, disclosure, or reproduction is prohibited
 * without the prior express written permission of TIBCO Software Inc.
 *
 * @subsection restrights Restricted Rights Legend
 * 
 * Use, duplication, or disclosure by the Government is subject to 
 * restrictions set forth in subparagraph (c)(1)(ii) of the 
 * rights in Technical Data and Computer Software clause at 
 * DFARS 252. 227-7013.
 *
 * TIBCO Software Inc 
 * 3303 Hillview Ave, Palo Alto, CA 94304
 */

#ifndef _INCLUDED_tib_ftl_h
#define _INCLUDED_tib_ftl_h

#include "tib/tibexp.h"
#include "tib/except.h"
#include "tib/types.h"
#include "tib/props.h"
#include "tib/conmatch.h"
#include "tib/queue.h"
#include "tib/fldref.h"
#include "tib/msg.h"
#include "tib/pub.h"
#include "tib/realm.h"
#include "tib/sub.h"
#include "tib/advisory.h"
#include "tib/tiblock.h"
#include "tib/monitor.h"


#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file ftl.h
 * 
 * @brief Start and stop FTL\n
 * Log levels
 *
 * This file defines calls that start and stop FTL, and configure
 * logging.
 */

/** @brief Release string identifying the linked FTL library.
 *   
 * Programs can use this string to output the FTL version in a start
 * banner or in debug output.
 *
 * @return The release string that identifies the FTL library
 */
TIB_API
const char*
tib_Version(void);

/** @brief Compiled compatibility version.
 *
 * Your program must compile and link the same compatibility version
 * of the FTL library.  @ref tib_Open checks this correspondence, and
 * fails if they do not match.  This constant defines the compiled
 * compatibility version (which is independent of the software release
 * version).
 */
#define TIB_COMPATIBILITY_VERSION (2)

/** @brief Initialize FTL
 * 
 * This call opens (that is, initializes) FTL within your program, including
 * threads and global data structures.
 * 
 * The first time a program calls it, this call initializes FTL.  If
 * the program (or a third-party package) calls it again, it
 * increments a reference counter, but does not reinitialize FTL.
 * Calls to @ref tib_Close decrement the FTL reference counter, but
 * actually close (that is, terminate) FTL only when the counter
 * reaches zero.
 *
 * In each program, the number of calls to @ref tib_Open() and
 * @ref tib_Close() must match.
 *
 * @param e The exception object captures information about failures.
 * @param compatible_version The program \e must supply the constant @ref
 * TIB_COMPATIBILITY_VERSION.
 *  
 * @return void
 */
TIB_API
void
tib_Open(
    tibEx       e,
    tibint32_t  compatible_version);

/** @brief Cleanup FTL.
 * 
 * This call closes (that is, terminates) FTL within your program, and
 * reclaims related resources that @ref tib_Open allocated and
 * initialized.
 *
 * Calls to @ref tib_Close decrement the FTL reference counter, but
 * actually close (that is, terminate) FTL only when the counter
 * reaches zero.
 *
 * In each program, the number of calls to @ref tib_Open() and
 * @ref tib_Close() must match.
 *
 * Programs must not call tib_Close within a callback.
 *
 * Programs must close realm objects before closing FTL.
 * 
 * @param e The exception object captures information about failures.
 *  
 * @return void
 */
TIB_API
void
tib_Close(
    tibEx   e);

  /** @name Log Level Constants
   *
   * @ref tib_SetLogLevel supports these constants as log level values.
   */

  //@{

  /** @brief Off -- disable all tracing.
  */
#define TIB_LOG_LEVEL_OFF       "off"

  /** @brief Severe -- output trace entries only for severe events.
  */
#define TIB_LOG_LEVEL_SEVERE    "severe"

  /** @brief Warn -- output trace entries for warning and severe
      events.
  */
#define TIB_LOG_LEVEL_WARN      "warn"

  /** @brief Info -- output trace entries for information, warning
  * and severe events.
  */
#define TIB_LOG_LEVEL_INFO      "info"

  /** @brief Verbose -- output trace entries verbose, information,
  * warning and severe events.
  */
#define TIB_LOG_LEVEL_VERBOSE   "verbose"

  /** @brief Debug -- output trace entries for all events (debug, verbose,
  * information, warning and severe events).
  */
#define TIB_LOG_LEVEL_DEBUG     "debug"

  //@}

/** @brief Set the FTL log trace level.
 * 
 * This call sets the global log trace level for low-level FTL internal
 * calls.
 *
 * Valid levels:
 * \li @ref TIB_LOG_LEVEL_OFF
 * \li @ref TIB_LOG_LEVEL_SEVERE
 * \li @ref TIB_LOG_LEVEL_WARN
 * \li @ref TIB_LOG_LEVEL_INFO
 * \li @ref TIB_LOG_LEVEL_VERBOSE
 * \li @ref TIB_LOG_LEVEL_DEBUG
 *
 * For information about tuning the log level separately for
 * individual elements, see the topic Log Levels in the book
 * \b TIBCO \b FTL \b Development.
 *
 * @param e The exception object captures information about failures.
 * @param level The call sets the level to this value.
 *  
 * @return void
 */
TIB_API
void
tib_SetLogLevel(
    tibEx       e,
    const char  *level);

/** @brief Log callback.
 * 
 * Programs may define a callback of this type, and register it
 * to process all FTL log statements.
 * 
 * <b> Log Callback Restrictions: </b>
 *
 * For important restrictions, see the section Log Output Targets in
 * <i> TIBCO FTL Development</i>.
 * 
 * @param timestamp FTL supplies the time at which it emitted the log statement.
 * @param statement FTL supplies the log statement.
 * @param closure FTL supplies this closure argument, which the program supplied
 *        to @ref tib_SetLogCallback when registering the callback.
 *  
 * @return void
 */
typedef void (*tibLogCallback) (tibDateTime    timestamp,
                                const char     *statement,
                                void           *closure);

/** @brief Register a global log callback.
 * 
 * If you register a log callback (at most one), then FTL directs
 * all log statements to that callback.
 *
 * @param e The exception object captures information about failures.
 * @param logCallback FTL invokes this callback to process all log statements.
 * @param properties Reserved for future use.
 *        \n To ensure forward compatibility, programmers must supply \c NULL.
 * @param closure FTL supplies this closure object to the callback
 *        with each log statement.
 *  
 * @return void
 */
TIB_API
void
tib_SetLogCallback(
    tibEx             e,
    tibLogCallback    logCallback,
    tibProperties     properties,
    void              *closure);

/** @brief Arrange rotating log files.
 *
 * The filename extension \c .0 indicates the current log file.
 * Rotation shifts each file by incrementing its numeric extension.
 * If the number of files would exceed the maximum,
 * rotation deletes the oldest file.
 * 
 * @param e The exception object captures information about failures.
 * @param filePrefix All log files begin with this filename prefix.
 * @param maxFileSize FTL rotates the log files when the current log file
 *        exceeds this limit (in bytes).
 *        This value must be greater than 102400 (100 kilobytes).
 * @param maxFiles FTL limits the number of log files to this maximum.
 * @param properties Reserved for future use.
 *        \n To ensure forward compatibility, programmers must supply \c NULL.
 *  
 * @return void
 */
TIB_API
void
tib_SetLogFiles(
    tibEx            e,
    const char       *filePrefix,
    tibint64_t       maxFileSize,
    tibint32_t       maxFiles,
    tibProperties    properties);

#ifdef  __cplusplus
}
#endif

#endif /* _INCLUDED_tib_ftl_h */
