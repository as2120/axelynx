#include <axelynx/axelynx.h>
using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();
        Window *wnd = eng->AddWindow(800,600,32,WM_WINDOWED,8);

        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();
		cam->SetDrawMode(Camera::DM_WIREFRAME);

        Surface *cube = StandartSurfaces::Grid(100,100,8,8,false);
        Entity *body = s->Add(cube);

		body->SetShader(StandartShaders::Render::Texturing());

		vec3 transl;

		float time =0;

        while(wnd->isRunning())
        {
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