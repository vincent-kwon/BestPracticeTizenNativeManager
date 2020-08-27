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
#include <dlog.h>
#include <glib.h>
#include <bp_manager_tizen_common.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "BPS_BP_MANAGER_TIZEN"

int bp_manager_tizen_get_one()
{
	dlog_print(DLOG_INFO, LOG_TAG, "bptizen_get_one is called.......");
	return 0;
}

int _bp_manager_initialize() 
{
	dlog_print(DLOG_INFO, LOG_TAG, "_bp_manager_initialize is called.......");
	return 0;
}

int main()
{
	GMainLoop *mainloop = NULL;

	LOGD("Enter main loop\n");

#ifdef TIZEN_TEST_GCOV
	setenv("GCOV_PREFIX", "/tmp/daemon", 1);
#endif
    _bp_manager_initialize();
	mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);

	return 0;
}

#if 0
	alarm_context.proxy = g_dbus_proxy_new_sync(alarm_context.connection,
			G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START_AT_CONSTRUCTION,
			NULL,
			"org.tizen.alarm.manager",
			"/org/tizen/alarm/manager",
			"org.tizen.alarm.manager",
			NULL,
			NULL);

	if (alarm_context.proxy == NULL) {
		LOGE("Creating a proxy is failed.");
		g_object_unref(alarm_context.connection);
		return ERR_ALARM_SYSTEM_FAIL;
	}

	sub_initialized = true;

	return ALARMMGR_RESULT_SUCCESS;

		return_code = __dbus_call_sync(context.proxy, "alarm_create_noti",
			param, &reply);
	if (return_code != ALARMMGR_RESULT_SUCCESS) {
		if (error_code)
			*error_code = return_code;
		return false;
	}

	static int __dbus_call_sync(GDBusProxy *proxy, const gchar *method_name,
		GVariant *param, GVariant **reply)
{
	int error_code = ALARMMGR_RESULT_SUCCESS;
	GError *error = NULL;

	*reply = g_dbus_proxy_call_sync(proxy, method_name, param,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	if (error) {
		if (error->code == G_DBUS_ERROR_ACCESS_DENIED)
			error_code = ERR_ALARM_NO_PERMISSION;
		else
			error_code = ERR_ALARM_SYSTEM_FAIL;

		LOGE("%s : g_dbus_proxy_call_sync() failed.\
				error_code[%d]. error->message is %s(%d)",
				method_name, error_code, error->message, error->code);

		g_error_free(error);
	}

	return error_code;
}
#endif	