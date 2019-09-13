/*
 * FogLAMP "asset" notification plugin.
 *
 * Copyright (c) 2019 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch
 */

#include <string>
#include <plugin_api.h>
#include <config_category.h>
#include <logger.h>
#include <asset.h>
#include <version.h>


#define PLUGIN_NAME "asset"

const char * default_config = QUOTE({
	"plugin" : { 
		"description" : "Asset notification plugin",
		"type" : "string",
		"default" : PLUGIN_NAME,
		"readonly" : "true" },
	"asset" : {
		"description" : "The asset name to create for the notification",
		"type" : "string",
		"default" : "event",
		"order" : "1",
		"displayName" : "Asset"
		},
	"description" : {
		"description" : "The event description to add",
		"type" : "string",
		"default" : "Notification alert",
		"order" : "2",
		"displayName" : "Description"
		},
	"enable": {
		"description": "A switch that can be used to enable or disable delivery of the asset notification plugin.",
		"type": "boolean",
		"displayName" : "Enabled",
		"default": "false", 
		"order" : "3" }
	});


using namespace std;
using namespace rapidjson;

/**
 * The Notification plugin interface
 */
extern "C" {

/**
 * The plugin information structure
 */
static PLUGIN_INFORMATION info = {
        PLUGIN_NAME,              // Name
        VERSION,                  // Version
        SP_INGEST,                // Flags
        PLUGIN_TYPE_NOTIFICATION_DELIVERY,       // Type
        "1.0.0",                  // Interface version
        default_config	          // Default plugin configuration
};

/**
 * Return the information about this plugin
 */
PLUGIN_INFORMATION *plugin_info()
{
	return &info;
}


/**
 * Initialise the plugin, called to get the plugin handle and setup the
 * plugin configuration
 *
 * @param config	The configuration category for the plugin
 * @return		An opaque handle that is used in all subsequent calls to the plugin
 */
PLUGIN_HANDLE plugin_init(ConfigCategory* config)
{
	Asset *asset = new Asset(config);
	
	return (PLUGIN_HANDLE)asset;
}

/**
 * Deliver received notification data
 *
 * @param handle		The plugin handle returned from plugin_init
 * @param deliveryName		The delivery category name
 * @param notificationName	The notification name
 * @param triggerReason		The trigger reason for notification
 * @param message		The message from notification
 */
bool plugin_deliver(PLUGIN_HANDLE handle,
                    const std::string& deliveryName,
                    const std::string& notificationName,
                    const std::string& triggerReason,
                    const std::string& message)
{
	Logger::getLogger()->info("Asset notification plugin_deliver(): deliveryName=%s, notificationName=%s, triggerReason=%s, message=%s",
							deliveryName.c_str(), notificationName.c_str(), triggerReason.c_str(), message.c_str());
	Asset *asset = (Asset *)handle;
	asset->notify(notificationName, triggerReason, message);
	return true;
}

/**
 * Register a callback function used to ingest an asset to the FogLAMP buffer
 */
void plugin_registerIngest(PLUGIN_HANDLE *handle, void *func, void *data)
{
	Logger::getLogger()->info("Asset notification plugin: plugin_registerIngrest()");
	Asset *asset = (Asset *)handle;
	
	asset->registerIngest((FuncPtr)func, data);
	return;
}

/**
 * Reconfigure the plugin
 */
void plugin_reconfigure(PLUGIN_HANDLE *handle, string& newConfig)
{
	Logger::getLogger()->info("Asset notification plugin: plugin_reconfigure()");
	Asset *asset = (Asset *)handle;
	
	asset->reconfigure(newConfig);
	return;
}

/**
 * Call the shutdown method in the plugin
 */
void plugin_shutdown(PLUGIN_HANDLE *handle)
{
	Asset *asset = (Asset *)handle;
	delete asset;
}

// End of extern "C"
};

