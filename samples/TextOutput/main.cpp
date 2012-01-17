#include <axelynx\axelynx.h>
using namespace axelynx;

const wchar_t* frases[]={
L"«Mətn» termininin iki əsas izahı vardır: «immanentli» (fəlsəfi mənalı) və «reprezentativli» (şəxsi məanalı). İmmanentli "
,L"Un text és un conjunt de paraules articulades que formen un discurs unitari, és una composició lingüística amb intenció "
,L"Тэкст (па-лацінску: textus — «тканіна, сувязь, пабудова») — паведамленьне, якое напісанае, надрукаванае (выказваньне,"
,L"Text označuje jakékoliv uskupení slov, které dohromady tvoří nějakou výpověď. Většinou ale bývá jako text označován nějaký."
,L"Termin tekst je preuzet od engleske riječi text, koja može imati više značenja u ovisnosti u kojem kontekstu se koristi."
,L"Tekst er skrift. Kombinationer af tegn kædet sammen til først ord og så sætninger. I den vestlige kulturkreds anvendes oftest."
,L"Text (von lateinisch textum ‚Gewebe, Zusammenfügung‘) bezeichnet eine abgegrenzte, zusammenhängende Äußerung in geschriebenls."
,L"A text, within literary theory, is a coherent set of symbols that transmits some kind of informative message.[citation needed]."
,L"Tekst (ladina keeles textum) on kitsamas tähenduses kirjakeele piiritletud ja kontsentreeritud vorm. Laiemas tähenduses võib"
,L"Un texto es una composición de signos codificado en un sistema de escritura (como un alfabeto) que forma una unidad de sentido. Su tamaño puede ser variable."
,L"Teksto estas registrebla formo de kohera lingva mesaĝo. Ofte oni uzas la vorton en pli limigita signifo pensante pri teksto skribita."
,L"Un texte est une succession de caractères organisée selon un langage. Cette définition désigne aussi bien une sémiotique du langage."
,L"במובן הפשוט 'מארג' (של מילים). בעברית: תמליל) הוא מונח שמקורו בתחום הבלשנות, המתאר רצף מילים, משפטים ופסקאות היוגויות אנושיות במסגרת מדעי החברה."
,L"Tekst (lat. textum: tkivo) je ograničena, povezana izjava pisanim jezikom, u širem smislu, i nepisanim jezikom, kao što je pjesma, film ili kazalište."
,L"Տեքստ (լատիներեն` texstum - բառերի կապ, հյուսվածք) - գրական երկի բնագիրը այն ձևով, ինչպես որ ստեղծվել է հեղինակի կողմից"
,L"テキストは、英単語の text が日本語に取り込まれた[要出典]語である。主に文章のことで、そこから転じて教科書、文字データなどの意味を持つ。"
,L"En text är en skriven eller muntlig utsaga som i sin kontext är meningsfull och där de språkliga enheterna har ett inbördes"
,L"Text (z lat. texere - tkať, stavať; textum - utkané) je ustálený jazykový prejav je výsledkom rečovej činnosti, t. j."
,L"Em linguística, a noção de texto é ampla e ainda aberta a uma definição mais precisa. Grosso modo, pode ser entendido como manifestaçãoos."
,L"Текст (от лат. textus «ткань; сплетение, связь, сочетание») — в общем плане связная и полная последовательность символов."
};

int main()
{
	Engine *eng = Engine::Init();
    Window *wnd = eng->AddWindow();
	wnd->VSync(false);

	Canvas *c = eng->GetCanvas();
	
	Font *fnt = eng->LoadFreeTypeFont(Font::Desc(L"../../../../samples/media/CODE2000.ttf"));
	c->SetFont(fnt);

	while(wnd->isRunning() && !KeyDown(KEY::ESCAPE))
    {
		c->Clear();
		c->SetBlendMode(BM_ADD);

		fnt->StartDraw(c);

		for(int i=0;i<100;++i)
		{
			int x = 0;
			int y = i*9;
			float a = rnd(0,360);

			c->SetPosition(x,y);
			//c->SetRotate(a);
			c->SetColor(rnd(1),rnd(1),rnd(1));

			int f = rand(sizeof(frases)/sizeof(frases[0]));

			fnt->Draw(c,frases[f],false);
		}

		fnt->EndDraw();

		wchar_t wbuff[80];
		swprintf(wbuff,L"FPS: %d",eng->GetStatistics()->GetFPS());

		c->SetBlendMode(BM_ADD);
		c->SetRotate(0);
		c->SetColor(1,1,1);

		c->Text(10,80,wbuff);
		
		wnd->Flip();
    }

    eng->Free();
	return 0;
}