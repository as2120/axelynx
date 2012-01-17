#include <axelynx/axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	Window *wnd = eng->AddWindow(800,600);
	//TODO: Place your resource loading and scene creating code here
	wnd->VSync(false);

	Scene *s = eng->AddScene();
	Camera *cam = s->AddCamera();
	Canvas *canvas = eng->GetCanvas();

	cam->Projection(45.0f,(float)wnd->GetWidth() / (float)wnd->GetHeight(),1.0f,10000.0f);

	Surface *headcrab = StandartSurfaces::Cube(100);

	RenderTarget *rtt = eng->CreateRenderTarget(2048,2048);
	Texture *diffus = rtt->CreateColorTexture(3,2);

	rtt->Bind();
	canvas->Clear();
	rtt->UnBind();

	int count = 100;
	Entity **cubes = new Entity*[count];

	for(int i=0;i<count;++i)
	{
		cubes[i] = s->Add(headcrab);

		cubes[i]->SetPosition(rnd(-2000,2000),rnd(-2000,2000),rnd(-2000,2000));
		cubes[i]->SetShader(StandartShaders::Render::Texturing());
		cubes[i]->SetTexture(diffus,0);
		cubes[i]->SetOrientation(rnd(0,360),rnd(0,360),rnd(0,360));
	}

	float dx=0,dy=0;

	vec3 transl;

	Font::Desc desc(L"../../../../samples/media/CODE2000.ttf");	
	desc.size = 16;
	Font *fnt = eng->LoadFont(desc);

	
	Shader *gass = StandartShaders::Draw::Position();
	gass->Bind();
	Shape *rect = Shape::Rect(2048,2048,false);
	gass->UnBind();

	while(wnd->isRunning() && !KeyDown(KEY::ESCAPE))
	{
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

		rtt->Bind();
		fnt->StartDraw(canvas);
		//canvas->Clear();

		canvas->SetPosition(vec2(0));
		canvas->SetRotate(0);
		canvas->SetColor(0.98,0.98,0.98);
		canvas->SetAlpha(1.0);
		canvas->SetBlendMode(BM_MULTIPLY);
		gass->Bind();
		canvas->Draw(rect);
		gass->UnBind();
		canvas->SetBlendMode(BM_ALPHA);

		for(int i=0;i<100;++i)
		{
			swprintf(wbuff,L"%d,%d,%d,%d",rand(0,999999),rand(0,999999),rand(0,999999),rand(0,999999));

			canvas->SetPosition(vec2(rnd(2048),rnd(2048)));
			canvas->SetColor(rnd(0.1),rnd(0.2,1.0),rnd(0.3));
			canvas->SetRotate(rnd(360));

			fnt->Draw(canvas,wbuff);
		}
		fnt->EndDraw();
		
		

		rtt->UnBind();
		canvas->SetBlendMode(BM_NONE);

		wnd->Flip();
	}
	//TODO: Place your deinitalize code here
	eng->Free();
}