#include <axelynx\axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
    Window *wnd = eng->AddWindow(800,600);
	wnd->VSync(false);

	Canvas *c = eng->GetCanvas();
	
	Font *fnt = eng->LoadFreeTypeFont(Font::Desc(L"../../../../samples/media/CODE2000.ttf"));
	c->SetFont(fnt);


	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();

	Surface *pipes = eng->LoadSurface(L"../../../../samples/media/pipes.axs");
	pipes->RecalcTangents();

	Surface *light_sphere = eng->LoadSurface(L"../../../../samples/media/geosphere.axs");

	Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rusted_metal.png");
	Texture *normalspecularmap = eng->LoadTexture(L"../../../../samples/media/rusted_metal_nsm.png");

	Shader *specularbump = eng->LoadShader(L"../../../../samples/media/specular_bump_mapping");
	specularbump->Compile();
	specularbump->SetUniform("lightRadius",256);

	for(int i=0;i<10;++i)
	{
		Entity *world = s->Add(pipes);
		world->SetShader(specularbump);

		world->SetPosition(rnd(-500,500),rnd(-500,500),rnd(-500,500));
		world->SetScale(rnd(0.1,3.0));
		world->SetOrientation(rnd(360),rnd(360),rnd(360));

		world->SetTexture(diffuse,0);
		world->SetTexture(normalspecularmap,1);
	}

	vec3 transl;

    while(wnd->isRunning())
    {
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

		transl *= 0.9;

		cam->Translate(transl * dt * 0.1);


		c->Clear();

		specularbump->SetUniform(Shader::SU_LIGHTPOS,cam->GetPosition(false));

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