#include "axelynx/Theora.h"
#include "ogg/ogg.h"
#include "theora/theora.h"
#include "CTexture.h"
#include "axelynx/File.h"

class CTheora : public axelynx::Theora
{
	std::wstring fn;
	FILE *currentfile;
	UINT texture;

	ogg_stream_state OGG_STREAM_STATE_THEORA;
	ogg_sync_state OGG_SYNC_STATE;
	ogg_page OGG_PAGE;
	ogg_packet OGG_PACKET;

	
	theora_comment THEORA_COMMENT;
	theora_info THEORA_INFO;
	theora_state THEORA_STATE;

	yuv_buffer YUV_BUFFER;

	unsigned int current_frame;

	double sizeframe;

	void report_colorspace(theora_info *ti);
	int dump_comments(theora_comment *tc);
	int get_data_from_physical_stream(FILE *INFILE_, ogg_sync_state *OGG_SYNC_STATE_);
	int push_page_into_logical_stream(ogg_page *page);

	LARGE_INTEGER begin_playback, current_time, freq;
	
	unsigned int width,height;
	CTexture *texY, *texU, *texV, *tex;

	unsigned char *frameY;
	unsigned char *frameUV;
	unsigned char *frameRGB;

	float frametime;
	__int64 lmcs;

	unsigned int ywidth,yheight,uvwidth,uvheight;
	unsigned int yrwidth,uvrwidth;
public:
	CTheora(axelynx::File file);
	virtual int Width();
	virtual int Height();

	virtual bool Play();
	virtual bool Stop();
	virtual bool Pause();

	virtual int CountFrames();
	virtual int CurrentFrame();

	virtual CTexture* GetDiffuseTexture();
	virtual CTexture* GetTextureY();
	virtual CTexture* GetTextureU();
	virtual CTexture* GetTextureV();

	virtual void SetDiffuseCalcMode(axelynx::Theora::DiffuseCalcMode mode);

	virtual float dWidthY();
	virtual float dWidthUV();
		
	virtual void Update();
	virtual ~CTheora();
};