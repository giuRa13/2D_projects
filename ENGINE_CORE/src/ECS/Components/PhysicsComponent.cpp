#include "Core/ECS/Components/PhysicsComponent.hpp"
#include "Core/CoreUtilities/CoreEngineData.hpp"
#include <Logger/Logger.hpp>

using namespace ENGINE_PHYSICS;

namespace ENGINE_CORE::ECS
{

    PhysicsComponent::PhysicsComponent(const PhysicsAttributes& physicsAttrib)
        : m_pRigidBody{nullptr}, m_pUserData{nullptr}, m_InitialAttribs{physicsAttrib}
    { }

    PhysicsComponent::PhysicsComponent()
        : PhysicsComponent{PhysicsAttributes{}}
    { }


    const bool PhysicsComponent::IsSensor() const
    {
        if(!m_pRigidBody)
            return false;

        return m_pRigidBody->GetFixtureList()->IsSensor();
    }


    void PhysicsComponent::Init(ENGINE_PHYSICS::PhysicsWorld pPhysicsWorld, int windowWidth, int windowHeight)
    {
        if(!pPhysicsWorld)
        {
            ENGINE_ERROR("Failed to Initialize PhysicsComponent - Physics World idìs nullptr");
            return;
        }

        bool bCircle{m_InitialAttribs.bCircle};

        // create body def
        b2BodyDef bodyDef{};
        bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.eType);
        auto PIXELS_TO_METERS = CoreEngineData::GetInstance().PixelsToMeters();

        // set initial position of body
        bodyDef.position.Set(
            (m_InitialAttribs.position.x + m_InitialAttribs.offset.x - (windowWidth * 0.5f) +
            ((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.x) * 
                m_InitialAttribs.scale.x ) * 0.5f) * PIXELS_TO_METERS,
            (m_InitialAttribs.position.y + m_InitialAttribs.offset.y - (windowHeight * 0.5f) +
            ((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y) * 
                m_InitialAttribs.scale.y ) * 0.5f) * PIXELS_TO_METERS
        );
        bodyDef.gravityScale = m_InitialAttribs.gravityScale;
        bodyDef.fixedRotation = m_InitialAttribs.bFixedRotation;

        // Create the Rigid Body
        m_pRigidBody = ENGINE_PHYSICS::MakeSharedBody(pPhysicsWorld->CreateBody(&bodyDef));
        if(!m_pRigidBody)
        {
            ENGINE_ERROR("Failed to create the Rigid Body");
            return;
        }

        // Create the Shape
        b2PolygonShape polyShape;
        b2CircleShape circleShape;

        if(bCircle)
        {
            circleShape.m_radius = PIXELS_TO_METERS * m_InitialAttribs.radius * m_InitialAttribs.scale.x ;
        }
        else if(m_InitialAttribs.bBoxShape)
        {
            polyShape.SetAsBox(
                PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
                PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f
            );
        }
        else
        {
            // create polygon shape
        }

        // Create the UserData
        m_pUserData = std::make_shared<UserData>();
        m_pUserData->userData = m_InitialAttribs.objectData;
        m_pUserData->type_id = entt::type_hash<ObjectData>::value();

        // Create the fixture def
        b2FixtureDef fixtureDef{};
        if(bCircle)
            fixtureDef.shape = &circleShape;
        else
            fixtureDef.shape = &polyShape;

        fixtureDef.density = m_InitialAttribs.density;
        fixtureDef.friction = m_InitialAttribs.friction;
        fixtureDef.restitution = m_InitialAttribs.restitution;
        fixtureDef.restitutionThreshold = m_InitialAttribs.restitutionThreshold;
        fixtureDef.isSensor = m_InitialAttribs.bIsSensor;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(m_pUserData.get());

        auto p_Fixture = m_pRigidBody->CreateFixture(&fixtureDef);
        if(!p_Fixture)
        {
            ENGINE_ERROR("Failed to create the Body Fixture");
        }


    }


    void PhysicsComponent::CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry)
    {
        lua.new_usertype<ObjectData>(
            "ObjectData",
            "type_id", entt::type_hash<ObjectData>::value,
            sol::call_constructor,
            sol::factories(
                [](const std::string& tag, const std::string& group, bool bCollider, bool bTrigger, std::uint32_t entityID)
                {
                    return ObjectData{
                        .tag = tag,
                        .group = group,
                        .bCollider = bCollider,
                        .bTrigger = bTrigger,
                        .entityID = entityID
                    };
                },
                [](const sol::table& objectData)
                {
                    return ObjectData{
                        .tag = objectData["tag"].get_or(std::string{""}),
                        .group = objectData["group"].get_or(std::string{""}),
                        .bCollider = objectData["bCollider"].get_or(false),
                        .bTrigger = objectData["bTriggere"].get_or(false),
                        .entityID = objectData["entityID"].get_or((std::uint32_t)0)
                    };
                }
            ),
            "tag", &ObjectData::tag,
			"group", &ObjectData::group,
			"bCollider", &ObjectData::bCollider,
			"bTrigger", &ObjectData::bTrigger,
			"entityID", &ObjectData::entityID,
            "contactEntities", &ObjectData::contactEntities,
            "to_string", &ObjectData::to_string
        );

        lua.new_enum<RigidBodyType>(
            "BodyType", {
                {"Static", RigidBodyType::STATIC},
                {"Kinematic", RigidBodyType::KINEMATIC},
                {"Dynamic", RigidBodyType::DYNAMIC},
            }
        );

        lua.new_usertype<PhysicsAttributes>(
            "PhysicsAttributes",
            sol::call_constructor,
            sol::factories(
                []{
                    return PhysicsAttributes{};
                },
                [](const sol::table& physAttr) {
					return PhysicsAttributes{
						.eType = physAttr["eType"].get_or(RigidBodyType::STATIC),
						.density = physAttr["density"].get_or(100.f),
						.friction = physAttr["friction"].get_or(0.2f),
						.restitution = physAttr["restitution"].get_or(0.2f),
						.restitutionThreshold = physAttr["restitutionThreshold"].get_or(0.2f),
						.radius = physAttr["radius"].get_or(0.f),
						.gravityScale = physAttr["gravityScale"].get_or(1.f),
						.position = glm::vec2{
							physAttr["position"]["x"].get_or(0.f),
							physAttr["position"]["y"].get_or(0.f)
						},
						.scale = glm::vec2{
							physAttr["scale"]["x"].get_or(0.f),
							physAttr["scale"]["y"].get_or(0.f)
						},
						.boxSize = glm::vec2{
							physAttr["boxSize"]["x"].get_or(0.f),
							physAttr["boxSize"]["y"].get_or(0.f)
						},
						.offset = glm::vec2{
							physAttr["offset"]["x"].get_or(0.f),
							physAttr["offset"]["y"].get_or(0.f)
						},
						.bCircle = physAttr["bCircle"].get_or(false),
						.bBoxShape = physAttr["bBoxShape"].get_or(true),
						.bFixedRotation = physAttr["bFixedRotation"].get_or(true),
						.bIsSensor = physAttr["bIsSensor"].get_or(false),
						.filterCategory = physAttr["filterCategory"].get_or((uint16_t)0),
						.filterMask = physAttr["filterMask"].get_or((uint16_t)0),
						.objectData = ObjectData{
							.tag = physAttr["objectData"]["tag"].get_or(std::string{""}),
							.group = physAttr["objectData"]["group"].get_or(std::string{""}),
							.bCollider = physAttr["objectData"]["bCollider"].get_or(false),
							.bTrigger = physAttr["objectData"]["bTrigger"].get_or(false),
							.entityID = physAttr["objectData"]["entityID"].get_or((std::uint32_t)0)
						}
					};
				}
            ),
            "eType", &PhysicsAttributes::eType,
            "density", &PhysicsAttributes::density,
            "friction", &PhysicsAttributes::friction,
            "restitution", &PhysicsAttributes::restitution,
            "restitutionThreshold", &PhysicsAttributes::restitutionThreshold,
            "radius", &PhysicsAttributes::radius,
            "gravityScale", &PhysicsAttributes::gravityScale,
            "position", &PhysicsAttributes::position,
            "scale", &PhysicsAttributes::scale,
            "boxSize", &PhysicsAttributes::boxSize,
            "offset", &PhysicsAttributes::offset,
            "bCircle", &PhysicsAttributes::bCircle,
            "bBoxShape", &PhysicsAttributes::bBoxShape,
            "bFixedRotation", &PhysicsAttributes::bFixedRotation,
            "bIsSensor", &PhysicsAttributes::bIsSensor,
            "objectData", &PhysicsAttributes::objectData
            // add filters and more
        );
        
        auto& pPhysicsWorld = registry.ctx().get<ENGINE_PHYSICS::PhysicsWorld>();
        if(!pPhysicsWorld)
        {
            return;
        }

        lua.new_usertype<PhysicsComponent>(
            "PhysicsComp",
            "type_id", &entt::type_hash<PhysicsComponent>::value,
            sol::call_constructor,
            sol::factories(
                [&](const PhysicsAttributes& attr) {
                    PhysicsComponent pc{attr};
                    pc.Init(pPhysicsWorld, 640, 480);
                    return pc;
                }
            ),
            "linear_impulse",
		    []( PhysicsComponent& pc, const glm::vec2& impulse ) {
			    auto body = pc.GetBody();
			    if ( !body )
			    {
				    // TODO: Add Error
				    return;
			    }

			    body->ApplyLinearImpulse( b2Vec2{ impulse.x, impulse.y }, body->GetPosition(), true );
		    },
            "angular_impulse",
		    []( PhysicsComponent& pc, float impulse ) {
			    auto body = pc.GetBody();
			    if ( !body )
			    {
				    // TODO: Add Error
				    return;
			    }

			    body->ApplyAngularImpulse( impulse, true );
		    },
            "set_linear_velocity",
		    []( PhysicsComponent& pc, const glm::vec2& velocity ) {
			    auto body = pc.GetBody();
			    if ( !body )
			    {
				    // TODO: Add Error
				    return;
			    }

			    body->SetLinearVelocity( b2Vec2{velocity.x, velocity.y} );
		    },
            "get_linear_velocity",
		    []( PhysicsComponent& pc) {
			    auto body = pc.GetBody();
			    if ( !body )
			    {
				    // TODO: Add Error
				    return glm::vec2{0.f};
			    }
                const auto linearVelocity = body->GetLinearVelocity();
			    return glm::vec2{linearVelocity.x, linearVelocity.y};
		    },
            "set_angular_velocity",
		    []( PhysicsComponent& pc, float angularVelocity ) {
			    auto body = pc.GetBody();
			    if ( !body )
			    {
				    // TODO: Add Error
				    return;
			    }

			    body->SetAngularVelocity(angularVelocity);
		    },
            "get_angular_velocity",
		    []( PhysicsComponent& pc) {
			    auto body = pc.GetBody();
			    if ( !body )
			    {
				    // TODO: Add Error
				    return 0.f;
			    }

			    return body->GetAngularVelocity();
		    },
            "set_gravity_scale", 
            [](PhysicsComponent& pc, float gravityScale){
                auto body = pc.GetBody();
			    if ( !body )
			    {
				    // TODO: Add Error
				    return ;
			    }

			    body->SetGravityScale(gravityScale);
            },
            "set_transform", 
            [](PhysicsComponent& pc, const glm::vec2& position){
                auto body = pc.GetBody();
                if(!body)
                {
                    //error
                    return;
                }

                auto& engineData = CoreEngineData::GetInstance();
                const auto p2m = engineData.PixelsToMeters();

                const auto scaleHalfHeight = engineData.ScaledHeight() * 0.5;
                const auto scaleHalfWidth = engineData.ScaledWidth() * 0.5;

                auto bx = (position.x * p2m) - scaleHalfWidth;
                auto by = (position.y * p2m) - scaleHalfHeight;

                body->SetTransform(b2Vec2{bx, by}, 0.f);
            }
        );
    }


}