#include <axelynx\axelynx.h>
using namespace axelynx;

int main()
{
	Engine *eng = Engine::Init();
    Window *wnd = eng->AddWindow(800,600);

	Canvas *c = eng->GetCanvas();
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