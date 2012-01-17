//#define LOG_ON_COUT

#include <axelynx\axelynx.h>

#include <iostream>
#include <wchar.h>

using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
	eng->AddWindow(800,600,32,WM_SIZEBLE);
	Canvas *c = eng->GetCanvas();
	eng->VSync(false);
	float a=0;
	const int rect_count = 10000;

	ImageSystem *folders = eng->LoadImageSystem(L"../../../../samples/media/Star-Saphire.png",rect_count);

	std::cout<<eng->GetInfo().vendor<<std::endl;
	std::cout<<eng->GetInfo().renderer<<std::endl;
	std::cout<<eng->GetInfo().driver<<std::endl;



	struct rectos
	{
	  float x,y;
	  float dx,dy;
	  float scale,rot;
	  float dscale,drot;
	  vec4 color;
	};

	rectos *rs = new rectos[rect_count];

	Image *f = folders->GetImage();
	for(int i=0;i<rect_count;++i)
	{
		  rs[i].x=rnd(0,750);
		  rs[i].y=rnd(0,550);
		  rs[i].dx=rnd(-100,100);
		  rs[i].dy=rnd(-100,100);
		  rs[i].scale=rnd(0.5f,1.5f);
		  rs[i].dscale=rnd(-0.5f,0.5f);
		  rs[i].rot=rnd(0,360);
		  rs[i].drot=rnd(-0.1,0.1);
		  rs[i].color = vec4(1);
	}

	
	int ct = Timer::Millisecs();

	int frames = 0;

	int mode = 0;

	//PixMap *cursor = eng->LoadPixMap(L"jpg.jpg");
	//Texture *pm = ifr->LoadTexture(L"folder.bmp");


	Timer::Delta();

	while(eng->isRunning()&& !KeyDown(KEY::ESCAPE) )
	{
		if(KeyDown(KEY::F1))
		{
			mode = 1-mode;
		}

		frames++;

		if(ct < Timer::Millisecs() - 1000)
		{
			wchar_t buff[80];

			swprintf(buff,L"FPS:%d",frames);

			eng->SetText(buff);

			frames =0;

			ct = Timer::Millisecs(); 
		}

		c->Clear();

		float dt = Timer::Delta() * 0.001;

		for(int i=0;i<rect_count;++i)
		{
			if(eng->isActive())
			{
				rs[i].x+=rs[i].dx*dt;
				if(rs[i].x>760 || rs[i].x<0)
					rs[i].dx=-rs[i].dx;
				rs[i].y+=rs[i].dy*dt;
				if(rs[i].y>560 || rs[i].y<0)
					rs[i].dy=-rs[i].dy;
				rs[i].scale+=rs[i].dscale*dt;
				if(rs[i].scale>1.5f || rs[i].scale<0.5)
					rs[i].dscale=-rs[i].dscale;
				rs[i].rot+=rs[i].drot*dt;
			}

			
			f->Position(vec2(rs[i].x,rs[i].y));
			f->Rotate(rs[i].rot);
			f->Scale(vec2(rs[i].scale*64,rs[i].scale*64));
			
			f->Draw();
		}
		
		c->SetBlendMode(BM_ALPHA);
		c->Draw(folders);
		c->SetBlendMode(BM_NONE);

		//cursor->Draw(MouseX(),MouseY());
		eng->Flip();
		eng->Sync();

	}

	eng->Free();
	std::cout<<"4"<<std::endl;
}