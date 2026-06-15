#include "viewportManager.hpp"
#include "viewport.hpp"
#include "rasterCore.hpp"
#include "renderApi.hpp"
#include "imguiLayer.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>

ViewportManager::ViewportManager() {}
ViewportManager::~ViewportManager() {}

Viewport* ViewportManager::addViewport(const ViewportData& data) {
	(void)data;
	return nullptr;
}

bool ViewportManager::init() {
	using namespace renderApi::instance;

	Config config;
	config.appName = "RT";
	config.appVersion = VK_MAKE_VERSION(1, 0, 0);
	config.engineName = "RT - Engine";
	config.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	config.apiVersion = VK_API_VERSION_1_3;

	// Validation layers (optionnel - nécessite le SDK Vulkan avec layers)
	// Si les layers ne sont pas installés, l'instance crée sans
	// config.layers.push_back("VK_LAYER_KHRONOS_validation");

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

bool ViewportManager::initWorkspace() {
	if (!_imguiLayer) {
		std::cerr << "ViewportManager: No ImGuiLayer set, cannot create workspace windows" << std::endl;
		return false;
	}
	
	// Create default workspace windows
	_imguiLayer->createWindow(ImGuiWindowType::SceneViewport);
	_imguiLayer->createWindow(ImGuiWindowType::CpuStats);
	
	std::cout << "ViewportManager: Workspace initialized with default windows" << std::endl;
	return true;
}

renderApi::device::GPU* ViewportManager::getGpu() const {
	return _gpu;
}

Viewport* ViewportManager::getViewport(const std::string& name) {
	(void)name;
	return nullptr;
}

Viewport* ViewportManager::getViewport(uint32_t id) {
	(void)id;
	return nullptr;
}

bool ViewportManager::removeViewport(const std::string& name) {
	(void)name;
	return true;
}

bool ViewportManager::removeViewport(uint32_t id) {
	(void)id;
	return true;
}

const std::vector<std::unique_ptr<Viewport>>& ViewportManager::getViewports() const {
	return _viewports;
}

size_t ViewportManager::getViewportCount() const {
	return _viewports.size();
}
