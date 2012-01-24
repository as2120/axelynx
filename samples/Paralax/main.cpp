#include <axelynx/axelynx.h>

using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();
        Window *wnd = eng->AddWindow();
        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();

		cam->ClearColor(0.15,0.15,0.15);
		
		Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rockwall.tga",Texture::Desc().Anisotropic(16.0));
		Texture *relief = eng->LoadTexture(L"../../../../samples/media/rockwall_relief.tga",Texture::Desc().Anisotropic(16.0));

		Material *paralax  = eng->CreateMaterial();
		paralax->SetShader(StandartShaders::Render::Paralax());
		paralax->SetTexture(diffuse,0);
		paralax->SetTexture(relief,1);

		Surface *cube = eng->LoadSurface(L"../../../../samples/media/Teapot_256.axs");
		cube->RecalcTangents();

        Entity *body = s->Add(cube);
        body->SetMaterial(paralax);

		StandartShaders::Render::Paralax()->SetUniform(Shader::SU_LIGHTPOS,vec3(20,10,50));

        body->SetPosition(0,0,-6);

        Entity *turret = s->Add(cube);
        turret->SetParent(body);
        turret->SetMaterial(paralax);
        turret->SetPosition(0,0.0,-1.25);
        turret->SetScale(0.25);

        turret = turret->Copy();
        turret->SetPosition(0,0,1.25);

        turret = turret->Copy();
        turret->SetPosition(0,-1.25,0);

        turret = turret->Copy();
        turret->SetPosition(0,1.25,0);

        turret = turret->Copy();
        turret->SetPosition(-1.25,0,0);

        turret = turret->Copy();
        turret->SetPosition(1.25,0,0);

        float dx=0,dy=0;

        while(wnd->isRunning())
        {
                if(KeyDown('A'))
                        dx -= 1.0;
                if(KeyDown('D'))
                        dx += 1.0;
                if(KeyDown('W'))
                        dy -= 1.0;
                if(KeyDown('S'))
                        dy += 1.0;
                //TODO: Place your rendering code here
                float dt = Timer::Delta() * 0.01;
				dt = 0.1;
                dx *= 0.95;
                dy *= 0.95;
                body->Turn(dt*dy,dt*dx,0);
                s->Render();
                wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}