#pragma once
#include "Utilities/Log.h"

class Locator {
public:
	static ILogger* GetLogger() {return _loggerService;};
	static void ProvideLogger(ILogger* loggerService) {
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

