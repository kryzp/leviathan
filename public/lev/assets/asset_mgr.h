#pragma once

// i think im going insane

#include <lev/containers/vector.h>
#include <lev/containers/hash_map.h>
#include <lev/core/util.h>
#include <lev/containers/string.h>

#define LEV_MAX_ASSET_TYPES 16

namespace lev
{
	class AssetLoaderBase { };

	template <class Asset>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		virtual Asset* load(const String& path) = 0;
	};

	class AssetMgr
	{
	public:
		AssetMgr();
		~AssetMgr();

		template <class Loader, class Asset>
		void register_loader();

		template <class Asset>
		Asset* load(const String& path);

		template <class Asset>
		void unload_all();

		template <class Asset>
		void free_load_data();

	private:
		struct AssetListBase { };
		template <typename T>
		struct AssetList : public AssetListBase { HashMap<lev::String, T*> assets; };

		template <class Asset>
		u16 retrieve_asset_id()
		{
			static u16 id = m_counter++;
			return id;
		}

		AssetLoaderBase* m_loaders[LEV_MAX_ASSET_TYPES];
		AssetListBase* m_assets[LEV_MAX_ASSET_TYPES];

		u16 m_counter = 0;
	};

	template <class Loader, class Asset>
	void AssetMgr::register_loader()
	{
		u16 id = retrieve_asset_id<Asset>();
		m_loaders[id] = new Loader();
	}

	template <class Asset>
	Asset* AssetMgr::load(const String& path)
	{
		if (!m_assets[retrieve_asset_id<Asset>()])
			m_assets[retrieve_asset_id<Asset>()] = new AssetList<Asset>();

		auto* asset_list = static_cast<AssetList<Asset>*>(m_assets[retrieve_asset_id<Asset>()]);

		if (asset_list->assets.contains(path))
			return asset_list->assets.at(path);

		Asset* obj = static_cast<AssetLoader<Asset>*>(
			m_loaders[retrieve_asset_id<Asset>()]
		)->load(path);

		asset_list->assets.insert(path, obj);

		return obj;
	}

	template <class Asset>
	void AssetMgr::unload_all()
	{
		for (auto& list : m_assets)
		{
			if (!list)
				continue;

			for (auto entry = static_cast<AssetList<Asset>*>(list)->assets.begin(); entry; entry = entry->next)
				delete entry->value;
		}
	}

	template <class Asset>
	void AssetMgr::free_load_data()
	{
		delete static_cast<AssetList<Asset>*>(m_assets[retrieve_asset_id<Asset>()]);
		delete static_cast<AssetLoader<Asset>*>(m_loaders[retrieve_asset_id<Asset>()]);
	}
}
