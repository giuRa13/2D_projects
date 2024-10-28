#include "Physics/Box2DWrappers.hpp"


void ENGINE_PHYSICS::BodyDestroyer::operator()(b2Body* body) const
{
    body->GetWorld()->DestroyBody(body);
}


