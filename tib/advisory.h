/*
 * Copyright (c) 2011-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: advisory.h 90137 2016-12-13 19:04:42Z $
 */

#ifndef _INCLUDED_tib_advisory_h
#define _INCLUDED_tib_advisory_h

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file advisory.h
 * 
 * @brief Asynchronous advisory messages carry information about the
 * operation of FTL.  Programs can receive this information through
 * subscribers on the advisory endpoint.
 *
 * This file defines constants related to advisory messages, their
 * field names and field values.
 *
 * For more information, see the chapter "Advisory Messages" in
 * the book <b> TIBCO FTL Development</b>.
 */


/** @brief Name of the special endpoint that carries FTL advisory
 * messages; string.
 *
 * To receive FTL advisory messages, create a subscriber on this
 * endpoint name.
 *
 * The realm object scopes the advisories that a subscriber on this
 * endpoint can receive.  That is, a subscriber to this
 * endpoint receives only those advisories produced by other
 * objects in the same program and the same realm (as the advisory
 * subscriber).
 *
 * It is illegal to create a publisher on this endpoint name.
 */
#define TIB_ADVISORY_ENDPOINT_NAME                      "_advisoryEndpoint"



  /** @brief A field with this field name distinguishes advisory messages.
   *
   * All advisory messages contain this field.\n
   * Only advisory messages may contain this field.
   *
   * Use this field only for matching purposes.
   * Your program code must never set this field in any message.
   * This field always has the integer value 1.
   */
#define TIB_ADVISORY_FIELD_ADVISORY                     "_advisory"

    


  /*------------------- SEVERITY -------------------*/

  /** @brief Advisory severity (field name).
   *
   * The string value of the \c severity field classifies each advisory
   * according to the ability of FTL to fulfill its function as a
   * message carrier.
   *
   * @see TIB_ADVISORY_SEVERITY_WARN
   * @see TIB_ADVISORY_SEVERITY_INFO
   * @see TIB_ADVISORY_SEVERITY_DEBUG
   */
#define TIB_ADVISORY_FIELD_SEVERITY                     "severity"

  /** @brief The advisory message is a warning (severity value).
   *
   * Warnings indicate that although FTL functionality is impaired,
   * FTL still continues to operate.  As a result of impaired
   * messaging, your application might behave incorrectly.
   *
   * This string value can appear in the \c severity field.
   *
   * @see TIB_ADVISORY_FIELD_SEVERITY
   */
#define TIB_ADVISORY_SEVERITY_WARN                      "WARN"

  /** @brief The advisory message is informational (severity value).
  
   * Informational advisories can report external state changes, or 
   * signal your program to change its operating state. 
   * Informational advisories do \b not indicate impaired FTL behavior.
   *
   * This string value can appear in the \c severity field.
   *
   * @see TIB_ADVISORY_FIELD_SEVERITY
   * @see TIB_GROUP_ADVISORY_NAME_ORDINAL_UPDATE
   * @see TIB_GROUP_ADVISORY_NAME_GROUP_STATUS
   */
#define TIB_ADVISORY_SEVERITY_INFO                      "INFO"

  /** @brief The advisory message is for debugging (severity value).
  
   * Debug advisories can report internal state changes,
   * or signal an administrator to investigate.
   * Debug advisories do \b not indicate impaired behavior.
   *
   * This string value can appear in the \c severity field.
   *
   * @see TIB_ADVISORY_FIELD_SEVERITY
   * @see TIB_ADVISORY_NAME_RETRANSMISSION
   */
#define TIB_ADVISORY_SEVERITY_DEBUG                     "DEBUG"
    
    
    
  /*------------------- MODULE -------------------*/

  /** @brief Module (field name).
   *
   * The string value of the \c module field denotes the part of FTL
   * to which the advisory pertains.
   *
   * @see TIB_ADVISORY_MODULE_BASE
   * @see TIB_GROUP_ADVISORY_MODULE_GROUP
   */
#define TIB_ADVISORY_FIELD_MODULE                       "module"

  /** @brief The advisory message pertains to the FTL API base library
   * implementation code (module).
   *
   * This string value can appear in the \c module field.
   *
   * @see TIB_ADVISORY_FIELD_MODULE
   */
#define TIB_ADVISORY_MODULE_BASE                        "BASE"
    
    
    
    
  /*------------------- NAME -------------------*/
    
  /** @brief Advisory name (field name).
   *
   * The string value of the \c name field denotes the the main issue that the
   * advisory reports.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_NAME_SUBSCRIBER_FORCE_CLOSE
   * @see TIB_ADVISORY_NAME_RESOURCE_UNAVAILABLE
   * @see TIB_ADVISORY_NAME_RESOURCE_AVAILABLE
   * @see TIB_ADVISORY_NAME_RETRANSMISSION
   * @see TIB_GROUP_ADVISORY_NAME_ORDINAL_UPDATE
   * @see TIB_GROUP_ADVISORY_NAME_GROUP_STATUS
   */
#define TIB_ADVISORY_FIELD_NAME                         "name"

  /** @brief The advisory message reports data loss.
   *
   * This string value can appear in the \c name field.
   *
   * The \c reason field distinguishes among
   * several reasons for data loss.
   *
   * @see TIB_ADVISORY_FIELD_NAME
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_REASON_QUEUE_LIMIT_EXCEEDED
   * @see TIB_ADVISORY_REASON_TPORT_DATALOSS
   * @see TIB_ADVISORY_REASON_SENDER_DISCARD
   * @see TIB_ADVISORY_REASON_RECONNECT_LOSS
   * @see TIB_ADVISORY_REASON_UPSTREAM_LOSS
   * @see TIB_ADVISORY_REASON_INCOMPLETE_MESSAGE
   * @see TIB_ADVISORY_REASON_FAILOVER_LOSS 
   * @see TIB_ADVISORY_REASON_STORE_DISCARD_DATALOSS 
   */
#define TIB_ADVISORY_NAME_DATALOSS                      "DATALOSS"

  /** @brief The advisory message reports that a persistence store
   * forced a durable subscriber to close.
   *
   * This string value can appear in the \c name field.
   *
   * The \c reason field indicates the reason that the store forced
   * the durable subscriber to close.
   *
   * @see TIB_ADVISORY_FIELD_NAME
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_REASON_DURABLE_SUBSCRIBER_COLLISION
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_SUBSCRIBER_NAME
   */
#define TIB_ADVISORY_NAME_SUBSCRIBER_FORCE_CLOSE        "SUBSCRIBER_FORCE_CLOSE"

  /** @brief The advisory message reports that a resource is unavailable.
   *
   * This string value can appear in the \c name field.
   *
   * The \c reason field notes the unavailable resource.
   *
   * @see TIB_ADVISORY_FIELD_NAME
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_REASON_PERSISTENCE_STORE_UNAVAILABLE
   */
#define TIB_ADVISORY_NAME_RESOURCE_UNAVAILABLE          "RESOURCE_UNAVAILABLE"

  /** @brief The advisory message reports that a resource is available.
   *
   * This string value can appear in the \c name field.
   *
   * The \c reason field notes the available resource.
   *
   * @see TIB_ADVISORY_FIELD_NAME
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_REASON_PERSISTENCE_STORE_AVAILABLE
   */
#define TIB_ADVISORY_NAME_RESOURCE_AVAILABLE            "RESOURCE_AVAILABLE"

  /** @brief The advisory message reports multicast retransmission.
   * 
   * This string value can appear in the \c name field.
   *
   * Retransmission is a normal occurrence for multicast transports.
   * Retransmission advisories give applications 
   * access to data about network conditions.
   *
   * The \c reason field distinguishes the
   * type of retransmission advisory.
   *
   * @see TIB_ADVISORY_FIELD_NAME
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_REASON_RETRANSMISSION_REQUEST
   * @see TIB_ADVISORY_REASON_RETRANSMISSION_SENT
   * @see TIB_ADVISORY_REASON_RETRANSMISSION_REQUEST_SUPPRESSED
   */
#define TIB_ADVISORY_NAME_RETRANSMISSION                "RETRANSMISSION"
    
  /** @brief The advisory message reports that a lock previously
   *         held by this application was stolen by some other
   *         application.
   *
   * This string value can appear in the \c name field.
   *
   *
   * @see TIB_ADVISORY_FIELD_NAME 
   * @see TIB_ADVISORY_REASON_LOCK_STOLEN 
   * @see TIB_ADVISORY_REASON_LOCK_LOST_ON_DISCONNECT 
   * @see TIB_ADVISORY_FIELD_LOCK_NAME
   */
#define TIB_ADVISORY_NAME_LOCK_LOST        "LOCK_LOST"

    
  /*------------------- REASON -------------------*/
    
  /** @brief Advisory reason (field name).
   *
   * The string value of the \c reason field provides additional information
   * about the reason for the advisory (adding detail to the \c name field).
   *
   * @see TIB_ADVISORY_REASON_QUEUE_LIMIT_EXCEEDED
   * @see TIB_ADVISORY_REASON_TPORT_DATALOSS
   * @see TIB_ADVISORY_REASON_SENDER_DISCARD
   * @see TIB_ADVISORY_REASON_RECONNECT_LOSS
   * @see TIB_ADVISORY_REASON_UPSTREAM_LOSS
   * @see TIB_ADVISORY_REASON_INCOMPLETE_MESSAGE
   * @see TIB_ADVISORY_REASON_FAILOVER_LOSS
   * @see TIB_ADVISORY_REASON_STORE_DISCARD_DATALOSS
   * @see TIB_ADVISORY_REASON_DIRECT_SUB_LOSS
   * @see TIB_ADVISORY_REASON_DURABLE_SUBSCRIBER_COLLISION
   * @see TIB_ADVISORY_REASON_PERSISTENCE_STORE_UNAVAILABLE
   * @see TIB_ADVISORY_REASON_PERSISTENCE_STORE_AVAILABLE
   * @see TIB_ADVISORY_REASON_RETRANSMISSION_REQUEST
   * @see TIB_ADVISORY_REASON_RETRANSMISSION_SENT
   * @see TIB_ADVISORY_REASON_RETRANSMISSION_REQUEST_SUPPRESSED
   * @see TIB_ADVISORY_REASON_DIRECT_SUB_LOSS
   */
#define TIB_ADVISORY_FIELD_REASON                       "reason"

  /** @brief The advisory message reports data loss; the reason is
   * overflow of an event queue.
   *
   * The queue has discarded some events, in accordance with its
   * property values.
   *
   * This string value can appear in the \c reason field.
   *
   * \b Common \b Diagnoses
   *
   * \li Lengthy processing in callbacks could delay prompt dispatch
   *     of the queue.
   *
   * \li The program could not process the volume of inbound messages
   *     from its subscribers.
   *
   * \li The program is starved for CPU cycles -- its host computer is
   *     too heavily loaded.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_QUEUE_NAME
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_QUEUE_LIMIT_EXCEEDED        "QUEUE_LIMIT_EXCEEDED"
    
  /** @brief The advisory reports data loss; the
   * reason is a transport malfunction.
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_TPORT_DATALOSS              "TPORT_DATALOSS"

  /** @brief The advisory reports data loss; the reason is a sending
   * transport has discarded data in accordance with its backlog
   * settings.
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_SENDER_DISCARD              "SENDER_DISCARD"

  /** @brief The advisory reports potential data loss;
   * the reason is a temporary network disconnect between two
   * connection-oriented transports (that is, TCP or RDMA).
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_RECONNECT_LOSS              "RECONNECT_LOSS"

  /** @brief The advisory reports potential data loss; the reason is
   * failover to a backup forwarding component.
   *
   * Only connection-oriented transports can
   * detect this condition.
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_FAILOVER_LOSS               "FAILOVER_LOSS"

  /** @brief The advisory reports potential data loss;
   * the reason is a data loss advisory detected upstream at a
   * forwarding component (such as a transport bridge).
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_UPSTREAM_LOSS               "UPSTREAM_LOSS"
    
  /** @brief The advisory reports data loss;
   * the reason is an incomplete message.
   *
   * The transport could not reassemble an inbound message because
   * some data fragments did not arrive.
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_INCOMPLETE_MESSAGE          "INCOMPLETE_MESSAGE"

    /** @brief The advisory reports that a persistence store forced a
     * subscriber to close; the reason is durable subscriber
     * collision.
     *
     * Each durable can serve at most one durable subscriber object at
     * a time. The store resolves collisions in favor of the newer
     * durable subscriber, forcing the older subscriber to close.
     *
     * This string value can appear in the \c reason field.
     *
     * @see TIB_ADVISORY_NAME_SUBSCRIBER_FORCE_CLOSE
     * @see TIB_ADVISORY_FIELD_REASON
     * @see TIB_ADVISORY_FIELD_ENDPOINTS
     * @see TIB_ADVISORY_FIELD_SUBSCRIBER_NAME
     */
#define TIB_ADVISORY_REASON_DURABLE_SUBSCRIBER_COLLISION "DURABLE_SUBSCRIBER_COLLISION"

    /** @brief The advisory reports resource unavailability;
     * the reason is that a persistence store is not available.
     *
     * This string value can appear in the \c reason field.
     *
     * @see TIB_ADVISORY_NAME_RESOURCE_UNAVAILABLE
     * @see TIB_ADVISORY_FIELD_REASON
     */
#define TIB_ADVISORY_REASON_PERSISTENCE_STORE_UNAVAILABLE "PERSISTENCE_STORE_UNAVAILABLE"

    /** @brief The advisory reports resource availability;
     * the reason is that a persistence store is available.
     *
     * This string value can appear in the \c reason field.
     *
     * @see TIB_ADVISORY_NAME_RESOURCE_AVAILABLE
     * @see TIB_ADVISORY_FIELD_REASON
     */
#define TIB_ADVISORY_REASON_PERSISTENCE_STORE_AVAILABLE   "PERSISTENCE_STORE_AVAILABLE"

   /** @brief The advisory reports that the subscribing endpoint
    * of a multicast transport in the application
    * requested retransmission from a publishing endpoint.
    *
    * This string value can appear in the \c reason field.
    *
    * @see TIB_ADVISORY_NAME_RETRANSMISSION
    * @see TIB_ADVISORY_FIELD_REASON
    * @see TIB_ADVISORY_SEVERITY_DEBUG
    * @see TIB_ADVISORY_FIELD_ENDPOINTS
    * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
    */
#define TIB_ADVISORY_REASON_RETRANSMISSION_REQUEST        "RETRANSMISSION_REQUEST"

   /** @brief The advisory reports that the publishing endpoint
    * of a multicast transport in the application
    * has sent a retransmission as requested.
    *
    * This string value can appear in the \c reason field.
    *
    * @see TIB_ADVISORY_NAME_RETRANSMISSION
    * @see TIB_ADVISORY_FIELD_REASON
    * @see TIB_ADVISORY_SEVERITY_DEBUG
    * @see TIB_ADVISORY_FIELD_ENDPOINTS
    * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
    */
#define TIB_ADVISORY_REASON_RETRANSMISSION_SENT           "RETRANSMISSION_SENT"

  /** @brief The advisory reports that the subscribing endpoint
   * of a multicast transport in the application
   * would have requested retransmission,
   * but request was suppressed. 
   *
   * Suppression can occur when a multicast transport
   * enables retransmission control,
   * and a subscribing endpoint misses packets
   * in excess of a specified threshold.
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_RETRANSMISSION
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_SEVERITY_WARN
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_RETRANSMISSION_REQUEST_SUPPRESSED   "RETRANSMISSION_REQUEST_SUPPRESSED"

  /** @brief The advisory reports data loss when
   * a subscriber to a last-value durable has missed
   * one or more messages from the persistence store.
   *
   * It can also report potential data loss because
   * the subscriber disconnected from the store.
   * 
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_SEVERITY_WARN
   * @see TIB_ADVISORY_FIELD_SUBSCRIBER_NAME
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_STORE_DISCARD_DATALOSS           "STORE_DISCARD_DATALOSS"

  /** @brief The advisory reports data loss;
   * the reason is direct subscriber detected data loss
   * in the inbound stream.
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_ENDPOINTS
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_DIRECT_SUB_LOSS     "DIRECT_SUBSCRIBER_LOSS"

  /** @brief The advisory reports lock lost notification;
   * the reason is a lock that was prevsiously held by this 
   * application was stolen by some other application. 
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_LOCK_LOST
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_LOCK_STOLEN             "LOCK_STOLEN"

  /** @brief The advisory reports lock lost notification;
   * the reason is a lock that was prevsiously held by this 
   * application was lost because of a disconnect from the 
   * persistence store. 
   *
   * This string value can appear in the \c reason field.
   *
   * @see TIB_ADVISORY_NAME_LOCK_LOST
   * @see TIB_ADVISORY_FIELD_REASON
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_REASON_LOCK_LOST_ON_DISCONNECT "LOCK_LOST_ON_DISCONNECT"


  /*------------------- TIMESTAMP -------------------*/
    
  /** @brief Timestamp (field name).
   *
   * The @ref tibDateTime value of the
   * \c timestamp field indicates the time that
   * FTL generated the advisory.
   *
   * Although DateTime values can represent time with nanosecond
   * precision, the actual resolution of the timestamp is only as
   * fine-grained as the host computer's operating system supports.
   *
   * @see tibDateTime
   */
#define TIB_ADVISORY_FIELD_TIMESTAMP                    "timestamp"


  /*------------------- AGGREGATION COUNT -------------------*/
    
  /** @brief Aggregation incidents (field name).
   *
   * Some advisory messages report aggregate data, representing
   * several incidents that occurred within a time interval.  The long
   * integer value of the \c aggregation_count field indicates the number
   * of incidents.
   *
   * In \c DATALOSS advisories with reason \c QUEUE_LIMIT_EXCEEDED, this
   * value reports the number of events that the queue discarded.
   *
   * @see TIB_ADVISORY_FIELD_AGGREGATION_TIME
   */
#define TIB_ADVISORY_FIELD_AGGREGATION_COUNT            "aggregation_count"

    
    
    
  /*------------------- AGGREGATION TIME -------------------*/
    
  /** @brief Aggregation time interval (field name).
   *
   * Some advisory messages report aggregate data, representing
   * several incidents that occurred within a time interval.  The
   * double floating point value (in seconds) of the \c aggregation_time
   * field indicates the length of the time interval.
   *
   * The time interval ends shortly before the timestamp on the
   * advisory message.
   *
   * @see TIB_ADVISORY_FIELD_AGGREGATION_COUNT
   */
#define TIB_ADVISORY_FIELD_AGGREGATION_TIME             "aggregation_time"

    
    
    
  /*------------------- SPECIAL FIELDS -------------------*/

  /** @brief Queue name (field name).
   *
   * This field appears in \c DATALOSS advisories with reason 
   * \c QUEUE_LIMIT_EXCEEDED.
   *
   * The string value of the \c queue_name field
   * identifies the queue that discarded excess events.
   *
   * If the queue's \c name property is not set, then the advisory
   * message omits this field.
   *
   * @see TIB_ADVISORY_REASON_QUEUE_LIMIT_EXCEEDED
   * @see TIB_EVENTQUEUE_PROPERTY_STRING_NAME
   */
#define TIB_ADVISORY_FIELD_QUEUE_NAME                   "queue_name"


  /** @brief Endpoints (field name).
   *
   * This field can appear in \c DATALOSS,
   * \c SUBSCRIBER_FORCE_CLOSE, and
   * \c RETRANSMISSION advisories.
   *
   * <ul>
   * <li> In \c DATALOSS advisories, the string array value of the
   * \c endpoints field lists the endpoints that could have lost data.
   *
   * Although data loss occurs primarily in a transport, its symptoms
   * could affect all endpoints that use the transport -- and by
   * extension, any subscriber on those endpoints.  Furthermore,
   * transport names are meaningful to administrators, but usually not
   * available to programmers.  This advisory field reports the set of
   * all endpoints through which the program could access the
   * problematic transport (according to the configuration in the
   * local realm object).
   *
   * This field does not appear in \c DATALOSS advisories with reason
   * \c QUEUE_LIMIT_EXCEEDED.
   *
   * <li> In \c SUBSCRIBER_FORCE_CLOSE advisories, the string array
   * value of the \c endpoints field identifies the endpoint of the
   * closed subscriber object.
   *
   * <li> In \c RETRANSMISSION advisories,
   * the string array value of the
   * \c endpoints field identifies the endpoint
   * involved in the retransmission event.
   * </ul>
   *
   * @see TIB_ADVISORY_NAME_DATALOSS
   * @see TIB_ADVISORY_NAME_SUBSCRIBER_FORCE_CLOSE
   * @see TIB_ADVISORY_NAME_RETRANSMISSION
   */
#define TIB_ADVISORY_FIELD_ENDPOINTS                    "endpoints"

    
  /** @brief Subscriber name (field name).
   *
   * This field can appear in \c SUBSCRIBER_FORCE_CLOSE advisories.
   *
   * The string value of the \c subscriber_name field
   * identifies the closed durable subscriber.
   *
   * @see TIB_ADVISORY_NAME_SUBSCRIBER_FORCE_CLOSE
   * @see TIB_ADVISORY_REASON_DURABLE_SUBSCRIBER_COLLISION
   */
#define TIB_ADVISORY_FIELD_SUBSCRIBER_NAME              "subscriber_name"

  /** @brief Lock Name (field name).
   *
   * This field can appear in \c LOCK_LOST advisories.
   *
   * The string value of the \c lock_name field identifies the 
   * name of the lock that was stolen by some other application. 
   *
   * @see TIB_ADVISORY_NAME_LOCK_LOST
   */
#define TIB_ADVISORY_FIELD_LOCK_NAME              "lock_name"

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_advisory_h */
