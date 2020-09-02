/*
 * Copyright (c) 2020 hobum.kwon Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <dlog.h>
#include <dbus/dbus.h>

#define LOG_TAG "BPS_BP_MANAGER_TIZEN"

const char* bp_manager_bus_name =       "org.tizen.bp.manager.tizen";
const char* bp_manager_object_path =    "/org/tizen/bps/bp/tizen/manager/object";
const char* bp_manager_interface_name = "org.tizen.bps.bp.manager.interface";
const char* bp_manager_method_name_1 = "get";
//const char* bp_manager_signal_name_1 = "launch_new_application";
const char* bpm_bus_name =       "org.tizen.bp.manager";
const char* bpm_object_path =    "/org/tizen/bp/manager";
const char* bpm_interface_name = "org.tizen.bp.manager.interface";
const char* bpm_method_name_1 = "get";
// const char* interface_name_of_the_method = "bps.bp.methodinterface1.name";
// const char* name_of_the_method = "bps.bp.method.name";
// const char* interface_name_of_the_signal = "request.to.launcherdaemon";
// const char* name_of_the_signal = "launch_new_application";

void reply_to_method_call(DBusMessage* msg, DBusConnection* conn)
{
   DBusMessage* reply;
   DBusMessageIter args;
   bool stat = true;
   dbus_uint32_t level = 21614;
   dbus_uint32_t serial = 0;
   char* param;
   dbus_uint32_t retVal = -1;

   //Read the arguments. This method assumes it has one String and Integer value
   //and returns one string value to the requester
   if (!dbus_message_iter_init(msg, &args)) 
   {
       dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Message has no arguments!\n");
       return;
   }
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
   {
       dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Argument is not string!\n");
       return;
   }
   else 
   {
       dbus_message_iter_get_basic(&args, &param);
       dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] METHOD received string is %s\n", param);
       if (!strcmp(param, "This must not be null")) 
	   {
    	   retVal = 1;
       }
       else {
    	   retVal = 2;
       }
   }

   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);

   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &retVal)) 
   {
      dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Out Of Memory!\n");
      exit(1);
   }

   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) 
   {
      dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);
   dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] METHOD replied INT \n");
   //Unref is done below
   //dbus_message_unref(reply);
}


int bp_manager_tizen_dbus_server_run() 
{
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	DBusPendingCall* pending;
	int ret = 0;
const char* param = "This must not be null";
    dlog_print(DLOG_INFO, LOG_TAG, "Fuck bp_manager_tizen_dbus_server_run\n");
    //initialize the error
	dbus_error_init(&err);

	//connect to the bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
	//request our name on the bus and check for errors

    dlog_print(DLOG_INFO, LOG_TAG, "Fuck bp_manager_tizen_dbus_server_run 1\n");
	ret = dbus_bus_request_name(conn, bp_manager_bus_name, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);

    dlog_print(DLOG_INFO, LOG_TAG, "Fuck bp_manager_tizen_dbus_server_run 2\n");
	//Check for dbus error
	if (dbus_error_is_set(&err)) 
	{
		dlog_print(DLOG_INFO, LOG_TAG, "Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}

    dlog_print(DLOG_INFO, LOG_TAG, "Fuck bp_manager_tizen_dbus_server_run 3\n");
	if (NULL == conn) 
	{
		dlog_print(DLOG_INFO, LOG_TAG,  "Connection Null\n");
		exit(1);
	}

    dlog_print(DLOG_INFO, LOG_TAG, "Fuck bp_manager_tizen_dbus_server_run 4\n");
	while(true) 
	{
		sleep(3);
		dlog_print(DLOG_INFO, LOG_TAG, "Fuck Start remote method call:: %s\n", bp_manager_bus_name);
		msg = dbus_message_new_method_call(bpm_bus_name, // target for the method call
									bpm_object_path, // object to call on
									bpm_interface_name, // interface to call on
									bpm_method_name_1); // method name
		dlog_print(DLOG_INFO, LOG_TAG, "End remote method call %s\n", bp_manager_bus_name);
		// append arguments
		dbus_message_iter_init_append(msg, &args);

		if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param)) {
			dlog_print(DLOG_INFO, LOG_TAG, "Out Of Memory!\n");
			exit(1);
		}

		// send message and get a handle for a reply
		if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
			dlog_print(DLOG_INFO, LOG_TAG, "[Client] Out Of Memory!\n");
			exit(1);
		}
		
		dlog_print(DLOG_INFO, LOG_TAG, "sent message..dbus_connection_send_with_reply");

		if (NULL == pending) {
			dlog_print(DLOG_INFO, LOG_TAG, "[Client] Pending Call Null\n");
			exit(1);
		}
		dbus_connection_flush(conn);

		if (NULL == msg) {
			dlog_print(DLOG_INFO, LOG_TAG, "[Client] Message Null\n");
			exit(1);
		}		
    }
        	// close the connection
	dbus_connection_close(conn);
    return 0;
}



