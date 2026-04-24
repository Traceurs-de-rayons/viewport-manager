#pragma once

#include "rasterTypes.hpp"
#include "return.hpp"
#include "viewportManagerType.hpp"
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

struct	SDL_Window;

class	ViewportManager;

struct ViewportData {
	uint32_t						id;
	std::string						name;
	uint32_t						width = 0;
	uint32_t						height = 0;
	bool							active = false;
	ViewportRenderMode				activeRenderMode = ViewportRenderMode::Rasterisation;
	RasterCore::Camera				camera = {};
	ViewportManager*				viewportManager = nullptr;
	// intermediete pipeline raster
	// main pipeline raster
	// post processing raster

	// intermediate pipeline rt
	// main pipeline rt
	// post processing rt

	static ViewportData DefaultRaster() {
	    return {
	        1,
	        "Default",
	    };
	}


};

class Viewport {
	private:
		ViewportData	_data;

	public:
		Viewport(ViewportData& data);
		~Viewport();

		uint32_t		getId() const;
		const std::string& getName() const;

		Result						setRenderMode(ViewportRenderMode mode);
		ViewportRenderMode			getRenderMode() const;

		void						setActive(bool active);
		bool						isActive() const;



		// void						render(); // check (auto run)

		// raster

		// Result						se`erCamera() const;
};
