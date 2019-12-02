#include "Engine/Locator.hpp"

IRenderer* Locator::_rendererService;
ILogger* Locator::_loggerService;
NullRenderer Locator::_nullRendererService;
NullLogger Locator::_nullLoggerService;