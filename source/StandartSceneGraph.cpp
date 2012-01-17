#include "StandartSceneGraph.h"

bool StandartSceneGraph::AddEntity(axelynx::Entity *entity)
{
	list.push_back(entity);
	return true;
}

bool StandartSceneGraph::AddStaticEntity(axelynx::Entity *entity)
{
	list.push_back(entity);
	return true;
}

bool StandartSceneGraph::Remove(axelynx::Entity *entity)
{
	int cnt = list.size();

	for(int i=0;i<cnt;++i)
	{
		if(list[i] == entity)
			list[i] = list[cnt-1];
	}

	list.erase(--list.end(),list.end());

	return true;
}

void StandartSceneGraph:: Render(const axelynx::Camera *cam)
{
	const axelynx::frustum &frustum = cam->GetFrustum();

	Entites::const_iterator ci = list.begin();
	Entites::const_iterator ei = list.end();
	for(;ci!=ei;++ci)
	{
		if((*ci)->inFrustum(frustum))
			(*ci)->Draw(cam);
	}
}

void StandartSceneGraph:: Update(float twin)
{

	//Entites::iterator ci = list.begin();
	//Entites::iterator ei = list.end();
	//for(;ci!=ei;++ci)
	//{

	//	if((*ci)->Update(twin))
	//	{
	//		(*ci) = (*(ei-1));
	//		--ei;
	//		list.pop_back();
	//	}		
	//}

	int size = list.size();

	for(int i=0;i<size;++i)
	{
		if(list[i]->Update(twin))
		{
			delete list[i];
			list[i] = list[size-1];
			size--;
		}
	}

	list.resize(size);
}

StandartSceneGraph::~StandartSceneGraph()
{
}

unsigned int StandartSceneGraph::CountEntities() const
{
	return list.size();
}