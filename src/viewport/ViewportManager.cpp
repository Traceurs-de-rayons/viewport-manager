#include "viewportManager.hpp"
#include "viewport.hpp"
#include "rasterCore.hpp"
#include "renderApi.hpp"
#include "buffer/buffer.hpp"
#include "image/image.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>

ViewportManager::ViewportManager() {}
ViewportManager::~ViewportManager() {}

Viewport* ViewportManager::addViewport(const ViewportData& data) {
	(void)data;
	// if (!impl_)
	// 	return nullptr;

	// if (!window) {
	// 	std::cerr << "ViewportManager: Cannot add viewport - null window" << std::endl;
	// 	return nullptr;
	// }

	// int w, h;
	// SDL_GetWindowSize(window, &w, &h);

	// std::string viewportName = name.empty() ? impl_->generateName() : name;

	// if (impl_->nameToIndex.find(viewportName) != impl_->nameToIndex.end()) {
	// 	std::cerr << "ViewportManager: Viewport '" << viewportName << "' already exists" << std::endl;
	// 	return nullptr;
	// }

	// uint32_t id = Viewport::Impl::nextId++;
	// auto viewport = Viewport::Impl::create(id, viewportName, w, h, ViewportOutput::Window, window, &impl_->sharedResources, this);

	// auto* ptr = viewport.get();
	// size_t index = impl_->viewports.size();

	// impl_->nameToIndex[viewportName] = index;
	// impl_->idToIndex[id] = index;
	// impl_->viewports.push_back(std::move(viewport));

	// std::cout << "ViewportManager: Added window viewport '" << viewportName << "' (ID: " << id << ")" << std::endl;
	// return ptr;
	return nullptr;
}

Viewport* ViewportManager::getViewport(const std::string& name) {
	return nullptr;
}

Viewport* ViewportManager::getViewport(uint32_t id) {
	return nullptr;
}

bool ViewportManager::removeViewport(const std::string& name) {
	return true;
}

bool ViewportManager::removeViewport(uint32_t id) {
	return true;
}

const std::vector<std::unique_ptr<Viewport>>& ViewportManager::getViewports() const {
	return _viewports;
}

size_t ViewportManager::getViewportCount() const {
	return _viewports.size();
}
