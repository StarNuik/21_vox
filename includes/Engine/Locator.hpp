#pragma once
#include "Utilities/Log.h"
#include "Render/Render.h"

class Locator {
public:
	static ILogger* getLogger() {return _loggerService;};
	static void provideLogger(ILogger* loggerService) {
		if (loggerService != NULL) {
			_loggerService = loggerService;
		} else {
			_loggerService = &_nullLoggerService;
		}
	};
private:
	static ILogger* _loggerService;
	static NullLogger _nullLoggerService;
};

