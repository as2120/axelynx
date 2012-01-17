
#include "CTheora.h"
#include "CFile.h"
#include "axelynx/Logger.h"
#include <iostream>

#ifdef _MSVCRT_
#include <intrin.h>
#endif

#include <stdexcept>

inline BYTE ClampFloatToByte(const float &value)
{
	BYTE result=(BYTE)value;

	value<0?result=0:NULL;
	value>255?result=255:NULL;

	return result;
}

CTheora::~CTheora()
{
	//delete[] fn;
	delete[] frameY;
	delete[] frameUV;
	delete[] frameRGB;

	delete texY;
	delete texV;
	delete texU;
	delete tex;

//закрываемси...
	theora_clear(&THEORA_STATE);
	theora_comment_clear(&THEORA_COMMENT);
	theora_info_clear(&THEORA_INFO);

	ogg_stream_clear(&OGG_STREAM_STATE_THEORA);
	ogg_sync_clear(&OGG_SYNC_STATE);

	//удаляем буфера
	//delete[] frame;

	//очищаем все структуры ogg/theora:
	memset(&OGG_STREAM_STATE_THEORA, 0x00, sizeof(ogg_stream_state));
	memset(&OGG_SYNC_STATE, 0x00, sizeof(ogg_sync_state));
	memset(&OGG_PAGE, 0x00, sizeof(ogg_page));
	memset(&OGG_PACKET, 0x00, sizeof(ogg_packet));

	memset(&THEORA_COMMENT, 0x00, sizeof(theora_comment));
	memset(&THEORA_INFO, 0x00, sizeof(theora_info));
	memset(&THEORA_STATE, 0x00, sizeof(theora_state));
	memset(&YUV_BUFFER, 0x00, sizeof(yuv_buffer));

	fclose(currentfile);
}


void CTheora::report_colorspace(theora_info *ti)
{
	switch(ti->colorspace)
	{
		case OC_CS_UNSPECIFIED:
			// nothing to report
			break;
		case OC_CS_ITU_REC_470M:

			LOG_WRITE(L"Encoder specified ITU Rec 470M (NTSC) color.");
			break;
		case OC_CS_ITU_REC_470BG:
			LOG_WRITE(L"Encoder specified ITU Rec 470BG (PAL) color.");
			break;
		default:
			LOG_WRITE(L"Warning: encoder specified unknown colorspace.");
			break;
	}
}
//---------------------------------------------------------------------------------
//печатаем информацию о хедере theora
int CTheora::dump_comments(theora_comment *tc)
{
//	std::string vendor("Encoded by ");
//	vendor+=tc->vendor;
//	debug::Log<<vendor<<std::endl;

	//for(int cc=0; cc<tc->comments; cc++)
	//{
	//	std::string usercomments=tc->user_comments[cc];
	//	debug::Log<<usercomments<<std::endl;
	//}

	return 0;
}

//---------------------------------------------------------------------------------
int CTheora::get_data_from_physical_stream(FILE *INFILE_, ogg_sync_state *OGG_SYNC_STATE_)
{
	const int DBS__=8192;
	char *buffer=ogg_sync_buffer(OGG_SYNC_STATE_,DBS__); //получаем указатель на буфер для данных

	size_t bytes=fread(buffer,1,DBS__,INFILE_); //читаем эти данные

	ogg_sync_wrote(OGG_SYNC_STATE_,(long)bytes); //указываем, сколько именно байт было записано в буфер (т.е. прочитано из файла - вдруг файл закончился!)

	return (int)bytes; //возвращаем, сколько прочитали
}
//---------------------------------------------------------------------------------
//функция, добавляет завершенные страницы в поток
int CTheora::push_page_into_logical_stream(ogg_page *page)
{
    #ifdef _MSVCRT_
	if(-1== ogg_stream_pagein(&OGG_STREAM_STATE_THEORA,page) )
		__nop();// __asm nop;	//означает, что мы пытаемся засунуть в логический поток страничку с другим серийным номером (например в логический поток theora суем пакет vorbis)
    #endif
	return 0;
}


CTheora::CTheora(axelynx::File file)
{

	fn = file.GetFilename();

//очищаем все структуры ogg/theora:
	memset(&OGG_STREAM_STATE_THEORA, 0x00, sizeof(ogg_stream_state));
	memset(&OGG_SYNC_STATE, 0x00, sizeof(ogg_sync_state));
	memset(&OGG_PAGE, 0x00, sizeof(ogg_page));
	memset(&OGG_PACKET, 0x00, sizeof(ogg_packet));

	memset(&THEORA_COMMENT, 0x00, sizeof(theora_comment));
	memset(&THEORA_INFO, 0x00, sizeof(theora_info));
	memset(&THEORA_STATE, 0x00, sizeof(theora_state));
	memset(&YUV_BUFFER, 0x00, sizeof(yuv_buffer));

	//видео файл
	currentfile = axReadFile(fn.c_str());

	if(currentfile==NULL)
	{

		LOG_WRITE(L"cannot open ogg-file");
	}
	//инициализируем основную структуру контейнера ogg
	memset(&OGG_STREAM_STATE_THEORA, 0x00, sizeof(ogg_stream_state));
	ogg_stream_clear(&OGG_STREAM_STATE_THEORA);
/*
	if (ogg_stream_init(&OGG_STREAM_STATE_THEORA, 1)  !=0)
	// 1 – серийный номер, с которым мы проассоциируем этот поток
		throw std::runtime_error("error during ogg_stream_init");
//*/
	//инициализируем "слой синхронизации" ogg
	ogg_sync_init(&OGG_SYNC_STATE);

	//очищаем все эти необходимые нам структуры для видео
	theora_comment_init(&THEORA_COMMENT);
	theora_info_init(&THEORA_INFO);

	//ну теперь ищем начало логического потока theora
	bool START_THEORA_HEADER_STREAM=true;
	int THEORA_HEADER_PACKETS=0;	//число обработанных пакетов заголовков theora

	do
	{
		//get_data_from_physical_stream - читает данные из FILE* и передает их в буфер приема ogg
		if( get_data_from_physical_stream(currentfile, &OGG_SYNC_STATE) ==0)
			throw std::runtime_error("eof searched, terminate...");	//кончился файл, на данном этапе это ошибка

		//ogg_sync_pageout - формирует страницу
		while( ogg_sync_pageout(&OGG_SYNC_STATE, &OGG_PAGE) >0)
			//1-больше данных не требуется
			//0-требуется больше данных для создания страницы
		{

			//что страница сформирована успешно

			// это страница заголовков? если нет, кончаем искать заголовки
			if( ogg_page_bos(&OGG_PAGE) ==false)
			{
				//нет, это не страница заголовков
				//значит, страницы заголовков всех логических потоков кончились и начались данные этих потоков
				//таким образом надо переходить к чтению страниц данных

				//закидываем эту страничку в логический видеопоток
				push_page_into_logical_stream(&OGG_PAGE);
				//push_page_into_logical_stream - закидывает страничку в логический поток theora, если
				//совпадает идентификатор логического потока
				//в противном случае страница игнорируется

				//выходим из циклов
				START_THEORA_HEADER_STREAM=false;
				break;
			}
			else
			{
				//да, это страница заголовков

				//тестовый логический поток
				ogg_stream_state OGG_STREAM_STATE_TEST;
				memset(&OGG_STREAM_STATE_TEST, 0x00, sizeof(ogg_stream_state));

				//THEORA_HEADER_PACKETS - число прочитанных заголовочных ПАКЕТОВ theora (не страниц)
				//по спецификации theora таких пакетов должно быть три
				if(THEORA_HEADER_PACKETS==0)
				{
				//инициализируем тестовый поток на тот же поток с таким же идентификатором потока, как и у текущей странички
				if(0!= ogg_stream_init(&OGG_STREAM_STATE_TEST,ogg_page_serialno(&OGG_PAGE)) )
					throw std::runtime_error("error during ogg_stream_init");
//*/
				//добавляем страницу в тестовый поток
				if(0!= ogg_stream_pagein(&OGG_STREAM_STATE_TEST,&OGG_PAGE) )
					throw std::runtime_error("error during ogg_stream_pagein");

				//декодируем данные из этого тестового потока в пакет
				if( ogg_stream_packetout(&OGG_STREAM_STATE_TEST,&OGG_PACKET) ==-1)
					throw std::runtime_error("error during ogg_stream_packetout");


					int dhr=theora_decode_header(&THEORA_INFO,&THEORA_COMMENT,&OGG_PACKET);
					//декодируем заголовок theora

					if(dhr<0)
					{
						//это не заголовок theora

						//очищаем структуру тестового потока
						ogg_stream_clear(&OGG_STREAM_STATE_TEST);
						//и продолжаем цикл в поисках заголовоков theora
					}
					else
					{
						//это заголовок theora!

						//вот таким образом "инициализируем" логический поток theora:
						memcpy(&OGG_STREAM_STATE_THEORA, &OGG_STREAM_STATE_TEST, sizeof(OGG_STREAM_STATE_TEST));
						//теперь из этого потока будут всегда сыпаться пакеты theora

						THEORA_HEADER_PACKETS++;

						//после того, как мы нашли заголовочную страницу логического потока theora,
						//нам необходимо прочитать все остальные заголовочные страницы других потоков
						//и отбросить их (если таковые, конечно, имеются)
					}
				}
				else
				{
					//мы прочитали как минимум один пакет theora, значит

					ogg_stream_clear(&OGG_STREAM_STATE_TEST);
				}
			}
		}

	}
	while(START_THEORA_HEADER_STREAM);

	//результат предыдущего кода:
	//1) мы создали логический поток theora, из которого можно лапать theora странички
	//2) мы инициализировали структуру OGG_STREAM_STATE_THEORA

	//сейчас надо получить еще два пакета заголовков theora (см. документацию) и можно
	//переходить к потоковому воспроизведению
	//THEORA_HEADER_PACKETS = 3;
	while(THEORA_HEADER_PACKETS<3)
	{
			int result=ogg_stream_packetout(&OGG_STREAM_STATE_THEORA,&OGG_PACKET);
			//если функция возвращает нуль, значит не хватает данных для декодирования
			//почему то этого НЕТ в спецификации libogg, или я плохо искал

			if(result<0)
			{
				//ошибка декодирования, поврежденный поток
				throw std::runtime_error("error during ogg_stream_packetout");
			}

			if(result>0)
			{
				//удалось успешно извлечь пакет информации theora

				int result2=theora_decode_header(&THEORA_INFO,&THEORA_COMMENT,&OGG_PACKET);

				if(result2<0)
				{
					//ошибка декодирования, поврежденный поток
					throw std::runtime_error("error during theora_decode_header (corrupt stream)");
				}

				++THEORA_HEADER_PACKETS;
			}

		//эту страничку обработали, надо извлечь новую
		//для этого проверям буфер чтения, вдруг там осталось что-нить похожее
		//на страничку. если не осталось, тогда просто читаем эти данные из файла:

		if( ogg_sync_pageout(&OGG_SYNC_STATE,&OGG_PAGE) >0)
			//ogg_sync_pageout - функция, берет данные из буфера приема ogg и записывает их в ogg_page
		{
			//мы нашли страничку в буфере и...
			push_page_into_logical_stream(&OGG_PAGE);
			//...пихаем эти данные в подходящий поток
		}
		else
		{
			//ничего мы в буфере не нашли
			int ret=get_data_from_physical_stream(currentfile,&OGG_SYNC_STATE);
			//надо больше данных! читаем их из файла

			if(ret==0)
			{
				//опять файл кончился!
				throw std::runtime_error("eof searched. terminate...");
			}
		}
	}

	// наконец мы получили, все, что хотели. инициализируем декодеры

	//инициализируем видео-поток
	theora_decode_init(&THEORA_STATE,&THEORA_INFO);
	report_colorspace(&THEORA_INFO);
	dump_comments(&THEORA_COMMENT);

	width = THEORA_INFO.frame_width;
	height = THEORA_INFO.frame_height;
	//с божьей помощью инициализировались успешно

//	debug::Log<<"width: "<<width<<std::endl;
//	debug::Log<<"height: "<<height<<std::endl;

	//другие переменные
	sizeframe=THEORA_INFO.fps_numerator/(float)THEORA_INFO.fps_denominator;
//	debug::Log<<"sizeframe: "<<sizeframe<<std::endl;

//	debug::Log<<"quality: "<<(THEORA_INFO.quality * 100 / 64)<<std::endl;

//	debug::Log<<"vendor:"<<THEORA_COMMENT.vendor<<std::endl;
//	debug::Log<<"filename comments:"<<std::endl;

	for(int i=0;i<THEORA_COMMENT.comments;++i)
	{
	//	debug::Log.write(THEORA_COMMENT.user_comments[i],THEORA_COMMENT.comment_lengths[i]);
	}

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&begin_playback);

	texY = 0;//new Texture(width,height,3,-1,GL_RGB);
	texU = 0;
	texV = 0;

	frameY = 0;
	frameUV = 0;

	//debug::Log<<"fps_denominator: "<<THEORA_INFO.fps_denominator<<std::endl;
	//debug::Log<<"fps_numerator: "<<THEORA_INFO.fps_numerator<<std::endl;
	frametime = 1000.0f*(float)THEORA_INFO.fps_numerator / (float)THEORA_INFO.fps_denominator;
		std::cout<<"frametime:"<<frametime<<std::endl;
	Update();
}

CTexture* CTheora::GetTextureY()
{
	return texY;
}

CTexture* CTheora::GetTextureU()
{
	return texU;
}

CTexture* CTheora::GetTextureV()
{
	return texV;
}

inline BYTE ClampIntToByte(int value)
{
  value>255?value=255:( value<0?value=0:NULL );

  return (BYTE)value;
}

void CTheora::Update()
{
	int mode = 0;
	if(mode<0)mode=0;
	if(mode>5)mode=5;

	while( ogg_stream_packetout(&OGG_STREAM_STATE_THEORA,&OGG_PACKET) <=0)
	{
		//не хватает данных в логическом потоке theora
		//надо надергать данных из физического потока и затолкать их в логический поток

		int ret=get_data_from_physical_stream(currentfile,&OGG_SYNC_STATE);	//читаем данные из файла
		if(ret==0)
		{
			return ;
		}

		while( ogg_sync_pageout(&OGG_SYNC_STATE,&OGG_PAGE) >0)
			//декодируем данные из буфера в страницы (ogg_page) пока они не кончатся в буфере
		{
			push_page_into_logical_stream(&OGG_PAGE);	//пихаем эти страницы в соотв. логические потоки
		}

	}

	if(OC_BADPACKET== theora_decode_packetin(&THEORA_STATE,&OGG_PACKET) )	//RTC check fails here
	{
		//ошибка декодирования
		throw std::runtime_error("error during theora_decode_packetin...");
	}

	//все данные получены, готовим кадр

	//декодируем страничку в YUV-виде в спец. структуру yuv_buffer
	if(0!= theora_decode_YUVout(&THEORA_STATE,&YUV_BUFFER) )
	{
		//ошибка декодирования
		throw std::runtime_error("error during theora_decode_YUVout...");
	}

	if(frameY == 0)
	{
		ywidth = YUV_BUFFER.y_stride;
		yheight = YUV_BUFFER.y_height;
		yrwidth = YUV_BUFFER.y_width;

		uvwidth = YUV_BUFFER.uv_stride / 2;
		uvheight = YUV_BUFFER.uv_height;
		uvrwidth = YUV_BUFFER.uv_width;

		axelynx::Texture::Desc desc;
		desc.width = ywidth;
		desc.height = yheight;
		desc.cpp = 1;
		desc.use_mipmaps = false;

		texY = new CTexture(desc);
		texY->Build(0,GL_RED,GL_R8);
		desc.width = uvwidth;
		desc.height = uvheight;
		texU = new CTexture(desc);
		texU->Build(0,GL_RED,GL_R8);
		texV = new CTexture(desc);
		texV->Build(0,GL_RED,GL_R8);

		desc.width = yrwidth;
		desc.height = yheight;
		desc.cpp = 3;
		tex = new CTexture(desc);
		tex->Build(0,GL_RGB,GL_RGB8);
		frameY = new unsigned char [ywidth*yheight];
		frameUV = new unsigned char [uvwidth*uvheight*2];

		frameRGB = new unsigned char [yrwidth*yheight*3];
	}

			//int ci=0;
			//for ( int nTempY = 0; nTempY < YUV_BUFFER.y_height; nTempY++ )
			//{
			//	int nYShift = YUV_BUFFER.y_stride * nTempY;
			//	int nUVShift = YUV_BUFFER.uv_stride * nTempY >> 1;
			//
			//	for ( int nTempX = 0; nTempX < YUV_BUFFER.y_width; nTempX++ )
			//	{
			//		int nHX = nTempX >>1;

			//		BYTE nY = *(BYTE*)( YUV_BUFFER.y + nYShift + nTempX );
			//		BYTE nU = *(BYTE*)( YUV_BUFFER.u + nUVShift + nHX );
			//		BYTE nV = *(BYTE*)( YUV_BUFFER.v + nUVShift + nHX );

			//		float r=nY+1.371f*(nV-128);
			//		float g=nY-0.698f*(nV-128)-10.336f*(nU-128);
			//		float b=nY+1.732f*(nU-128);

			//		frameRGB[ci+0]=ClampFloatToByte(r);
			//		frameRGB[ci+1]=ClampFloatToByte(g);
			//		frameRGB[ci+2]=ClampFloatToByte(b);

			//		ci+=3;
			//	}
			//}

  /*static*/ int b0_[256];
  /*static*/ int b1_[256];
  /*static*/ int b2_[256];
  /*static*/ int b3_[256];

  // расчет таблиц, заодно их кэширование
  for(signed int cc=0; cc<256; cc++)
  {
      b0_[cc] = ( 113443 * (cc-128) + 32768 ) >> 16;
  }

  for(signed int cc=0; cc<256; cc++)
  {
      b1_[cc] = (  45744 * (cc-128) + 32768 ) >> 16;
  }

  for(signed int cc=0; cc<256; cc++)
  {
      b2_[cc] = (  22020 * (cc-128) + 32768 ) >> 16;
  }

  for(signed int cc=0; cc<256; cc++)
  {
      b3_[cc] = ( 113508 * (cc-128) + 32768 ) >> 16;
  }

  //оптимизированный код преобразования
  register BYTE *frame_byte=frameRGB;

  for( int nTempY = 0; nTempY < YUV_BUFFER.y_height; nTempY++ )
  {
    int nYShift = YUV_BUFFER.y_stride * nTempY;
    int nUVShift = YUV_BUFFER.uv_stride * ( nTempY >> 1 );

    for( int nTempX = 0; nTempX < YUV_BUFFER.y_width; nTempX++ )
    {
      int nTempX_ = nTempX >> 1;

      /*register*/
       int nY = (int) ( *reinterpret_cast<unsigned char*>( YUV_BUFFER.y + nYShift + nTempX) );
      /*register*/ unsigned char nU = *(YUV_BUFFER.u + nUVShift + nTempX_);
      /*register*/ unsigned char nV = *(YUV_BUFFER.v + nUVShift + nTempX_);

      /*register*/ int r = nY + b0_[nV];
      /*register*/ int g = nY - b1_[nV] - b2_[nU];
      /*register*/ int b = nY + b3_[nU];

      frame_byte[0]= ClampIntToByte(r);
      frame_byte[1]= ClampIntToByte(g);
      frame_byte[2]= ClampIntToByte(b);
      frame_byte+=3;

    }
  }


			tex->Bind(0);
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,yrwidth,yheight,GL_RGB,GL_UNSIGNED_BYTE,frameRGB);
			tex->UnBind();

	++current_frame;
}

int CTheora::Width()
{
	return width;
}

int CTheora::Height()
{
	return height;
}

float CTheora::dWidthY()
{
	return (float)YUV_BUFFER.y_width / (float)YUV_BUFFER.y_stride;
}

float CTheora::dWidthUV()
{
	return (float)YUV_BUFFER.uv_width / (float)YUV_BUFFER.uv_stride;
}

CTexture* CTheora::GetDiffuseTexture()
{
	return tex;
}

bool CTheora::Play()
{
	return true;
}

bool CTheora::Stop()
{
	return true;
}
bool CTheora::Pause()
{
	return true;
}

int CTheora::CountFrames()
{
	return 0;
}

int CTheora::CurrentFrame()
{
	return 0;
}

void CTheora::SetDiffuseCalcMode(axelynx::Theora::DiffuseCalcMode mode)
{
}
