#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "sprite.h"

class GameObject
{
public:
	GameObject();
	GameObject(
		Sprite* sprite, 
		bool solided = false,
		vec2 velocity = vec2(0.0f),
		bool destoryed = false);
	GameObject(
		Texture2D* tex, 
		const vec2 position,
		const vec2 size,
		const vec3 color = vec3(1.0f),
		const bool solided = false,
		const vec2 velocity = vec2(0.0f),
		bool staticed = false,
		const float rotation = 0.0f,
		const bool destoryed = false);
	~GameObject();
	virtual void draw(ShaderManager* shader);
	bool isDestoryed();
	void destory();
	bool isSolid();
	void realign(vec2 position, vec2 size);
	vec2 getSize();
	vec2 getPosition();
	void setPosition(vec2 pos);
	void otranslatex(float v);
	virtual bool checkCollision(GameObject& object);
protected:
	Sprite* sprite;
	vec2 velocity;
	bool solid;
	bool destoryed;
	bool staticed;
	vec2 position, size;
	vec3 color;
	float rotation = 0.0f;
	bool AABBCollision(GameObject& object);
};

class Ball : public GameObject {
public:
	float radius;
	bool stuck;
	Ball(
		Texture2D* tex,
		const vec2 position,
		const float radius,
		const vec2 velocity = vec2(1.0f),
		const vec3 color = vec3(1.0f),
		const bool stuck = true
	);
	void move(int winWidth);
	void reset(vec2 position, vec2 velocity);
	virtual bool checkCollision(GameObject& object) override;
protected:
	bool CircleCollision(GameObject& object);
private:
};

#endif // !GAMEOBJECT_H

