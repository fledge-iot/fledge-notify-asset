#ifndef _ASSET_H
#define _ASSET_H
#include <config_category.h>
#include <string>
#include <logger.h>
#include <storage_client.h>

typedef void (*FuncPtr)(void *, void *);
/**
 * A simple asset notification class that sends an asset
 * via Fledge to the systems north of Fledge
 */
class Asset {
	public:
		Asset(ConfigCategory *config);
		~Asset();
		void	notify(const std::string& notificationName, const std::string& triggerReason, const std::string& message);
		void	reconfigure(const std::string& newConfig);
		void	registerIngest(FuncPtr ingest, void *data)
		{
			m_ingest = ingest;
			m_data = data;
		};
	private:
		std::string	m_asset;
		std::string	m_description;
		FuncPtr		m_ingest;
		void		*m_data;
};
#endif
