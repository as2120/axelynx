// axelynxclr.h

#pragma once

#include <axelynx/axelynx.h>
#include <vcclr.h>

using namespace System;


namespace Axelynx {
	int const VERSION = axelynx::AXELYNX_VERSION;
	#include "interface/clrMath.h"
	#include "interface/clrFont.h"
	#include "interface/clrCanvas.h"
	#include "interface/clrWindow.h"
	#include "interface/clrEngine.h"
}
