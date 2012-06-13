#pragma once 

#include "AnimGeometry.h"
#include "math\vec3.h"
#include "math\vec2.h"

namespace axelynx
{
	class MorfedMesh : public AnimGeometry
	{
	public:
		class Frame
		{
		public:
			struct Vertex
			{
				vec3 position;
				vec3 normal;
				vec3 tangent;
				vec2 texcoord;
			};

			virtual Vertex& GetVertex(int index) = 0;
			virtual const Vertex& GetVertex(int index) const = 0;


			virtual int CountVertices() const = 0;

			virtual bool Resize(int count_vertices, int count_faces) = 0;

			virtual ~Frame()
			{
			}
		};

		struct Face
		{
			unsigned __int16 i0;
			unsigned __int16 i1;
			unsigned __int16 i2;
		};

		virtual Face& GetFace(int index) = 0;
		virtual const Face& GetFace(int index) const = 0;

		virtual int CountFaces() const = 0;

		virtual std::wstring GetClass() const
		{
			return L"morfed mesh";
		}

		virtual ~MorfedMesh()
		{
		}
		
		virtual int CountFrames() const = 0;
		virtual Frame& GetFrame(int index) = 0;
		virtual const Frame& GetFrame(int index) const = 0;
		virtual bool Resize(int frames_count) = 0;

		virtual bool Lock() = 0;
		virtual bool UnLock() = 0;
	};
};