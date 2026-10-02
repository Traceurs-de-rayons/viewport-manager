#include "viewportManager.hpp"
#include "viewport.hpp"
#include "rasterCore.hpp"
#include "renderApi.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>

ViewportManager::ViewportManager() {}
ViewportManager::~ViewportManager() {}

ViewportManager::ViewportManager(ViewportManager&&) noexcept = default;
ViewportManager& ViewportManager::operator=(ViewportManager&&) noexcept = default;

Viewport* ViewportManager::addViewport(const ViewportData& data) {
	if (!_gpu) {
		std::cerr << "ViewportManager: Cannot add viewport - GPU not initialized" << std::endl;
		return nullptr;
	}

	ViewportData viewportData = data;

	if (viewportData.name.empty())
		viewportData.name = "Viewport_" + std::to_string(_nextViewportId);

	if (getViewport(viewportData.name) != nullptr) {
		std::cerr << "ViewportManager: Viewport '" << viewportData.name << "' already exists" << std::endl;
		return nullptr;
	}

	viewportData.id = static_cast<uint32_t>(_nextViewportId++);

	auto viewport = std::make_unique<Viewport>(viewportData, _gpu);
	Viewport* ptr = viewport.get();

	
	if (_defaultSceneResources && _defaultSceneResources->isLoaded()) {
		Result result = ptr->setSceneResources(_defaultSceneResources, _defaultSceneName);
		if (result.code == ResultCode::Error)
			std::cerr << "ViewportManager: " << result.message << std::endl;
	}

	_viewports.push_back(std::move(viewport));

	std::cout << "ViewportManager: Added viewport '" << ptr->getName()
			  << "' (ID: " << ptr->getId() << ")" << std::endl;
	return ptr;
}

Viewport* ViewportManager::getViewport(const std::string& name) {
	for (auto& viewport : _viewports) {
		if (viewport && viewport->getName() == name)
			return viewport.get();
	}
	return nullptr;
}

Viewport* ViewportManager::getViewport(uint32_t id) {
	for (auto& viewport : _viewports) {
		if (viewport && viewport->getId() == id)
			return viewport.get();
	}
	return nullptr;
}

bool ViewportManager::removeViewport(const std::string& name) {
	for (auto it = _viewports.begin(); it != _viewports.end(); ++it) {
		if (*it && (*it)->getName() == name) {
			std::cout << "ViewportManager: Removed viewport '" << name << "'" << std::endl;
			_viewports.erase(it);
			return true;
		}
	}
	return false;
}

bool ViewportManager::removeViewport(uint32_t id) {
	for (auto it = _viewports.begin(); it != _viewports.end(); ++it) {
		if (*it && (*it)->getId() == id) {
			std::cout << "ViewportManager: Removed viewport (ID: " << id << ")" << std::endl;
			_viewports.erase(it);
			return true;
		}
	}
	return false;
}

const std::vector<std::unique_ptr<Viewport>>& ViewportManager::getViewports() const {
	return _viewports;
}

size_t ViewportManager::getViewportCount() const {
	return _viewports.size();
}

bool ViewportManager::init() {
	using namespace renderApi::instance;

	Config config;
	config.appName = "RT";
	config.appVersion = VK_MAKE_VERSION(1, 0, 0);
	config.engineName = "RT - Engine";
	config.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	config.apiVersion = VK_API_VERSION_1_3;

	InitInstanceResult result = renderApi::initNewInstance(config);
	if (result != INIT_VK_INSTANCE_SUCCESS) {
		std::cerr << "ViewportManager: Failed to initialize Vulkan instance: " << result << std::endl;
		return false;
	}

	_vkMainInstance = &renderApi::getInstances().back();

	renderApi::device::Config gpuConfig;
	gpuConfig.graphics = 1;
	gpuConfig.compute = 1;
	gpuConfig.transfer = 1;

	auto deviceResult = _vkMainInstance->addGPU(gpuConfig);
	if (deviceResult != renderApi::device::InitDeviceResult::INIT_DEVICE_SUCCESS) {
		std::cerr << "ViewportManager: Failed to add GPU" << std::endl;
		return false;
	}

	_gpu = _vkMainInstance->getGPU(0);
	if (!_gpu) {
		std::cerr << "ViewportManager: Failed to get GPU" << std::endl;
		return false;
	}

	std::cout << "ViewportManager: Initialized with GPU: " << _gpu->name << std::endl;
	return true;
}

void ViewportManager::setDefaultScene(RasterCore::SharedGpuResources* resources, const std::string& sceneName) {
	_defaultSceneResources = resources;
	_defaultSceneName = sceneName;

	if (!_defaultSceneResources)
		return;

	
	for (auto& viewport : _viewports) {
		if (viewport && !viewport->hasScene()) {
			Result result = viewport->setSceneResources(_defaultSceneResources, _defaultSceneName);
			if (result.code == ResultCode::Error)
				std::cerr << "ViewportManager: " << result.message << std::endl;
		}
	}
}

void ViewportManager::detachScene(RasterCore::SharedGpuResources* resources) {
	if (!resources)
		return;

	for (auto& viewport : _viewports) {
		if (viewport && viewport->getSceneResources() == resources) {
			std::cout << "ViewportManager: Detaching viewport '" << viewport->getName()
					  << "' from scene '" << viewport->getSceneName() << "'" << std::endl;
			if (_defaultSceneResources && _defaultSceneResources != resources)
				viewport->setSceneResources(_defaultSceneResources, _defaultSceneName);
			else
				viewport->setSceneResources(nullptr, "");
		}
	}
}

void ViewportManager::renderAll() {
	for (auto& viewport : _viewports) {
		if (viewport && viewport->isActive())
			viewport->render();
	}
}

renderApi::device::GPU* ViewportManager::getGpu() const {
	return _gpu;
}
