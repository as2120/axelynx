#pragma once

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the AXELYNX_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// AXELYNX_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include "axlib.h"

#include "Logger.h"
#include "File.h"
#include "Pak.h"
#include "Engine.h"
#include "Input.h"
#include "ImageFormat.h"
#include "math/axmath.h"
#include "AxelynxSystemShaders.h"
#include "StandartSurfaces.h"
#include "Timer.h"
#include "Shell.h"

#include "utils/axstring.h"
// This class is exported from the axelynx.dll

namespace axelynx
{
}
