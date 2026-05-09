#pragma once

class StateInput
{
    public:
        static void Update();

        static bool IsKeyDown(int scancode);

        static bool QuitRequested();

        static int GetMouseDeltaX();
        static int GetMouseDeltaY();
        static void UpdateMouse();

    private:
        static bool s_quit;

        static int s_mouseDeltaX;
        static int s_mouseDeltaY;
};