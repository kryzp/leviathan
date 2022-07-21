#pragma once

// i think im going insane

#include <lev/containers/vector.h>
#include <lev/containers/hash_map.h>
#include <lev/core/util.h>
#include <lev/containers/string.h>

#define LEV_MAX_ASSET_TYPES 16

namespace lv
{
	class AssetLoaderBase : public NonCopyable, public NonMovable { };

	template <class TAsset>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		virtual Ref<TAsset> load(const String& path) = 0;
	};

	class AssetMgr : public NonCopyable, public NonMovable
	{
		struct AssetListBase { };
		template <typename TAsset>
		struct AssetList : public AssetListBase { HashMap<String, Weak<TAsset>> assets; };

	public:
		AssetMgr();
		~AssetMgr();

		template <class TLoader, class TAsset>
		void register_loader();

		template <class TAsset>
		Ref<TAsset> load(const String& path);

		template <class TAsset>
		void free_load_data();

	private:
		template <class TAsset>
		u16 retrieve_asset_id()
		{
			static u16 id = m_counter++;
			return id;
		}

		AssetLoaderBase* m_loaders[LEV_MAX_ASSET_TYPES];
		AssetListBase* m_assets[LEV_MAX_ASSET_TYPES];

		u16 m_counter = 0;
	};

	template <class TLoader, class TAsset>
	void AssetMgr::register_loader()
	{
		u16 id = retrieve_asset_id<TAsset>();
		m_loaders[id] = new TLoader();
	}

	template <class TAsset>
	Ref<TAsset> AssetMgr::load(const String& path)
	{
		if (!m_assets[retrieve_asset_id<TAsset>()])
			m_assets[retrieve_asset_id<TAsset>()] = new AssetList<TAsset>();

		auto* asset_list = static_cast<AssetList<TAsset>*>(m_assets[retrieve_asset_id<TAsset>()]);

		// return the requested asset if its already loaded
		if (asset_list->assets.contains(path))
			return Ref<TAsset>(asset_list->assets.at(path));

		// otherwise load a new asset
		Ref<TAsset> obj = static_cast<AssetLoader<TAsset>*>(
			m_loaders[retrieve_asset_id<TAsset>()]
		)->load(path);

		// store it as a weak ptr
		Weak<TAsset> weakptr = obj;
		asset_list->assets.insert(path, weakptr);

		return obj;
	}

	template <class TAsset>
	void AssetMgr::free_load_data()
	{
		delete static_cast<AssetList<TAsset>*>(m_assets[retrieve_asset_id<TAsset>()]);
		delete static_cast<AssetLoader<TAsset>*>(m_loaders[retrieve_asset_id<TAsset>()]);
	}
}
