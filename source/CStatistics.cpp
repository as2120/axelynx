

#include "CStatistics.h"
#include "axelynx/Timer.h"

CStatistics * CStatistics::instance_ =0;

int CStatistics::tris_rendered=0;
int CStatistics::vertices_rendered=0;
int CStatistics::dips=0;
int CStatistics::shaders=0;
int CStatistics::textures=0;
int CStatistics::materials=0;
int CStatistics::uniforms=0;
int CStatistics::uniformsdata=0;
int CStatistics::fpsframe=0;
int CStatistics::fps=0;
int CStatistics::fpstime=0;

void CStatistics::ClearFrame()
{
	tris_rendered=0;
	vertices_rendered=0;
	dips=0;
	shaders=0;
	textures=0;
	materials=0;
	uniforms=0;
	uniformsdata=0;
	//  fps
		int t = axelynx::Timer::Millisecs();
		fpsframe++;
		if(fpstime < t - 1000)
		{
			fps      = fpsframe;
			fpsframe = 0;
			fpstime  = t; 
		}
}
