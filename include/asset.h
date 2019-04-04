#ifndef _ASSET_H
#define _ASSET_H
#include <config_category.h>
#include <string>
#include <logger.h>
#include <storage_client.h>

/**
 * A simple asse notification class that sends an asset
 * via FogLAMP to the systems north of FogLAMP
 */
class Asset {
	public:
		Asset(ConfigCategory *config);
		~Asset();
		void	notify(const std::string& notificationName, const std::string& triggerReason, const std::string& message);
		void	reconfigure(const std::string& newConfig);
	private:
		std::string	m_asset;
		std::string	m_description;
		StorageClient	*m_storage;
};
#endif
