#include <axelynx\axelynx.h>
using namespace axelynx;



class DefferedLight
{
	Entity *ent_;
	float size_;
	float spd_ ;
public:

	static bool OnRender(Entity* ent)
	{
		auto shader = Shader::GetCurrent();
		if(shader)
			shader->SetUniform("lightsize",float(ent->tag / 256.0f));
		return true;
	}

	void initialize()
	{
		ent_ -> SetPosition(rnd(-300,300),rnd(200,500),rnd(-300,300));
		size_ = rnd(5,150);
		spd_ = 0.1 / sqrt(size_);

		ent_ -> SetScale(size_  /158.0);
		ent_->tag = size_ * 256.0f;
	}

	DefferedLight(Entity *ent)
	{
		ent_ = ent;
		initialize();
		ent_->OnRender(DefferedLight::OnRender);
	}

	~DefferedLight();

	void update(float twin)
	{
		ent_->Translate(vec3(0,-spd_ * twin,0));

		if(ent_->GetPosition(false).y < -400)
			initialize();
	}
};

int main()
{
	Engine *eng = Engine::Init();
    Window *wnd = eng->AddWindow(800,600);

	wnd->VSync(false);

	Canvas *c = eng->GetCanvas();
	
	Font *fnt = eng->LoadFreeTypeFont(Font::Desc(L"../../../../samples/media/CODE2000.ttf"));
	c->SetFont(fnt);


	Scene *s = eng->AddScene();

	const int GEOMETRY_GROUP = 0;
	const int LIGHT_GROUP = 1;

	s->SetSceneGraph(LIGHT_GROUP,eng->GetDefaultSceneGraph()); //создаем ценеграйф для сфер света

	Camera *cam = s->AddCamera();

	Surface *pipes = eng->LoadSurface(L"../../../../samples/media/pipes.axs");
	//pipes->RecalcTangents();
	//Entity *cashed = s->Add(pipes);
	//cashed->Save(L"../../../../samples/media/pipes.axe");

	Surface *light_sphere = eng->LoadSurface(L"../../../../samples/media/geosphere.axs");

	Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rusted_metal.png");
	Texture *normalspecularmap = eng->LoadTexture(L"../../../../samples/media/rusted_metal_nsm.png");

	Shader *specularbump = eng->LoadShader(L"../../../../samples/media/specular_bump_mapping_deffered");
	Shader *deffered_lighting = eng->LoadShader(L"../../../../samples/media/deffered_lighting");

	specularbump->Compile();
	specularbump->SetUniform("lightRadius",256);

	deffered_lighting->Compile();

	Material *pipesMat = eng->CreateMaterial();

	pipesMat->SetTexture(diffuse,0);
	pipesMat->SetTexture(normalspecularmap,1);
	pipesMat->SetCullingMode(CM_FRONT);

	pipesMat->SetShader(specularbump);

	//pipesMat->SetZEarlyPassShader(StandartShaders::Render::TransformOnly());

	Pivot *geometries = s->AddPivot();
	Pivot *lights = s->AddPivot();

	for(int i=0;i<10;++i)
	{
		Entity *world = s->Add(pipes,GEOMETRY_GROUP);
		world->SetMaterial(pipesMat);

		world->SetPosition(rnd(-200,200),rnd(-200,200),rnd(-200,200));
		world->SetScale(rnd(0.1,3.0));
		world->SetOrientation(rnd(360),rnd(360),rnd(360));
	}

	vec3 transl;

	RenderTarget *rtt = eng->CreateRenderTarget();

	Texture *depth_tex = rtt->CreateDepthTexture();

	Texture *position_tex = rtt->CreateColorTexture(4,4,2);
	Texture *color_tex = rtt->CreateColorTexture(4,2,0);
	Texture *normal_tex = rtt->CreateColorTexture(4,2,1);

	StandartShaders::Draw::PositionUV()->Bind();
	Shape *screensize_rect = Shape::Rect(wnd->GetWidth(),wnd->GetHeight(),false);

	int cnt_lights = 10;
	std::list<DefferedLight*> def_lights;

	for(int i=0;i<cnt_lights;++i)
	{
		Entity *ent = s->Add(light_sphere,LIGHT_GROUP);
		ent->SetShader(deffered_lighting);

		def_lights.push_back(new DefferedLight(ent));
	}

    while(wnd->isRunning())
    {
		if(KeyHit(KEY::SPACE))
		{
			delete specularbump;
			specularbump = eng->LoadShader(L"../../../../samples/media/specular_bump_mapping_deffered");
			specularbump->Compile();
			specularbump->SetUniform("lightRadius",256);
			pipesMat->SetShader(specularbump);
		}

		float dt = Timer::Delta();

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

		transl *= 0.95;

		cam->Translate(transl * dt * 0.01);




		specularbump->SetUniform(Shader::SU_EYEPOS,cam->GetPosition(false));
		specularbump->SetUniform(Shader::SU_LIGHTPOS,vec3(0,0,0));

		//specularbump->SetUniform(Shader::SU_LIGHTPOS,cam->GetPosition(false));

		s->SetVisibleGroup(GEOMETRY_GROUP,true);
		s->SetVisibleGroup(LIGHT_GROUP,false);

		rtt->Bind();
		s->Render();
		rtt->UnBind();

		s->SetVisibleGroup(GEOMETRY_GROUP,false);
		s->SetVisibleGroup(LIGHT_GROUP,true);

		for(auto ci = def_lights.begin(), ei = def_lights.end(); ci != ei; ++ci)
		{
			(*ci)->update(dt);
		}

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