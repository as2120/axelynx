#include <axelynx\axelynx.h>
#include <iostream>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();			//инициалищация движка
	Window *wnd = eng->AddWindow(800,600);	//инициализация окна
	wnd->VSync(false);

	Scene *scene = eng->AddScene();			//добавляем сцену
	Camera *cam = scene->AddCamera();		//добавляем камеру

	Surface *cube = StandartSurfaces::Cube();	//создаем меш куба (из стандартных мешей)
	Material *cube_material = eng->CreateMaterial(); //создаем пустой материал
	cube_material->SetShader(StandartShaders::Render::Texturing()); //назначаем материалу стандартный шейдер (освещение)
	cube_material->SetBlend(BM_ADD);
	//StandartShaders::Render::OnlyNormals()->SetUniform(Shader::SU_LIGHTPOS,vec3(-100,200,-300)); //задаем стандартному шейдеру позицию источника освещения

	class Turner : public Entity::Action
	{
		float dx,dy,dz;
	public:
		Turner()
		{
			dx = rnd(-0.5,0.5);
			dy = rnd(-0.5,0.5);
			dz = rnd(-0.5,0.5);
		}

		virtual bool operator()(Entity* ent,float twin)
		{
			ent->Turn(dx*twin,dy*twin,dz*twin);
			return false;
		}
	};

	for(int i = 0;i<1000;++i) 
	{
		Entity* ent = scene->Add(cube); //добавляем в сцену entity с мешем куба
		ent->SetMaterial(cube_material); //назначаем материал
		ent->SetPosition(rnd(-50,50),rnd(-50,50),rnd(-50,50)); //задаем позицию
		ent->SetOrientation(rnd(0,360),rnd(0,360),rnd(0,360));
		ent->OnUpdate(new Turner());
	}

	std::cout<<"trace 1"<<std::endl;
	Theora *video = eng->LoadTheoraVideo(L"../../../../samples/media/bleach.asf");
	Texture *tex = video->GetDiffuseTexture();
	
	//eng->VSync(true);

	Texture *tex2 = eng->LoadTexture(L"../../../../samples/media/hypercube.jpg");
	cube_material->SetTexture(tex);
	int fr = 0;

	while(wnd->isRunning())	//пока окно не закрыли
	{
		if(KeyHit(KEY::F1))
			wnd->ChangeResolution(800,600);
		if(KeyHit(KEY::F2))
			wnd->ChangeResolution(800,600,WM_FULLSCREEN);
		if(KeyHit(KEY::F3))
			wnd->ChangeResolution();

		if(++fr % 2)
			video->Update();

		float dt = Timer::Delta()*0.1; //дельтатайм возвращает миллисекунды в флоат виде
		cam->Turn(0.1f *dt,-0.3 * dt,0); //поворачиваем камеру
		eng->GetCanvas()->Clear();

		scene->Update(dt);

		scene->Render(); //рендерим сцену

		wchar_t buff[255];
		swprintf(buff,L"FPS: %d",eng->GetStatistics()->GetFPS());
		wnd->SetText(buff);

		wnd->Flip();	//отобразить содержимое и синхронизировать с системой
	}

	eng->Free(); //освободить ресурсы
	return 0;
}