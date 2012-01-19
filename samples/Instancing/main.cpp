#include <axelynx/axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	Window *wnd = eng->AddWindow(800,600);
	wnd->VSync(false);
	//TODO: Place your resource loading and scene creating code here

	Scene *s1 = eng->AddScene();
	Scene *s2 = eng->AddScene();
	Camera *cam = s1->AddCamera();
	s2->Shared(cam);

	Surface *cube = StandartSurfaces::Cube();
	
	
	Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/Axelynx_04_mini.jpg",Texture::Desc().Anisotropic(16));
	
	Material *m = eng->CreateMaterial();
	m->SetTexture(diffuse);
	m->SetShader(StandartShaders::Render::Texturing());

	for(int i=0;i<21;++i)
	{
		for(int j=0;j<21;++j)
		{
			for(int k=0;k<21;++k)
			{
				Entity *ent = s1->Add(cube);
				ent->SetMaterial(m);
				ent->SetPosition((float)i*2.2,(float)j*2.2,(float)k*2.2);
			}
		}
	}

	InstanceGroup *cubes_ig = eng->CreateInstanceGroup();
	cubes_ig->SetGeometry(cube);
	Entity* cubes = s2->Add(cubes_ig);
	cubes->SetTexture(diffuse);
	cubes->SetShader(StandartShaders::Render::TexturingInstanced());
	cubes->SetBlend(BM_NONE);

	for(int i=0;i<100;++i)
	{
		for(int j=0;j<20;++j)
		{
			for(int k=0;k<100;++k)
			{
				Instance *ins = cubes_ig->AddInstance();
				ins->SetPosition((float)i*2.2,(float)j*2.2,(float)k*2.2);
			}
		}
	}

	float dx=0,dy=0;
	vec3 transl;

	Scene* cur_scene = s1;

	cam->SetPosition(23,23,110);
	
	bool use_zpass = false;

	while(wnd->isRunning() && !KeyDown(KEY::ESCAPE))
	{

		if(KeyDown(KEY::F1))
			cur_scene = s1;
		if(KeyDown(KEY::F2))
			cur_scene = s2;
		if(KeyDown(KEY::F3))
			use_zpass = true;
		if(KeyDown(KEY::F4))
			use_zpass = false;

		//TODO: Place your rendering code here
		float dt = Timer::Delta() * 0.01;

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

		dx *= 0.95;
		dy *= 0.95;

		if(use_zpass)
		{
			cubes->SetShader(StandartShaders::Render::ZPassTexturingInstanced());
			cur_scene->ZPassRender();
			cubes->SetShader(StandartShaders::Render::TexturingInstanced());
		}

		cur_scene->Render();

		wchar_t wbuff[100];
		if(cur_scene == s1)
			swprintf(wbuff,L"CopyEntity, FPS: %d Tris: %d, DIPs: %d",eng->GetStatistics()->GetFPS()
				,eng->GetStatistics()->TrisRendered()
				,eng->GetStatistics()->DIPs());
		else
			swprintf(wbuff,L"Instancing, FPS: %d Tris: %d, DIPs: %d",eng->GetStatistics()->GetFPS()
				,eng->GetStatistics()->TrisRendered()
				,eng->GetStatistics()->DIPs());
		wnd->SetText(wbuff);

		wnd->Flip();
	}
	//TODO: Place your deinitalize code here
	eng->Free();
}