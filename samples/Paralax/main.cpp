#include <axelynx/axelynx.h>
#include <iostream>

using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();
        Window *wnd = eng->AddWindow();
        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();

		cam->ClearColor(0.15,0.15,0.15);
		
		Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rockwall.tga",Texture::Desc().Anisotropic(16.0));
		Texture *relief = eng->LoadTexture(L"../../../../samples/media/rockwall_relief.tga",Texture::Desc().Anisotropic(16.0));

		Material *paralax  = eng->CreateMaterial();
		paralax->SetShader(StandartShaders::Render::Paralax());
		paralax->SetTexture(diffuse,0);
		paralax->SetTexture(relief,1);

		Surface *cube = eng->LoadSurface(L"../../../../samples/media/Teapot_4096.axs");
		cube->RecalcTangents();

        Entity *body = s->Add(cube);
        body->SetMaterial(paralax);

		StandartShaders::Render::Paralax()->SetUniform(Shader::SU_LIGHTPOS,vec3(20,10,50));

        body->SetPosition(0,0,-6);

        Entity *turret = s->Add(cube);
        turret->SetParent(body);
        turret->SetMaterial(paralax);
        turret->SetPosition(0,0.0,-1.25);
        turret->SetScale(0.25);

        turret = turret->Copy();
        turret->SetPosition(0,0,1.25);

        turret = turret->Copy();
        turret->SetPosition(0,-1.25,0);

        turret = turret->Copy();
        turret->SetPosition(0,1.25,0);

        turret = turret->Copy();
        turret->SetPosition(-1.25,0,0);

        turret = turret->Copy();
        turret->SetPosition(1.25,0,0);

        float dx=0,dy=0;

		for(int i=0;i<cube->CountTriangles();++i)
		{
			int index0,index1,index2;
			cube->GetTriangle(i,index0,index1,index2);
			std::cout<<"tris "<<i<<" ("<<index0<<","<<index1<<","<<index2<<")"<<std::endl;
		}

		for(int i=0;i<cube->CountVertices();++i)
		{
			vec2 uv0;
			vec2 uv1;

			vec3 position;
			vec3 normal;
			vec3 tangent;
			vec4 color;

			cube->GetVertexPosition(i,position);
			cube->GetVertexNormal(i,normal);
			cube->GetVertexTangent(i,tangent);
			cube->GetVertexColor(i,color);
			cube->GetVertexTexCoord(i,uv0,0);
			cube->GetVertexTexCoord(i,uv1,1);
			std::cout<<"vertex "<<i<<std::endl;
			std::cout<<"    position ("<<position.x<<","<<position.y<<","<<position.z<<")"<<std::endl;
			std::cout<<"    normal   ("<<normal.x<<","<<normal.y<<","<<normal.z<<")"<<std::endl;
			std::cout<<"    tangent  ("<<tangent.x<<","<<tangent.y<<","<<tangent.z<<")"<<std::endl;
			std::cout<<"    color    ("<<color.x<<","<<color.y<<","<<color.z<<")"<<std::endl;

			std::cout<<"    uv0      ("<<uv0.x<<","<<uv0.y<<")"<<std::endl;
			std::cout<<"    uv1      ("<<uv1.x<<","<<uv1.y<<")"<<std::endl;
		}

        while(wnd->isRunning())
        {
                if(KeyDown('A'))
                        dx -= 1.0;
                if(KeyDown('D'))
                        dx += 1.0;
                if(KeyDown('W'))
                        dy -= 1.0;
                if(KeyDown('S'))
                        dy += 1.0;
                //TODO: Place your rendering code here
                float dt = Timer::Delta() * 0.01;
				dt = 0.1;
                dx *= 0.95;
                dy *= 0.95;
                body->Turn(dt*dy,dt*dx,0);
                s->Render();
                wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}