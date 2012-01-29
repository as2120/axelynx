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

	Surface *cube = StandartSurfaces::Cube();
	Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/dharma_box_mips.dds");

	for(int i=0;i<64;++i)
	{
		for(int j=0;j<64;++j)
		{
			Entity *ent = s->Add(cube);
			ent->SetPosition(i*3,0,j*3);
			ent->SetShader(StandartShaders::Render::Texturing());
			ent->SetTexture(diffuse);
		}
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