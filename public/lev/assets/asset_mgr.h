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

		virtual void load(Ref<Asset>& obj, const String& path) = 0;
	};

	class AssetMgr
	{
	public:
		AssetMgr();

		template <class Loader, class Asset, typename... Args>
		void register_loader(Args&&... args);

		template <class Asset>
		Ref<Asset> load(const String& name, const String& path);

		template <class Asset>
		void unload(const String& name);

		template <class Asset>
		Ref<Asset> get(const String& name);

		template <class Asset>
		bool has(const String& name);

	private:
		struct AssetListBase { };

		template <typename T>
		struct AssetList : public AssetListBase
		{
			HashMap<String, Ref<T>> assets;

			AssetList() = default;
			~AssetList() = default;
		};

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

	template <class Loader, class Asset, typename... Args>
	void AssetMgr::register_loader(Args&&... args)
	{
		u16 id = retrieve_asset_id<Asset>();
		m_loaders[id] = new Loader(std::forward<Args>(args)...);
	}

	template <class Asset>
	Ref<Asset> AssetMgr::load(const String& name, const String& path)
	{
		Ref<Asset> obj;

		static_cast<AssetLoader<Asset>*>(
			m_loaders[retrieve_asset_id<Asset>()]
		)->load(obj, path);

		AssetListBase* base = m_assets[retrieve_asset_id<Asset>()];

		if (!base)
			base = new AssetList<Asset>();

		static_cast<AssetList<Asset>*>(base)->assets.insert(name, obj);

		return obj;
	}

	template <class Asset>
	void AssetMgr::unload(const String& name)
	{
		static_cast<AssetList<Asset>*>(
			m_assets[retrieve_asset_id<Asset>()]
		)->assets.remove(name);
	}

	template <class Asset>
	Ref<Asset> AssetMgr::get(const String& name)
	{
		return static_cast<AssetList<Asset>*>(
			m_assets[retrieve_asset_id<Asset>()]
		)->assets.at(name);
	}

	template <class Asset>
	bool AssetMgr::has(const String& name)
	{
		return static_cast<AssetList<Asset>*>(
			m_assets[retrieve_asset_id<Asset>()]
		)->assets.contains(name);
	}
}
