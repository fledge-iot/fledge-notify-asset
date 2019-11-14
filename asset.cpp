/*
 * FogLAMP "asset" notification delivery plugin.
 *
 * Copyright (c) 2019 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch           
 */
#include "asset.h"
#include <logger.h>
#include <reading.h>

using namespace std;
using namespace rapidjson;


/**
 * Construct a asset notification plugin
 *
 * @param category	The configuration of the plugin
 */
Asset::Asset(ConfigCategory *category)
{
	m_asset = category->getValue("asset");
	m_description = category->getValue("description");
	// TODO We have no access to the storage layer
	m_ingest = NULL;
}

/**
 * The destructure for the asset plugin
 */
Asset::~Asset()
{
}

/**
 * Send a notification via the Asset by ingesting into the FogLAMP storage layer
 *
 * @param notificationName 	The name of this notification
 * @param triggerReason		Why the notification is being sent
 * @param message		The message to send
 */
void Asset::notify(const string& notificationName, const string& triggerReason, const string& message)
{
vector<Datapoint *>	datapoints;

	if (!m_ingest)
	{
		return;
	}

	DatapointValue dpv1(m_description);
	datapoints.push_back(new Datapoint("description", dpv1)); 

	Document doc;
	doc.Parse(triggerReason.c_str());
	if (!doc.HasParseError())
	{
		if (doc.HasMember("reason"))
		{
			if (doc["reason"].IsString())
			{
		       		DatapointValue dpv3(doc["reason"].GetString());
				datapoints.push_back(new Datapoint("event", dpv3));
			}
			else if (doc["reason"].IsInt64())
			{
		       		DatapointValue dpv3(doc["reason"].GetInt64());
				datapoints.push_back(new Datapoint("event", dpv3));
			}
			else
			{
				Logger::getLogger()->error("The reason returned from the rule for delivery is of a bad type");
			}
		}
	}
	DatapointValue dpv4(notificationName);
	datapoints.push_back(new Datapoint("rule", dpv4));
	Reading asset(m_asset, datapoints);

	Logger::getLogger()->info("Asset notification: %s", asset.toJSON().c_str());

	(*m_ingest)(m_data, &asset);
}

/**
 * Reconfigure the asset delivery plugin
 *
 * @param newConfig	The new configuration
 */
void Asset::reconfigure(const string& newConfig)
{
	ConfigCategory category("new", newConfig);
	m_asset = category.getValue("asset");
	m_description = category.getValue("description");
}
