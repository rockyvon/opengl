#include "stdafx.h"
#include "object.h"


GameObject::GameObject()
{
}

GameObject::GameObject(Sprite* sprite, bool solid, vec2 velocity, bool destoryed) :
	sprite(sprite), 
	velocity(velocity), 
	solid(solid),
	destoryed(destoryed)
{
}

GameObject::GameObject(
	Texture2D* tex,
	const vec2 position,
	const vec2 size,
	const vec3 color,
	bool solid,
	const vec2 velocity,
	bool staticed,
	const float rotation,
	const bool destoryed) :
	sprite(new Sprite(tex)),
	position(position),
	size(size),
	color(color),
	rotation(rotation),
	velocity(velocity),
	staticed(staticed),
	solid(solid),
	destoryed(destoryed)
{
}

GameObject::~GameObject()
{
}

void GameObject::draw(ShaderManager* shader)
{
	sprite->draw(shader, position, size, rotation, color);
}

bool GameObject::isDestoryed()
{
	return destoryed;
}

void GameObject::destory()
{
	destoryed = true;
}

bool GameObject::isSolid()
{
	return solid;
}

void GameObject::realign(vec2 position, vec2 size)
{
	this->position = position;
	this->size = size;
}

vec2 GameObject::getSize()
{
	return size;
}

vec2 GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(vec2 pos)
{
	this->position = pos;
}

void GameObject::otranslatex(float v)
{
	position.x += v;
}

bool GameObject::checkCollision(GameObject & object)
{
	return AABBCollision(object);
}

bool GameObject::AABBCollision(GameObject & object)
{
	bool cx = position.x + size.x >= object.position.x &&
		object.position.x + object.size.x >= position.x;

	bool cy = position.y + size.y >= object.position.y &&
		object.position.y + object.size.y >= position.y;

	return cx && cy;
}

Ball::Ball(Texture2D * tex, const vec2 position, const float radius, const vec2 velocity, const vec3 color, const bool stuck):
	GameObject(tex, position, vec2(radius * 2), color, true, velocity),
	radius(radius),
	stuck(stuck)
{
}

void Ball::move(int winWidth)
{
	if (stuck)
		return;
	position += velocity;
	if (position.x <= 0.0f) {
		velocity.x *= -1;
		position.x = 0.0f;
	}
	else if (position.x + size.x >= winWidth) {
		velocity.x *= -1;
		position.x = winWidth - size.x;
	}
	if (position.y <= 0) {
		velocity.y *= -1;
		position.y = 0.0f;
	}
}

void Ball::reset(vec2 position, vec2 velocity)
{
	this->position = position;
	this->velocity = velocity;
	this->stuck = true;
}

bool Ball::checkCollision(GameObject & object)
{
	return AABBCollision(object);
}

bool Ball::CircleCollision(GameObject & object)
{
	// 获取圆的中心 
	vec2 center(position +radius);
	// 计算AABB的信息（中心、半边长）
	vec2 aabb_half_extents(object.getSize().x / 2, object.getSize().y / 2);
	vec2 aabb_center(
		object.getPosition().x + aabb_half_extents.x,
		object.getPosition().y + aabb_half_extents.y
	);
	// 获取两个中心的差矢量
	vec2 difference = center - aabb_center;
	vec2 clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
	vec2 closest = aabb_center + clamped;
	// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
	difference = closest - center;
	return length(difference) < radius;
}
