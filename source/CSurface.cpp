
#include "CSurface.h"
#include "CFile.h"
#include "CShader.h"
#include "saSpec.h"
#include "CStatistics.h"
#include <iostream>
#include <cmath>
#include <vector>

static unsigned int gl_index_type[5] = {0,GL_UNSIGNED_BYTE,GL_UNSIGNED_SHORT,0,GL_UNSIGNED_INT};

CSurface::CSurface(int count_vertices, int count_indices)
	:count_vertices_(count_vertices),count_indices_(count_indices),RemakeVBO(true)
{
	draw_mode_ = GL_TRIANGLES;

	positions= new axelynx::vec3[count_vertices_];
	normals= new axelynx::vec3[count_vertices_];
	tangents= new axelynx::vec3[count_vertices_];
	uv0 = new axelynx::vec2[count_vertices_];
	uv1 = new axelynx::vec2[count_vertices_];
	colors= new axelynx::vec4[count_vertices_];

	index_size_ = 4;
	if(count_vertices>255)
		index_size_ = 2;
	if(count_vertices>65535)
		index_size_ = 4;

	indices_ = new unsigned char[count_indices_*index_size_];
	ZeroMemory(indices_,count_indices_ * index_size_);
	vao_ =0;
	vbo_ =0;
	uabo_ =0;
	ibo_ =0;

	radius_ =0;
	use_lightmap_ = true;

	count_tris_ = 0;

	packmode_ = axelynx::Surface::VP_AUTO;
}

CSurface::CSurface()
{
	positions= 0;
	normals= 0;
	tangents= 0;
	uv0 = 0;
	uv1 = 0;
	colors= 0;


	indices_ = 0;
}

bool CSurface::Draw() const
{
	//glDisable(GL_CULL_FACE);
	OPENGL_CHECK_FOR_ERRORS();
	static GLuint current_vao_ = -1;
	//if(current_vao_ != vao_)
	{
		glBindVertexArray(vao_);
		current_vao_ = vao_;
	}

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	CStatistics::dips++;
	CStatistics::tris_rendered += count_tris_;
	CStatistics::vertices_rendered += count_vertices_;
	OPENGL_CHECK_FOR_ERRORS();
	glDrawRangeElements(GL_TRIANGLES,0,count_indices_ -1,count_indices_,gl_index_type[index_size_],0);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}


int CSurface::CountTriangles()const
{
	return 0;
}

int CSurface::CountVertices() const
{
	return 0;
}


void CSurface::Translate(const axelynx::vec3& translate)
{

}

void CSurface::Rotate(const axelynx::quat& rotate)
{

}

void CSurface::Scale(const axelynx::vec3& scale)
{
}



void CSurface::UserVertexAttribs(const char* name,signed char components,axelynx::VertexType vt, bool normalized)
{

}

int CSurface::GetUserAttributeLocation(const char* name) const
{
	return 0;
}

int CSurface::UserAttribute(int location, void *data, int count)
{
	return 0;
}


int CSurface::SetVertexPosition(int index, const axelynx::vec3& position)
{
	positions[index] = position;
	return 0;
}

int CSurface::SetVertexNormal(int index, const axelynx::vec3& normal)
{
	normals[index] = normal;
	return 0;
}

int CSurface::SetVertexTangent(int index, const axelynx::vec3& tangent)
{
	tangents[index] = tangent;
	return 0;
}

int CSurface::SetVertexTexCoord(int index, const axelynx::vec2& uv, int layer)
{
	if(layer)
		uv1[index] = uv;
	else
		uv0[index] = uv;
	return 0;
}

int CSurface::SetVertexColor(int index, const axelynx::vec4& color)
{
	return 0;
}

int CSurface::SetTriangle(int index, int index0,int index1, int index2)
{
	if(index_size_==1)
	{
		static_cast<unsigned char*>(indices_)[index*3 + 0] = index0;
		static_cast<unsigned char*>(indices_)[index*3 + 1] = index1;
		static_cast<unsigned char*>(indices_)[index*3 + 2] = index2;
	}
	if(index_size_==2)
	{
		static_cast<unsigned short*>(indices_)[index*3 + 0] = index0;
		static_cast<unsigned short*>(indices_)[index*3 + 1] = index1;
		static_cast<unsigned short*>(indices_)[index*3 + 2] = index2;
	}
	if(index_size_==4)
	{
		static_cast<unsigned int*>(indices_)[index*3 + 0] = index0;
		static_cast<unsigned int*>(indices_)[index*3 + 1] = index1;
		static_cast<unsigned int*>(indices_)[index*3 + 2] = index2;
	}
	return 0;
}

bool CSurface::Lock()
{
	return true;
}

bool CSurface::UnLock()
{
	OPENGL_CHECK_FOR_ERRORS();
	MakeVBO();
	return true;
}

CSurface::~CSurface()
{
}

CSurface* CSurface::LoadSBS(axelynx::File file) //sigel binary surface
{
	class Vertex
	{
	public:
		float u0,v0;
		float r,g,b,a;
		float nx,ny,nz;
		float x,y,z;
	};


	file.Open();

	char *label = new char[7];
	file.Read(label,6);
	label[6]=0;

	if(strcmp(label,"sgsbin")!=0)
	{
		file.Close();
		return 0;
	}

	int buffsizes[3];

	file.Read(buffsizes,12);

	int ci = buffsizes[0];
	int cv = buffsizes[1];

	CSurface *s = new CSurface(cv,ci);

	int auxbuffs = buffsizes[2];
	//GL_TRIANGLES

	file.Read(s->draw_mode_);
	file.Read(ci);
	file.Read(cv);

	int ct = ci/3;
	Vertex *v = new Vertex[cv];

	unsigned short *inds = new unsigned short[ci];

	file.Read(inds,ci*sizeof(unsigned short));
	file.Read(v,cv*sizeof(Vertex));

	for(int i=0;i<cv;++i)
	{
		s->positions[i]= axelynx::vec3(v[i].x,v[i].y,v[i].z);

		//s->positioins[i].x *= 0.01f;
		//s->positioins[i].y *= 0.01f;
		//s->positioins[i].z *= 0.01f;

		s->normals[i]= axelynx::vec3(v[i].nx,v[i].ny,v[i].nz);
		s->tangents[i]= axelynx::vec3(v[i].nx,v[i].ny,v[i].nz);
		s->uv0[i]= axelynx::vec2(v[i].u0,v[i].v0);
		s->uv0[i].y -= 1.0f;
		s->colors[i]= axelynx::vec4(v[i].r,v[i].g,v[i].b,v[i].a);
	}

	for(int i=0;i<ci;++i)
	{
		static_cast<unsigned short*>(s->indices_)[i] = inds[i];
	}

	file.Close();
	s->use_lightmap_ = false;
	s->RecalcTangents();
	s->MakeVBO();


	return s;
}

CSurface* CSurface::LoadAXS(axelynx::File file) //axelynx surface
{
	file.Open();

	int v;
	int f;
	int t0;
	int t1;
	int tf0;
	int tf1;

	file.Read(v);
	file.Read(f);
	file.Read(t0);
	file.Read(t1);
	file.Read(tf0);
	file.Read(tf1);

	std::cout<<"v:"<<v<<std::endl;
	std::cout<<"f:"<<f<<std::endl;
	std::cout<<"t0:"<<t0<<std::endl;
	std::cout<<"t1:"<<t1<<std::endl;
	std::cout<<"tf0:"<<tf0<<std::endl;
	std::cout<<"tf1:"<<tf1<<std::endl;


	CSurface *s = new CSurface(t0,tf0*3);

	int *faces = new int[f*3];
	int *tfaces0 = (tf0>0)?(new int[tf0*3]):0;
	int *tfaces1 = (tf1>0)?(new int[tf1*3]):0;

	axelynx::vec2 *uv0 = (t0>0)?(new axelynx::vec2[t0]):0;
	axelynx::vec2 *uv1 = (t1>0)?(new axelynx::vec2[t1]):0;
	axelynx::vec3 *vpos = new axelynx::vec3[v];
	axelynx::vec3 *vnorm = new axelynx::vec3[v];

	file.Read(faces,f*12);

	if(tfaces0)
		file.Read(tfaces0,f*12);
	if(tfaces1)
		file.Read(tfaces1,f*12);

	if(uv0)
		file.Read(uv0,t0*8);

	if(uv1)
		file.Read(uv1,t1*8);

	file.Read(vpos,v*12);
	file.Read(vnorm,v*12);

	for(int i=0;i<tf0;++i)
	{
		int ti = i*3;
		s->SetTriangle(i,tfaces0[ti+0],tfaces0[ti+1],tfaces0[ti+2]);
	}


	for(int i=0;i<tf0;++i)
	{
		int id = i*3;
		int vid = tfaces0[id+0];
		s->SetVertexPosition(vid,vpos[faces[id+0]]);
		s->SetVertexNormal(vid,vnorm[faces[id+0]]);
		if(uv0)
			s->SetVertexTexCoord(vid,uv0[tfaces0[id+0]],0);
		if(uv1)
			s->SetVertexTexCoord(vid,uv1[tfaces1[id+0]],1);
		vid = tfaces0[id+1];
		s->SetVertexPosition(vid,vpos[faces[id+1]]);
		s->SetVertexNormal(vid,vnorm[faces[id+1]]);
		if(uv0)
			s->SetVertexTexCoord(vid,uv0[tfaces0[id+1]],0);
		if(uv1)
			s->SetVertexTexCoord(vid,uv1[tfaces1[id+1]],1);
		vid = tfaces0[id+2];
		s->SetVertexPosition(vid,vpos[faces[id+2]]);
		s->SetVertexNormal(vid,vnorm[faces[id+2]]);
		if(uv0)
			s->SetVertexTexCoord(vid,uv0[tfaces0[id+2]],0);
		if(uv1)
			s->SetVertexTexCoord(vid,uv1[tfaces1[id+2]],1);
	}
	

	s->use_lightmap_ = true;
	s->RecalcTangents();
	s->MakeVBO();
	return s;
}

bool CSurface::MakeVBO()
{
	OPENGL_CHECK_FOR_ERRORS();
	//RecalcTangents();
	std::cout<<"MakeVBO"<<std::endl;

	if(!vao_)
		glDeleteVertexArrays(1,&vao_);

	if(!ibo_)
		glDeleteBuffers(1,&ibo_);

	if(!vbo_)
		glDeleteBuffers(1,&vbo_);

	if(!uabo_)
		glDeleteBuffers(1,&uabo_);
	OPENGL_CHECK_FOR_ERRORS();
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glGenBuffers(1, &ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	struct ColorData
	{
		unsigned char r,g,b,a;
	};

	struct LightMapUV
	{
		unsigned short u,v;
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_indices_ * index_size_, indices_, GL_STATIC_DRAW);
	OPENGL_CHECK_FOR_ERRORS();
	__declspec(align(1))
	struct Vertex
	{
		float x,y,z;
		float nx,ny,nz;
		float tx,ty,tz;
		float u0,v0;
		float u1,v1;
		float r,g,b,a;
	};

	__declspec(align(1))
	struct PackedVertex
	{
		float x,y,z;
		signed char nx,ny,nz,fake0;
		signed char tx,ty,tz,fake1;
		float u0,v0;
		union
		{
			ColorData color;
			LightMapUV lm;
		};
	};

	std::cout<<"count_vertices_ "<<count_vertices_<<std::endl;
	std::cout<<"count_indices_ "<<count_indices_<<std::endl;

	bool packedvertex = false;

	if(packmode_ == axelynx::Surface::VP_PACK)
		packedvertex = true;

	if(packmode_ == axelynx::Surface::VP_AUTO && count_vertices_>1023)
		packedvertex = true;

	OPENGL_CHECK_FOR_ERRORS();

	if(packedvertex)
	{
		std::cout<<"pack"<<std::endl;
		PackedVertex *gpu = new PackedVertex[count_vertices_];
	
		for(int i=0;i<count_vertices_;++i)
		{
			gpu[i].x=positions[i].x;
			gpu[i].y=positions[i].y;
			gpu[i].z=positions[i].z;

			gpu[i].nx = static_cast<signed char>(normals[i].x * 127.0f);
			gpu[i].ny = static_cast<signed char>(normals[i].y * 127.0f);
			gpu[i].nz = static_cast<signed char>(normals[i].z * 127.0f);

			gpu[i].tx = static_cast<signed char>(tangents[i].x * 127.0f);
			gpu[i].ty = static_cast<signed char>(tangents[i].y * 127.0f);
			gpu[i].tz = static_cast<signed char>(tangents[i].z * 127.0f);


			gpu[i].u0 =uv0[i].x;
			gpu[i].v0 = uv0[i].y;

			if(use_lightmap_)
			{
				gpu[i].lm.u = static_cast<unsigned short>(uv1[i].x);
				gpu[i].lm.v = static_cast<unsigned short>(uv1[i].y);
			}
			else
			{
				gpu[i].color.r = static_cast<unsigned char>(colors[i].x * 255.0f);
				gpu[i].color.g = static_cast<unsigned char>(colors[i].y * 255.0f);
				gpu[i].color.b = static_cast<unsigned char>(colors[i].z * 255.0f);
				gpu[i].color.a = static_cast<unsigned char>(colors[i].w * 255.0f);
			}
		}

		glBufferData(GL_ARRAY_BUFFER, count_vertices_ * sizeof(PackedVertex), gpu, GL_STATIC_DRAW);


		glVertexAttribPointer(VA_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(PackedVertex), reinterpret_cast<const GLvoid*>(0));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_POSITION, 0);
		glEnableVertexAttribArray(VA_POSITION);

		glVertexAttribPointer(VA_NORMAL, 3, GL_BYTE, GL_TRUE,sizeof(PackedVertex), reinterpret_cast<const GLvoid*>(12));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_NORMAL, 0);
		glEnableVertexAttribArray(VA_NORMAL);

		glVertexAttribPointer(VA_TANGENT, 3, GL_BYTE, GL_TRUE,sizeof(PackedVertex), reinterpret_cast<const GLvoid*>(16));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_TANGENT, 0);
		glEnableVertexAttribArray(VA_TANGENT);

		glVertexAttribPointer(VA_TEXCOORD0, 2, GL_FLOAT, GL_FALSE,sizeof(PackedVertex), reinterpret_cast<const GLvoid*>(20));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_TEXCOORD0, 0);
		glEnableVertexAttribArray(VA_TEXCOORD0);

		if(use_lightmap_)
		{
			glVertexAttribPointer(VA_TEXCOORD1, 2, GL_UNSIGNED_SHORT, GL_TRUE,sizeof(PackedVertex), reinterpret_cast<const GLvoid*>(28));
			if(glVertexAttribDivisor)
				glVertexAttribDivisor(VA_TEXCOORD1, 0);
			glEnableVertexAttribArray(VA_TEXCOORD1);
		}
		else
		{
			glVertexAttribPointer(VA_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE,sizeof(PackedVertex), reinterpret_cast<const GLvoid*>(28));
			if(glVertexAttribDivisor)
				glVertexAttribDivisor(VA_COLOR, 0);
			glEnableVertexAttribArray(VA_COLOR);
		}
	}
	else
	{
		std::cout<<"unpack"<<std::endl;
		Vertex *gpu = new Vertex[count_vertices_];
	
		for(int i=0;i<count_vertices_;++i)
		{
			gpu[i].x=positions[i].x;
			gpu[i].y=positions[i].y;
			gpu[i].z=positions[i].z;

			gpu[i].nx = normals[i].x;
			gpu[i].ny = normals[i].y;
			gpu[i].nz = normals[i].z;

			gpu[i].tx = tangents[i].x;
			gpu[i].ty = tangents[i].y;
			gpu[i].tz = tangents[i].z;


			gpu[i].u0 =uv0[i].x;
			gpu[i].v0 = uv0[i].y;

			gpu[i].u1 = uv1[i].x;
			gpu[i].v1 = uv1[i].y;

			gpu[i].r = colors[i].x;
			gpu[i].g = colors[i].y;
			gpu[i].b = colors[i].z;
			gpu[i].a = colors[i].w;
		}

		glBufferData(GL_ARRAY_BUFFER, count_vertices_ * sizeof(Vertex), gpu, GL_STATIC_DRAW);

		glVertexAttribPointer(VA_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex), reinterpret_cast<const GLvoid*>(0));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_POSITION, 0);
		glEnableVertexAttribArray(VA_POSITION);

		glVertexAttribPointer(VA_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_NORMAL, 0);
		glEnableVertexAttribArray(VA_NORMAL);

		glVertexAttribPointer(VA_TANGENT, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex), reinterpret_cast<const GLvoid*>(24));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_TANGENT, 0);
		glEnableVertexAttribArray(VA_TANGENT);

		glVertexAttribPointer(VA_TEXCOORD0, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex), reinterpret_cast<const GLvoid*>(36));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_TEXCOORD0, 0);
		glEnableVertexAttribArray(VA_TEXCOORD0);

		glVertexAttribPointer(VA_TEXCOORD1, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex), reinterpret_cast<const GLvoid*>(44));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_TEXCOORD1, 0);
		glEnableVertexAttribArray(VA_TEXCOORD1);

		glVertexAttribPointer(VA_COLOR, 4, GL_FLOAT, GL_FALSE,sizeof(Vertex), reinterpret_cast<const GLvoid*>(52));
		if(glVertexAttribDivisor)
			glVertexAttribDivisor(VA_COLOR, 0);
		glEnableVertexAttribArray(VA_COLOR);
	}

	count_tris_ = count_indices_ / 3;
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}


void CalcTriangleBasis( const axelynx::vec3& E, const axelynx::vec3& F, const axelynx::vec3& G, float sE,
		float tE, float sF, float tF, float sG, float tG, axelynx::vec3& tangentX,
		axelynx::vec3& tangentY )
{
	using namespace axelynx;

	vec3 P = F - E;
	vec3 Q = G - E;
	float s1 = sF - sE;
	float t1 = tF - tE;
	float s2 = sG - sE;
	float t2 = tG - tE;
	float pqMatrix[2][3];
	pqMatrix[0][0] = P[0];
	pqMatrix[0][1] = P[1];
	pqMatrix[0][2] = P[2];
	pqMatrix[1][0] = Q[0];
	pqMatrix[1][1] = Q[1];
	pqMatrix[1][2] = Q[2];
	float temp = 1.0f / ( s1 * t2 - s2 * t1);
	float stMatrix[2][2];
	stMatrix[0][0] =  t2 * temp;
	stMatrix[0][1] = -t1 * temp;
	stMatrix[1][0] = -s2 * temp;
	stMatrix[1][1] =  s1 * temp;
	float tbMatrix[2][3];
	// stMatrix * pqMatrix
	tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
	tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
	tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
	tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
	tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
	tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];

	tangentX.x =  tbMatrix[0][0];
	tangentX.y =  tbMatrix[0][1];
	tangentX.z =  tbMatrix[0][2];


	tangentY.x = tbMatrix[1][0];
	tangentY.y = tbMatrix[1][1];
	tangentY.z = tbMatrix[1][2];
	tangentX.normalize();
	tangentY.normalize();
}

axelynx::vec3 ClosestPointOnLine( const axelynx::vec3& a, const axelynx::vec3& b, const axelynx::vec3& p )
{
	using namespace axelynx;

	vec3 c = p - a;
	vec3 V = b - a;
	float d = V.length();
	V.normalize();
	float t = V.dot(c);		// скалярное произведение векторов
 
	// проверка на выход за границы отрезка
	if ( t < 0.0f )
		return a;
	if ( t > d )
		return b;
 
	// Вернем точку между a и b
	V *= t;
	return ( a + V );
}

axelynx::vec3 Ortogonalize( const axelynx::vec3& v1, const axelynx::vec3& v2 )
{
	using namespace axelynx;

	vec3 v2ProjV1 = ClosestPointOnLine( v1, -v1, v2 );
	vec3 res = v2 - v2ProjV1;
	res.normalize();
	return res;
}

bool CSurface::RecalcTangents()
{
	using namespace axelynx; //for vec3

	register int i, j;
	std::vector<vec3> ntangents;	// касательные вектора к треугольнику
 
	// проходимся по всем треугольникам, и для каждого считаем базис
	for ( i = 0; i < count_indices_; i+=3 )
	{
		int ind0 = _getIndex( i + 0);
		int ind1 = _getIndex(i + 1);
		int ind2 = _getIndex(i + 2);
 
		vec3 v1 = positions[ind0];
		vec3 v2 = positions[ind1];
		vec3 v3 = positions[ind2];

		float s1      = uv0[ind0].x;
		float t1      = uv0[ind0].y;
		float s2      = uv0[ind1].x;
		float t2      = uv0[ind1].y;
		float s3      = uv0[ind2].x;
		float t3      = uv0[ind2].y;
 
		vec3  t, b;
		CalcTriangleBasis( v1, v2, v3, s1, t1, s2, t2, s3, t3, t, b );

		ntangents.push_back( t );
	}
 
	// теперь пройдемся по всем вершинам, для каждой из них найдем
	// грани ее содержащие, и запишем все это хозяйство на будущее =)
	
	for ( i = 0; i < count_vertices_; i++ )
	{
		std::vector<vec3> rt;

		for ( j = 0; j < count_indices_; j+=3 )
		{
			if ( _getIndex( j + 0 ) == i || _getIndex( j + 1) == i || _getIndex( j + 2 ) == i )
			{
				// нашли грань которой эта вершина принадлежит.
				// добавим вектора этой грани в наш массив
				rt.push_back( ntangents[ j / 3 ] );
			}
		}
 
		// все прилежащие вектора нашли, теперь просуммируем их
		// и разделим на их количество, т.е. усредним.
		vec3 tangentRes( 0, 0, 0 );
		for ( j = 0; j < static_cast<int>(rt.size()); j++ )
		{
			tangentRes += rt[ j ];
		}

		tangentRes *= (1.0f/float( rt.size() ));
 
		// а теперь то, о чем многие забывают. Как мы помним,
		// TBN базис представляет собой всего-навсего систему координат.
		// поэтому все три направляющие вектора этого базиса
		// обязаны быть попарно перпендикулярны. Вот об этом мы
		// сейчас и побеспокоимся, выполнив ортогонализацию
		// векторов методом Грама-Шмидта
 
		tangentRes = Ortogonalize( normals[ i ], tangentRes );
 
		// вот и все, теперь просто запишем результат
 
		tangents[i] =  tangentRes;
	}

	return true;
}

float CSurface::GetRadius() const
{
	if(radius_ < 0.001f)
	{
		float mqr =0;

		for(int i=0;i<count_vertices_;++i)
		{
			const axelynx::vec3 &pos= positions[i];
			float qr = pos.x*pos.x + pos.y*pos.y +pos.z * pos.z;
			if(qr>mqr)
				mqr = qr;
		}

		radius_ = sqrtf(mqr);
	}

	return radius_;
}

bool CSurface::SetVertexPackMode(const axelynx::Surface::VertexPackMode &pm)
{
	packmode_ = pm;
	return true;
}

int CSurface::GetBinarySize() const
{
	int size = 0;

	size+= sizeof(draw_mode_);
	size+=sizeof(count_vertices_);
	size+=sizeof(count_indices_);
	size+=sizeof(radius_);
	size+=sizeof(use_lightmap_);
	size+=sizeof(count_tris_);
	size+=sizeof(packmode_);

	size+=sizeof(count_vertices_ * sizeof(axelynx::vec3));
	size+=sizeof(count_vertices_* sizeof(axelynx::vec3));
	size+=sizeof(count_vertices_* sizeof(axelynx::vec3));
	size+=sizeof(count_vertices_* sizeof(axelynx::vec2));
	size+=sizeof(count_vertices_* sizeof(axelynx::vec2));
	size+=sizeof(count_vertices_* sizeof(axelynx::vec4));

	size+=sizeof(indices_,count_indices_ * sizeof(int));

	return size;
}

int CSurface::WriteBinary(axelynx::File file) const
{
	//file.Create();

	file.Write(draw_mode_);
	file.Write(count_vertices_);
	file.Write(count_indices_);
	file.Write(radius_);
	file.Write(use_lightmap_);
	file.Write(count_tris_);
	file.Write(packmode_);

	file.Write(positions,count_vertices_ * sizeof(axelynx::vec3));
	file.Write(normals,count_vertices_* sizeof(axelynx::vec3));
	file.Write(tangents,count_vertices_* sizeof(axelynx::vec3));
	file.Write(uv0,count_vertices_* sizeof(axelynx::vec2));
	file.Write(uv1,count_vertices_* sizeof(axelynx::vec2));
	file.Write(colors,count_vertices_* sizeof(axelynx::vec4));

	file.Write(indices_,count_indices_ * sizeof(int));

	//file.Close();
	return 0;
}

int CSurface::RestoreFromBinary(axelynx::File file)
{
	file.Read(draw_mode_);
	file.Read(count_vertices_);
	file.Read(count_indices_);
	file.Read(radius_);
	file.Read(use_lightmap_);
	file.Read(count_tris_);
	file.Read(packmode_);

	if(!positions)
		delete[] positions;
	if(!normals)
		delete[] normals;
	if(!tangents)
		delete[] tangents;
	if(!uv0)
		delete[] uv0;
	if(!uv1)
		delete[] uv1;
	if(!colors)
		delete[] colors;
	if(!indices_)
		delete[] indices_;

	positions = new axelynx::vec3[count_vertices_];
	normals = new axelynx::vec3[count_vertices_];
	tangents = new axelynx::vec3[count_vertices_];
	uv0 = new axelynx::vec2[count_vertices_];
	uv1 = new axelynx::vec2[count_vertices_];
	colors = new axelynx::vec4[count_vertices_];

	indices_ = new unsigned int[count_indices_];

	file.Read(positions,count_vertices_ * sizeof(axelynx::vec3));
	file.Read(normals,count_vertices_* sizeof(axelynx::vec3));
	file.Read(tangents,count_vertices_* sizeof(axelynx::vec3));
	file.Read(uv0,count_vertices_* sizeof(axelynx::vec2));
	file.Read(uv1,count_vertices_* sizeof(axelynx::vec2));
	file.Read(colors,count_vertices_* sizeof(axelynx::vec4));

	file.Read(indices_,count_indices_ * sizeof(int));

	MakeVBO();
	return 0;
}