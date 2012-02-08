#include <axelynx\axelynx.h>
using namespace axelynx;


void DrawTexture(Canvas *c, Texture *tex,axDrawable *shape)
{
	c->SetBlendMode(BM_NONE);
	c->SetPosition(0,c->GetHeight());
	c->SetScale(1,-1);
	StandartShaders::Draw::PositionUV()->Bind();
	tex->Bind();
	c->Draw(shape);
	tex->UnBind();
	StandartShaders::Draw::PositionUV()->UnBind();
}

class DefferedLight
{
	Entity *ent_;
	SpriteSystem::Sprite *s_;
	float size_;
	float spd_ ;
public:

	static bool OnRender(Entity* ent)
	{
		auto shader = Shader::GetCurrent();
		if(shader)
		{
			shader->SetUniform("lightpos",ent->GetPosition(false));
			shader->SetUniform("lightsize",float(ent->tag / 256.0f));
		}
		return true;
	}

	void initialize()
	{
		
		size_ = rnd(45,120);

		ent_ -> SetPosition(rnd(-500,350),rnd(500,900)+size_,rnd(-500,500));

		spd_ = 0.61 / sqrt(size_);

		ent_ -> SetScale(size_);

		ent_->tag = size_ * 256.0f;
	}

	DefferedLight(Entity *ent,SpriteSystem::Sprite *s)
	{
		ent_ = ent;
		s_ = s;
		initialize();
		ent_ -> SetPosition(rnd(-500,350),rnd(-500,900)+size_,rnd(-500,500));
		ent_->OnRender(DefferedLight::OnRender);
	}

	~DefferedLight();

	void update(float twin)
	{
		ent_->Translate(vec3(0,-spd_ * twin,0));
		s_ ->SetPosition(ent_->GetPosition(false));

		s_ -> SetSize(sqrt(size_)+rnd(5.0));
		s_ ->SetColor(rnd(0.8,1.0),rnd(0.7,0.8),rnd(0.5,0.7),1);

		if(ent_->GetPosition(false).y < -600 - size_)
			initialize();
	}
};

int main()
{
	Engine *eng = Engine::Init();
    Window *wnd = eng->AddWindow(1280,720);
	//Window *wnd = eng->AddWindow(640,480);

	wnd->VSync(false);

	Canvas *c = eng->GetCanvas();
	
	Font *fnt = eng->LoadFreeTypeFont(Font::Desc(L"../../../../samples/media/CODE2000.ttf"));
	c->SetFont(fnt);


	Scene *s = eng->AddScene();

	const int GEOMETRY_GROUP = 0;
	const int LIGHT_GROUP = 1;

	s->SetSceneGraph(LIGHT_GROUP,eng->GetDefaultSceneGraph()); //создаем ценеграйф для сфер света

	Camera *cam = s->AddCamera();
	cam->Projection(80.0f,float(wnd->GetWidth())/float(wnd->GetHeight()),5.0f,10000.0f);

	Surface *pipes = eng->LoadSurface(L"../../../../samples/media/pipes.axs");
	pipes->RecalcTangents();
	//Entity *cashed = s->Add(pipes);
	//cashed->Save(L"../../../../samples/media/pipes.axe");

	Surface *light_sphere = eng->LoadSurface(L"../../../../samples/media/geosphere.axs");
	light_sphere->AutoCenter();

	Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rusted_metal.png");
	Texture *normalspecularmap = eng->LoadTexture(L"../../../../samples/media/forestfloornrmii7.jpg");

	//Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rusted_metal.png");
	//Texture *normalspecularmap = eng->LoadTexture(L"../../../../samples/media/rusted_metal_nsm.png");
	

	Shader *specularbump = eng->LoadShader(L"../../../../samples/media/specular_bump_mapping_deffered");
	Shader *deffered_lighting = eng->LoadShader(L"../../../../samples/media/deffered_lighting");

	specularbump->Compile();
	specularbump->SetUniform("lightRadius",256);

	deffered_lighting->Compile();
	deffered_lighting->SetUniform("screen_size",vec2(c->GetWidth(),c->GetHeight()));

	Material *pipesMat = eng->CreateMaterial();

	pipesMat->SetTexture(diffuse,0);
	pipesMat->SetTexture(normalspecularmap,1);
	pipesMat->SetCullingMode(CM_FRONT);

	pipesMat->SetShader(specularbump);
	pipesMat->SetZEarlyPassShader(StandartShaders::Render::TransformOnly());

	pipesMat->SetDepthWriteMode(DW_WRITE);
	pipesMat->SetDepthTestMode(DT_TEST);

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

		//world->SetScale(9.1,9.1,9.1);
	}

	vec3 transl;

	RenderTarget *rtt = eng->CreateRenderTarget();

	Texture *depth_tex = rtt->CreateDepthTexture();


	Texture *color_tex = rtt->CreateColorTexture(4,1,0);
	Texture *normal_tex = rtt->CreateColorTexture(4,1,1);
	Texture *position_tex = rtt->CreateColorTexture(4,2,2);

	Texture *flare_tex = eng->LoadTexture(L"../../../../samples/media/flare.png");

	
	StandartShaders::Draw::PositionUV()->Bind();
	Shape *screensize_rect = Shape::Rect(wnd->GetWidth(),wnd->GetHeight(),false);

	int cnt_lights = 1000;
	std::list<DefferedLight*> def_lights;

	float rad = light_sphere->GetRadius();

	SpriteSystem *ss = eng->CreateSpriteSystem(cnt_lights);
	ss->SetRecalcMode(SpriteSystem::RM_ONDRAW);

	Entity *ss_ent = s->Add(ss,LIGHT_GROUP);

	ss_ent->SetShader(StandartShaders::Render::PointSprite());
	ss_ent->SetTexture(flare_tex,0);
	ss_ent->SetBlend(BM_ADD);

	Material *def_light_mat = eng->CreateMaterial();

	def_light_mat->SetShader(deffered_lighting);
	//ent->SetShader(StandartShaders::Render::OnlyNormals());
	def_light_mat->SetTexture(color_tex,0);
	def_light_mat->SetTexture(normal_tex,1);
	def_light_mat->SetTexture(position_tex,2);
	def_light_mat->SetBlend(BM_ADD);
	def_light_mat->SetCullingMode(CM_DISABLE);
	def_light_mat->SetDepthTestMode(DT_NONE);
	def_light_mat->SetDepthWriteMode(DW_NONE);

	for(int i=0;i<cnt_lights;++i)
	{
		Entity *ent = s->Add(light_sphere,LIGHT_GROUP);
		ent->SetMaterial(def_light_mat);

		SpriteSystem::Sprite *s = ss->AddInstance();

		def_lights.push_back(new DefferedLight(ent,s));

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

		deffered_lighting->SetUniform(Shader::SU_EYEPOS,cam->GetPosition(false));

		s->SetVisibleGroup(GEOMETRY_GROUP,true);
		s->SetVisibleGroup(LIGHT_GROUP,false);

		rtt->Bind();
		s->Render();
		rtt->UnBind();

		s->ZPassRender();



		s->SetVisibleGroup(GEOMETRY_GROUP,false);
		s->SetVisibleGroup(LIGHT_GROUP,true);

		for(auto ci = def_lights.begin(), ei = def_lights.end(); ci != ei; ++ci)
		{
			(*ci)->update(dt);
		}

		s->Render();

		//s->SetVisibleGroup(GEOMETRY_GROUP,true);
		//s->SetVisibleGroup(LIGHT_GROUP,false);
		//s->Render();

		if(KeyDown('C')) DrawTexture(c,color_tex,screensize_rect);
		if(KeyDown('N')) DrawTexture(c,normal_tex,screensize_rect);
		if(KeyDown('P')) DrawTexture(c,position_tex,screensize_rect);
		

		c->SetBlendMode(BM_ALPHA);
		c->SetRotate(0);
		c->SetColor(1,1,1);
		c->SetPosition(10,10);
		c->SetScale(1,1);
		vec3 campos = cam->GetPosition(false);

		c->Print(L"FPS: %d, trises: %d",eng->GetStatistics()->GetFPS(), eng->GetStatistics()->TrisRendered());

		wnd->Flip();
    }

    eng->Free();
	return 0;
}