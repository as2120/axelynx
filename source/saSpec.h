#pragma once
#include "gl/axgl.h"

//system vertex attributs specification

const int VA_POSITION	=0; //system
const int VA_NORMAL		=1; //system
const int VA_TANGENT	=2; //system
const int VA_COLOR		=3; //system
const int VA_TEXCOORD0	=4; //system
const int VA_TEXCOORD1	=5; //system
const int VA_BONEID		=6; //engine
const int VA_WEIGHT		=7; //engine
const int VA_POINT_SIZE	=8; //engine
const int VA_NEXT_POSITION=9; //system
const int VA_NEXT_NORMAL	=10; //system
const int VA_NEXT_TANGENT	=11; //system
const int VA_NEXT_TEXCOORD0	=12; //system

struct VertexAttribInfo
{
	const char *name;
	const int components;
	GLenum format;
	GLenum normalized;
};

const VertexAttribInfo sysattribs[] ={
	{"position",3,GL_FLOAT,GL_FALSE},
	{"normal",3,GL_BYTE,GL_TRUE},
	{"tangent",3,GL_BYTE,GL_TRUE},
	{"color",4,GL_UNSIGNED_BYTE,GL_TRUE},
	{"texcoord0",2,GL_FLOAT,GL_FALSE},
	{"texcoord1",2,GL_FLOAT,GL_FALSE},
	{"boneid",4,GL_UNSIGNED_BYTE,GL_FALSE},
	{"weight",4,GL_FLOAT,GL_TRUE},
	{"pointsize",1,GL_FLOAT,GL_FALSE},
	{"nextposition",3,GL_FLOAT,GL_FALSE},
{"nextnormal",3,GL_FLOAT,GL_TRUE},
{"nexttangent",3,GL_FLOAT,GL_TRUE},
{"nexttexcoord",2,GL_FLOAT,GL_FALSE}};


bool SetVertexAttribPointer(int attribut, int position, int stride);
bool SetShaderSystemAttribLocation(int attribut);