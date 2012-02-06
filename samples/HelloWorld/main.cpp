#include <axelynx\axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
    Window *wnd = eng->AddWindow(800,600);
	wnd->VSync(false);

	Canvas *c = eng->GetCanvas();
	
	Font *fnt = eng->LoadFreeTypeFont(Font::Desc(L"../../../../samples/media/CODE2000.ttf"));
	c->SetFont(fnt);

    while(wnd->isRunning())
    {
		c->Clear();
		c->SetBlendMode(BM_ALPHA);


		c->Text(10,10,L"Hello from axelynx");
		
		wnd->Flip();
    }

    eng->Free();
	return 0;
}