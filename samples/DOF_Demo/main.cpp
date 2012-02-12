#include <axelynx/axelynx.h>


using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();

        Window *wnd = eng->AddWindow(800,600,32,WM_WINDOWED,4);

		wnd->VSync(false);

        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();
		Canvas *c = eng->GetCanvas();

		//cam->SetDrawMode(Camera::DM_WIREFRAME);

        Surface *cube = StandartSurfaces::Grid(256,256,16,16,true);
        Entity *body = s->Add(cube);
		//body->SetScale(256);

		Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/rockwall.tga",Texture::Desc().Anisotropic(16.0));
		Texture *relief = eng->LoadTexture(L"../../../../samples/media/rockwall_relief.tga",Texture::Desc().Anisotropic(16.0));

		Material *paralax  = eng->CreateMaterial();
		paralax->SetShader(StandartShaders::Render::Paralax());
		paralax->SetTexture(diffuse,0);
		paralax->SetTexture(relief,1);
		StandartShaders::Render::Paralax()->SetUniform("numSteps",8.0f);
		body->SetMaterial(paralax);

		Surface *s_teapot = eng->LoadSurface(L"../../../../samples/media/Teapot_4096.axs");

		Texture *t_dharmabox = eng->LoadTexture(L"../../../../samples/media/dharma_box_mips.dds");

		const int COUNT_TEAPOTS = 100;

		Entity* ents[COUNT_TEAPOTS];
		quat rots[COUNT_TEAPOTS];

		for(int i=0;i<COUNT_TEAPOTS;++i)
		{
			ents[i] = s->Add(s_teapot);
			ents[i]->SetShader(StandartShaders::Render::TexturingLighting());
			ents[i]->SetTexture(t_dharmabox);
			ents[i]->SetPosition(rnd(-128,128),rnd(2,10),rnd(-128,128));
			ents[i]->SetScale(rnd(1,5));
			ents[i]->SetOrientation(rnd(360),rnd(360),rnd(360));

			rots[i] = quat(vec3(rnd(-0.1,0.1),rnd(-0.5,0.5),rnd(-0.1,0.1)));

		}


		vec3 transl;

		float time =0;

		bool wire = false;

		RenderTarget *rtt = eng->CreateRenderTarget(800,600);
		Texture *full_tex = rtt->CreateColorTexture(4,1,0,true);
		Texture *full_depth = rtt->CreateDepthTexture();

		StandartShaders::Draw::PositionUV()->Bind();
		Shape *rect = Shape::Rect(800,600,false);
		Shape *half_rect = Shape::Rect(400,300,false);

		StandartShaders::Draw::PositionColor()->Bind();
		Shape *colored_rect = Shape::Rect(800,600,false);

		RenderTarget *half_rtt = eng->CreateRenderTarget(400,300);
		RenderTarget *half_rtt_result = eng->CreateRenderTarget(400,300);

		Texture::Desc desc;
		desc.TexType(Texture::TT_3D).UseMipmaps(false).Size(400,300,8);
		desc.cpp = 4;
		desc.bpc = 1;

		Texture *dof_tex_vert = eng->CreateTexture(desc);
		Texture *dof_tex_result = eng->CreateTexture(desc);

		for(int i=0;i<8;++i)
		{
			half_rtt->Bind3DTexture(dof_tex_vert,i,i);
		}

		for(int i=0;i<8;++i)
		{
			half_rtt_result->Bind3DTexture(dof_tex_result,i,i);
		}

		Shader *dof = eng->LoadShader(L"../../../../samples/media/dof");
		dof->Compile();

		Shader *fake_dof = eng->LoadShader(L"../../../../samples/media/fake_dof");
		fake_dof->Compile();

		Shader *vertical_blur = eng->LoadShader(L"../../../../samples/media/vertical_blur");
		vertical_blur->Compile();

		Shader *horizontal_blur = eng->LoadShader(L"../../../../samples/media/horizontal_blur");
		horizontal_blur->Compile();

		fake_dof->SetUniform("depth",1);

		fake_dof->SetUniform("focal_distance",30.0f);
		fake_dof->SetUniform("focal_range",300.0f);

		fake_dof->SetUniform("zFar",1000.0f);
		fake_dof->SetUniform("zNear",1.0f);

		dof->SetUniform("depth",1);
		dof->SetUniform("dof_tex",2);

		dof->SetUniform("focal_distance",30.0f);
		dof->SetUniform("focal_range",300.0f);

		dof->SetUniform("zFar",1000.0f);
		dof->SetUniform("zNear",1.0f);

		cam->Projection(90.0f,800.0f/600.0f,1.0,1000.0);


		vertical_blur->SetUniform("delta",1.0f / 300.0f);
		horizontal_blur->SetUniform("delta",1.0f / 400.0f);

		bool fake = true;



        while(wnd->isRunning())
        {
			if(KeyHit('F'))
			{
				fake = !fake;
			}

			for(int i=0;i<COUNT_TEAPOTS;++i)
			{
				ents[i]->Turn(rots[i]);
			}
			c->Clear();

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

			StandartShaders::Render::Paralax()->SetUniform(Shader::SU_EYEPOS,cam->GetPosition(false));
			StandartShaders::Render::Paralax()->SetUniform(Shader::SU_LIGHTPOS,cam->GetPosition(false));
			StandartShaders::Render::TexturingLighting()->SetUniform(Shader::SU_LIGHTPOS,cam->GetPosition(false));

			rtt->Bind();
			s->Render();
			rtt->UnBind();

			full_tex->RegenerateMipmaps();

			if(fake)
			{

				full_tex->Bind(0);
				full_depth->Bind(1);
				fake_dof->Bind();
				c->SetScale(1,-1);
				c->SetPosition(0,600);
				c->Draw(rect);
				fake_dof->UnBind();
				full_depth->UnBind();
				full_tex->UnBind();
			}
			else
			{
				half_rtt->Bind();
				full_tex->Bind(0);
				vertical_blur->Bind();				
				c->Clear();
				c->SetScale(1,-1);
				c->SetPosition(0,300);
				c->Draw(half_rect);
				vertical_blur->UnBind();
				full_tex->UnBind();
				half_rtt->UnBind();

				half_rtt_result->Bind();
				dof_tex_vert->Bind(0);
				horizontal_blur->Bind();
				c->SetScale(1,-1);
				c->SetPosition(0,300);
				c->Draw(half_rect);
				horizontal_blur->UnBind();
				dof_tex_vert->UnBind();
				half_rtt_result->UnBind();

				full_tex->Bind(0);
				full_depth->Bind(1);
				dof_tex_vert->Bind(2);
				dof->Bind();
				c->SetScale(1,-1);
				c->SetPosition(0,600);
				c->Draw(rect);
				dof->UnBind();

				dof_tex_vert->UnBind();
				full_depth->UnBind();
				full_tex->UnBind();

			}

			c->SetPosition(10,10);
			c->Print("FPS:%d",eng->GetStatistics()->GetFPS());

			c->SetBlendMode(BM_NONE);
            wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}