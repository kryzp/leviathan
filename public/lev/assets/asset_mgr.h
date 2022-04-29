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

	private:
		template <class Asset>
		void free_load_data();

		struct AssetListBase { };
		template <typename T>
		struct AssetList : public AssetListBase { Vector<T*> assets; };

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
		Asset* obj = static_cast<AssetLoader<Asset>*>(
			m_loaders[retrieve_asset_id<Asset>()]
		)->load(path);

		AssetListBase* base = m_assets[retrieve_asset_id<Asset>()];

		if (!base)
			base = new AssetList<Asset>();

		static_cast<AssetList<Asset>*>(base)->assets.push_back(obj);

		return obj;
	}

	template <class Asset>
	void AssetMgr::unload_all()
	{
		for (auto& list : m_assets)
		{
			if (!list)
				continue;

			for (auto& asset : static_cast<AssetList<Asset>*>(list)->assets)
				delete asset;
		}
	}

	template <class Asset>
	void AssetMgr::free_load_data()
	{
		delete static_cast<AssetList<Asset>*>(m_assets[retrieve_asset_id<Asset>()]);
		delete static_cast<AssetLoader<Asset>*>(m_loaders[retrieve_asset_id<Asset>()]);
	}
}
