#include <axelynx/axelynx.h>


using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();

        Window *wnd = eng->AddWindow();
		PhysicsContext *physics = eng->AddPhysicsContext();

		wnd->VSync(true);

        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();

		s->InitializePhysics(physics);

        Camera *cam = s->AddCamera();
		cam->ClearColor(0.2,0.3,0.4);


		Canvas *c = eng->GetCanvas();


		Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/floor.jpg",Texture::Desc().Anisotropic(8.0));

		Texture *sphere_diffuse = eng->LoadTexture(L"../../../../samples/media/16tile01.jpg",Texture::Desc().Anisotropic(2.0));
		Texture *box_diffuse = eng->LoadTexture(L"../../../../samples/media/ConcreteNew.png",Texture::Desc().Anisotropic(2.0));
		
		Entity *floor = s->Add(StandartSurfaces::Grid(1.0,1.0,16,16));
		floor->SetScale(512,1,512);

		floor->SetTexture(diffuse);
		floor->SetShader(StandartShaders::Render::Texturing());

		Shape *floor_shape = physics->AddInfinityPlane();

		Shape *sphere_shape = physics->AddSphere(1);
		Shape *box_shape = physics->AddBox(vec3(1.0f));

		Surface *sphere_surface = StandartSurfaces::Cube(1);
		Surface *box_surface = StandartSurfaces::Cube(1);

		Body *plane_body = s->AddBody(floor_shape,0,vec3(0,-1,0));	 //static body	

		//Shape *plane = physics->AddInfinityPlane(vec3(0),vec3(0,1,0));
		

		Font::Desc desc(L"arial.ttf");	
		desc.size = 16;
		Font *fnt = eng->LoadFont(desc);
		c->SetFont(fnt);

		bool wire = false;
		vec3 transl;

        while(wnd->isRunning())
        {
			if(KeyHit('R'))
			{
				wire = ! wire;
				cam->SetDrawMode(wire?Camera::DM_WIREFRAME:Camera::DM_SOLID);
			};

			if(KeyHit(1))
			{
				Entity* ent = s->Add(box_surface);

				quat orient = quat(vec3(rnd(-1,1),rnd(-1,1),rnd(-1,1)).normalizedCopy(),rnd(-1,1));

				vec3 impuls = cam->GetOrientation().ztransform(-1500.0);

				Body* body = s->AddBody(box_shape,50,cam->GetPosition());

				body->AddImpulse(impuls);

				ent->SetShader(StandartShaders::Render::TexturingLighting());
				ent->SetTexture(box_diffuse);
				ent->SetBody(body);
				//body->SetPosition(cam->GetPosition());

			}

			if(KeyHit(2))
			{
			}

            //TODO: Place your rendering code here
			float dt = Timer::Delta();
			//time += dt*0.001;


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

			vec3 oldpos = cam->GetPosition(false);
			if(oldpos.y < 2)
			{
				oldpos.y = 2;
				cam->SetPosition(oldpos);
			}

			s->Update(1.0f / 60.0f);
			s->Render();
			c->SetBlendMode(BM_ALPHA);
			c->SetColor(1,1,1);
			c->SetAlpha(1);
			c->SetRotate(0);
			c->SetScale(1);
			c->SetPosition(10,10);
			c->Print(L"FPS:%d, DIP`s:%d, tris: %d",eng->GetStatistics()->GetFPS(),eng->GetStatistics()->DIPs(),eng->GetStatistics()->TrisRendered());
			//fnt->Draw(c,L"trtrt");
			c->SetBlendMode(BM_NONE);
            wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}