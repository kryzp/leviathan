#pragma once

// i think im going insane

#include <lev/containers/vector.h>
#include <lev/containers/hash_map.h>
#include <lev/core/util.h>
#include <lev/containers/string.h>

#define LEV_MAX_ASSET_TYPES 16

namespace lev
{
	struct ShaderLoadData
	{
		bool is_source = false;
		const char* vertex = nullptr;
		const char* fragment = nullptr;
		const char* geometry = nullptr;
		const char* compute = nullptr;

		explicit ShaderLoadData(const char* compute, bool is_source = false)
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
	class AssetRegistry
	{
	public:
		static AssetRegistry* inst()
		{
			static AssetRegistry* instance = nullptr;
			if (!instance) { instance = new AssetRegistry(); }
			return instance;
		}

		template <class Asset> struct Meta { static u16 id; };

		template <class Asset>
		u16 register_id()
		{
			AssetRegistry::Meta<Asset>::id = m_counter++;
			return m_counter - 1;
		}

	private:
		u16 m_counter = 0;
	};

	template <class Asset>
	u16 AssetRegistry::Meta<Asset>::id = AssetRegistry::inst()->register_id<Asset>();

	class AssetLoaderBase { };

	template <class Asset, typename... LoadData>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		//struct Meta { static u16 asset_id; };

		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		virtual void load(Ref<Asset>& obj, LoadData... data) = 0;
	};

	//template <class Asset, typename... LoadData>
	//u16 AssetLoader<Asset, LoadData...>::Meta::asset_id = AssetRegistry::Meta<Asset>::id;

	class AssetMgr
	{
	public:
		static AssetMgr* inst()
		{
			static AssetMgr* instance = nullptr;
			if (!instance) { instance = new AssetMgr(); }
			return instance;
		}

		AssetMgr();

		template <class Loader, class Asset, typename... Args>
		void register_loader(Args&&... args);

		template <class Asset, typename... LoadData>
		Ref<Asset> load(const String& name, LoadData... data);

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

		AssetLoaderBase* m_loaders[LEV_MAX_ASSET_TYPES];
		AssetListBase* m_assets[LEV_MAX_ASSET_TYPES];
	};

	template <class Loader, class Asset, typename... Args>
	void AssetMgr::register_loader(Args&&... args)
	{
		m_loaders[AssetRegistry::Meta<Asset>::id] = new Loader(std::forward<Args>(args)...);
	}

	template <class Asset, typename... LoadData>
	Ref<Asset> AssetMgr::load(const String& name, LoadData... data)
	{
		Ref<Asset> obj;

		static_cast<AssetLoader<Asset, LoadData...>*>(
			m_loaders[AssetRegistry::Meta<Asset>::id]
		)->load(obj, data...);

		AssetListBase* base = m_assets[AssetRegistry::Meta<Asset>::id];

		if (!base)
			base = new AssetList<Asset>();

		static_cast<AssetList<Asset>*>(base)->assets.insert(name, obj);

		return obj;
	}

	template <class Asset>
	void AssetMgr::unload(const String& name)
	{
		static_cast<AssetList<Asset>*>(
			m_assets[AssetRegistry::Meta<Asset>::id]
		)->assets.remove(name);
	}

	template <class Asset>
	Ref<Asset> AssetMgr::get(const String& name)
	{
		return static_cast<AssetList<Asset>*>(
			m_assets[AssetRegistry::Meta<Asset>::id]
		)->assets.at(name);
	}

	template <class Asset>
	bool AssetMgr::has(const String& name)
	{
		return static_cast<AssetList<Asset>*>(
			m_assets[AssetRegistry::Meta<Asset>::id]
		)->assets.contains(name);
	}
}
