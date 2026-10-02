#include "viewport.hpp"
#include "rasterCore.hpp"
#include "renderDevice.hpp"

#include <iostream>

Viewport::Viewport(const ViewportData& data, renderApi::device::GPU* gpu)
	: _data(data), _gpu(gpu) {
}

Viewport::~Viewport() {
	if (_gpu)
		vkDeviceWaitIdle(_gpu->device);
	_pipeline.reset();
}

bool Viewport::initPipeline() {
	if (!_sceneResources || !_gpu)
		return false;

	if (!_sceneResources->isLoaded())
		return false;

	RasterCore::InitOptions options;
	options.width = _data.width;
	options.height = _data.height;
	options.colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
	options.sharedResources = _sceneResources;

	RasterCore::InitResult result = RasterCore::initRasterisation(options);
	if (!result.success || !result.pipeline) {
		std::cerr << "Viewport '" << _data.name << "': failed to init pipeline: "
				  << result.errorMessage << std::endl;
		return false;
	}

	_pipeline = result.pipeline;
	_pipeline->setCamera(_data.camera);

	std::cout << "Viewport '" << _data.name << "': pipeline created ("
			  << _data.width << "x" << _data.height
			  << ", scene='" << _sceneName << "')" << std::endl;
	return true;
}

uint32_t Viewport::getId() const {
	return _data.id;
}

const std::string& Viewport::getName() const {
	return _data.name;
}

Result Viewport::setRenderMode(ViewportRenderMode mode) {
	_data.activeRenderMode = mode;
	return Result::ok();
}

ViewportRenderMode Viewport::getRenderMode() const {
	return _data.activeRenderMode;
}

void Viewport::setActive(bool active) {
	_data.active = active;
}

bool Viewport::isActive() const {
	return _data.active;
}

Result Viewport::setSceneResources(RasterCore::SharedGpuResources* resources, const std::string& sceneName) {
	if (resources == _sceneResources && sceneName == _sceneName)
		return Result::ok();

	
	if (_gpu)
		vkDeviceWaitIdle(_gpu->device);

	_pipeline.reset();
	_sceneResources = resources;
	_sceneName = sceneName;

	if (_sceneResources && !initPipeline())
		return Result::error("Viewport '" + _data.name + "': failed to bind scene '" + _sceneName + "'");

	return Result::ok();
}

RasterCore::SharedGpuResources* Viewport::getSceneResources() const {
	return _sceneResources;
}

const std::string& Viewport::getSceneName() const {
	return _sceneName;
}

bool Viewport::hasScene() const {
	return _sceneResources != nullptr;
}

Result Viewport::resize(uint32_t width, uint32_t height) {
	if (width == 0 || height == 0)
		return Result::error("Viewport '" + _data.name + "': invalid resize dimensions");

	if (width == _data.width && height == _data.height)
		return Result::ok();

	_data.width = width;
	_data.height = height;

	if (!_pipeline) {
		if (_sceneResources && !initPipeline())
			return Result::error("Viewport '" + _data.name + "': failed to create pipeline");
		return Result::ok();
	}

	std::string error;
	if (!_pipeline->resize(width, height, &error))
		return Result::error("Viewport '" + _data.name + "': failed to resize pipeline: " + error);

	return Result::ok();
}

uint32_t Viewport::getWidth() const {
	return _data.width;
}

uint32_t Viewport::getHeight() const {
	return _data.height;
}

void Viewport::render() {
	if (!_data.active || !_pipeline)
		return;

	_pipeline->setCamera(_data.camera);
	_pipeline->drawFrame();
}

void Viewport::setCamera(const RasterCore::Camera& camera) {
	_data.camera = camera;
}

RasterCore::Camera& Viewport::getCamera() {
	return _data.camera;
}

RasterCore::RasterPipeline* Viewport::getPipeline() const {
	return _pipeline.get();
}

VkImageView Viewport::getColorImageView() const {
	if (!_pipeline)
		return VK_NULL_HANDLE;
	return static_cast<VkImageView>(_pipeline->getColorImageView());
}
