

#include "CEarlyZ.h"

#include "gl/axgl.h"

bool CEarlyZ::in_pass = false;
bool CEarlyZ::on_pass = false;


void CEarlyZ::StartEarlyZPass()
{
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

	in_pass = true;
}
void CEarlyZ::EndEarlyZPass()
{
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	in_pass = false;
}

void CEarlyZ::StartRenderPass()
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	on_pass = true;
}

void CEarlyZ::EndRenderPass()
{
	on_pass = false;
}