#pragma once
#include "Utilities/Log.h"
#include "Render/Render.h"

class Locator {
public:
	static IRenderer* getRenderer() {return _rendererService;};
	static ILogger* getLogger() {return _loggerService;};
	static void provideRenderer(IRenderer* rendererService) {
		if (rendererService != NULL) {
			_rendererService = rendererService;
		} else {
			_rendererService = &_nullRendererService;
		}
	};
	static void provideLogger(ILogger* loggerService) {
		if (loggerService != NULL) {
			_loggerService = loggerService;
		} else {
			_loggerService = &_nullLoggerService;
		}
	};
private:
	static IRenderer* _rendererService;
	static ILogger* _loggerService;
	static NullRenderer _nullRendererService;
	static NullLogger _nullLoggerService;
};

// IRenderer* Locator::_rendererService;
// ILogger* Locator::_loggerService;
// NullRenderer Locator::_nullRendererService;
// NullLogger Locator::_nullLoggerService;