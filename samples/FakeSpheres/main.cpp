#include <axelynx/axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	eng->Settings().Shaders.UseCash = false;

	Window *wnd = eng->AddWindow(800,600);
	wnd->VSync(false);
	//TODO: Place your resource loading and scene creating code here

	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();
	Canvas *c = eng->GetCanvas();

	float dx=0,dy=0;

	vec3 transl;

	//StandartShaders::Render::QuadSprite()->SaveFile(L"fakespheres");
	Shader * fakeSpheresShader = eng->LoadShader(L"../../../../samples/media/fakespheres");

	int count = 200000;

	SpriteSystem *ss = eng->CreateSpriteSystem(count);

	Entity *ent = s->Add(ss);

	ent->SetShader(fakeSpheresShader);

	float cube_size = 400;

	for(int i=0;i<count;i++)
	{
		SpriteSystem::Sprite *s = ss->AddInstance();
		s->SetSize(7);
		s->SetColor(rnd(1),rnd(1),rnd(1));
		s->SetPosition(rnd(-cube_size,cube_size),rnd(-cube_size,cube_size),rnd(-cube_size,cube_size));
	}

	ss->SetRecalcMode(SpriteSystem::RM_MANUAL);
	ss->Recalc();
	
	//Texture *diffuse_map = eng->LoadTexture(L"Panels_Diffuse.png");

	Texture *diffuse_map = eng->LoadCubeMapTexture(L"../../../../samples/media/Panels_Diffuse.png",L"../../../../samples/media/Panels_Diffuse.png",L"../../../../samples/media/Panels_Diffuse.png",L"../../../../samples/media/Panels_Diffuse.png",L"../../../../samples/media/Panels_Diffuse.png",L"../../../../samples/media/Panels_Diffuse.png",
		Texture::Desc(),L"png");
	Texture *normal_map = eng->LoadCubeMapTexture(L"../../../../samples/media/Panels_Normal_Tangent.png",L"../../../../samples/media/Panels_Normal_Tangent.png",L"../../../../samples/media/Panels_Normal_Tangent.png",L"../../../../samples/media/Panels_Normal_Tangent.png",L"../../../../samples/media/Panels_Normal_Tangent.png",L"../../../../samples/media/Panels_Normal_Tangent.png",
		Texture::Desc(),L"png");
	Texture *mask_map = eng->LoadTexture(L"../../../../samples/media/Panels_refmask.png");

	ent->SetTexture(diffuse_map,0)->SetTexture(normal_map,1)->SetTexture(mask_map,2);

	//ent->SetBlend(BM_ADD);
	//ent->SetDepthWriteMode(DW_WRITE);

	Font::Desc fdesc(L"../../../../samples/media/ufont.ttf");
	fdesc.size = 24;

	Font *ufont = eng->LoadFont(fdesc);

	Shader *zEarlyShader = eng->LoadShader(L"../../../../samples/media/zonly");
	
	Entity *zent = s->Add(ss);
	zent->SetShader(zEarlyShader);

//	ent->SetShader(zEarlyShader);

	bool zearly = false;
	bool animatecam = true;

	while(wnd->isRunning() && !KeyDown(KEY::ESCAPE))
	{
		if(KeyHit(KEY::F2))
			zearly = !zearly;
		

		if(KeyHit(KEY::SPACE))
			animatecam = !animatecam;
		

		//TODO: Place your rendering code here
		float dt = Timer::Delta() * 0.01;

		if(animatecam)
		{
			cam->Turn(0.2,0.4,0.0);
		}
		else
		{
			quat qcam(vec3(float(MouseY()-(wnd->GetWidth()/2))*0.11f,float(MouseX()-(wnd->GetHeight()/2))*0.11f,0));
			cam->SetOrientation(qcam);
		}
		

		vec3 move;

		if(KeyDown('W'))
			move.z -= 1;
		if(KeyDown('S'))
			move.z += 1;
		if(KeyDown('A'))
			move.x -= 1;
		if(KeyDown('D'))
			move.x += 1;

		move = cam->GetOrientation().ToMat3() * move;
		transl += move;

		transl *= 0.9;

		cam->Translate(transl * dt);

		fakeSpheresShader->SetUniform("campos",cam->GetPosition());

		if(zearly)
		{
			zent->SetVisible(true);
			ent->SetVisible(false);
			s->ZPassRender();

			ent->SetVisible(true);
			zent->SetVisible(false);
		}

		s->Render();

		wchar_t wbuff[100];
		c->SetBlendMode(BM_ALPHA);

		swprintf(wbuff,L"fps = %d",eng->GetStatistics()->GetFPS());
		c->SetPosition(vec2(10,10));
		ufont->Draw(c,wbuff);

		if(zearly)
			swprintf(wbuff,L"zearly on [F2]");
		else
			swprintf(wbuff,L"zearly off [F2]");

		c->SetPosition(vec2(10,50));
		ufont->Draw(c,wbuff);


		wnd->Flip();
	}
	//TODO: Place your deinitalize code here
	eng->Free();
}