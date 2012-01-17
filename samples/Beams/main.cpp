#include <axelynx\axelynx.h>
#include <iostream>
#include <wchar.h>

using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	Window *wnd = eng->AddWindow();
	Canvas *c = eng->GetCanvas();

	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();
	cam->Projection(30,float(wnd->GetWidth())/float(wnd->GetHeight()),1.0f,5000.0f);
	cam->ClearColor(0.04,0.04,0.04);
	int ct = Timer::Millisecs();
	int frames = 0;

	eng->VSync(false);

	vec3 transl;

	RaySystem *rs = eng->CreateRaySystem();
	rs->SetRenderType(RaySystem::RT_VOLUMETRIC);

	Material *ld = eng->CreateMaterial();
	ld->SetShader(StandartShaders::Render::OnlyNormals());

	Surface *cube = StandartSurfaces::Cube();

	const int cnt = 10000;


	RaySystem::LightRay *rays[cnt];

	for(int i=0;i<cnt;++i)
	{

		float r = rnd(0.75,1.0);
		float g = rnd(0.3,0.75);
		float b = rnd(0.0,0.3);

		rays[i] = rs->AddLightRay();
		rays[i]->SetWidth(rnd(2.5,4.5));

		rays[i]->SetColor(r,g,b);
	}

	Texture::Desc defdesc;

	Texture *tex = eng->LoadTexture(L"../../../../samples/media/blue_glow.bmp",defdesc);
	Material *m = eng->CreateMaterial();
	m->SetShader(StandartShaders::Render::VolumetricLines());
	m->SetBlend(BM_ADD);
	m->SetTexture(tex);
	Entity *ent = s->Add(rs);
	ent->SetMaterial(m);

	rs->SetRecalcMode(RaySystem::RM_MANUAL);
	rs->Recalc();
	//
	bool animated = true;

	vec3 start = vec3(0,0,0);
	float da = 0;
	while(eng->isRunning()&& !KeyDown(KEY::ESCAPE))
	{
		//rs->Recalc();
		if(KeyHit(KEY::SPACE))
			animated = !animated;

		if(animated)
		{
			for(int i=0;i<cnt;++i)
			{
				float sin,cos;
				float sin2,cos2;

				sincos(float(i) * 0.1 + da,sin,cos);
				sincos((float(i) * 0.1 + da)*0.71,sin2,cos2);
				float delta = float(i) * 0.1;
				vec3 start(sin2*delta*0.5,-500*sin*cos,cos2*delta*(-0.7));
				vec3 end(sin*delta,500,cos*delta);
				rays[i]->Place(start,end);
			}
			rs->Recalc();
		}
		float dt = Timer::Delta() / 10.0;
		da += 0.001;

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
		s->Render();
		if(ct < Timer::Millisecs() - 1000)
		{
			wchar_t buff[80];
			swprintf(buff,L"FPS:%d DIPs:%d tris:%d verts:%d",frames,eng->GetStatistics()->DIPs()
				,eng->GetStatistics()->TrisRendered()
				,eng->GetStatistics()->VerticesRendered());
			eng->SetText(buff);
			frames =0;
			ct = Timer::Millisecs(); 
		}

		//c->Clear();


		//TODO: Place your code here
		eng->Flip();
		eng->Sync();

	}

	eng->Free();
}