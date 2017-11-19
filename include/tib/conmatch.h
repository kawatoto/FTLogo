/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: conmatch.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_conmatch_h
#define _INCLUDED_tib_conmatch_h

#include "tib/except.h"
#include "tib/types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file conmatch.h
 * 
 * @brief Content matchers filter messages based on their content.
 */


/** @brief A content matcher object filters messages based on their content.
 *
 * Programs may supply a content matcher object to @ref tibSubscriber_Create.
 */
  typedef struct __tibContentMatcher *tibContentMatcher;

/** @brief Create a content matcher object.
 * 
 * An application can create a matcher to register interest in a
 * stream of messages.
 *
 * Content matcher objects are not thread-safe.
 * 
 * \b Match \b Syntax:
 *
 * \c { \e "fieldname1" \c : \e value1 \c ,  ...  \c , \e "fieldnameN" \c : \e valueN \c } \n
 *
 * \li Enclose the list of field:value pairs in curly braces.
 * \li Separate field:value pairs with commas.
 * \li Enclose field names in double-quote characters.
 *     Precede quote characters with an escape character, as needed.
 * \li Do \e not enclose boolean tokens in double-quote characters.
 * \li Each field name can appear at most once.
 * \li Values can be long integers or strings -- or the special boolean tokens
 *     \c true or \c false.
 * \li When \e value is a string, its maximum length is 256 characters.
 * \li Whitespace is ignored (except within double-quote characters).
 *
 * \b Match \b Semantics:
 *
 * \li If the match string specifies a field with boolean token \c true,
 *     that field must be \e present in the message in order to match.
 * \li If the match string specifies a field with boolean token \c false,
 *     that field must be \e absent from the message in order to match.
 * \li If the match string specifies a field with either a string or
 *     long integer value, that field must be present in the message with that value.
 * \li When comparing a field name or a value, all comparisons must be exact.
 *      Matching does not support wildcards nor regular expressions.
 *
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration information
 *              about the application and formats.
 * @param matchString This string determines message interest.
 * 
 * @return On success, this call returns a new content matcher object.\n
 *         On failure, this call returns \c NULL.
 */
TIB_API
tibContentMatcher
tibContentMatcher_Create(
    tibEx               e,
    tibRealm            realm,
    const char*         matchString);

/** Destroy a content matcher object.
 * 
 * An application can destroy a content matcher object to reclaim its
 * resources.  Destroying a matcher does not invalidate objects
 * created with the matcher.  (For example, the subscriber creation
 * call copies its matcher argument into the new subscriber.)
 *
 * @param e The exception object captures information about failures.
 * @param matcher The call destroys this content matcher object.
 *  
 * @return void
 */
TIB_API
void
tibContentMatcher_Destroy(
    tibEx             e,
    tibContentMatcher matcher);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_conmatch_h */
