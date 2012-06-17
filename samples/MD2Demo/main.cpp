#include <axelynx/axelynx.h>


using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();

        Window *wnd = eng->AddWindow();

		wnd->VSync(false);

        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();
		cam->ClearColor(0.2,0.3,0.4);

		Canvas *c = eng->GetCanvas();

		//cam->SetDrawMode(Camera::DM_WIREFRAME);

        //Surface *cube = StandartSurfaces::Grid(256,256,16,16,true);
        //Entity *body = s->Add(cube);
		//body->SetScale(256);

		Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/drfreak.tga",Texture::Desc().Anisotropic(16.0));
		Texture *normalmap = eng->LoadTexture(L"../../../../samples/media/drfreak_bump.tga",Texture::Desc().Anisotropic(16.0));
		MorfedMesh *mesh = eng->LoadMorfedMesh(L"../../../../samples/media/drfreak.md2");

		Timer::Delta();
		mesh->RecalcTangents();
		float a = Timer::Delta();
		int b = 1;

		Shader *shader = StandartShaders::Render::MorfedMeshBumpMapping();

		const int COUNT_TEAPOTS = 400;

		AnimEntity* ents[COUNT_TEAPOTS];
		quat rots[COUNT_TEAPOTS];

		for(int i=0;i<COUNT_TEAPOTS;++i)
		{
			ents[i] = s->Add(mesh);
			ents[i]->SetShader(shader);
			ents[i]->SetTexture(diffuse,0);
			ents[i]->SetTexture(normalmap,1);

			ents[i]->SetPosition((i / 20)*8,0,(i % 20)*8);
			ents[i]->SetOrientation(0,rnd(360),0);
			ents[i]->SetScale(0.1);
			ents[i]->SetFrame(rnd(0,mesh->CountFrames()));

			rots[i] = quat(vec3(0,rnd(-0.5,0.5),0));
		}

		vec3 transl;

		float time =0;

		bool wire = false;

		
		Font::Desc desc(L"arial.ttf");	
		desc.size = 16;
		Font *fnt = eng->LoadFont(desc);
		c->SetFont(fnt);

		
		shader->SetUniform("lightpos",vec3(-5.0,27.0,3.0));

        while(wnd->isRunning())
        {
			if(KeyHit('R'))
			{
				wire = ! wire;
				cam->SetDrawMode(wire?Camera::DM_WIREFRAME:Camera::DM_SOLID);
			};

            //TODO: Place your rendering code here
			float dt = Timer::Delta();
			time += dt*0.001;


			quat qcam(vec3(float(MouseY()-300)*0.2f,float(MouseX()-400)*0.2f,0));
			cam->SetOrientation(qcam);

			vec3 move;

			if(KeyDown('W'))
				move.z -= 0.1;
			if(KeyDown('S'))
				move.z += 0.1;
			if(KeyDown('A'))
				move.x -= 0.1;
			if(KeyDown('D'))
				move.x += 0.1;

			move = cam->GetOrientation().ToMat3() * move;
			transl += move;

			transl *= 0.9;

			cam->Translate(transl * dt * 0.1);
			s->Update(dt*0.01);
			s->Render();
			c->SetBlendMode(BM_ALPHA);
			c->SetColor(1,1,1);
			c->SetAlpha(1);
			c->SetRotate(0);
			c->SetScale(1);
			c->SetPosition(10,10);
			c->Print(L"FPS:%d, DIP`s:%d, tris: %d",eng->GetStatistics()->GetFPS(),eng->GetStatistics()->DIPs(),eng->GetStatistics()->TrisRendered());
			//fnt->Draw(c,L"trtrt");
			c->SetBlendMode(BM_NONE);
            wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}