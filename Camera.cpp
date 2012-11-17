#include "Camera.h"

Camera::Camera(const std::string& name) : Node(name)
{ }

Camera::Camera(const std::string& name, float x, float y, float z) : Node(name, x, y, z)
{ }

Camera::Camera(const std::string& name, sf::Vector3f pos) : Node(name, pos)
{ }

Camera::Camera(const std::string& name, sf::Vector3f pos, sf::Vector3f view,  sf::Vector3i up) : Node(name, pos),
                                                                mView(view),              mUp(up)
{ }

Camera::~Camera()
{
}


void Camera::setPosition(sf::Vector3f pos)
{
    mPos = pos;
}

void Camera::setView(sf::Vector3f view)
{
    mView = view;
}

void Camera::moveCamera(float speed)
{
    sf::Vector3f vVector = mView-mPos;
    float norm = sqrt(vVector.x*vVector.x + vVector.y*vVector.y + vVector.z*vVector.z);
    vVector = (mView - mPos)/norm;	// Get the view vector

    // Get new position, and new viewpoint
	mPos.x  = mPos.x  + vVector.x * speed;
	mPos.z  = mPos.z  + vVector.z * speed;
	mView.x = mView.x + vVector.x * speed;
	mView.z = mView.z + vVector.z * speed;
}

void Camera::moveCameraUp(float speed)
{
    sf::Vector3f vVector = mView-mPos;
    float norm = sqrt(vVector.x*vVector.x + vVector.y*vVector.y + vVector.z*vVector.z);
    vVector = (mView - mPos)/norm;	// Get the view vector

    sf::Vector3f vOrthoVector;              // Orthogonal vector for the view vector

	vOrthoVector.x = vVector.x;
	vOrthoVector.y = -vVector.z;
	vOrthoVector.z = vVector.y;

	// left positive cameraspeed and right negative -cameraspeed.
	mPos.y  = mPos.y  + vOrthoVector.y * speed;
	mPos.z  = mPos.z  + vOrthoVector.z * speed;
	mView.y = mView.y + vOrthoVector.y * speed;
	mView.z = mView.z + vOrthoVector.z * speed;
}


void Camera::strafeCamera(float speed)
{
    sf::Vector3f vVector = mView-mPos;
    float norm = sqrt(vVector.x*vVector.x + vVector.y*vVector.y + vVector.z*vVector.z);
    vVector = (mView - mPos)/norm;	// Get the view vector

    sf::Vector3f vOrthoVector;              // Orthogonal vector for the view vector

	vOrthoVector.x = -vVector.z;
	vOrthoVector.z =  vVector.x;

	// left positive cameraspeed and right negative -cameraspeed.
	mPos.x  = mPos.x  + vOrthoVector.x * speed;
	mPos.z  = mPos.z  + vOrthoVector.z * speed;
	mView.x = mView.x + vOrthoVector.x * speed;
	mView.z = mView.z + vOrthoVector.z * speed;
}

void Camera::rotateView(float speed)
{
    sf::Vector3f vVector = mView-mPos;

	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}


void Camera::mouseMove(const sf::Vector2f& mousePos, const sf::Vector2f& windowSize)
{
	int mid_x = windowSize.x / 2;
	int mid_y = windowSize.y / 2;
	float angle_y  = 0.0f;
	float angle_z  = 0.0f;

	if( (mousePos.x == mid_x) && (mousePos.y == mid_y) ) return;

	//SetCursorPos(mid_x, mid_y);	// Set the mouse cursor in the center of the window

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;
	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	// The higher the value is the faster the camera looks around.
	mView.y += angle_z * 2;

	// limit the rotation around the x-axis
	if((mView.y - mPos.y) > 4)  mView.y = mPos.y + 4;
	if((mView.y - mPos.y) <-4)  mView.y = mPos.y - 4;

	rotateView(-angle_y); // Rotate
}
