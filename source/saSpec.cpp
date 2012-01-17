#include "saSpec.h"

bool SetVertexAttribPointer(int attribut, int position, int stride)
{
	glVertexAttribPointer(attribut, sysattribs[attribut].components, sysattribs[attribut].format, sysattribs[attribut].normalized,
		stride, reinterpret_cast<const GLvoid*>(position));

	glEnableVertexAttribArray(attribut);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

bool SetShaderSystemAttribLocation(int attribut)
{
	return false;
}