#include <axelynx/axelynx.h>
#include <iostream>
#include <wchar.h>

using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	eng->AddWindow();
	Canvas *c = eng->GetCanvas();

	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();
	cam->Projection(30,800.0f/600.0f,1.0f,5000.0f);

	int ct = Timer::Millisecs();
	int frames = 0;

	eng->VSync(false);

	vec3 transl;

	const int count = 1000000;
	SpriteSystem *ss = eng->CreateSpriteSystem(count);
	Texture::Desc desc;
	desc.compressed = 1;
	Texture *flare3d = eng->LoadTextureArray(L"../../../../samples/media/boomstrip2.bmp",64,64,40,1);
	Texture *flare = eng->LoadTexture(L"../../../../samples/media/Fire.bmp");

	SpriteSystem::Sprite **sprites = new SpriteSystem::Sprite*[count];

	int index = 0;
	for(int i = 0;i < count; ++i)
	{
		sprites[i] = ss->AddInstance();

		sprites[i]->SetPosition(rnd(-250,250),rnd(-250,250),rnd(-250,250));
		sprites[i]->SetSize(rnd(4,8));
	}
	//eng->Settings().Shaders.UseCash = false;
	Timer::Delta();
	Shader *booms = eng->LoadShader(L"../../../../samples/media/booms_array");
	wchar_t buff[80];
	swprintf(buff,L"loading time: %fms",Timer::Delta());
	shell::ShowMessage(buff);

	Entity *ent = s->Add(ss);
	ent->SetTexture(flare3d);
	ent->SetShader(booms);
	//ent->SetShader(StandartShaders::Render::PointSprite());
	ent->SetBlend(BM_ADD);
	ent->SetPosition(vec3(0,0,0));

	mat4 proj = cam->GetProjectionMatrix();

	cam->ClearColor(0,0,0,1);
	for(int i=0;i<4;++i)
	{
		for(int j=0;j<4;++j)
		{
			std::cout<<proj(i,j)<<char(9);
		}
		std::cout<<std::endl;
	}

	ss->SetRecalcMode(SpriteSystem::RM_MANUAL);
	ss->Recalc();


	int rm=0;
	float fframe = 0;
	while(eng->isRunning()&& !KeyDown(KEY::ESCAPE) )
	{
		float dt = Timer::Delta() * 0.01;
		fframe += dt * 0.1f;
		booms->Bind();
		booms->SetUniform("fframe",fframe*40.0f);
		booms->UnBind();

		rm++;
		if(KeyDown(KEY::F2))
		{
			ent->SetShader(StandartShaders::Render::PointSprite());
		}
		if(KeyDown(KEY::F3))
		{
			ent->SetShader(StandartShaders::Render::QuadSprite());
		}
		//ss->Recalc();

		quat qcam(vec3(float(MouseY()-300)*0.2f,float(MouseX()-400)*0.2f,0));
		cam->SetOrientation(qcam);

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

		frames++;

		if(ct < Timer::Millisecs() - 1000)
		{
			wchar_t buff[80];
			swprintf(buff,L"FPS:%d",frames);
			eng->SetText(buff);
			frames =0;
			ct = Timer::Millisecs(); 
		}

		c->Clear();

		s->Render();
		//TODO: Place your code here
		eng->Flip();
		eng->Sync();

	}

	eng->Free();
}