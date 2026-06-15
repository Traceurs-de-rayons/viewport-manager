#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "renderDevice.hpp"

struct SDL_Window;
class ImGuiLayer;

class Viewport;
struct ViewportData;
struct SharedGpuResources;

class ViewportManager {
	private:
		std::vector<std::unique_ptr<Viewport>>	_viewports;
		uint64_t 								_nextViewportId = 0;
		renderApi::device::GPU* 				_gpu = nullptr;
		renderApi::instance::RenderInstance* 			_vkMainInstance = nullptr;
		ImGuiLayer*								_imguiLayer = nullptr;

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
		bool initWorkspace();
		
		void setImGuiLayer(ImGuiLayer* layer) { _imguiLayer = layer; }

		renderApi::device::GPU* getGpu() const;
		
		// todoo

		// void renderAll();

		// void pauseAllViewportTasks();
		// void resumeAllViewportTasks();

};
