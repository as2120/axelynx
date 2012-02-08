#include <axelynx\axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
    Window *wnd = eng->AddWindow(800,600);
	wnd->VSync(true);

	Canvas *c = eng->GetCanvas();
	
	Font *fnt = eng->LoadFreeTypeFont(Font::Desc(L"../../../../samples/media/CODE2000.ttf"));
	c->SetFont(fnt);

	Scene *s = eng->AddScene();
    Camera *cam = s->AddCamera();
	//cam->SetDrawMode(Camera::DM_WIREFRAME);

    Surface *skydome= StandartSurfaces::SkyDome(500,32);
    Entity *sky = s->Add(skydome);

	Material *sky_material = eng->CreateMaterial();

	sky_material->SetShader(StandartShaders::Render::Texturing());
	Texture *sky_tex = eng->LoadTexture(L"../../../../samples/media/Sky.jpg");
	Texture *heightmap = eng->LoadTexture(L"../../../../samples/media/heightmap_low.png");
	Texture *normalmap = eng->LoadTexture(L"../../../../samples/media/normalmap_low.png");
	Texture *terrain_tex = eng->LoadTexture(L"../../../../samples/media/Tex.jpg");
	sky_material->SetTexture(sky_tex);
	sky_material->SetCullingMode(axelynx::CM_DISABLE);

	sky->SetMaterial(sky_material);

	Surface *terrain_grid = StandartSurfaces::RegularGrid(256,256,false);
	Entity *terrain = s->Add(terrain_grid);
	
	Shader *irregular_landscape = eng->LoadShader(L"../../../../samples/media/irregular_landscape");
	Shader *water_shader = eng->LoadShader(L"../../../../samples/media/water");

	terrain->SetShader(irregular_landscape);
	terrain->SetTexture(terrain_tex,0);
	terrain->SetTexture(heightmap,1);
	terrain->SetTexture(normalmap,2);

	terrain->SetScale(400,20,400);
	terrain->SetPosition(0,-5,0);

	Texture *water_nm = eng->LoadTexture(L"../../../../samples/media/6675-normal.jpg");
	vec3 transl;

	Surface *water_grid = StandartSurfaces::RegularGrid(8,8,false);
	
	Entity *water = s->Add(water_grid);
	water->SetShader(water_shader);
	water->SetScale(1000,1,1000);

	Texture* water_refl[64];
	for(int i=0;i<64;++i)
	{
		char buff[255];
		sprintf(buff,"../../../../samples/media/normal%d.png",i);

		water_refl[i] = eng->LoadTexture(buff);
	}

	float time =0;
	bool wire  = false;

	int water_index  =0;

	RenderTarget *rtt_refl = eng->CreateRenderTarget();
	rtt_refl->CreateDepthTexture();
	Texture *reflection = rtt_refl->CreateColorTexture();

	RenderTarget *rtt_refr = eng->CreateRenderTarget();
	Texture *world_z = rtt_refr->CreateDepthTexture();
	Texture *refraction = rtt_refr->CreateColorTexture();


	water->SetTexture(water_nm,0);

	water->SetTexture(sky_tex,1);
	water->SetTexture(refraction,2);
	water->SetTexture(reflection,3);
	water->SetTexture(world_z,4);

	cam->SetPosition(0,50,0);

	water_shader->Compile();
	water_shader->SetUniform("screen_size",vec2(c->GetWidth(),c->GetHeight()));

	cam->Projection(75,float(c->GetWidth())/float(c->GetHeight()),1,1000);

	water_shader->SetUniform("zNear",1.0f);
	water_shader->SetUniform("zFar",1000.0f);

    while(wnd->isRunning())
    {
		water_index++;
		if(water_index>63)
			water_index = 0;

		water->SetTexture(water_refl[water_index],0);

		if(KeyHit('R'))
		{
			wire = ! wire;
			cam->SetDrawMode(wire?Camera::DM_WIREFRAME:Camera::DM_SOLID);
		}

        //TODO: Place your rendering code here
		float dt = Timer::Delta();
		time += dt*0.001;
		water_shader->SetUniform("time",time);

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

		//sky->SetPosition(cam);
		//sky->SetPositionY(0);
		
		irregular_landscape->SetUniform(Shader::SU_EYEPOS,cam->GetPosition(false));
		irregular_landscape->SetUniform("viewproj",cam->GetViewProjectionMatrix());

		sky->SetPosition(cam->GetPosition(false));

		water->SetVisible(false);
		rtt_refr->Bind();
		s->Render();
		rtt_refr->UnBind();
		water->SetVisible(true);

		s->Render();

		wchar_t wbuff[80];
		swprintf(wbuff,L"FPS: %d",eng->GetStatistics()->GetFPS());

		c->SetBlendMode(BM_ALPHA);
		c->SetRotate(0);
		c->SetColor(1,1,1);

		c->Text(10,10,wbuff);

        wnd->Flip();
    }

    eng->Free();
	return 0;
}