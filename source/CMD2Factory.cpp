
#include "CMD2Factory.h"
#include "CMorfedMesh.h"


#include "anorms.h"

struct md2Index
{
   short s;                            // First texture coordinate index.
   short t;                            // Second texture coordinate index.
};


struct md2TriangleIndice
{
   unsigned short triIndex[3];         // Triangle index.
   unsigned short texCoordIndex[3];    // Texture coord index.
};


struct md2FrameIndex
{
   unsigned char vertex[3];            // Vertices of a frame.
   unsigned char normal;               // Light normal used for this frame.
};


struct md2Frame
{
   float scale[3];                     // Scale value used to make the model bigger.
   float translate[3];                 // Translate value for this frame.
   char name[16];                      // Frame name.
   md2FrameIndex frameInfo[1];          // Index into the frame data (frame_index).
};

axelynx::MorfedMesh * CMD2Factory::CreateFromFile(axelynx::File file)
{
	using axelynx::vec3;
	using axelynx::vec2;

	struct MD2Header
	{
	   __int32 id;                             // Md2 ID number (IDP2 is the id for .md2 models).
	   __int32 version;                        // Version of the md2 (should be 8).
	   __int32 skinWidth;                      // Width of the models texture.
	   __int32 skinHeight;                     // Height of the models texture.
	   __int32 frameSize;                      // Size of frames per bytes.
	   __int32 totalSkins;                     // Number of skins the model uses.
	   __int32 numVertices;                    // Number of vertices (points) the model has.
	   __int32 numTexCoords;                   // Number of texture coordinates the model has.
	   __int32 numTriangles;                   // Number of triangles the model has.
	   __int32 numGLCommands;                  // Number of OpenGL commands the model uses.
	   __int32 numFrames;                      // Number of animation frames the model has.
	   __int32 offsetSkins;                    // Offset to the skin name (position in the file).
	   __int32 offsetTexCoords;                // Offset to where the texture coordinates are.
	   __int32 offsetTriangles;                // Offset to where the triangle data is.
	   __int32 offsetFrames;                   // Offset to where the frames start.
	   __int32 offsetGLCommands;               // Offset to where the OpenGL commands are.
	   __int32 offsetEndOfFile;                // Offset to where the file ends.
	} ;

	file.Open();


	unsigned __int8* buffer = static_cast<unsigned __int8*>(file.GetPointer());

	bool need_delete_buffer = false;

	if(!buffer)
	{
		buffer = new unsigned __int8[file.Size()];
		file.Read(buffer,file.Size());
	}

	MD2Header *md2header = (MD2Header*)buffer;

   // Again like with extracting the header we will need a pointer to get the
   // data out of the buffer.
   md2Index *pTexList;

   // offsetTexCoords is a value in bytes that holds where the start of the
   // texture coordinates are located in the .md2 file.  Lets move to that place
   // in the buffer so we can read them in.
   pTexList = (md2Index*)&buffer[md2header->offsetTexCoords];

   md2TriangleIndice *pTriList;

   // Get the data.
   pTriList = (md2TriangleIndice*)&buffer[md2header->offsetTriangles];

	CMorfedMesh *mesh = new CMorfedMesh(md2header->numFrames,md2header->numTriangles * 3,md2header->numTriangles);
	
	for(int j =0;j<md2header->numTriangles;++j)
	{
		auto face = mesh->GetFace(j);
		face.i0 = pTriList[j].triIndex[0];
		face.i1 = pTriList[j].triIndex[1];
		face.i2 = pTriList[j].triIndex[2];
	}

	float delta_u = 1.0f / 288.0f;
	float delta_v = 1.0f / 195.0f;

	for(int i=0;i<md2header->numFrames;++i)
	{
		md2Frame *framedata = (md2Frame*)&buffer[md2header->offsetFrames + md2header->frameSize * i];

		CMorfedMesh::CMorfedFrame& frame = mesh->GetFrame(i);
		for(int j =0;j<md2header->numTriangles;++j)
		{
			axelynx::MorfedMesh::Frame::Vertex& vertex = frame.GetVertex(j);
			vertex.position = vec3(framedata->scale[0] * framedata->frameInfo[j].vertex[0] + framedata->translate[0]
			,framedata->scale[2] * framedata->frameInfo[j].vertex[2] + framedata->translate[2]
			,framedata->scale[1] * framedata->frameInfo[j].vertex[1] + framedata->translate[1]);

			int normalindex = framedata->frameInfo[j].normal;

			vertex.normal = vec3(precalc_normal[normalindex][0]
								,precalc_normal[normalindex][1]
								,precalc_normal[normalindex][2]);


			vertex.texcoord = vec2(float(pTexList[pTriList[j].texCoordIndex[0]].s) * delta_u
				,float(pTexList[pTriList[j].texCoordIndex[1]].s) * delta_v);

		}
	}

	if(need_delete_buffer)
	{
		delete[] buffer;
	}

	file.Close();
	mesh->UnLock();
	return mesh;
}