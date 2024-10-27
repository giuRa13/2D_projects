#include "Core/Scripting/RendererBindings.hpp"
#include <Rendering/Essentials/Primitives.hpp>
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Core/Renderer.hpp>
#include "Core/ECS/Registry.hpp"
#include <Logger/Logger.hpp>

using namespace ENGINE_RENDERING;


void ENGINE_CORE::Scripting::RendererBinder::CreateRenderingBind(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
{

    lua.new_usertype<Line>( 
        "Line",
        sol::call_constructor,
        sol::factories( 
            []( const glm::vec2& p1, const glm::vec2& p2, const Color& color ) 
            {
                return Line{ .p1 = p1, .p2 = p2, .color = color };
            }
        ),
        "p1", &Line::p1,
        "p2", &Line::p2,
        "color", &Line::color 
    );

    lua.new_usertype<Rect>(
        "Rect",
        sol::call_constructor,
        sol::factories( 
            []( const glm::vec2& position, float width, float height, const Color& color ) 
            {
                return Rect{ .position = position, .width = width, .height = height, .color = color };
            }
        ),
        "position", &Rect::position,
        "width", &Rect::width,
        "height", &Rect::height,
        "color", &Rect::color 
    );

    
    // Renderer
    auto& renderer = registry.GetContext<std::shared_ptr<Renderer>>();
    if(!renderer)
    {
        ENGINE_ERROR("Failed to bind the Renderer to Lua, not in the Registry context");
    }

    lua.set_function( 
        "DrawRect",
		sol::overload( 
            [ & ]( const Rect& rect ) { 
                renderer->DrawRect( rect ); 
            },
			[ & ]( const glm::vec2& position, float width, float height, const Color& color ) {
				renderer->DrawRect( position, width, height, color );
			} 
        ) 
    );

	lua.set_function( 
        "DrawLine",
		sol::overload( 
            [ & ]( const Line& line ) { 
                renderer->DrawLine( line );
            },
			[ & ]( const glm::vec2& p1, const glm::vec2& p2, const Color& color ) {
				renderer->DrawLine( p1, p2, color );
			} 
        ) 
    );


    // Camera
    auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();
	if ( !camera )
	{
		ENGINE_ERROR( "Failed to Bind the Camera to Lua - Failed to get the Camera from the Registry Context!" );
		return;
	}

	lua.new_usertype<Camera2D>(
		"Camera",
		sol::no_constructor,
		"get", 
        [ & ] { return *camera; },
		"position", 
        [ & ] { return camera->GetPosition(); },
		"set_position", 
        [ & ]( const glm::vec2 newPosition ) { camera->SetPosition( newPosition ); },
		"scale", 
        [ & ] { return camera->GetScale(); },
		"set_scale", 
        [ & ]( float scale ) { camera->SetScale( scale ); }
    );

}