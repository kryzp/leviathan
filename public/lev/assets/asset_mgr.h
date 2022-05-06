#pragma once

// i think im going insane

#include <lev/containers/vector.h>
#include <lev/containers/hash_map.h>
#include <lev/core/util.h>
#include <lev/containers/string.h>

#define LEV_MAX_ASSET_TYPES 16

namespace lev
{
	class AssetLoaderBase : public NonCopyable, public NonMovable { };

	template <class Asset>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		virtual Ref<Asset> load(const String& path) = 0;
	};

	class AssetMgr : public NonCopyable, public NonMovable
	{
		struct AssetListBase { };
		template <typename Asset>
		struct AssetList : public AssetListBase { HashMap<lev::String, Weak<Asset>> assets; };

	public:
		AssetMgr();
		~AssetMgr();

		template <class Loader, class Asset>
		void register_loader();

		template <class Asset>
		Ref<Asset> load(const String& path);

		template <class Asset>
		void free_load_data();

	private:
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
	Ref<Asset> AssetMgr::load(const String& path)
	{
		if (!m_assets[retrieve_asset_id<Asset>()])
			m_assets[retrieve_asset_id<Asset>()] = new AssetList<Asset>();

		auto* asset_list = static_cast<AssetList<Asset>*>(m_assets[retrieve_asset_id<Asset>()]);

		// return the requested asset if its already loaded
		if (asset_list->assets.contains(path))
			return Ref<Asset>(asset_list->assets.at(path));

		// otherwise load a new asset
		Ref<Asset> obj = static_cast<AssetLoader<Asset>*>(
			m_loaders[retrieve_asset_id<Asset>()]
		)->load(path);

		// store it as a weak ptr
		Weak<Asset> weakptr = obj;
		asset_list->assets.insert(path, weakptr);

		return obj;
	}

	template <class Asset>
	void AssetMgr::free_load_data()
	{
		delete static_cast<AssetList<Asset>*>(m_assets[retrieve_asset_id<Asset>()]);
		delete static_cast<AssetLoader<Asset>*>(m_loaders[retrieve_asset_id<Asset>()]);
	}
}
