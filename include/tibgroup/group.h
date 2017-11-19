/* 
 * Copyright (c) 2011-2016 TIBCO Software Inc. 
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 * 
 * $Id: group.h 90137 2016-12-13 19:04:42Z $
 * 
 */

#ifndef _INCLUDED_group_h
#define _INCLUDED_group_h

#include "tib/ftl.h"
#include "tibgrpex.h"

#if defined(__cplusplus)
extern "C" {
#endif


/**
 * @file group.h
 *
 * @brief The group facility can coordinate fault-tolerant operation,
 * or distribute operating roles among application process instances.
 *
 * This file defines the public API of the group facility.  
 
 * A group server tracks group members, and assigns an ordinal to each member.
 * The group facility raises advisories to inform the member of its
 * current ordinal.  Based on its ordinal, each member operates in a particular
 * application-specific role.
 *
 * The group server also reports the group status as members join and leave
 * the group, or disconnect from the server.  The group facility raises advisories
 * to inform members and observers of such changes in status.
 */

/** @brief Property name for group activation interval; \c double.
 *
 * Supply this (optional) property to @ref tibGroup_Join, which
 * derives heartbeat and timeout intervals from this property
 * value (in seconds). 
 * When absent, the default value is 5 seconds.
 */
#define TIB_GROUP_PROPERTY_DOUBLE_ACTIVATION_INTERVAL "com.tibco.ftl.group.activationinterval"

/** @brief Property name for group member descriptor; \c tibMessage.
 *
 * To identify a group member to the other members,
 * supply this optional property to @ref tibGroup_Join.
 *
 * The application programmer determines the content of the descriptor message.
 * In best practice, each group member process supplies a unique descriptor.
 */
#define TIB_GROUP_PROPERTY_MESSAGE_MEMBER_DESCRIPTOR "com.tibco.ftl.group.memberdescriptor"

/** @brief Property name to monitor a group as an observer; \c tibbool_t.
 *
 * To monitor a group as an observer,
 * supply this optional property with value \c tibtrue
 * to @ref tibGroup_Join.
 *
 * An observer never receives an ordinal, and does not trigger group member events.
 */
#define TIB_GROUP_PROPERTY_BOOLEAN_OBSERVER "com.tibco.ftl.group.observer"


  /** @brief The advisory message pertains to the group library
   * (module).
   *
   * This string value can appear in the \c module field.
   *
   * @see TIB_ADVISORY_FIELD_MODULE
   */
#define TIB_GROUP_ADVISORY_MODULE_GROUP "GROUP"

  /** @brief The advisory message reports an ordinal update.
   *
   * This string value can appear in the \c name field.
   *
   * @see TIB_ADVISORY_FIELD_NAME
   * @see TIB_GROUP_ADVISORY_FIELD_GROUP
   * @see TIB_GROUP_ADVISORY_FIELD_ORDINAL
   * @see TIB_ADVISORY_SEVERITY_INFO
   */
#define TIB_GROUP_ADVISORY_NAME_ORDINAL_UPDATE "ORDINAL_UPDATE"

/** @brief Group name (field name).
 *
 * The string value of the \c group name field is the name of the
 * group to which the advisory pertains.
 */
#define TIB_GROUP_ADVISORY_FIELD_GROUP "group"

/** @brief Group member ordinal (field name).
 *
 * The positive long value of the \c ordinal field represents the new
 * ordinal of the group member.
 *
 * The value -1 indicates that the group object is disconnected from
 * the group server.  The group object automatically attempts to
 * reconnect, and continues until the program explicitly destroys it.
 * Meanwhile, the group server could reassign the member's previous
 * ordinal to another group member.
 *
 * Zero is a reserved value.
 */
#define TIB_GROUP_ADVISORY_FIELD_ORDINAL "ordinal"

/** @brief The advisory message reports a group status update.
 *
 * This string value can appear in the \c name field.
 *
 * @see TIB_ADVISORY_FIELD_NAME
 * @see TIB_GROUP_ADVISORY_FIELD_GROUP
 * @see TIB_GROUP_FIELD_GROUP_MEMBER_STATUS_LIST
 * @see TIB_GROUP_FIELD_GROUP_MEMBER_DESCRIPTOR
 * @see TIB_GROUP_FIELD_GROUP_MEMBER_EVENT
 * @see TIB_GROUP_FIELD_GROUP_SERVER_AVAILABLE
 * @see TIB_ADVISORY_SEVERITY_INFO
 */
#define TIB_GROUP_ADVISORY_NAME_GROUP_STATUS "GROUP_STATUS"

/** @brief Group member status list  (field name).
 *
 * This field can appear in group status advisory messages.
 * Its value is an array of member status submessages.
 * Each submessage reports the status of one group member.
 */
#define TIB_GROUP_FIELD_GROUP_MEMBER_STATUS_LIST "group_member_status_list"

/** @brief Group member descriptor (field name).
 *
 * This field can can appear in a member status submessage.
 * Its value is a group member descriptor,
 *   that is, a message that identifies a group member.
 */
#define TIB_GROUP_FIELD_GROUP_MEMBER_DESCRIPTOR "group_member_descriptor"

/** @brief Group member event (field name).
 *  
 * This field can appear in a member status submessage.
 * Its value is a @ref tibMemberEvent,
 *   that is, a long value that indicates the group member's current state
 *   or recent state change.
 */
#define TIB_GROUP_FIELD_GROUP_MEMBER_EVENT  "group_member_event"

/** @brief Group member connection to group server (field name).
 * 
 * This field can appear in a group status message.
 * Its value is a @ref tibMemberServerConnection,
 *   that is, a long value that indicates whether the application process
 *   is connected to the group server.
 */    
#define  TIB_GROUP_FIELD_GROUP_SERVER_AVAILABLE  "group_server_available"

/** @brief Group member event; enumeration.
 * 
 * The group member event field of a member status submessage
 * can contain a value of this enumerated type.
 *
 * @see TIB_GROUP_FIELD_GROUP_MEMBER_EVENT
 */
typedef enum
{
    GROUP_MEMBER_JOINED = 0,  /**< The member joined the group, or is a member connected to the group server. */
    GROUP_MEMBER_LEFT,        /**< The member left the group using an API call. */
    GROUP_MEMBER_DISCONNECTED /**< The member unexpectedly disconnected from the group server. */
} tibMemberEvent;

/** @brief Group member connection to group server; enumeration.
 *  
 *  The group server available field of a group status advisory message
 *  can contain a value of this enumerated type.
 *
 * @see TIB_GROUP_FIELD_GROUP_SERVER_AVAILABLE
 */
typedef enum
{
    GROUP_SERVER_UNAVAILABLE = 0, /**< The application is \b not connected to the group server. */
    GROUP_SERVER_AVAILABLE        /**< The application is connected to the group server. */
} tibMemberServerConnection;

/** @brief A group object represents the membership or potential membership
 * of an application process in a group.
 */
typedef struct __tibGroupId     *tibGroup;

/** @brief Join a group, and create a group object.
 *
 * The group object connects to the group server.
 *
 * In best practice, programs first subscribe to the \c ORDINAL_UPDATE
 * advisory, and then join a group.
 *
 * @param e The exception object captures information about failures.
 * @param realm The call contacts the group server, which operates
 *              inside the realm server for this realm object.
 *              Furthermore, the group facility raises advisory
 *              messages within this realm.
 * @param groupName The program supplies a NULL-terminated string.
 * @param props Optional; to omit, supply \c NULL. \n
 *              Properties configure interaction with the group server
 *              and with other group members.
 *              <ul>
 *               <li> @ref TIB_GROUP_PROPERTY_DOUBLE_ACTIVATION_INTERVAL </li>
 *               <li> @ref TIB_GROUP_PROPERTY_MESSAGE_MEMBER_DESCRIPTOR </li>
 *               <li> @ref TIB_GROUP_PROPERTY_BOOLEAN_OBSERVER </li>
 *              </ul>
 *
 * @return This call returns a group object.
 */
TIBGROUP_API
tibGroup
tibGroup_Join(
    tibEx          e,
    tibRealm       realm,
    const char     *groupName,
    tibProperties  props);

/** @brief Leave a group, and destroy the group object.
 *
 * This call informs the group server that the member is leaving the
 * group.  In response, the group server revises the ordinals of
 * the remaining group members appropriately.
 *
 * This call releases all resources held by the group object.
 *
 * Before your program calls @ref tibRealm_Close, it must first
 * destroy all the group objects that it has created.
 *
 * @param e The exception object captures information about failures.
 * @param group The call leaves this group.
 *
 * @return void
 */
TIBGROUP_API
void
tibGroup_Leave(
    tibEx          e,
    tibGroup       group);

/** @brief Get a copy of the group name.
 *
 * This call copies the group name string that your program supplied to
 * @ref tibGroup_Join.
 *
 * If a program passes \c NULL as the buffer argument, this call
 * returns the length of the group name string, including a NULL
 * terminating character, without actually writing the string.
 *
 * If the non-NULL buffer is too short for the entire name
 * string, the call writes to the end of the buffer, truncating the
 * string, and returns the actual number of bytes in the buffer.
 *
 * @param e The exception object captures information about failures.
 * @param group The call gets a copy of the name from this group object.
 * @param buffer The call copies the name into this string buffer.
 * @param buflen The program supplies the length of buffer (in bytes).
 *
 * @return This call returns the length of the group name string,
 * including the NULL terminating character.
 *
 * @see tibGroup_Join
 */
TIBGROUP_API
tibint32_t
tibGroup_GetName(
    tibEx          e,
    tibGroup       group,
    char           *buffer,
    tibint32_t     buflen);

/** @brief Release string identifying the linked TIBCO FTL group library.
 *   
 * Programs can use this string to output the TIBCO FTL group version in a start
 * banner or in debug output.
 *
 * @return The release string that identifies the TIBCO FTL group library
 */
TIBGROUP_API
const char*
tibGroup_Version(
    void);

#if defined(__cplusplus)
}
#endif

#endif
