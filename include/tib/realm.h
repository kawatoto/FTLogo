/*
 * Copyright (c) 2009-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: realm.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_realm_h
#define _INCLUDED_tib_realm_h

#include "tib/types.h"
#include "tib/pub.h"
#include "tib/sub.h"
#include "tib/tiblock.h"
#include "tib/tibexp.h"
#include "tib/map.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file realm.h
 * 
 * @brief Realm objects contain definitions of message formats,
 * endpoints and transport maps.
 *
 * This file defines realm objects and calls that manipulate them.
 */

/** @brief Property name for realm server username; string.
 *
 * Supply this property to @ref tibRealm_Connect, which identifies
 *      your program to the realm server with the username value you supply.
 */
#define TIB_REALM_PROPERTY_STRING_USERNAME "com.tibco.ftl.client.username"

/** @brief Property name for realm server password; string.
 *
 * Supply this property to @ref tibRealm_Connect, which authenticates
 *      your program to the realm server with the password value you supply.
 */
#define TIB_REALM_PROPERTY_STRING_USERPASSWORD "com.tibco.ftl.client.userpassword"

/** @brief Property name for URL of backup realm server for fault tolerance; string.
 *
 * Supply this property to @ref tibRealm_Connect.
 * The realm object attempts to connect to a backup server at this URL
 * if it cannot connect to its regular server.
 */
#define TIB_REALM_PROPERTY_STRING_SECONDARY_SERVER "com.tibco.ftl.client.secondary"

/** @brief Trust any realm server; property value constant.
 *
 * This constant is one possible value of the property
 * @ref TIB_REALM_PROPERTY_LONG_TRUST_TYPE
 * in the method @ref tibRealm_Connect.
 *
 * The client trusts any realm server without
 * verifying trust in the server's certificate.
 * Do <b>not</b> use this value except for
 * convenience in development and testing.
 * It is <b>not</b> secure.
 */
#define TIB_REALM_HTTPS_CONNECTION_TRUST_EVERYONE                0

/** @brief Trust a realm server if its certificate
 *         is in a trust file;
 *         property value constant.
 *
 * This constant is one possible value of the property
 * @ref TIB_REALM_PROPERTY_LONG_TRUST_TYPE
 * in the method @ref tibRealm_Connect.
 *
 * The client trusts the realm server based on the
 * trust file
 * created by the realm server
 * and distributed by the administrator.
 *
 * Specify the file path of the trust file
 * in the property
 * @ref TIB_REALM_PROPERTY_STRING_TRUST_FILE.
 */
#define TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_FILE     1

/** @brief Trust a realm server if its certificate
 *         is in a trust string;
 *         property value constant.
 *
 * This constant is one possible value of the property
 * @ref TIB_REALM_PROPERTY_LONG_TRUST_TYPE
 * in the method @ref tibRealm_Connect.
 *
 * The client trusts the realm server based on 
 * a trust string.
 *
 * Specify the trust string in the property
 * @ref TIB_REALM_PROPERTY_STRING_TRUST_PEM_STRING.
 */
#define TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_STRING     2

/** @brief Property name for secure realm server trust type; long.
 *
 * When connecting to a secure realm server,
 * supply this property to @ref tibRealm_Connect.
 * Its value determines
 * trust in the realm server's certificate.
 *
 * @see TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_FILE
 * @see TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_STRING
 * @see TIB_REALM_HTTPS_CONNECTION_TRUST_EVERYONE
 */
#define TIB_REALM_PROPERTY_LONG_TRUST_TYPE                       "com.tibco.ftl.trust.type"

/** @brief Property name for secure realm server trust file; string.
 *
 * When connecting to a secure realm server,
 * supply this property to @ref tibRealm_Connect.
 *
 * If the realm connect call specifies the trust type as
 * @ref TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_FILE,
 * use this property to indicate the location of the trust file.
 */
#define TIB_REALM_PROPERTY_STRING_TRUST_FILE                    "com.tibco.ftl.trust.file"

/** @brief Property name for secure realm server trust data; string.
 *
 * When connecting to a secure realm server,
 * supply this property to @ref tibRealm_Connect.
 *
 * If the realm connect call specifies the trust type as
 * @ref TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_STRING,
 * use this property to supply the trust string
 * in PEM format.
 */
#define TIB_REALM_PROPERTY_STRING_TRUST_PEM_STRING              "com.tibco.ftl.trust.pem.string"

/** @brief Property name for application instance identifier; string.
 *
 * We recommend including this property in the realm connect call of
 * every application program.  It gives administrators flexible
 * control over application deployment and over the transports that
 * carry message traffic.
 *
 * For example, the program obtains the application identifier as a
 * command line argument (or through another configuration mechanism).
 * If an application identifier is present, then the program passes it
 * to @ref tibRealm_Connect as the value of this property.
 *
 * Within the realm, administrators can define one or more application
 * instances, which tailor the connections between endpoints and transports.
 * The value of this property selects from among those instances.
 */
#define TIB_REALM_PROPERTY_STRING_APPINSTANCE_IDENTIFIER "com.tibco.ftl.client.appinstance.identifier"

/** @brief Property name for a client label; string.
 *
 * We recommend including this property in the realm connect call of
 * every application program.  It gives administrators a consistent view
 * of monitoring data by an "application stripe" or "business instance name".
 *
 * For example, if an administrator wishes to view day over day monitoring data 
 * for a given application then a client label should be provided.
 * 
 * If a client label is present, then the program passes it
 * to @ref tibRealm_Connect as the value of this property.
 *
 * This field is not used internally.
 */
#define TIB_REALM_PROPERTY_STRING_CLIENT_LABEL "com.tibco.ftl.client.label"

/// @brief Conditions in notifications.
typedef enum
{
  /** The realm server administratively disabled this client.
   *
   * The current realm object no longer supports FTL functionality.
   * Nonetheless, the program can still
   * call close or destroy functions on FTL objects in the realm.
   *
   * To resume FTL functionality, some programs may
   * first call @ref tibRealm_Close on the disabled realm object,
   * and then create a new realm object by calling
   * @ref tibRealm_Connect.
   *
   * Other programs may restart or exit.
   */
  TIB_CLIENT_DISABLED = 0

} tibRealmNotificationType;

/** @brief Notification handler callback.
 * 
 * In some situations, FTL must notify programs of conditions
 * that preclude the use of event queues (as the communications
 * medium).  Instead of sending an advisory, FTL notifies the program
 * through an out-of-band mechanism.
 * 
 * Programs define a notification handler callback (of this type) to
 * process these out-of-band notifications.  Programs register the callback
 * using @ref tibRealm_SetNotificationHandler.
 * 
 * @param e FTL supplies callbacks with a clear exception object;
 *          your callback code may use it in its FTL API calls.\n
 *          When the completion callback returns, FTL does not examine
 *          the exception object for errors, nor does FTL return that
 *          exception to your program code.
 * @param type A notification code, which indicates the type of condition.
 * @param reason A string describing the reason for the notification.
 * @param closure The closure associated with the handler.
 *  
 * @return void
 */
typedef void
(*tibRealmNotificationHandler) (
    tibEx                       e,
    tibRealmNotificationType    type,
    const char                  *reason,
    void                        *closure);

/** @brief Connect to a realm server, and create a realm object.
 *
 * The realm server defines information about endpoints, transports
 * and formats, which lets applications communicate within a realm.
 * The realm object is a local copy (within your program) of the
 * subset of the realm information that pertains to your application.
 *
 * @note We recommend that each application process maintain only one
 * connection to each relevant realm server.  Realm objects are
 * thread-safe; you can safely use the same realm object in all the
 * threads of an application program.  We strongly discourage
 * duplicate connections to the same realm server in several threads,
 * which would duplicate the realm object.
 *
 * @param e The exception object captures information about failures.
 * @param serverUrl The call contacts the realm server at this URL.
 * @param appName The program supplies a NULL-terminated string.
 *                The call sends this application name to the realm server.
 *                The server responds with an application-specific subset
 *                of the realm information.
 *                To use the default application, supply \c NULL.
 * @param props Properties configure interaction with the realm server.
 *              Required for connecting to a secure realm server, 
 *              for authentication, and for fault tolerance.
 *              Otherwise optional; to omit, supply \c NULL.
 *              <ul>
 *               <li> TLS Secure Realm Server Properties
 *                <ul>
 *                 <li> @ref TIB_REALM_PROPERTY_LONG_TRUST_TYPE </li>
 *                 <li> @ref TIB_REALM_PROPERTY_STRING_TRUST_FILE </li>
 *                 <li> @ref TIB_REALM_PROPERTY_STRING_TRUST_PEM_STRING </li>
 *                </ul>
 *               </li>
 *               <li> Authenication Properties
 *                <ul>
 *                 <li> @ref TIB_REALM_PROPERTY_STRING_USERNAME </li>
 *                 <li> @ref TIB_REALM_PROPERTY_STRING_USERPASSWORD </li>
 *                </ul>
 *               </li>
 *               <li> Fault Tolerance Property
 *                <ul>
 *                 <li> @ref TIB_REALM_PROPERTY_STRING_SECONDARY_SERVER </li>
 *                </ul>
 *               </li>
 *               <li> Application Instance Property
 *                <ul>
 *                 <li> @ref TIB_REALM_PROPERTY_STRING_APPINSTANCE_IDENTIFIER </li>
 *                </ul>
 *               </li>
 *              </ul>
 *
 * @return This call returns a realm object.
 *
 * @see TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_FILE
 * @see TIB_REALM_HTTPS_CONNECTION_USE_SPECIFIED_TRUST_STRING
 * @see TIB_REALM_HTTPS_CONNECTION_TRUST_EVERYONE
 */
TIB_API
tibRealm
tibRealm_Connect(
    tibEx           e,
    const char      *serverUrl,
    const char      *appName,
    tibProperties   props);

/** @brief Destroy a dynamic durable.
 *
 * Destroying a dynamic durable removes it from the store.
 *
 * If one client destroys a dynamic durable while other clients have
 * subscribers to it, subsequent behavior is undefined.
 *
 * It is an error to use this call
 * to unsubscribe from a static durable.
 *
 * @param e The exception object captures information about failures.
 * @param realm The call destroys a dynamic durable in this realm.
 * @param endpointName The call destroys a dynamic durable
 *                     in the store associated with this endpoint.
 * @param dynamicDurableName The call destroys the dynamic durable with this name.
 *
 * @return void
 */
TIB_API
void
tibRealm_Unsubscribe(
    tibEx               e,
    tibRealm            realm,
    const char          *endpointName,
    const char          *dynamicDurableName);

/** @brief Close a realm object.
 * 
 * Closing a realm object frees all the resources associated with the
 * realm, and invalidates all objects created using the realm
 * (including messages, publishers, subscribers and content matchers).
 *
 * However, closing a local realm object does not change or destroy
 * any definitions on the realm server.
 *
 * Programs must not call tibRealm_Close within a callback.
 *
 * Programs must close realm objects before closing FTL.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The call closes this realm.
 *  
 * @return void
 */
TIB_API
void
tibRealm_Close(
    tibEx               e,
    tibRealm            realm
);

/** @brief Register the notification handler callback.
 *
 * Programs may register at most one notification handler per realm.
 * We recommend that all programs do so.
 * For background information, see @ref tibRealmNotificationHandler.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The call sets the notification handler for this realm.
 * @param handler The call sets this callback function to handle notifications.
 * @param closure Notification passes this closure to the callback.
 *
 * @return void
 */
TIB_API
void
tibRealm_SetNotificationHandler(
    tibEx                       e,
    tibRealm                    realm,
    tibRealmNotificationHandler handler,
    void                        *closure);


/** @brief Get a copy of the realm server URL.
 *
 * This call copies the URL string that your program supplied to
 * @ref tibRealm_Connect.
 *
 * If a program passes \c NULL as the buffer argument, this call
 * returns the length of the URL string (including a NULL
 * terminating character), without actually writing it.
 *
 * If the non-null buffer is too short for the entire URL
 * string, the call writes to the end of the buffer, truncating the
 * string, and returns the actual number of bytes in the buffer.
 *
 * @param e The exception object captures information about failures.
 * @param realm The call gets a copy of the server URL from this
 * realm.
 * @param buffer The call copies the URL into this string buffer.
 * @param size The program supplies the length of buffer (in bytes).
 *
 * @return This call returns the length of the realm server URL string
 * (including the NULL terminating character).
 *
 * @see tibRealm_Connect
 */
TIB_API
tibint32_t
tibRealm_GetServerUrl(
    tibEx       e,
    tibRealm    realm,
    char        *buffer,
    tibint32_t  size);

/** @brief Get a copy of the realm connection properties.
 *
 * This call returns a copy of the properties object that your program
 * supplied to @ref tibRealm_Connect.  Your program must destroy this
 * properties object to reclaim its resources
 * (see @ref tibProperties_Destroy).
 *
 * If your program did not supply any properties to the realm connect
 * call, then this call returns \c NULL.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The call returns a copy of the connect properties from
 * this realm.
 *
 * @return This call returns a properties object.
 *
 * @see tibRealm_Connect
 */
TIB_API
tibProperties
tibRealm_GetProperties(
    tibEx                       e,
    tibRealm                    realm);

/**
 * @brief Create a lock object.
 *
 * Cooperating application programs can use locks to implement
 * exclusive access to a map within a persistence store. 
 *  
 * Lock objects with the same lock name represent the same lock
 * within the persistence store.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration 
 *              information about the application, endpoints,
 *              transports and formats.
 * @param lockName The lock object represents the lock with this name in the store. 
 * @param props Reserved for future use.\n
 *              To ensure forward compatibility, programmers must supply \c NULL.
 * 
 * @return A new lock object.
 */
TIB_API
tibLock
tibRealm_CreateLock(
    tibEx              e,
    tibRealm           realm,
    const char         *lockName,
    tibProperties      props);

/** Create a map object.
 *
 * Map objects with the same map name represent the same map
 * within the persistence store.
 *
 * If the store does not already contain the named map,
 * this call creates the map as a dynamic durable.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration 
 *              information about the application, endpoints,
 *              transports and formats.
 * @param endpointName The map object represents a map within
 *                     the store associated with this endpoint.
 * @param mapName The map object represents the map with this name.
 * @param props Reserved for future use.\n
 *              To ensure forward compatibility, programmers must supply \c NULL.
 * 
 * @return A new map object. 
 */
TIB_API
tibMap
tibRealm_CreateMap(
    tibEx            e,
    tibRealm         realm,
    const char       *endpointName,
    const char       *mapName,
    tibProperties    props);

/** Delete a map from a persistence store.
 * 
 * @param e The exception object captures information about failures.
 * @param realm The realm object is the source of configuration 
 *              information about the application, endpoints,
 *              transports and formats.
 * @param endpointName The call removes a map from
 *                     the store associated with this endpoint.
 * @param mapName The call removes the map with this name from the store.
 * @param props Reserved for future use.\n
 *              To ensure forward compatibility, programmers must supply \c NULL.
 * 
 * @return void 
 */
TIB_API
void
tibRealm_RemoveMap(
    tibEx            e,
    tibRealm         realm,
    const char       *endpointName,
    const char       *mapName,
    tibProperties    props);

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_realm_h */
