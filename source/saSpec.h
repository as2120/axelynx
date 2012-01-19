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
	const char *definename;
};

const VertexAttribInfo sysattribs[] ={
	{"position",3,GL_FLOAT,GL_FALSE,"VA_POSITION"},
	{"normal",3,GL_BYTE,GL_TRUE,"VA_NORMAL"},
	{"tangent",3,GL_BYTE,GL_TRUE,"VA_TANGENT"},
	{"color",4,GL_UNSIGNED_BYTE,GL_TRUE,"VA_COLOR"},
	{"texcoord0",2,GL_FLOAT,GL_FALSE,"VA_TEXCOORD0"},
	{"texcoord1",2,GL_FLOAT,GL_FALSE,"VA_TEXCOORD1"},
	{"boneid",4,GL_UNSIGNED_BYTE,GL_FALSE,"VA_BONEID"},
	{"weight",4,GL_FLOAT,GL_TRUE,"VA_WEIGHT"},
	{"pointsize",1,GL_FLOAT,GL_FALSE,"VA_POINTSIZE"},
	{"nextposition",3,GL_FLOAT,GL_FALSE,"VA_NEXTPOSITION"},
	{"nextnormal",3,GL_FLOAT,GL_TRUE,"VA_NEXTNORMAL"},
	{"nexttangent",3,GL_FLOAT,GL_TRUE,"VA_NEXTTANGENT"},
	{"nexttexcoord",2,GL_FLOAT,GL_FALSE,"VA_NEXTTEXCOORD"}};


bool SetVertexAttribPointer(int attribut, int position, int stride);
bool SetShaderSystemAttribLocation(int attribut);