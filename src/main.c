#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#include "android_native_app_glue.h"
#include "sys/types.h"

// struct android_app;
// typedef struct JNIEnv JNIEnv;
// typedef struct JavaVM JavaVM;
struct android_app *GetAndroidApp();

#define MAX_INPUT_CHARS 24
static char input_chars[MAX_INPUT_CHARS] = {0};
static int input_chars_count = 0;

static void push_char(int c)
{
	if (input_chars_count >= MAX_INPUT_CHARS) return;
	input_chars[input_chars_count++] = (char)c;
}

void show_keyboard(bool show)
{
	struct android_app *app = GetAndroidApp();
	if (!app) return;

	JNIEnv *env = NULL;
	JavaVM *vm = app->activity->vm;
	(*vm)->AttachCurrentThread(vm, &env, NULL);

	jobject activity = app->activity->clazz;
	jclass activityClass = (*env)->GetObjectClass(env, activity);
	jmethodID showKeyboardMethod = (*env)->GetMethodID(env, activityClass, "showKeyboard", "(Z)V");

	if (showKeyboardMethod) {
		(*env)->CallVoidMethod(env, activity, showKeyboardMethod, (jboolean)show);
	}

	(*env)->DeleteLocalRef(env, activityClass);
	(*vm)->DetachCurrentThread(vm);

}

JNIEXPORT void JNICALL
Java_com_raylib_game_NativeLoader_nativePushInput(JNIEnv *env, jobject this, jint code, jint character)
{
	if (character > 0) push_char(character);
}

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

	show_keyboard(true);


	while(!WindowShouldClose()) {
		if (GetTouchPointCount() > 0) {
			show_keyboard(true);
		}

		BeginDrawing();
			ClearBackground(BLACK);
			DrawText(
				message,
				message_positon.x, message_positon.y,
				font_size,
				RAYWHITE);

			char buffer[0xff];
			snprintf(buffer, sizeof(buffer), "%*s", input_chars_count, input_chars);
			DrawText(buffer, 32, 32, 32, BLUE);
			if (input_chars_count > 0) {
				for (int i = 0; i < input_chars_count; i++) {
					TraceLog(LOG_INFO, "key: %c", (char)input_chars[i]);
				}
			}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
