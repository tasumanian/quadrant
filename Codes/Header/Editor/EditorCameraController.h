#pragma once

class CameraComponent;

class EditorCameraController
{
public:

    void Update(
        CameraComponent* camera,
        float dt);
};