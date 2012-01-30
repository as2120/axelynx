

#include "CCamera.h"
#include "CCanvas.h"
#include "gl/axgl.h"
#include <cmath>
#include <iostream>
#include "CScene.h"
#include <math.h>

const CCamera * CCamera::current_ =0;

CCamera::CCamera()
{
	CCanvas * c = CCanvas::Instance();

	float aspect_ratio = static_cast<float>(c->GetWidth()) / static_cast<float>(c->GetHeight());

	Projection(45.0f,aspect_ratio,1.0f,1000.0f);
	Viewport(0,0,c->GetWidth(),c->GetHeight());
	ccm_  = 1;

	ccr_ = 0;
	ccg_ = 0;
	ccb_ = 0;
	cca_ = 1;

	need_recalc_ = true;

	ClearColorMode(true);
	ClearDepthMode(true);

	drawmode_ =  axelynx::Camera::DM_SOLID;
}

const axelynx::mat4& CCamera::GetViewMatrix() const
{
	if(isMustRecalcTM())
	{
		axelynx::mat4 rotation = this->GetTransformMatrix();
		rotation(3,0) = 0;
		rotation(3,1) = 0;
		rotation(3,2) = 0;

		axelynx::mat4 translate = axelynx::mat4::Identity();
		translate(3,0) = -this->GetPosition().x;
		translate(3,1) = -this->GetPosition().y;
		translate(3,2) = -this->GetPosition().z;

		view_ = translate * rotation;
	}

	return view_;
}


const axelynx::mat4& CCamera::GetProjectionMatrix() const
{
	return projection_;
}

const axelynx::mat4& CCamera::GetViewProjectionMatrix() const
{
	if(need_recalc_ || isMustRecalcTM())
	{
		viewprojection_ = GetViewMatrix() * projection_;
		need_recalc_ = false;
	}
	return viewprojection_;
}

axelynx::frustum CCamera::GetFrustum() const
{
	axelynx::frustum frustum;

   float   clip[16];
   float   t;

   axelynx::mat4 view = GetViewMatrix();
   axelynx::mat4 projection = GetProjectionMatrix();

   /* Комбинируем матрицы(перемножаем) */
   clip[ 0] = view[ 0] * projection[ 0] + view[ 1] * projection[ 4] + view[ 2] * projection[ 8] + view[ 3] * projection[12];
   clip[ 1] = view[ 0] * projection[ 1] + view[ 1] * projection[ 5] + view[ 2] * projection[ 9] + view[ 3] * projection[13];
   clip[ 2] = view[ 0] * projection[ 2] + view[ 1] * projection[ 6] + view[ 2] * projection[10] + view[ 3] * projection[14];
   clip[ 3] = view[ 0] * projection[ 3] + view[ 1] * projection[ 7] + view[ 2] * projection[11] + view[ 3] * projection[15];

   clip[ 4] = view[ 4] * projection[ 0] + view[ 5] * projection[ 4] + view[ 6] * projection[ 8] + view[ 7] * projection[12];
   clip[ 5] = view[ 4] * projection[ 1] + view[ 5] * projection[ 5] + view[ 6] * projection[ 9] + view[ 7] * projection[13];
   clip[ 6] = view[ 4] * projection[ 2] + view[ 5] * projection[ 6] + view[ 6] * projection[10] + view[ 7] * projection[14];
   clip[ 7] = view[ 4] * projection[ 3] + view[ 5] * projection[ 7] + view[ 6] * projection[11] + view[ 7] * projection[15];

   clip[ 8] = view[ 8] * projection[ 0] + view[ 9] * projection[ 4] + view[10] * projection[ 8] + view[11] * projection[12];
   clip[ 9] = view[ 8] * projection[ 1] + view[ 9] * projection[ 5] + view[10] * projection[ 9] + view[11] * projection[13];
   clip[10] = view[ 8] * projection[ 2] + view[ 9] * projection[ 6] + view[10] * projection[10] + view[11] * projection[14];
   clip[11] = view[ 8] * projection[ 3] + view[ 9] * projection[ 7] + view[10] * projection[11] + view[11] * projection[15];

   clip[12] = view[12] * projection[ 0] + view[13] * projection[ 4] + view[14] * projection[ 8] + view[15] * projection[12];
   clip[13] = view[12] * projection[ 1] + view[13] * projection[ 5] + view[14] * projection[ 9] + view[15] * projection[13];
   clip[14] = view[12] * projection[ 2] + view[13] * projection[ 6] + view[14] * projection[10] + view[15] * projection[14];
   clip[15] = view[12] * projection[ 3] + view[13] * projection[ 7] + view[14] * projection[11] + view[15] * projection[15];

   /* Находим A, B, C, D для ПРАВОЙ плоскости */
   frustum(0,0) = clip[ 3] - clip[ 0];
   frustum(0,1) = clip[ 7] - clip[ 4];
   frustum(0,2) = clip[11] - clip[ 8];
   frustum(0,3) = clip[15] - clip[12];

   /* Приводим уравнение плоскости к нормальному виду */
   t = sqrt( frustum(0,0) * frustum(0,0) + frustum(0,1) * frustum(0,1) + frustum(0,2) * frustum(0,2));
   frustum(0,0) /= t;
   frustum(0,1) /= t;
   frustum(0,2) /= t;
   frustum(0,3) /= t;

   /* Находим A, B, C, D для ЛЕВОЙ плоскости */
   frustum(1,0) = clip[ 3] + clip[ 0];
   frustum(1,1) = clip[ 7] + clip[ 4];
   frustum(1,2) = clip[11] + clip[ 8];
   frustum(1,3) = clip[15] + clip[12];

   /* Приводим уравнение плоскости к нормальному виду */
   t = sqrt( frustum(1,0) * frustum(1,0) + frustum(1,1) * frustum(1,1) + frustum(1,2) * frustum(1,2) );
   frustum(1,0) /= t;
   frustum(1,1) /= t;
   frustum(1,2) /= t;
   frustum(1,3) /= t;

   /* Находим A, B, C, D для НИЖНЕЙ плоскости */
   frustum(2,0) = clip[ 3] + clip[ 1];
   frustum(2,1) = clip[ 7] + clip[ 5];
   frustum(2,2) = clip[11] + clip[ 9];
   frustum(2,3) = clip[15] + clip[13];

   /* Приводим уравнение плоскости к нормальному */
   t = sqrt( frustum(2,0) * frustum(2,0) + frustum(2,1) * frustum(2,1) + frustum(2,2) * frustum(2,2) );

   frustum(2,0) /= t;
   frustum(2,1) /= t;
   frustum(2,2) /= t;
   frustum(2,3) /= t;

   /* ВЕРХНЯЯ плоскость */
   frustum(3,0) = clip[ 3] - clip[ 1];
   frustum(3,1) = clip[ 7] - clip[ 5];
   frustum(3,2) = clip[11] - clip[ 9];
   frustum(3,3) = clip[15] - clip[13];

   /* Нормальный вид */
   t = sqrt( frustum(3,0) * frustum(3,0) + frustum(3,1) * frustum(3,1) + frustum(3,2) * frustum(3,2) );

   frustum(3,0) /= t;
   frustum(3,1) /= t;
   frustum(3,2) /= t;
   frustum(3,3) /= t;

   /* ЗАДНЯЯ плоскость */
   frustum(4,0) = clip[ 3] - clip[ 2];
   frustum(4,1) = clip[ 7] - clip[ 6];
   frustum(4,2) = clip[11] - clip[10];
   frustum(4,3) = clip[15] - clip[14];

   /* Нормальный вид */
   t = sqrt( frustum(4,0) * frustum(4,0) + frustum(4,1) * frustum(4,1) + frustum(4,2) * frustum(4,2) );

   frustum(4,0) /= t;
   frustum(4,1) /= t;
   frustum(4,2) /= t;
   frustum(4,3) /= t;

   /* ПЕРЕДНЯЯ плоскость */
   frustum(5,0) = clip[ 3] + clip[ 2];
   frustum(5,1) = clip[ 7] + clip[ 6];
   frustum(5,2) = clip[11] + clip[10];
   frustum(5,3) = clip[15] + clip[14];

   /* Нормальный вид */
   t = sqrt( frustum(5,0) * frustum(5,0) + frustum(5,1) * frustum(5,1) + frustum(5,2) * frustum(5,2) );
   frustum(5,0) /= t;
   frustum(5,1) /= t;
   frustum(5,2) /= t;
   frustum(5,3) /= t;

   return frustum;
}

void CCamera::Viewport(int x1, int y1, int x2, int y2)
{
	x1_ = x1;
	y1_ = y1;
	x2_ = x2;
	y2_ = y2;
}

void CCamera::Projection(float fov, float aspect, float znear, float zfar)
{
	fov = fov / 180.0f * 3.1415f;
	projection_ = axelynx::mat4::Perspective(fov,aspect,znear,zfar);
}

void CCamera::Ortho(float xnear,float xfar, float ynear, float yfar, float znear, float zfar)
{
	projection_ = axelynx::mat4::Ortho(xnear,xfar,ynear,yfar,znear,zfar);
}

CCamera::~CCamera()
{
}

void CCamera::Range(float znear, float zfar)
{
	znear_ = znear;
	zfar_ = zfar;
}

void CCamera::ClearColorMode(bool mode)
{
	ccm_ = mode;
}

void CCamera::ClearDepthMode(bool mode)
{
	cdm_ = mode;
}

void CCamera::ClearColor(float r, float g, float b, float a)
{
	ccr_ = r;
	ccg_ = g;
	ccb_ = b;
	cca_ = a;
}

void CCamera::FOV(float fov)
{
	fov_ = fov;
}

bool CCamera::Bind(int force_clear_depth) const
{
	//std::cout<<"x1:"<<x1_<<"y1:"<<y1_<<"x2:"<<x2_<<"y2:"<<y2_<<std::endl;
	OPENGL_CHECK_FOR_ERRORS();
	glViewport(x1_,y1_,x2_-x1_,y2_-y1_);
	glEnable(GL_SCISSOR_TEST);
	glScissor(x1_,y1_,x2_-x1_,y2_-y1_);
	OPENGL_CHECK_FOR_ERRORS();	
	glClearColor(ccr_, ccg_, ccb_,cca_);
	glClearDepth(1.0f);
	glDepthRange(0.0f,1.0f);
	OPENGL_CHECK_FOR_ERRORS();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	OPENGL_CHECK_FOR_ERRORS();
	bool cdm = cdm_;
	if(force_clear_depth<0)
		cdm = false;
	if(force_clear_depth>0)
		cdm = true;
	if(cdm)
		glDepthMask(true);
	OPENGL_CHECK_FOR_ERRORS();
	if(ccm_ || cdm)
		glClear((GL_COLOR_BUFFER_BIT * ccm_) | (GL_DEPTH_BUFFER_BIT * cdm));
	OPENGL_CHECK_FOR_ERRORS();
	glDisable(GL_SCISSOR_TEST);
	need_recalc_ = true;
	OPENGL_CHECK_FOR_ERRORS();
	//glEnable(GL_POLYGON_MODE);
	//OPENGL_CHECK_FOR_ERRORS();
	switch(drawmode_)
	{
	case axelynx::Camera::DM_SOLID:
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;
	case axelynx::Camera::DM_WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		break;
	case axelynx::Camera::DM_POINTS:
		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
		break;
	default:
		assert(!"unknow mode");
	};
	OPENGL_CHECK_FOR_ERRORS();
	current_ = this;
	return true;
}

bool CCamera::UnBind() const
{
	current_ = 0;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	return true;
}

void MultiplyMatrices4by4OpenGL_FLOAT(float *result, const float *matrix1, const float *matrix2)
  {
    result[0]=matrix1[0]*matrix2[0]+
      matrix1[4]*matrix2[1]+
      matrix1[8]*matrix2[2]+
      matrix1[12]*matrix2[3];
    result[4]=matrix1[0]*matrix2[4]+
      matrix1[4]*matrix2[5]+
      matrix1[8]*matrix2[6]+
      matrix1[12]*matrix2[7];
    result[8]=matrix1[0]*matrix2[8]+
      matrix1[4]*matrix2[9]+
      matrix1[8]*matrix2[10]+
      matrix1[12]*matrix2[11];
    result[12]=matrix1[0]*matrix2[12]+
      matrix1[4]*matrix2[13]+
      matrix1[8]*matrix2[14]+
      matrix1[12]*matrix2[15];
    result[1]=matrix1[1]*matrix2[0]+
      matrix1[5]*matrix2[1]+
      matrix1[9]*matrix2[2]+
      matrix1[13]*matrix2[3];
    result[5]=matrix1[1]*matrix2[4]+
      matrix1[5]*matrix2[5]+
      matrix1[9]*matrix2[6]+
      matrix1[13]*matrix2[7];
    result[9]=matrix1[1]*matrix2[8]+
      matrix1[5]*matrix2[9]+
      matrix1[9]*matrix2[10]+
      matrix1[13]*matrix2[11];
    result[13]=matrix1[1]*matrix2[12]+
      matrix1[5]*matrix2[13]+
      matrix1[9]*matrix2[14]+
      matrix1[13]*matrix2[15];
    result[2]=matrix1[2]*matrix2[0]+
      matrix1[6]*matrix2[1]+
      matrix1[10]*matrix2[2]+
      matrix1[14]*matrix2[3];
    result[6]=matrix1[2]*matrix2[4]+
      matrix1[6]*matrix2[5]+
      matrix1[10]*matrix2[6]+
      matrix1[14]*matrix2[7];
    result[10]=matrix1[2]*matrix2[8]+
      matrix1[6]*matrix2[9]+
      matrix1[10]*matrix2[10]+
      matrix1[14]*matrix2[11];
    result[14]=matrix1[2]*matrix2[12]+
      matrix1[6]*matrix2[13]+
      matrix1[10]*matrix2[14]+
      matrix1[14]*matrix2[15];
    result[3]=matrix1[3]*matrix2[0]+
      matrix1[7]*matrix2[1]+
      matrix1[11]*matrix2[2]+
      matrix1[15]*matrix2[3];
    result[7]=matrix1[3]*matrix2[4]+
      matrix1[7]*matrix2[5]+
      matrix1[11]*matrix2[6]+
      matrix1[15]*matrix2[7];
    result[11]=matrix1[3]*matrix2[8]+
      matrix1[7]*matrix2[9]+
      matrix1[11]*matrix2[10]+
      matrix1[15]*matrix2[11];
    result[15]=matrix1[3]*matrix2[12]+
      matrix1[7]*matrix2[13]+
      matrix1[11]*matrix2[14]+
      matrix1[15]*matrix2[15];
  }
 
  void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector)
  {
    resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
    resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
    resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
    resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
  }
 
#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]
 
  //This code comes directly from GLU except that it is for float
  int glhInvertMatrixf2(float *m, float *out)
  {
   float wtmp[4][8];
   float m0, m1, m2, m3, s;
   float *r0, *r1, *r2, *r3;
   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
   r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
      r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
      r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
      r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
      r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
      r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
      r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
      r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
      r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
      r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
      r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
      r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
   /* choose pivot - or die */
   if (fabs(r3[0]) > fabs(r2[0]))
      SWAP_ROWS_FLOAT(r3, r2);
   if (fabs(r2[0]) > fabs(r1[0]))
      SWAP_ROWS_FLOAT(r2, r1);
   if (fabs(r1[0]) > fabs(r0[0]))
      SWAP_ROWS_FLOAT(r1, r0);
   if (0.0 == r0[0])
      return 0;
   /* eliminate first variable     */
   m1 = r1[0] / r0[0];
   m2 = r2[0] / r0[0];
   m3 = r3[0] / r0[0];
   s = r0[1];
   r1[1] -= m1 * s;
   r2[1] -= m2 * s;
   r3[1] -= m3 * s;
   s = r0[2];
   r1[2] -= m1 * s;
   r2[2] -= m2 * s;
   r3[2] -= m3 * s;
   s = r0[3];
   r1[3] -= m1 * s;
   r2[3] -= m2 * s;
   r3[3] -= m3 * s;
   s = r0[4];
   if (s != 0.0) {
      r1[4] -= m1 * s;
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r0[5];
   if (s != 0.0) {
      r1[5] -= m1 * s;
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r0[6];
   if (s != 0.0) {
      r1[6] -= m1 * s;
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r0[7];
   if (s != 0.0) {
      r1[7] -= m1 * s;
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }
   /* choose pivot - or die */
   if (fabs(r3[1]) > fabs(r2[1]))
      SWAP_ROWS_FLOAT(r3, r2);
   if (fabs(r2[1]) > fabs(r1[1]))
      SWAP_ROWS_FLOAT(r2, r1);
   if (0.0 == r1[1])
      return 0;
   /* eliminate second variable */
   m2 = r2[1] / r1[1];
   m3 = r3[1] / r1[1];
   r2[2] -= m2 * r1[2];
   r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3];
   r3[3] -= m3 * r1[3];
   s = r1[4];
   if (0.0 != s) {
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r1[5];
   if (0.0 != s) {
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r1[6];
   if (0.0 != s) {
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r1[7];
   if (0.0 != s) {
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }
   /* choose pivot - or die */
   if (fabs(r3[2]) > fabs(r2[2]))
      SWAP_ROWS_FLOAT(r3, r2);
   if (0.0 == r2[2])
      return 0;
   /* eliminate third variable */
   m3 = r3[2] / r2[2];
   r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
      r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
   /* last check */
   if (0.0 == r3[3])
      return 0;
   s = 1.0f / r3[3];		/* now back substitute row 3 */
   r3[4] *= s;
   r3[5] *= s;
   r3[6] *= s;
   r3[7] *= s;
   m2 = r2[3];			/* now back substitute row 2 */
   s = 1.0f / r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
      r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
      r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
      r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
   m1 = r1[2];			/* now back substitute row 1 */
   s = 1.0f / r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
      r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
      r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
   m0 = r0[1];			/* now back substitute row 0 */
   s = 1.0f / r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
      r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
   MAT(out, 0, 0) = r0[4];
   MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
   MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
   MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
   MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
   MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
   MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
   MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
   MAT(out, 3, 3) = r3[7];
   return 1;
  }

int glhProjectf(float objx, float objy, float objz, const float *modelview, const float *projection, const int *viewport, float *windowCoordinate)
{
    //Transformation vectors
    float fTempo[8];
    //Modelview transform
    fTempo[0]=modelview[0]*objx+modelview[4]*objy+modelview[8]*objz+modelview[12];  //w is always 1
    fTempo[1]=modelview[1]*objx+modelview[5]*objy+modelview[9]*objz+modelview[13];
    fTempo[2]=modelview[2]*objx+modelview[6]*objy+modelview[10]*objz+modelview[14];
    fTempo[3]=modelview[3]*objx+modelview[7]*objy+modelview[11]*objz+modelview[15];
    //Projection transform, the final row of projection matrix is always [0 0 -1 0]
    //so we optimize for that.
    fTempo[4]=projection[0]*fTempo[0]+projection[4]*fTempo[1]+projection[8]*fTempo[2]+projection[12]*fTempo[3];
    fTempo[5]=projection[1]*fTempo[0]+projection[5]*fTempo[1]+projection[9]*fTempo[2]+projection[13]*fTempo[3];
    fTempo[6]=projection[2]*fTempo[0]+projection[6]*fTempo[1]+projection[10]*fTempo[2]+projection[14]*fTempo[3];
    fTempo[7]=-fTempo[2];
    //The result normalizes between -1 and 1
    if(fTempo[7]==0.0)	//The w value
        return 0;
    fTempo[7]=1.0f/fTempo[7];
    //Perspective division
    fTempo[4]*=fTempo[7];
    fTempo[5]*=fTempo[7];
    fTempo[6]*=fTempo[7];
    //Window coordinates
    //Map x, y to range 0-1
    windowCoordinate[0]=(fTempo[4]*0.5f+0.5f)*viewport[2]+viewport[0];
    windowCoordinate[1]=(fTempo[5]*0.5f+0.5f)*viewport[3]+viewport[1];
    //This is only correct when glDepthRange(0.0, 1.0)
    windowCoordinate[2]=(1.0f+fTempo[6])*0.5f;	//Between 0 and 1
    return 1;
}
 
int glhUnProjectf(float winx, float winy, float winz, const float *modelview, const float *projection, const int *viewport, float *objectCoordinate)
{
    //Transformation matrices
    float m[16], A[16];
    float in[4], out[4];
    //Calculation for inverting a matrix, compute projection x modelview
    //and store in A[16]
    MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
    //Now compute the inverse of matrix A
    if(glhInvertMatrixf2(A, m)==0)
        return 0;
    //Transformation of normalized coordinates between -1 and 1
    in[0]=(winx-(float)viewport[0])/(float)viewport[2]*2.0f-1.0f;
    in[1]=(winy-(float)viewport[1])/(float)viewport[3]*2.0f-1.0f;
    in[2]=2.0f*winz-1.0f;
    in[3]=1.0f;
    //Objects coordinates
    MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
    if(out[3]==0.0)
        return 0;
    out[3]=1.0f/out[3];
    objectCoordinate[0]=out[0]*out[3];
    objectCoordinate[1]=out[1]*out[3];
    objectCoordinate[2]=out[2]*out[3];
    return 1;
}

axelynx::vec3 CCamera::Project(axelynx::vec3 point) const
{
	const axelynx::mat4& view = GetViewMatrix();
	const axelynx::mat4& proj = GetProjectionMatrix();
	int viewport[4];

	viewport[0] = x1_;
	viewport[1] = y1_;
	viewport[2] = x2_;
	viewport[3] = y2_;

	axelynx::vec3 result;
	glhProjectf(point.x,point.y,point.z,view.m,proj,viewport,result);
	result.y = y2_ - result.y;

	return result;
}

axelynx::vec3 CCamera::UnProject(axelynx::vec3 coords) const
{
	const axelynx::mat4& view = GetViewMatrix();
	const axelynx::mat4& proj = GetProjectionMatrix();
	int viewport[4];

	viewport[0] = x1_;
	viewport[1] = y1_;
	viewport[2] = x2_;
	viewport[3] = y2_;

	axelynx::vec3 result;
	glhUnProjectf(coords.x,y2_ - coords.y,coords.z,view.m,proj,viewport,result);

	return result;
}

axelynx::PickResult CCamera::Pick(axelynx::vec2 coords) const
{
	vec3 startpoint = UnProject(axelynx::vec3(coords.x,coords.y,0));
	vec3 endpoint = UnProject(axelynx::vec3(coords.x,coords.y,1));

	CScene *scene = dynamic_cast<CScene*>(my_scene_);

	return scene->Pick(startpoint,endpoint);
}

bool CCamera::SetEnable(bool value)
{
	enabled_ = value;
	return true;
}

bool CCamera::isEnabled() const
{
	return enabled_;
}