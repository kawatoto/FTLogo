/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: fldref.h 90137 2016-12-13 19:04:42Z $
 */
#ifndef _INCLUDED_tib_fldref_h
#define _INCLUDED_tib_fldref_h

#include "tib/tibexp.h"
#include "tib/except.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file fldref.h
 * 
 * @brief Field references enable efficiency gains when accessing message fields.
 *
 * This file defines field reference objects and the calls that manipulate them.
 */

/** @brief Field reference object type.
 *
 * The API provides two versions of each message field accessor call --
 * one accepts a \e field \e name, while the other accepts a \e field \e
 * reference \e object (@ref tibFieldRef).  Access by field
 * reference is more efficient than access by name alone.
 *
 * Field reference objects contain a field name, along with internal
 * information that enables efficient access.
 *
 * Programs can repeatedly use a field reference object to efficiently
 * access a field -- even across messages of different formats.
 *
 * For example, if formats \c A and \c B both have a field named \c
 * foo, then a field reference object with field name \c foo accesses
 * the correct field in messages of either format.
 *
 * Field reference objects are thread-safe.  (However, the field
 * reference that @ref tibMessageIterator_GetNext returns is an
 * exception to this rule.)
 */
  typedef struct __tibFieldRef *tibFieldRef;  

/** @brief Create a field reference object.
 *
 * This call uses its field name argument to look up internal
 * information that enables efficient access, and caches that
 * information in the new field reference object.
 *
 * @param e The exception object captures information about failures.
 * @param fieldName The call embeds this field name in the new field reference object.
 *  
 * @return a new @ref tibFieldRef object
 */
TIB_API
tibFieldRef
tibFieldRef_Create(
    tibEx e,
    const char*     fieldName);

/** @brief Destroy a field reference object.
 * 
 * Destroying a field reference object frees all resources associated
 * with the field reference.
 *
 * It is illegal to destroy a field reference object obtained from
 * @ref tibMessageIterator_GetNext.
 * 
 * @param e The exception object captures information about failures.
 * @param f The call destroys this field reference object.
 *  
 * @return void
 */
TIB_API
void
tibFieldRef_Destroy(
    tibEx e,
    tibFieldRef         f);

/** @brief Get the field name from a field reference object.
 * 
 * The name that this call returns becomes invalid when the field
 * reference object becomes invalid.
 * 
 * @param e The exception object captures information about failures.
 * @param f The call returns the name from this field reference object.
 *  
 * @return the field name of the reference
 */
TIB_API
const char*
tibFieldRef_GetFieldName(
    tibEx e,
    tibFieldRef  f);

#if defined(__cplusplus)
}
#endif

#endif  /* _INCLUDED_tib_fldhndl_h */

