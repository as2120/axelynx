#include <axelynx/axelynx.h>
using namespace axelynx;

int main()
{

	Engine *eng = Engine::Init();
	eng->Settings().Shaders.UseCash = 0;

	Window *wnd = eng->AddWindow(1280,720);
	//TODO: Place your resource loading and scene creating code here
	wnd->VSync(false);

	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();
	Canvas *canvas = eng->GetCanvas();

	cam->Projection(60.0f,(float)wnd->GetWidth() / (float)wnd->GetHeight(),1.0f,5000.0f);

	const int shadow_size = 2048;

	Camera *shadow_cam = s->AddCamera();
	shadow_cam->Projection(90.0f,1.0,1.0,2000.0f);
	shadow_cam->Viewport(0,0,shadow_size,shadow_size);
	shadow_cam->SetEnable(false);

	auto sm_tex_desc  =Texture::Desc().Size(shadow_size,shadow_size).TexType(Texture::TT_CUBE).UseMipmaps(false);
	sm_tex_desc.bpc = 4;
	sm_tex_desc.cpp = 1;
	Texture *shadowmap = eng->CreateTexture(sm_tex_desc);

	Font::Desc desc(L"../../../../samples/media/CODE2000.ttf");	
	desc.size = 16;
	Font *fnt = eng->LoadFont(desc);

	vec3 transl;

	Surface *grid = StandartSurfaces::Plane();

	Surface *cube = StandartSurfaces::Cube();
	SpriteSystem *ss = eng->CreateSpriteSystem(1);
	Surface *hebemission = eng->LoadSurface(L"../../../../samples/media/hebemissin.sbs");
	Surface *headcrab = eng->LoadSurface(L"../../../../samples/media/hc.sbs");

	Texture* floor = eng->LoadTexture(L"../../../../samples/media/floor.jpg");
	Texture* flare = eng->LoadTexture(L"../../../../samples/media/flare.png");
	Texture* logo = eng->LoadTexture(L"../../../../samples/media/logo.jpg");

	Texture* hc_diffus = eng->LoadTexture(L"../../../../samples/media/hc.jpg");
	Texture* hc_normal = eng->LoadTexture(L"../../../../samples/media/hc_normal.jpg");

	Shader *texturing = eng->LoadShader(L"../../../../samples/media/texturing");
	Shader *lighting = eng->LoadShader(L"../../../../samples/media/lighting");
	Shader *bump_mapping = eng->LoadShader(L"../../../../samples/media/bump_mapping");
	Shader *shadowmap_shader = eng->LoadShader(L"../../../../samples/media/shadowmap");

	Entity* efloor = s->Add(cube);
	efloor->SetScale(1024,0.5f,1024);
	efloor->SetTexture(floor);
	efloor->SetTexture(shadowmap,1);
	efloor->SetShader(texturing);

	Entity *statue = s->Add(hebemission);
	statue->SetShader(lighting);
	statue->SetScale(2);
	statue->SetOrientation(-90,0,0);
	statue->SetPosition(0,70,0);
	statue->SetTexture(shadowmap,0);
	int count_cubes = 50;
	int count_hc = 25;

	class Turner : public Entity::Action
	{
		vec3 turn;
	public:
		Turner()
		{
			turn  =vec3(rnd(-0.1,0.1),rnd(-0.1,0.1),rnd(-0.1,0.1));
		}
		virtual bool operator()(Entity* ent,float twin)
		{
			ent->Turn(turn * twin);
			return false;
		}
	};

	for(int i=0;i<count_cubes;++i)
	{
		Entity* ent = s->Add(cube);
		ent->SetTexture(logo);
		ent->SetShader(texturing);
		ent->SetTexture(shadowmap,1);
		float size = rnd(2,15);
		ent->SetScale(size,size,size);
		ent->SetPosition(rnd(-1024,1024),0.25 + size,rnd(-1024,1024));
	}

	for(int i=0;i<count_hc;++i)
	{
		Entity* ent = s->Add(headcrab);
		ent->SetTexture(hc_diffus,0);
		ent->SetTexture(hc_normal,1);
		ent->SetTexture(shadowmap,2);
		ent->SetShader(bump_mapping);
		ent->SetPosition(rnd(-1024,1024),rnd(4,100),rnd(-1024,1024));
		ent->SetScale(0.5,0.5,0.5);
		ent->OnUpdate(new Turner());
	}

	Entity *light_ent = s->Add(ss);
	light_ent->SetShader(StandartShaders::Render::QuadSprite());
	light_ent->SetTexture(flare);
	light_ent->SetBlend(BM_ADD);

	SpriteSystem::Sprite *light = ss->AddInstance();
	light->SetSize(120);
	
	RenderTarget *rt = eng->CreateRenderTarget(shadow_size,shadow_size);
	rt->BindColorTexture(shadowmap);
	s->SetShadowShader(shadowmap_shader);

	bool animate_light = true;


	while(wnd->isRunning() && !KeyDown(KEY::ESCAPE))
	{
		if(KeyDown('U'))
		{
			cam->SetDrawMode(Camera::DM_SOLID);
		}
		
		if(KeyDown('I'))
			cam->SetDrawMode(Camera::DM_WIREFRAME);
		if(KeyDown('O'))
			cam->SetDrawMode(Camera::DM_POINTS);

		float angle = Timer::Millisecs() * 0.0002;
		float x,z;
		sincos(angle,x,z);

		if(animate_light)
			light->SetPosition(x*400,150,z*400);

		if(KeyHit(KEY::SPACE))
			animate_light = !animate_light;

		texturing->SetUniform(Shader::SU_LIGHTPOS,light->GetPosition());
		bump_mapping->SetUniform(Shader::SU_LIGHTPOS,light->GetPosition());
		lighting->SetUniform(Shader::SU_LIGHTPOS,light->GetPosition());
		shadowmap_shader->SetUniform(Shader::SU_LIGHTPOS,light->GetPosition());

		float dt = Timer::Delta() / 10.0;

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
		//		cam->SetPosition(light);
		//cam->SetOrientation(0,-90,0);
		s->Update(dt);


		//Render to shadow cubemap
		shadow_cam->SetPosition(light);
		shadow_cam->SetEnable(true);
		cam->SetEnable(false);


		//positive x

		rt->BindCubemap(shadowmap,0);
		rt->Bind();	
		shadow_cam->SetOrientation(0,90,0);
		//mat4 tr_matrix = shadow_cam->GetTransformMatrix();---
		s->RenderShadow();
		rt->UnBind();
		//rt->UnBind();
		//negative x

		rt->BindCubemap(shadowmap,1);
		rt->Bind();	
		shadow_cam->SetOrientation(0,-90,0);
		s->RenderShadow();
		rt->UnBind();
		//positive y

		rt->BindCubemap(shadowmap,2);
		rt->Bind();	
		shadow_cam->SetOrientation(90,0,0);
		s->RenderShadow();
		rt->UnBind();
		//negative y
		rt->BindCubemap(shadowmap,3);
		rt->Bind();	
		shadow_cam->SetOrientation(-90,0,0);
		s->RenderShadow();
		rt->UnBind();
		//positive z
		rt->BindCubemap(shadowmap,4);
		rt->Bind();	
		shadow_cam->SetOrientation(0,0,0);
		s->RenderShadow();
		rt->UnBind();
		//negative z
		rt->BindCubemap(shadowmap,5);
		rt->Bind();	
		shadow_cam->SetOrientation(0,180,0);
		s->RenderShadow();
		rt->UnBind();

		//Render scene
		shadow_cam->SetEnable(false);
		cam->SetEnable(true);
		s->Render();

		vec3 startpoiont = cam->UnProject(vec3(MouseX(),MouseY(),0));
		vec3 endpoiont = cam->UnProject(vec3(MouseX(),MouseY(),1));

		wchar_t wbuff[1000];
		swprintf(wbuff,L"FPS = %d",eng->GetStatistics()->GetFPS());
		
		

		canvas->SetBlendMode(BM_ALPHA);
		canvas->SetPosition(vec2(0));
		canvas->SetRotate(0);
		canvas->SetColor(1.0,0,0);

		fnt->Draw(canvas,wbuff);

		canvas->SetBlendMode(BM_NONE);

		wnd->Flip();
	}
	//TODO: Place your deinitalize code here
	eng->Free();
}