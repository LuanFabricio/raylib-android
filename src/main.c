#include "raylib.h"

int main(void)
{
	InitWindow(GetRenderWidth(), GetRenderHeight(), "game");
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	SetTargetFPS(60);

	const char* message = "Hello, from raylib on Android!";
	const int font_size = 64;
	const int message_width = MeasureText(message, font_size);

	Vector2 message_positon = {
		.x = (GetScreenWidth() - message_width) / 2.f,
		.y = (GetScreenHeight() - font_size) / 2.f,
	};

	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
			DrawText(
				message,
				message_positon.x, message_positon.y,
				font_size,
				RAYWHITE);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
