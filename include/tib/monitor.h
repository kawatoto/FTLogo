/*
 * Copyright (c) 2009-2017 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: monitor.h 91185 2017-02-01 07:58:59Z $
 */

#ifndef _INCLUDED_tib_monitor_h
#define _INCLUDED_tib_monitor_h

#if defined(__cplusplus)
extern "C" {
#endif
  
/**
 * @file monitor.h
 * 
 * @brief This file defines constants related to
 * monitoring data messages,
 * their field names, and their field values.
 *
 * Monitoring data messages carry monitoring metrics data about
 * %FTL programs.
 * Programs can receive the stream of monitoring data messages
 * by subscribing to the monitoring endpoint.
 *
 * @see For more information, see the book
 * <i> TIBCO FTL Monitoring</i>.
 */

/** @brief Name of the special endpoint that carries FTL monitoring 
 * messages; string.
 *
 * To receive FTL monitoring messages,
 * create a subscriber on this endpoint.
 *
 * Subscribers on this endpoint receive
 * the stream of monitoring data messages 
 * from the realm server.
 *
 * It is illegal to create a publisher on this endpoint.
 */
#define TIB_MONITORING_ENDPOINT_NAME "_monitoringEndpoint"

/** @name Monitoring Message Fields
 *  @brief Fields in monitoring messages.
 */
/** @{ */
/** Monitoring message type (long value).
 *  @sa ::TIB_MONITORING_MSG_TYPE_METRICS
 *  @sa ::TIB_MONITORING_MSG_TYPE_SERVER_METRICS
 *  @sa ::TIB_MONITORING_MSG_TYPE_EVENT
 */
#define TIB_MONITORING_FIELD_MSG_TYPE "msg_type"
/** Client label (string value). */
#define TIB_MONITORING_FIELD_CLIENT_LABEL "client_label"
/** Client status (long value). */
#define TIB_MONITORING_FIELD_CLIENT_STATUS "client_status"
/** Metrics message array (array of messages). Each message contains a single metric. */
#define TIB_MONITORING_FIELD_METRICS "metrics"
/** Time at which the metric was generated (long value), expressed in milliseconds since the epoch. */
#define TIB_MONITORING_FIELD_TIMESTAMP "timestamp"
/** Application name (string value). */
#define TIB_MONITORING_FIELD_APPINFO_APPLICATION "application"
/** Application instance (string value). */
#define TIB_MONITORING_FIELD_APPINFO_APPINSTANCE "application_instance"
/** Client ID (string value). */
#define TIB_MONITORING_FIELD_APPINFO_CLIENT_ID "client_id"
/** Server ID (string value). */
#define TIB_MONITORING_FIELD_APPINFO_SERVER_ID "server_id"
/** Process ID (long value). */
#define TIB_MONITORING_FIELD_HOSTINFO_PID "pid"
/** FTL user name (string value). */
#define TIB_MONITORING_FIELD_HOSTINFO_FTL_USER "ftl_user"
/** Host effective user name (string value). */
#define TIB_MONITORING_FIELD_HOSTINFO_EFFECTIVE_USER "effective_user"
/** FTL version client is running (string value). */
#define TIB_MONITORING_FIELD_HOSTINFO_VERSION "version"
/** Hostname on which client is running (string value). */
#define TIB_MONITORING_FIELD_HOSTINFO_HOST "host"
/** IP address of client (string value). */
#define TIB_MONITORING_FIELD_HOSTINFO_IP "ip"
/** Server uptime in seconds (long value). */
#define TIB_MONITORING_FIELD_SERVER_UPTIME "server_uptime"
/** Realm server label (string value). */
#define TIB_MONITORING_FIELD_REALM_SERVER_LABEL "realm_server_label"

/** @} */

/** @name Data Sample Submessage Fields
 *  @brief Fields contained within data sample submessages.
 *  @sa ::TIB_MONITORING_FIELD_METRICS
 */
/** @{ */
/** Context for this metric (string value). */
#define TIB_MONITORING_FIELD_METRIC_CONTEXT "context"
/** Object type tagging for this context (string value). */
#define TIB_MONITORING_FIELD_METRIC_CONTEXT_TYPE "context_type"
/** Unique metric ID (long value). */
#define TIB_MONITORING_FIELD_METRIC_ID "id"
/** Metric type (long value). */
#define TIB_MONITORING_FIELD_METRIC_TYPE "type"
/** Metric value (long value). */
#define TIB_MONITORING_FIELD_METRIC_VALUE "value"
/** Metric name (string value). */
#define TIB_MONITORING_FIELD_METRIC_NAME "name"
/** Metric semantics (long value). */
#define TIB_MONITORING_FIELD_METRIC_SEMANTICS "semantics"
/** @} */

/** @name Context type values 
 */
/** @{ */
/** Undefined */
#define TIB_MONITORING_UNDEFINED                        0
/** Application */
#define TIB_MONITORING_APPLICATION                      1
/** Transport */
#define TIB_MONITORING_TRANSPORT                        2
/** Endpoint */
#define TIB_MONITORING_ENDPOINT                         3
/** Queue */
#define TIB_MONITORING_QUEUE                            4
/** Persistence server */
#define TIB_MONITORING_PERSISTENCE_SERVER               5
/** Persistence store */
#define TIB_MONITORING_PERSISTENCE_STORE                6
/** Persistence store manager*/
#define TIB_MONITORING_PERSISTENCE_STORE_MANAGER        7
/** Bridge */
#define TIB_MONITORING_BRIDGE                           8
/** eFTL server */
#define TIB_MONITORING_EFTL_SERVER                      9
/** Realm Server */
#define TIB_MONITORING_REALM_SERVER                     10
/** @} */
    
/** @name Monitoring Event Field
 */
/** @{ */
/** Event type (long value). */
#define TIB_MONITORING_FIELD_EVENT_TYPE "event_type"
/** @} */

/** @name Monitoring Message Types
 */
/** @{ */
/** The message contains client metrics. */
#define TIB_MONITORING_MSG_TYPE_METRICS 1
/** The message contains realm server metrics. */
#define TIB_MONITORING_MSG_TYPE_SERVER_METRICS 2
/** The message is a monitoring event message. */
#define TIB_MONITORING_MSG_TYPE_EVENT 3
/** @} */

/** @name Client Status Values
 */
/** @{ */
/** The client is running. */
#define TIB_MONITORING_CLIENT_STATUS_RUNNING       1
/** The client needs to be restarted. */
#define TIB_MONITORING_CLIENT_STATUS_NEEDS_RESTART 2
/** The client has timed out. */
#define TIB_MONITORING_CLIENT_STATUS_TIMED_OUT     3
/** The client has indicated an exception. */
#define TIB_MONITORING_CLIENT_STATUS_EXCEPTION     4
/** The client is out of sync. */
#define TIB_MONITORING_CLIENT_STATUS_OUT_OF_SYNC   5
/** The client is offline. */
#define TIB_MONITORING_CLIENT_STATUS_OFF_LINE      -1
/** The client has been destroyed. */
#define TIB_MONITORING_CLIENT_STATUS_DESTROYED     -2
/** @} */

/** @name Event Types
 *  Enumerates the possible types of an event message.
 *  @see TIB_MONITORING_FIELD_EVENT_TYPE
 */
/** @{ */
/** The client is connected.  */
#define TIB_MONITORING_EVENT_TYPE_CONNECTED         1
/** The client is disconnected.  */
#define TIB_MONITORING_EVENT_TYPE_DISCONNECTED      2
/** The client has changed client_status states.  */
#define TIB_MONITORING_EVENT_TYPE_CLIENT_STATUS_CHANGE 3
/** @} */

/** @name Monitoring Metric Types
 *
 * For descriptions of implemented metric types,
 * see "Catalogs of Metrics"
 * in the book <i> TIBCO FTL Monitoring</i>.
 * (Some types are defined but not implemented.)
 */
/** @{ */
/** Unknown. */
#define TIB_MONITORING_TYPE_UNKNOWN 0
/** Messages sent. */
#define TIB_MONITORING_TYPE_MESSAGES_SENT 1
/** Messages received. */
#define TIB_MONITORING_TYPE_MESSAGES_RECEIVED 2
/** Bytes sent. */
#define TIB_MONITORING_TYPE_BYTES_SENT 11
/** Bytes received. */
#define TIB_MONITORING_TYPE_BYTES_RECEIVED 12
/** Data lost. */
#define TIB_MONITORING_TYPE_DATA_LOST 31
/** Format unavailable. */
#define TIB_MONITORING_TYPE_FORMAT_UNAVAILABLE 32
/** Queue backlog. */
#define TIB_MONITORING_TYPE_QUEUE_BACKLOG 41
/** Queue discards. */
#define TIB_MONITORING_TYPE_QUEUE_DISCARDS 42
/** Dynamic formats. */
#define TIB_MONITORING_TYPE_DYNAMIC_FORMATS 51
/** Packets sent. */
#define TIB_MONITORING_TYPE_PACKETS_SENT 61
/** Packets received. */
#define TIB_MONITORING_TYPE_PACKETS_RECEIVED 62
/** Packets retransmitted. */
#define TIB_MONITORING_TYPE_PACKETS_RETRANSMITTED 63
/** Packets missed. */
#define TIB_MONITORING_TYPE_PACKETS_MISSED 64
/** Packets lost outbound. */
#define TIB_MONITORING_TYPE_PACKETS_LOST_OUTBOUND 65
/** Packets lost inbound. */
#define TIB_MONITORING_TYPE_PACKETS_LOST_INBOUND 66
/** Store mismatch messages. */
#define TIB_MONITORING_TYPE_STORE_MISMATCH_MESSAGES 67
/** Process resident/working set size in KB. */
#define TIB_MONITORING_TYPE_PROCESS_RSS_KB 70
/** Process peak resident/working set size in KB. */
#define TIB_MONITORING_TYPE_PROCESS_PEAK_RSS_KB 71
/** Process VM size in KB. */
#define TIB_MONITORING_TYPE_PROCESS_VM_KB 72
/** User CPU time in microseconds. */
#define TIB_MONITORING_TYPE_USER_CPU_TIME 73
/** System CPU time in microseconds. */
#define TIB_MONITORING_TYPE_SYSTEM_CPU_TIME 74
/** Message count. */
#define TIB_MONITORING_TYPE_MESSAGE_COUNT 1000
/** Message size. */
#define TIB_MONITORING_TYPE_MESSAGE_SIZE 1001
/** Records synced. */
#define TIB_MONITORING_TYPE_RECORDS_SYNCED 1002
/** Records available for sync. */
#define TIB_MONITORING_TYPE_RECORDS_AVAIL_FOR_SYNC 1003
/** Records caught up. */
#define TIB_MONITORING_TYPE_RECORDS_CAUGHT_UP 1004
/** Records to catch up. */
#define TIB_MONITORING_TYPE_RECORDS_TO_CATCH_UP 1005
/** Durable count. */
#define TIB_MONITORING_TYPE_DURABLE_COUNT 2000
/** Quorum state. */
#define TIB_MONITORING_TYPE_QUORUM_STATE 3000
/** Quorum number. */
#define TIB_MONITORING_TYPE_QUORUM_NUMBER 3001
/** History quorum number. */
#define TIB_MONITORING_TYPE_HISTORY_QUORUM_NUMBER 3002
/** History update version. */
#define TIB_MONITORING_TYPE_HISTORY_UPDATE_VERSION 3003
/** History timestamp. */
#define TIB_MONITORING_TYPE_HISTORY_TIMESTAMP 3004
/** Pending updates. */
#define TIB_MONITORING_TYPE_PENDING_UPDATES 3005
/** Committed updates. */
#define TIB_MONITORING_TYPE_COMMITTED_UPDATES 3006
/** Commit latency. */
#define TIB_MONITORING_TYPE_COMMIT_LATENCY 3007
/** History update version - non replicated. */
#define TIB_MONITORING_TYPE_HISTORY_UPDATE_VERSION_NON_REPLICATED 3008
/** Disk state. */
#define TIB_MONITORING_TYPE_DISK_STATE 3009
/** Consistent Quorum Number. */
#define TIB_MONITORING_TYPE_CONSISTENT_QUORUM_NUMBER 3010
/** Consistent Version. */
#define TIB_MONITORING_TYPE_CONSISTENT_VERSION 3011
/** Consistent Timestamp. */
#define TIB_MONITORING_TYPE_CONSISTENT_TIMESTAMP 3012
/** Connection count. */
#define TIB_MONITORING_TYPE_CONNECTION_COUNT 4000
/** Subscription count. */
#define TIB_MONITORING_TYPE_SUBSCRIPTION_COUNT 4001
/** Inbound eFTL message count. */
#define TIB_MONITORING_TYPE_INBOUND_EFTL_MESSAGE_COUNT 4002
/** Outbound eFTL message count. */
#define TIB_MONITORING_TYPE_OUTBOUND_EFTL_MESSAGE_COUNT 4003
/** Inbound cluster message count. */
#define TIB_MONITORING_TYPE_INBOUND_CLUSTER_MESSAGE_COUNT 4004
/** Outbound cluster message count. */
#define TIB_MONITORING_TYPE_OUTBOUND_CLUSTER_MESSAGE_COUNT 4005
/** Inbound FTL message count. */
#define TIB_MONITORING_TYPE_INBOUND_FTL_MESSAGE_COUNT 4006
/** Outbound FTL message count. */
#define TIB_MONITORING_TYPE_OUTBOUND_FTL_MESSAGE_COUNT 4007
/** Protocol message count. */
#define TIB_MONITORING_TYPE_PROTOCOL_MESSAGE_COUNT 4008
/** Filtered message count. */
#define TIB_MONITORING_TYPE_FILTERED_MESSAGE_COUNT 4009
/** Discarded message count. */
#define TIB_MONITORING_TYPE_DISCARDED_MESSAGE_COUNT 4010
/** Dataloss message count. */
#define TIB_MONITORING_TYPE_DATALOSS_MESSAGE_COUNT 4011
/** Pending connection count. */
#define TIB_MONITORING_TYPE_PENDING_CONNECTION_COUNT 4012
/** Suspended connection count. */
#define TIB_MONITORING_TYPE_SUSPENDED_CONNECTION_COUNT 4013
/** Realm server client count. */
#define TIB_MONITORING_TYPE_CLIENT_COUNT 5000
/** Realm server satellite count. */
#define TIB_MONITORING_TYPE_SATELLITE_COUNT 5001
/** Count of clients currently in the RUNNING state. */
#define TIB_MONITORING_TYPE_CLIENT_RUNNING_COUNT 5002
/** Count of clients currently in the NEEDS_RESTART state. */
#define TIB_MONITORING_TYPE_CLIENT_NEEDS_RESTART_COUNT 5003
/** Count of clients currently in the TIMED_OUT state. */
#define TIB_MONITORING_TYPE_CLIENT_TIMED_OUT_COUNT 5004
/** Count of clients currently in the EXCEPTION state. */
#define TIB_MONITORING_TYPE_CLIENT_EXCEPTION_COUNT 5005
/** Count of clients currently in the OUT_OF_SYNC state. */
#define TIB_MONITORING_TYPE_CLIENT_OUT_OF_SYNC_COUNT 5006
/** Count of clients currently in the OFF_LINE state. */
#define TIB_MONITORING_TYPE_CLIENT_OFF_LINE_COUNT 5007
/** Count of clients currently in the DESTROYED state. */
#define TIB_MONITORING_TYPE_CLIENT_DESTROYED_COUNT 5008
/** Count of successful client connects. */
#define TIB_MONITORING_TYPE_CLIENT_CONNECT_COUNT 5009
/** Count of successful client reconnects. */
#define TIB_MONITORING_TYPE_CLIENT_RECONNECT_COUNT 5010
/** Count of send to client inbox failures. */
#define TIB_MONITORING_TYPE_SEND_TO_INBOX_FAILURES 5011
/** Bridge active state. */
#define TIB_MONITORING_TYPE_BRIDGE_ACTIVE 6000
/** Start of dynamically created stats */
#define TIB_MONITORING_TYPE_DYNAMIC_STAT 100000
/** @} */

/** @name Monitoring Metric Semantic Values
 */
/** @{ */
/** The metric has counter semantics: it is nondecreasing. */
#define TIB_MONITORING_METRIC_SEMANTICS_COUNTER 1
/** The metric has gauge semantics: it may increase and decrease. */
#define TIB_MONITORING_METRIC_SEMANTICS_GAUGE 2
/** @} */

#if defined(__cplusplus)
}
#endif

#endif /* _INCLUDED_tib_monitor_h */
