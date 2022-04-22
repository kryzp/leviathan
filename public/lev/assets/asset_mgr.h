#pragma once

// i think im going insane

#include <lev/containers/vector.h>
#include <lev/containers/hash_map.h>
#include <lev/core/util.h>
#include <lev/containers/string.h>

#define LEV_MAX_ASSET_HOLDERS 16

namespace lev
{
	struct ShaderLoadData
	{
		bool is_source = false;
		const char* vertex = nullptr;
		const char* fragment = nullptr;
		const char* geometry = nullptr;
		const char* compute = nullptr;

		ShaderLoadData(const char* compute, bool is_source = false)
			: compute(compute)
            , is_source(is_source)
		{
		}

		ShaderLoadData(const char* vertex, const char* fragment, const char* geometry = nullptr, bool is_source = false)
			: vertex(vertex)
			, fragment(fragment)
			, geometry(geometry)
            , is_source(is_source)
		{
		}
	};

	struct FontLoadData
	{
		float size;
		const char* path;

		FontLoadData(float size, const char* path)
			: size(size)
			, path(path)
		{
		}
	};
}

///////////////////////////////////////////////////////

namespace lev
{
	class AssetLoaderRegistry
	{
		LEV_SINGLETON_CLASS(AssetLoaderRegistry);

	public:
		template <class Asset, typename... LoadData> // todo is 'typename... LoadData' really needed here?
		u16 retrieve_id()
		{
			static u16 id = m_counter++;
			return id;
		}

	private:
		u16 m_counter = 0;
	};

	class AssetLoaderBase { };

	template <class Asset, typename... LoadData>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		struct Meta { static u16 id; };

		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		virtual Ref<Asset> load(const String& name, LoadData... data) = 0;
		//virtual void unload(const char* name) = 0; //!!! TODO NOT DONE

		virtual Ref<Asset> get(const String& name) = 0;
		virtual bool has(const String& name) = 0;
	};

	template <class Asset, typename... LoadData>
	u16 AssetLoader<Asset, LoadData...>::Meta::id = AssetLoaderRegistry::inst()->retrieve_id<Asset>();

	class AssetMgr
	{
		LEV_SINGLETON_CLASS(AssetMgr);

	public:
		AssetMgr();

		template <class Loader, typename... Args>
		void register_loader(Args&&... args);

		template <class Asset, typename... LoadData>
		AssetLoader<Asset, LoadData...>* retrieve_loader();

		template <class Asset, typename... LoadData>
		Ref<Asset> load(const char* name, LoadData... data);

		//template <class Asset, typename... LoadData>
		//void unload(const char* name, LoadData... data);

		template <class Asset, typename... LoadData>
		Ref<Asset> get(const char* name);

		template <class Asset, typename... LoadData>
		bool has(const char* name);

	private:
		HashMap<u64, AssetLoaderBase*> m_loaders;
	};

	template <class Loader, typename... Args>
	void AssetMgr::register_loader(Args&&... args)
	{
		m_loaders.insert(Loader::Meta::id, new Loader(std::forward<Args>(args)...));
	}

	template <class Asset, typename... LoadData>
	AssetLoader<Asset, LoadData...>* AssetMgr::retrieve_loader()
	{
		return static_cast<AssetLoader<Asset, LoadData...>*>(
            m_loaders.at(AssetLoader<Asset, LoadData...>::Meta::id)
		);
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
