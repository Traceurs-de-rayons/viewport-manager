#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "renderDevice.hpp"


namespace RasterCore {
	struct SharedGpuResources;
}

class Viewport;
struct ViewportData;

class ViewportManager {
	private:
		std::vector<std::unique_ptr<Viewport>>	_viewports;
		uint64_t								_nextViewportId = 0;
		renderApi::device::GPU*					_gpu = nullptr;
		renderApi::instance::RenderInstance*	_vkMainInstance = nullptr;
		RasterCore::SharedGpuResources*			_defaultSceneResources = nullptr;
		std::string								_defaultSceneName;

	public:
		ViewportManager();
		~ViewportManager();

		ViewportManager(const ViewportManager&) = delete;
		ViewportManager& operator=(const ViewportManager&) = delete;
		ViewportManager(ViewportManager&&) noexcept;
		ViewportManager& operator=(ViewportManager&&) noexcept;

		Viewport* addViewport(const ViewportData& data);

		Viewport* getViewport(const std::string& name);
		Viewport* getViewport(uint32_t id);

		bool removeViewport(const std::string& name);
		bool removeViewport(uint32_t id);

		const std::vector<std::unique_ptr<Viewport>>& getViewports() const;

		size_t getViewportCount() const;

		bool init();

		void setDefaultScene(RasterCore::SharedGpuResources* resources, const std::string& sceneName);
		RasterCore::SharedGpuResources* getDefaultSceneResources() const { return _defaultSceneResources; }

		void detachScene(RasterCore::SharedGpuResources* resources);

		void renderAll();

		renderApi::device::GPU* getGpu() const;
};
