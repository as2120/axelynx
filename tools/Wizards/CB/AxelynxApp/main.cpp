#include <axelynx/axelynx.h>
using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();
        Window *wnd = eng->AddWindow(800,600);
        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();
        Surface *cube = StandartSurfaces::Cube();
        Entity *body = s->Add(cube);
	StandartShaders::Render::OnlyNormals()->SetUniform(Shader::SU_LIGHTPOS,vec3(0,5,7));
        body->SetShader(StandartShaders::Render::OnlyNormals());
        body->SetPosition(0,0,-6);

        Entity *turret = s->Add(cube);
        turret->SetParent(body);
        turret->SetShader(StandartShaders::Render::OnlyNormals());
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

                dx *= 0.95;
                dy *= 0.95;
                body->Turn(dt*dy,dt*dx,0);
                s->Render();
                wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}
