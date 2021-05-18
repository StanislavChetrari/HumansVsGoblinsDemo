#include <stdio.h>

#include <Window.h>
#include <Map.h>
#include <MathUtils.h>
#include <Texture.h>
#include <Camera.h>
#include <RayCasting.h>
#include <RenderMap2_5D.h>
#include <RenderMap2D.h>
#include <Physics.h>

#include <PropertiesFileParser.h>

int main(int argc, char** argv)
{
	struct TMap map;
	TMap_InitZero(&map);
	union TMapErrorData map_error_data;
	enum EnumMapError map_error = TMap_LoadFromFile(&map, "Resources/Maps/demoMapInterior.map", &map_error_data);
	if (mapNoError != map_error)
	{
		size_t msg_size = TMap_GetErrorStrSize(&map_error, &map_error_data);
		char* msg = malloc(msg_size);
		if (msg)
		{
			TMap_GetErrorStr(&map_error, &map_error_data, msg);
			printf("Error when loading map. %s\n", msg);
			free(msg);
			msg = NULL;
			return 1;
		}
	}

	TMap_PrintScreen(&map);

	struct TWindow window;
	TWindow_InitZero(&window);
	if (0 != TWindow_Create(&window, "2.5DGame (tab for 2d view) WASD-move, ArrowsLeftRight-rotate, -+ change FOV, O/P-scale 2d rendering", 800, 600, -1, -1, 0, 60))
	{
		printf("Error InitWindow!\n");
		return 1;
	}

	struct TCamera camera;
	TCamera_InitZero(&camera);
	float camera_rotation = 90.0f;
	float camera_fov      = 60.0f;
	float scale_2d_render = 1.0f;
	struct Vec2f forward = { cosf(ToRadians(camera_rotation)), sinf(ToRadians(camera_rotation)) };
	TCamera_Create(&camera, ToRadians(camera_fov), (float)window.width / (float)window.height, &map.playerStartPos, &forward);

	SDL_Event event;

	int keys_wasd[4]       = { 0 };
	int keys_left_right[2] = { 0 };
	int keys_minus_plus[2] = { 0 };
	int keys_o_p[2]        = { 0 };
	int draw_2d = 0;
	int mouse_x = 0;
	int mouse_y = 0;

	uint32_t last_time = SDL_GetTicks();
	int quit_game = 0;
	while (0 == quit_game)
	{
		uint32_t now_time = SDL_GetTicks();
		uint32_t delta_time = now_time - last_time;
		last_time = now_time;

		if(delta_time < (uint32_t)(1000 / window.framerate))
			SDL_Delay((uint32_t)(1000 / window.framerate - delta_time));

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					quit_game = 1;
					break;

				case SDL_KEYDOWN:
				{
					if (event.key.keysym.sym == SDLK_w) keys_wasd[0] = 1;
					if (event.key.keysym.sym == SDLK_s) keys_wasd[2] = 1;
					if (event.key.keysym.sym == SDLK_a) keys_wasd[1] = 1;
					if (event.key.keysym.sym == SDLK_d) keys_wasd[3] = 1;

					if (event.key.keysym.sym == SDLK_LEFT)  keys_left_right[0] = 1;
					if (event.key.keysym.sym == SDLK_RIGHT) keys_left_right[1] = 1;

					if (event.key.keysym.sym == SDLK_KP_MINUS) keys_minus_plus[0] = 1;
					if (event.key.keysym.sym == SDLK_KP_PLUS) keys_minus_plus[1]  = 1;

					if (event.key.keysym.sym == SDLK_o) keys_o_p[0] = 1;
					if (event.key.keysym.sym == SDLK_p) keys_o_p[1] = 1;
				}break;
				case SDL_KEYUP:
				{
					if (event.key.keysym.sym == SDLK_w) keys_wasd[0] = 0;
					if (event.key.keysym.sym == SDLK_s) keys_wasd[2] = 0;
					if (event.key.keysym.sym == SDLK_a) keys_wasd[1] = 0;
					if (event.key.keysym.sym == SDLK_d) keys_wasd[3] = 0;

					if (event.key.keysym.sym == SDLK_LEFT)  keys_left_right[0] = 0;
					if (event.key.keysym.sym == SDLK_RIGHT) keys_left_right[1] = 0;

					if (event.key.keysym.sym == SDLK_KP_MINUS) keys_minus_plus[0] = 0;
					if (event.key.keysym.sym == SDLK_KP_PLUS) keys_minus_plus[1]  = 0;

					if (event.key.keysym.sym == SDLK_o) keys_o_p[0] = 0;
					if (event.key.keysym.sym == SDLK_p) keys_o_p[1] = 0;

					if (event.key.keysym.sym == SDLK_TAB) draw_2d = !draw_2d;
				}break;
				case SDL_MOUSEMOTION:
					mouse_x = event.motion.x;
					mouse_y = event.motion.y;
					break;
			}
		}

		// Movement and collision
		float keyboard_speed = 0.001f;

		int   changed_position   = 0;
		struct Vec2f movement_direction = { 0.0f, 0.0f };

		if (keys_wasd[0]) // w
		{
			movement_direction.x += camera.forward.x;
			movement_direction.y += camera.forward.y;

			changed_position = 1;
		}
		if (keys_wasd[2]) // s
		{
			movement_direction.x += -camera.forward.x;
			movement_direction.y += -camera.forward.y;

			changed_position = 1;
		}
		if (keys_wasd[1]) // a
		{
			movement_direction.x += -camera.right.x;
			movement_direction.y += -camera.right.y;

			changed_position = 1;
		}
		if (keys_wasd[3]) // d
		{
			movement_direction.x += camera.right.x;
			movement_direction.y += camera.right.y;

			changed_position = 1;
		}
		
		if (changed_position)
		{
			if (fabsf(movement_direction.x) > 0.0001f || fabsf(movement_direction.y) > 0.0001f)
			{
				Vec2f_Normalize(&movement_direction);
				struct Vec2f movement_vector = { 0.0f, 0.0f };
				movement_vector.x = keyboard_speed * movement_direction.x * delta_time;
				movement_vector.y = keyboard_speed * movement_direction.y * delta_time;

				Physics_MoveCamera(&camera, &map, &movement_vector);
			}
		}
		// End movement and collision

		// Camera rotation
		float mouse_speed = 0.1f;

		if (keys_left_right[0]) // left
		{
			camera_rotation += mouse_speed * delta_time;
			if (camera_rotation >= 360.0f)
				camera_rotation -= 360.0f;
			else if (camera_rotation <= 0)
				camera_rotation = 360.0f + camera_rotation;

			struct Vec2f forward = { cosf(ToRadians(camera_rotation)), sinf(ToRadians(camera_rotation)) };
			TCamera_SetForward(&camera, &forward);
		}

		if (keys_left_right[1]) // right
		{
			camera_rotation -= mouse_speed * delta_time;
			if (camera_rotation >= 360.0f)
				camera_rotation -= 360.0f;
			else if (camera_rotation <= 0)
				camera_rotation = 360.0f + camera_rotation;

			struct Vec2f forward = { cosf(ToRadians(camera_rotation)), sinf(ToRadians(camera_rotation)) };
			TCamera_SetForward(&camera, &forward);
		}
		// End camera rotation

		// Camera FOV
		float fov_change_speed = 0.02f;
		if (keys_minus_plus[0]) // minus
		{
			camera_fov -= fov_change_speed * delta_time;
			if (camera_fov < 0.1f)
				camera_fov = 0.1f;

			TCamera_SetFOV(&camera, ToRadians(camera_fov));

			printf("FOV: %f\n", camera_fov);
		}
		if (keys_minus_plus[1]) // plus
		{
			camera_fov += fov_change_speed * delta_time;
			if (camera_fov > 179.9f)
				camera_fov = 179.9f;

			TCamera_SetFOV(&camera, ToRadians(camera_fov));

			printf("FOV: %f\n", camera_fov);
		}
		// End camera FOV

		// Scale 2D render
		float scale_2d_speed = 0.001f;
		if (keys_o_p[0]) // o
		{
			scale_2d_render -= scale_2d_speed * delta_time;
			if (scale_2d_render <= 0.01f)
				scale_2d_render = 0.01f;
		}
		else if (keys_o_p[1]) // p
		{
			scale_2d_render += scale_2d_speed * delta_time;
		}
		// End scale 2D render

		TWindow_RenderStart(&window);
		TWindow_SetRenderDrawColor(&window, 0, 0, 0, 255);
		TWindow_RenderClear(&window);
		
		if(draw_2d)
			RenderMap2D(&map, &camera, &window, scale_2d_render);
		else
			RenderMap2_5D(&map, &camera, &window);

		TWindow_RenderStop(&window);

		TWindow_RenderPresent(&window);
	}

	TMap_Clear(&map);

	TWindow_Clear(&window);

	return 0;
}