#include <axelynx/axelynx.h>
using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();

        Window *wnd = eng->AddWindow(800,600,32,WM_WINDOWED,4);

        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();
		//cam->SetDrawMode(Camera::DM_WIREFRAME);

        Surface *cube = StandartSurfaces::Grid(256,256,16,16,true);
        Entity *body = s->Add(cube);

		Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rockwall.tga",Texture::Desc().Anisotropic(16.0));
		Texture *relief = eng->LoadTexture(L"../../../../samples/media/rockwall_relief.tga",Texture::Desc().Anisotropic(16.0));

		Material *paralax  = eng->CreateMaterial();
		paralax->SetShader(StandartShaders::Render::Texturing());
		paralax->SetTexture(diffuse,0);
		paralax->SetTexture(relief,1);

		body->SetMaterial(paralax);

		Surface *s_teapot = eng->LoadSurface(L"../../../../samples/media/Teapot_4096.axs");

		Texture *t_dharmabox = eng->LoadTexture(L"../../../../samples/media/dharma_box_mips.dds");

		const int COUNT_TEAPOTS = 100;

		Entity* ents[COUNT_TEAPOTS];

		for(int i=0;i<COUNT_TEAPOTS;++i)
		{
			ents[i] = s->Add(s_teapot);
			ents[i]->SetShader(StandartShaders::Render::Texturing());
			ents[i]->SetTexture(t_dharmabox);
			ents[i]->SetPosition(rnd(-128,128),rnd(2,10),rnd(-128,128));
			ents[i]->SetScale(rnd(1,5));
			ents[i]->SetOrientation(rnd(360),rnd(360),rnd(360));
		}


		vec3 transl;

		float time =0;

		bool wire = false;

		RenderTarget *rtt = eng->CreateRenderTarget(400,300);

        while(wnd->isRunning())
        {
			if(KeyHit('R'))
			{
				wire = ! wire;
				cam->SetDrawMode(wire?Camera::DM_WIREFRAME:Camera::DM_SOLID);
			};

            //TODO: Place your rendering code here
			float dt = Timer::Delta();
			time += dt*0.001;


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

            s->Render();
            wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}