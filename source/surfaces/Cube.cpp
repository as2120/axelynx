#include <axelynx/StandartSurfaces.h>
#include "../CSurface.h"

#include <cmath>
AXELYNX_API axelynx::Surface * axelynx::StandartSurfaces::Cube(float size)
{
	using namespace axelynx;

	CSurface *surf = new CSurface(24,36);
	surf->Lock();
//нижн€€ грань
	surf->SetVertexPosition(0,-size,-size,-size);
	surf->SetVertexPosition(1, size,-size,-size);
	surf->SetVertexPosition(2,-size,-size, size);
	surf->SetVertexPosition(3, size,-size, size);

	surf->SetVertexTexCoord(0,vec2(0,0));
	surf->SetVertexTexCoord(1,vec2(1,0));
	surf->SetVertexTexCoord(2,vec2(0,1));
	surf->SetVertexTexCoord(3,vec2(1,1));

	surf->SetVertexNormal(0,0,-1,0);
	surf->SetVertexNormal(1,0,-1,0);
	surf->SetVertexNormal(2,0,-1,0);
	surf->SetVertexNormal(3,0,-1,0);

	//верхн€€ грань
	surf->SetVertexPosition(4,-size, size,-size);
	surf->SetVertexPosition(5, size, size,-size);
	surf->SetVertexPosition(6,-size, size, size);
	surf->SetVertexPosition(7, size, size, size);

	surf->SetVertexTexCoord(4,vec2(0,1));
	surf->SetVertexTexCoord(5,vec2(1,1));
	surf->SetVertexTexCoord(6,vec2(0,0));
	surf->SetVertexTexCoord(7,vec2(1,0));

	surf->SetVertexNormal(4,0,1,0);
	surf->SetVertexNormal(5,0,1,0);
	surf->SetVertexNormal(6,0,1,0);
	surf->SetVertexNormal(7,0,1,0);

	//передн€€ грань
	surf->SetVertexPosition(8,-size,-size,-size);
	surf->SetVertexPosition(9, size,-size,-size);
	surf->SetVertexPosition(10,-size, size,-size);
	surf->SetVertexPosition(11, size, size,-size);

	surf->SetVertexTexCoord(8,vec2(1,0));
	surf->SetVertexTexCoord(9,vec2(0,0));
	surf->SetVertexTexCoord(10,vec2(1,1));
	surf->SetVertexTexCoord(11,vec2(0,1));

	surf->SetVertexNormal(8,0,0,-1);
	surf->SetVertexNormal(9,0,0,-1);
	surf->SetVertexNormal(10,0,0,-1);
	surf->SetVertexNormal(11,0,0,-1);

	//задн€€ грань
	surf->SetVertexPosition(12,-size,-size, size);
	surf->SetVertexPosition(13, size,-size, size);
	surf->SetVertexPosition(14,-size, size, size);
	surf->SetVertexPosition(15, size, size, size);

	surf->SetVertexTexCoord(12,vec2(0,0));
	surf->SetVertexTexCoord(13,vec2(1,0));
	surf->SetVertexTexCoord(14,vec2(0,1));
	surf->SetVertexTexCoord(15,vec2(1,1));

	surf->SetVertexNormal(12,0,0,1);
	surf->SetVertexNormal(13,0,0,1);
	surf->SetVertexNormal(14,0,0,1);
	surf->SetVertexNormal(15,0,0,1);

	//лева€ грань
	surf->SetVertexPosition(16,-size,-size,-size);
	surf->SetVertexPosition(17,-size,-size, size);
	surf->SetVertexPosition(18,-size, size,-size);
	surf->SetVertexPosition(19,-size, size, size);

	surf->SetVertexTexCoord(16,vec2(0,0));
	surf->SetVertexTexCoord(17,vec2(1,0));
	surf->SetVertexTexCoord(18,vec2(0,1));
	surf->SetVertexTexCoord(19,vec2(1,1));

	surf->SetVertexNormal(16,-1,0,0);
	surf->SetVertexNormal(17,-1,0,0);
	surf->SetVertexNormal(18,-1,0,0);
	surf->SetVertexNormal(19,-1,0,0);

	//права€ грань
	surf->SetVertexPosition(20, size,-size,-size);
	surf->SetVertexPosition(21, size,-size, size);
	surf->SetVertexPosition(22, size, size,-size);
	surf->SetVertexPosition(23, size, size, size);

	surf->SetVertexTexCoord(20,vec2(1,0));
	surf->SetVertexTexCoord(21,vec2(0,0));
	surf->SetVertexTexCoord(22,vec2(1,1));
	surf->SetVertexTexCoord(23,vec2(0,1));

	surf->SetVertexNormal(20,1,0,0);
	surf->SetVertexNormal(21,1,0,0);
	surf->SetVertexNormal(22,1,0,0);
	surf->SetVertexNormal(23,1,0,0);

	for(int i=0;i<24;i++)
	{
		surf->SetVertexColor(i,vec4(1.0f,1.0f,1.0f,1.0f));
	}

	surf->SetTriangle(0, 0, 3, 2);
	surf->SetTriangle(1, 0, 1, 3);

	surf->SetTriangle(2, 4, 6, 5);
	surf->SetTriangle(3, 6, 7, 5);

	surf->SetTriangle(4, 10,11,8);
	surf->SetTriangle(5, 11,9, 8);

	surf->SetTriangle(6, 14,13,15);
	surf->SetTriangle(7, 14,12,13);

	surf->SetTriangle(8, 16,19,18);
	surf->SetTriangle(9, 17,19,16);

	surf->SetTriangle(10,23,21,22);
	surf->SetTriangle(11,21,20,22);

	surf->RecalcTangents();
	surf->UnLock();
	return surf;
}

AXELYNX_API axelynx::Surface * axelynx::StandartSurfaces::Quad(float size)
{
	return new CSurface(4,2);
}

AXELYNX_API axelynx::Surface * axelynx::StandartSurfaces::Plane(float sizex, float sizez, float tile_u, float tile_v)
{
	CSurface *surf = new CSurface(4,6);
	surf->Lock();
	surf->SetVertexPosition(0,-sizex, 0,-sizez);
	surf->SetVertexPosition(1, sizex, 0,-sizez);
	surf->SetVertexPosition(2,-sizex, 0, sizez);
	surf->SetVertexPosition(3, sizex, 0, sizez);

	surf->SetVertexTexCoord(0,vec2(0,tile_v));
	surf->SetVertexTexCoord(1,vec2(tile_u,tile_v));
	surf->SetVertexTexCoord(2,vec2(0,0));
	surf->SetVertexTexCoord(3,vec2(tile_u,0));

	surf->SetVertexNormal(0,0,1,0);
	surf->SetVertexNormal(1,0,1,0);
	surf->SetVertexNormal(2,0,1,0);
	surf->SetVertexNormal(3,0,1,0);

	surf->SetVertexColor(0,1,1,1);
	surf->SetVertexColor(1,1,1,1);
	surf->SetVertexColor(2,1,1,1);
	surf->SetVertexColor(3,1,1,1);

	surf->SetTriangle(0, 0, 2, 1);
	surf->SetTriangle(1, 2, 3, 1);
	surf->RecalcTangents();
	surf->UnLock();
	return surf;
}

AXELYNX_API axelynx::Surface * axelynx::StandartSurfaces::Grid(float sizex,float sizez, int cellsx, int cellsz,bool calc_tangents)
{
	int count_quads = cellsx * cellsz;
	int count_vertices = count_quads * 4;
	int count_indices = count_quads * 6;

	CSurface *surf = new CSurface(count_vertices,count_indices);
	surf->UseLightmap();

	surf->Lock();

	int vindex = 0;
	int triindex = 0;

	axelynx::vec2 cellsize;
	axelynx::vec2 startpos(sizex*(-0.5),sizez*(-0.5));
	cellsize.x = sizex / cellsx;
	cellsize.y = sizez / cellsz;

	for(int i = 0;i<cellsx;++i)
	{
		for(int j=0;j<cellsz;++j)
		{
			float cur_x = startpos.x + (i * cellsize.x);
			float cur_z = startpos.y + (j * cellsize.y);
			float dest_x = startpos.x + ((i+1) * cellsize.x);
			float dest_z = startpos.y + ((j+1) * cellsize.y);

			surf->SetVertexPosition(vindex + 0,cur_x, 0,cur_z);
			surf->SetVertexPosition(vindex + 1, dest_x, 0,cur_z);
			surf->SetVertexPosition(vindex + 2,cur_x, 0, dest_z);
			surf->SetVertexPosition(vindex + 3, dest_x, 0, dest_z);

			surf->SetVertexTexCoord(vindex + 0,vec2(0,1),0);
			surf->SetVertexTexCoord(vindex + 1,vec2(1,1),0);
			surf->SetVertexTexCoord(vindex + 2,vec2(0,0),0);
			surf->SetVertexTexCoord(vindex + 3,vec2(1,0),0);

			float global_u_0 = float(i) / float(cellsx);
			float global_v_0 = float(j) / float(cellsz);

			float global_u_1 = float(i+1) / float(cellsx);
			float global_v_1 = float(j+1) / float(cellsz);

			surf->SetVertexTexCoord(vindex + 0,vec2(global_u_0,global_v_1),1);
			surf->SetVertexTexCoord(vindex + 1,vec2(global_u_1,global_v_1),1);
			surf->SetVertexTexCoord(vindex + 2,vec2(global_u_0,global_v_0),1);
			surf->SetVertexTexCoord(vindex + 3,vec2(global_u_1,global_v_0),1);

			surf->SetVertexNormal(vindex + 0,0,1,0);
			surf->SetVertexNormal(vindex + 1,0,1,0);
			surf->SetVertexNormal(vindex + 2,0,1,0);
			surf->SetVertexNormal(vindex + 3,0,1,0);

			surf->SetTriangle(triindex + 0,vindex + 0,vindex + 2,vindex + 1);
			surf->SetTriangle(triindex + 1,vindex + 2,vindex + 3,vindex + 1);

			vindex += 4;
			triindex += 2;
		}
	}

	if(calc_tangents)
		surf->RecalcTangents();

	surf->UnLock();
	return surf;
}

AXELYNX_API axelynx::Surface * axelynx::StandartSurfaces::SkyDome(float size, int segments)
{
	int count_vertices = segments * segments;
	int count_indices = ((segments - 1)* (segments*2) + segments - 2) * 3;



	CSurface *surf = new CSurface(count_vertices,count_indices);
	surf->UseLightmap();
	surf->Lock();

	int vindex = 0;
	int triindex = 0;

	for(int i=0;i<segments;++i)
	{
		for(int j=0;j<segments;++j)
		{
			float y = sin(float(segments-i)/float(segments) * 1.57) * size - (size*0.5);

			float radius = cos(float(segments-i)/float(segments) * 1.57) * size;
			float x = sin(float(j)/float(segments) * 2.0 * 3.1415) * radius;
			float z = cos(float(j)/float(segments) * 2.0 * 3.1415) * radius;


			surf->SetVertexPosition(vindex,x,y,z);
			surf->SetVertexTexCoord(vindex,0.5 + x / (size * 2.0),0.5 + z / (size * 2.0),0);

			vindex++;
		}
	}

	for(int i=0;i<segments-2;++i)
	{
		surf->SetTriangle(triindex,0,i+1,i+2);
		triindex++;
	}

	for(int i=0;i<segments-1;++i)
	{
		for(int j=0;j<segments-1;++j)
		{
			surf->SetTriangle(triindex,(i*segments)+1+j,(i*segments)+j,(i*segments)+segments+j);
			triindex++;
			surf->SetTriangle(triindex,(i*segments)+1+j,(i*segments)+segments+j,(i*segments)+segments+1+j);
			triindex++;
		}

		surf->SetTriangle(triindex,(i*segments)+0,(i*segments)+segments-1,(i*segments)+(segments * 2)-1);
		triindex++;
		surf->SetTriangle(triindex,(i*segments)+0,(i*segments)+(segments * 2)-1,(i*segments)+segments);
		triindex++;
	}
	surf->UnLock();
	return surf;
}

AXELYNX_API axelynx::Surface * axelynx::StandartSurfaces::RegularGrid(int cellsx, int cellsz,bool calc_tangents)
{
	int count_vertices = cellsx * cellsz;
	int count_indices = (cellsx-1) * (cellsz-1) * 6;

	CSurface *surf = new CSurface(count_vertices,count_indices);
	surf->UseColoredVertices();

	surf->Lock();

	int vindex = 0;
	int triindex = 0;

	axelynx::vec2 cellsize;
	axelynx::vec2 startpos(-0.5,-0.5);
	cellsize.x = 1.0 / cellsx;
	cellsize.y = 1.0 / cellsz;

	for(int i = 0;i<cellsx;++i)
	{
		for(int j=0;j<cellsz;++j)
		{
			float x = startpos.x + (i * cellsize.x);
			float z = startpos.y + (j * cellsize.y);
			surf->SetVertexPosition(vindex + 0,x, 0,z);

			float u = float(i) / float(cellsx);
			float v = float(j) / float(cellsz);
			surf->SetVertexTexCoord(vindex + 0,vec2(u,v),0);

			surf->SetVertexNormal(vindex + 0,0,1,0);

			vindex ++;
		}
	}

	for(int i = 0;i<cellsx-1;++i)
	{
		for(int j=0;j<cellsz-1;++j)
		{
			surf->SetTriangle(triindex+0,(j*cellsx)+0+i,(j*cellsx)+1+i,(j*cellsx)+(cellsx)+i);
			surf->SetTriangle(triindex+1,(j*cellsx)+1+i,(j*cellsx)+(cellsx)+i,(j*cellsx)+(cellsx+1)+i);

			triindex +=2;
		}
	}

	if(calc_tangents)
		surf->RecalcTangents();

	surf->UnLock();
	return surf;
}
