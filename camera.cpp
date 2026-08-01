#include "camera.h"

glm::mat4 Camera::GetViewMatrix() const
{
    glm::vec3 pos = transform.position;
    glm::vec3 forward = transform.Forward();
    return glm::lookAt(pos, pos + forward, transform.Up());
}
Camera::Camera()
{
    //èâä˙ê›íË
    //yaw = 0.0f;
	//pitch = 0.0f;
    transform.position = glm::vec3(0.0f, 0.0f, 3.0f);

    transform.rotation.z = -90.0f;
    transform.rotation.y = 0.0f;

}