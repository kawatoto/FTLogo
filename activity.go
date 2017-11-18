package activityTest

/*
#cgo CFLAGS: -std=gnu11 -m64 -O2 -Wall -Wshadow -I/opt/tibco/ftl/5.2/include/
#cgo LDFLAGS: -L/opt/tibco/ftl/5.2/lib/ -ltib -ltibutil

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "tib/ftl.h"

#define DEFAULT_URL "http://localhost:8080"
#define CHECK(ex) \
{                              \
    if(tibEx_GetErrorCode(ex)) \
    {                          \
       char exStr[1024];       \
       fprintf(stderr, "%s: %d\n", __FILE__, __LINE__); \
       tibEx_ToString(ex, exStr, sizeof(exStr));        \
       fprintf(stderr, "%s\n", exStr);                  \
       tib_Close(ex);                                   \
       exit(-1);                                        \
    }                                                   \
}

const char      *realmServer    = DEFAULT_URL;

int sendFTLMessage(char *realmserver, tibPublisher publisher, tibMessage message)
{
    tibEx               ex;
    tibRealm            realm = NULL;
    tibPublisher        pub;
    tibMessage          msg;

    ex = tibEx_Create();

    tib_Open(ex, TIB_COMPATIBILITY_VERSION);

    // connect to the realmserver to get a config, 'default' app is returned if the appName is NULL.
    realm = tibRealm_Connect(ex, realmServer, NULL, NULL);

    // Create the publisher object
    pub = tibPublisher_Create(ex, realm, NULL, NULL);
    CHECK(ex);

    // create the hello world msg.
    msg = tibMessage_Create(ex, realm, NULL);
    tibMessage_SetString(ex, msg, "type", "hello");
    tibMessage_SetString(ex, msg, "message", "hello world earth");

    // send hello world ftl msg
    printf("sending 'hello world' message\n");
    fflush(stdout);

    tibPublisher_Send(ex, pub, msg);

    // cleanup
    tibMessage_Destroy(ex, msg);

    tibPublisher_Close(ex, pub);
    tibRealm_Close(ex, realm);

    tib_Close(ex);

    // Check if it worked before destroying the exception
    CHECK(ex);
    tibEx_Destroy(ex);

    return 0;
}

void inC() {
    printf("I am in C code now!\n");
}*/
import "C"

import (
	"fmt"

	"github.com/TIBCOSoftware/flogo-lib/core/activity"
	"github.com/TIBCOSoftware/flogo-lib/logger"
)

// THIS IS ADDED
// log is the default package logger which we'll use to log
var log = logger.GetLogger("activity-helloworld")

// MyActivity is a stub for your Activity implementation
type MyActivity struct {
	metadata *activity.Metadata
}

// NewActivity creates a new activity
func NewActivity(metadata *activity.Metadata) activity.Activity {
	return &MyActivity{metadata: metadata}
}

// Metadata implements activity.Activity.Metadata
func (a *MyActivity) Metadata() *activity.Metadata {
	return a.metadata
}

// THIS HAS CHANGED

// Eval implements activity.Activity.Eval
func (a *MyActivity) Eval(context activity.Context) (done bool, err error) {
	// Get the activity data from the context
	name := context.GetInput("name").(string)
	salutation := context.GetInput("salutation").(string)

	// Use the log object to log the greeting
	log.Debugf("The Flogo engine says [%s] to [%s]", salutation, name)

	// Set the result as part of the context
	context.SetOutput("result", "The Flogo engine says "+salutation+" to "+name)

	//C Code
	fmt.Println("I am in Go code now!")
	C.inC()

	// Signal to the Flogo engine that the activity is completed
	return true, nil
}
