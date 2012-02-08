#include <axelynx\axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	Window *wnd = eng->AddWindow(800,600);
	//TODO: Place your resource loading and scene creating code here

	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();
	Surface *cube = StandartSurfaces::Cube(1);
	Entity *body = s->Add(cube);
	body->SetShader(StandartShaders::Render::OnlyNormals());
	body->SetPosition(0,0,-300);
	body->SetScale(100);
	Material *backface = eng->CreateMaterial();
	Material *volumetric = eng->CreateMaterial();
	backface->SetShader(eng->LoadShader(L"../../../../samples/media/backface"));
	volumetric->SetShader(eng->LoadShader(L"../../../../samples/media/volumetric"));
	backface->SetCullingMode(CM_FRONT);

	float dx=0,dy=0;

	body->SetMaterial(backface);

	Font *fnt = eng->LoadFreeTypeFont(L"../../../../samples/media/font.ttf");

	Canvas *c = eng->GetCanvas();
	
	RenderTarget *back = eng->CreateRenderTarget();
	Texture *raystart = back->CreateColorTexture();
	Texture *volume = eng->LoadTexture(L"../../../../samples/media/volume.dds");

	volumetric->SetTexture(volume);
	volumetric->SetTexture(raystart,1);

	wnd->VSync(false);
	vec3 transl;

	while(wnd->isRunning())
	{

		quat qcam(vec3(float(MouseY()-300)*0.2f,float(MouseX()-400)*0.2f,0));
		cam->SetOrientation(qcam);
		//TODO: Place your rendering code here
		float dt = Timer::Delta() * 0.01;

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

		back->Bind();
		body->SetMaterial(backface);
		s->Render();
		back->UnBind();

		body->SetMaterial(volumetric);
		s->Render();

		c->SetBlendMode(BM_ALPHA);
		wchar_t wbuff[80];
		swprintf(wbuff,L"FPS = %d",eng->GetStatistics()->GetFPS());
		fnt->Draw(c,wbuff);
		wnd->Flip();
	}
	//TODO: Place your deinitalize code here
	eng->Free();
}