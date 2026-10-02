#pragma once

#include "rasterTypes.hpp"
#include "return.hpp"
#include "viewportManagerType.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vulkan/vulkan_core.h>

struct	SDL_Window;

class	ViewportManager;

namespace RasterCore {
	class RasterPipeline;
	struct SharedGpuResources;
}

namespace renderApi::device {
	struct GPU;
}

struct ViewportData {
	uint32_t						id = 0;
	std::string						name;
	uint32_t						width = 1280;
	uint32_t						height = 720;
	bool							active = true;
	ViewportRenderMode				activeRenderMode = ViewportRenderMode::Rasterisation;
	RasterCore::Camera				camera = {};

	static ViewportData DefaultRaster() {
		ViewportData data;
		data.id = 0;
		data.name = "Default";
		return data;
	}
};

class Viewport {
	private:
		ViewportData							_data;
		RasterCore::SharedGpuResources*			_sceneResources = nullptr;
		std::string								_sceneName;
		renderApi::device::GPU*					_gpu = nullptr;
		std::shared_ptr<RasterCore::RasterPipeline>	_pipeline;

		bool						initPipeline();

	public:
		Viewport(const ViewportData& data, renderApi::device::GPU* gpu);
		~Viewport();

		Viewport(const Viewport&) = delete;
		Viewport& operator=(const Viewport&) = delete;

		uint32_t					getId() const;
		const std::string&			getName() const;

		Result						setRenderMode(ViewportRenderMode mode);
		ViewportRenderMode			getRenderMode() const;

		void						setActive(bool active);
		bool						isActive() const;

		Result						setSceneResources(RasterCore::SharedGpuResources* resources, const std::string& sceneName);
		RasterCore::SharedGpuResources*	getSceneResources() const;
		const std::string&			getSceneName() const;
		bool						hasScene() const;

		Result						resize(uint32_t width, uint32_t height);
		uint32_t					getWidth() const;
		uint32_t					getHeight() const;

		void						render();

		void						setCamera(const RasterCore::Camera& camera);
		RasterCore::Camera&			getCamera();

		RasterCore::RasterPipeline*	getPipeline() const;
		VkImageView					getColorImageView() const;
};
