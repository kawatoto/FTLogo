/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: props.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_props_h
#define _INCLUDED_tib_props_h

#include "tib/except.h"
#include "tib/types.h"
#include "tib/tibexp.h"
#include "tib/msg.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file props.h
 * 
 * @brief Properties objects represent a set of configuration
 * properties (name/value pairs).
 *
 * This file defines properties objects and calls that manipulate them.
 */

/** @brief Properties objects hold name/value pairs.
 *
 * Programs pass properties to FTL object creation calls.
 *
 * Many property names include an indication of the required datatype for the value.
 * For example, @ref TIB_EVENTQUEUE_PROPERTY_INT_DISCARD_POLICY
 * requires an integer value.
 *
 * Properties objects are not thread-safe.
 */
typedef struct __tibProperties  *tibProperties;

/** @brief Create an empty property list object without any default values.
 * 
 * As a mechanism for configuring objects, some FTL creation calls
 * accept a properties object, which contains paired property names
 * and values.
 *
 * Properties objects are not thread-safe.
 * 
 * @param e The exception object captures information about failures.
 * 
 * @return On success, this call returns a new properties object.\n
 *         On failure, this call returns \c NULL.
 */
TIB_API
tibProperties
tibProperties_Create(
    tibEx                       e
);

/** @brief Set a boolean property.
 * 
 * \li If \e name is not yet in the \e properties object, this
 *      call adds the name and value pair.
 * \li If \e name is already in the \e properties object, this call
 *      changes its value.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call modifies this properties object.
 * @param name The call sets this property.
 * @param value The call sets the property to this value.
 * 
 * @throws TIB_ILLEGAL_STATE indicates the property already exists,
 *         but its value has a different type.
 */
TIB_API
void
tibProperties_SetBoolean(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name,
    tibbool_t                   value
);

/** @brief Get a boolean property.
 * 
 * Return the boolean value of property \e name.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call searches this properties object.
 * @param name The call gets the value of this property.
 *  
 * @return a boolean property value
 *
 * @throws TIB_NOT_FOUND indicates the property does not exist.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property exists, but its
 *         value has a different type.
 */
TIB_API
tibbool_t
tibProperties_GetBoolean(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);

/** @brief Set an integer property.
 * 
 * \li If \e name is not yet in the \e properties object, this
 *      call adds the name and value pair.
 * \li If \e name is already in the \e properties object, this call
 *      changes its value.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call modifies this properties object.
 * @param name The call sets this property.
 * @param value The call sets the property to this value.
 * 
 * @throws TIB_ILLEGAL_STATE indicates the property already exists,
 *         but its value has a different type.
 */
TIB_API
void
tibProperties_SetInt(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name,
    tibint32_t                  value
);

/** @brief Get an integer property.
 * 
 * Return the integer value of property \e name.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call searches this properties object.
 * @param name The call gets the value of this property.
 *  
 * @return an integer property value
 *
 * @throws TIB_NOT_FOUND indicates the property does not exist.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property exists, but its
 *         value has a different type.
 */
TIB_API
tibint32_t
tibProperties_GetInt(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);

/** @brief Set a long integer property.
 * 
 * \li If \e name is not yet in the \e properties object, this
 *      call adds the name and value pair.
 * \li If \e name is already in the \e properties object, this call
 *      changes its value.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call modifies this properties object.
 * @param name The call sets this property.
 * @param value The call sets the property to this value.
 * 
 * @throws TIB_ILLEGAL_STATE indicates the property already exists,
 *         but its value has a different type.
 */
TIB_API
void
tibProperties_SetLong(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name,
    tibint64_t                  value
);

/** @brief Get a long integer property.
 * 
 * Return the long integer value of property \e name.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call searches this properties object.
 * @param name The call gets the value of this property.
 *  
 * @return a long integer property value
 *
 * @throws TIB_NOT_FOUND indicates the property does not exist.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property exists, but its
 *         value has a different type.
 */
TIB_API
tibint64_t
tibProperties_GetLong(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);

/** @brief Set a string property.
 * 
 * \li If \e name is not yet in the \e properties object, this
 *      call adds the name and value pair.
 * \li If \e name is already in the \e properties object, this call
 *      changes its value.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call modifies this properties object.
 * @param name The call sets this property.
 * @param value The call sets the property to this value.
 * 
 * @note The call copies the \e value, so you may re-use the string
 *       storage after this call returns.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property already exists,
 *         but its value has a different type.
 */
TIB_API
void
tibProperties_SetString(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name,
    const char*                 value
);

/** @brief Get a string property.
 * 
 * Return the string value of property \e name.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call searches this properties object.
 * @param name The call gets the value of this property.
 *  
 * @return a string property value
 *
 * @throws TIB_NOT_FOUND indicates the property does not exist.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property exists, but its
 *         value has a different type.
 */
TIB_API
const char*
tibProperties_GetString(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);

/** @brief Set a tibMessage property.
 * 
 * \li If \e name is not yet in the \e properties object, this
 *      call adds the name and value pair.
 * \li If \e name is already in the \e properties object, this call
 *      changes its value.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call modifies this properties object.
 * @param name The call sets this property.
 * @param value The call sets the property to this value.
 * 
 * @note The call copies the \e value, so you may re-use the tibMessage
 *       storage after this call returns.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property already exists,
 *         but its value has a different type.
 */
TIB_API
void
tibProperties_SetMessage(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name,
    const tibMessage            value
);

/** @brief Get a tibMessage property.
 * 
 * Return the tibMessage value of property \e name.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call searches this properties object.
 * @param name The call gets the value of this property.
 *  
 * @return a read-only tibMessage property value
 *
 * @throws TIB_NOT_FOUND indicates the property does not exist.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property exists, but its
 *         value has a different type.
 */
TIB_API
const tibMessage
tibProperties_GetMessage(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);


/** @brief Set a double property.
 * 
 * \li If \e name is not yet in the \e properties object, this
 *      call adds the name and value pair.
 * \li If \e name is already in the \e properties object, this call
 *      changes its value.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call modifies this properties object.
 * @param name The call sets this property.
 * @param value The call sets the property to this value.
 * 
 * @throws TIB_ILLEGAL_STATE indicates the property already exists,
 *         but its value has a different type.
 */
TIB_API
void
tibProperties_SetDouble(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name,
    tibdouble_t                 value
);

/** @brief Get a double property.
 * 
 * Return the double value of property \e name.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call searches this properties object.
 * @param name The call gets the value of this property.
 *  
 * @return a double property value
 *
 * @throws TIB_NOT_FOUND indicates the property does not exist.
 *
 * @throws TIB_ILLEGAL_STATE indicates the property exists, but its
 *         value has a different type.
 */
TIB_API
tibdouble_t
tibProperties_GetDouble(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);

/** @brief Check whether a property exists.
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call searches this properties object.
 * @param name The call checks for this property.
 *  
 * @return tibtrue if the property exists, tibfalse otherwise.
 *
 */
TIB_API
tibbool_t
tibProperties_Exists(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);

/** @brief Remove a property.
 * 
 * Remove the property \e name from \e properties. 
 *
 * @param e The exception object captures information about failures.
 * @param properties The call removes the property from this properties object.
 * @param name The call removes this property.
 *
 * @return @ref tibtrue indicates the call removed the property.\n
 *         @ref tibfalse indicates the property did not exist.
 */
TIB_API
tibbool_t
tibProperties_Remove(
    tibEx                       e,
    tibProperties               properties,
    const char*                 name
);

/** @brief Destroy a property list object.
 * 
 * An application can destroy a properties object to reclaim its
 * resources.  Destroying a properties object does not invalidate
 * objects created with it.  (For example, the queue creation call
 * copies property values into the new queue.)
 * 
 * @param e The exception object captures information about failures.
 * @param properties The call destroys this properties object.
 *  
 * @return void
 */
TIB_API
void
tibProperties_Destroy(
    tibEx                       e,
    tibProperties               properties
);


TIB_API
tibProperties
tibProperties_Copy(
    tibEx                       e,
    tibProperties               properties
);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_props_h */
