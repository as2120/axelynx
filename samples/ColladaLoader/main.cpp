#include <axelynx\axelynx.h>

using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	Window *wnd = eng->AddWindow(800,600);
	//TODO: Place your resource loading and scene creating code here

	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();
	cam->SetPosition(0,0,200);
	//Entity *battlecruiser = B3DLoader::Load("Battlecruiser_imp.b3d",s);
	Timer::Delta();
	Entity *battlecruiser = s->LoadEntity("../../../../samples/media/bigscene.dae");
	battlecruiser->SetShader(StandartShaders::Render::Texturing());

	File file("../../../../samples/media/bigscene.axe");
	file.Create();
	battlecruiser->Save(file);
	file.Close();

	

	Texture *tex = eng->LoadTexture(L"../../../../samples/media/Fieldstone.png");
	//Surface *sb_s = eng->RestoreSurface("apache.abs");

	wchar_t buff[90];
	swprintf(buff,L"loading time: %f",Timer::Delta());
	shell::ShowMessage(buff);

//	Entity *battlecruiser = s->Add(sb_s);

	//battlecruiser->SetShader(StandartShaders::Render::TexturingLighting());
	
	battlecruiser->SetTexture(tex);
	vec3 transl;

	cam->ClearColor(0.2,0.2,0.2);

	while(wnd->isRunning() && !KeyDown(KEY::ESCAPE))
	{


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

		s->Render();
		wnd->Flip();
	}
	//TODO: Place your deinitalize code here
	eng->Free();
}