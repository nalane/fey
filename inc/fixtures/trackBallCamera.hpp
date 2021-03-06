#pragma once

/* 
 * Camera that allows third person perspective
 */

#include "camera.hpp"

class trackBallCamera : public camera {
private:
  double rho;   // Distance from target
  double theta; // Angle in degrees from x axis
  double phi;   // Angle in degrees from y axis

  void resetView();
  
public:
  trackBallCamera() : rho(0.0), theta(0.0), phi(90.0) { }
  trackBallCamera(const glm::mat4& projection, const glm::mat4& view) : camera(projection, view),
									    rho(0.0), theta(0.0), phi(90.0) { }
  trackBallCamera(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& upVector,
		  float fieldOfView, float aspectRatio = defaultAspectRatio, float nearClip = 0.1, float farClip = 100.0) :
    camera(pos, target, upVector, fieldOfView, aspectRatio, nearClip, farClip),
    rho(0.0), theta(0.0), phi(90.0) { }

  void pan(double deltaTheta, double deltaPhi);
  void zoom(double deltaRho);
};
