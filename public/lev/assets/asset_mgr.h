#pragma once

// i think im going insane

#include <lev/containers/vector.h>
#include <lev/containers/hash_map.h>
#include <lev/core/util.h>

#define LEV_MAX_ASSET_HOLDERS 16

namespace lev
{
	struct ShaderLoadData
	{
		int type;
		bool is_source;
		const char* vertex;
		const char* fragment;
		const char* geometry;
		const char* compute;
	};

	struct FontLoadData
	{
		float size;
		const char* path;
	};
}

///////////////////////////////////////////////////////

namespace lev
{
	class AssetLoaderRegistry
	{
		LEV_SINGLETON_CLASS(AssetLoaderRegistry)

	public:
		template <class Asset, typename... LoadData>
		u64 retrieve_id()
		{
			static u64 id = m_counter++;
			return id;
		}

	private:
		u64 m_counter = 0;
	};

	class AssetLoaderBase { };

	template <class Asset, typename... LoadData>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		struct Meta { static u64 id; };

		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		virtual Ref<Asset> load(const char* name, LoadData... data) = 0;
		virtual Ref<Asset> get(const char* name) = 0;
		virtual bool has(const char* name) = 0;
	};

	template <class Asset, typename... LoadData>
	u64 AssetLoader<Asset, LoadData...>::Meta::id = AssetLoaderRegistry::inst().retrieve_id<Asset>();

	class AssetMgr
	{
		LEV_SINGLETON_CLASS(AssetMgr)

	public:
		AssetMgr();

		template <class Loader, typename... Args>
		void register_loader(Args&&... args);

		template <class Asset, typename... LoadData>
		AssetLoader<Asset, LoadData...>* retrieve_loader();

		template <class Asset, typename... LoadData>
		Ref<Asset> load(const char* name, LoadData... data);

		template <class Asset, typename... LoadData>
		Ref<Asset> get(const char* name);

		template <class Asset, typename... LoadData>
		bool has(const char* name);

	private:
		HashMap<u64, Ref<AssetLoaderBase>> m_loaders;
	};

	template <class Loader, typename... Args>
	void AssetMgr::register_loader(Args&&... args)
	{
		m_loaders.insert(Loader::Meta::id, create_ref<Loader>(args...));
	}

	template <class Asset, typename... LoadData>
	AssetLoader<Asset, LoadData...>* AssetMgr::retrieve_loader()
	{
		return (AssetLoader<Asset, LoadData...>*)(m_loaders.get(AssetLoader<Asset, LoadData...>::Meta::id).get());
	}

	template <class Asset, typename... LoadData>
	Ref<Asset> AssetMgr::load(const char* name, LoadData... data)
	{
		return retrieve_loader<Asset, LoadData...>()->load(name, data...);
	}

	template <class Asset, typename... LoadData>
	Ref<Asset> AssetMgr::get(const char* name)
	{
		return retrieve_loader<Asset, LoadData...>()->get(name);
	}

	template <class Asset, typename... LoadData>
	bool AssetMgr::has(const char* name)
	{
		return retrieve_loader<Asset, LoadData...>()->has(name);
	}
}
