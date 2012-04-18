// ttttest.cpp : Defines the entry point for the application.
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <axelynx/axelynx.h>
#include <tchar.h>

using namespace axelynx;


int main(int argc, const char* argv[])
{
	
	Engine *eng = Engine::Init();
	Window *wnd = eng->AddWindow(2560,1440,32,WM_SPLASH,4);
	wnd->SetText(L"PROJECT_NAME");
	wnd->VSync(false);
    //TODO: Place your resource loading and scene creating code here

    Scene *s = eng->AddScene();
    Camera *cam = s->AddCamera();
    Surface *cube = StandartSurfaces::Cube();
    Entity *body = s->Add(cube);

	StandartShaders::Render::OnlyNormals()->SetUniform(Shader::SU_LIGHTPOS,vec3(0,5,7));

	Shader *shader = eng->LoadShader("YUV2toRGB");
	shader->Compile();
	shader->SetUniform("video_width",1280.0f);
	shader->SetUniform("video_height",720.0f);

	Texture *tex = eng->LoadTexture(L"yuv2.png",Texture::Desc().Anisotropic(16.0f),L"png"); //RG texture
	tex->SetRepeatMode(Texture::RM_MIRROR);
    body->SetShader(shader);
    body->SetPosition(0,0,-60);
	body->SetTexture(tex);

    
	body->SetScale(10.0 * 1.33,10.0,10.0);

    float dx=0,dy=0;
		vec3 transl;
    while(wnd->isRunning())
    {
		//TODO: Place your rendering code here
		float dt = Timer::Delta() * 0.01;

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
            wnd->Flip();
    }
    //TODO: Place your deinitalize code here
    eng->Free();
	return 0;
}