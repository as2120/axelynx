#include <axelynx\SceneGraph.h>
#include <list>
#include <vector>

class StandartSceneGraph : public axelynx::SceneGraph
{
	typedef std::vector<axelynx::Entity*> Entites;
	Entites list;

public:
	virtual bool AddEntity(axelynx::Entity *entity);
	virtual bool AddStaticEntity(axelynx::Entity *entity);
	virtual bool Remove(axelynx::Entity *entity);

	virtual void Render(const axelynx::Camera *cam);
	virtual void Update(float twin = 1.0f);

	virtual unsigned int CountEntities() const;

	virtual ~StandartSceneGraph();
};