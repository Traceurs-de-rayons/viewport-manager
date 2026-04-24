#include "viewport.hpp"
#include "return.hpp"
#include "rasterCore.hpp"

#include <iostream>

Viewport::Viewport(ViewportData& data) : _data(data)
{
	// Le constructeur est maintenant correctement formaté et
	// utilise une liste d'initialisation pour _data.
}

Viewport::~Viewport()
{
	// Définition du destructeur qui était déclaré dans le .hpp
}

uint32_t Viewport::getId() const {
	return _data.id;
}

const std::string& Viewport::getName() const {
	return _data.name;
}

Result Viewport::setRenderMode(ViewportRenderMode mode) {
	// À implémenter: vérifier si le mode est valide
	_data.activeRenderMode = mode;
	return ((Result){.code = ResultCode::Ok});
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