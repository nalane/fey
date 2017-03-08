#include "trackBallCamera.hpp"
#include <cmath>

void trackBallCamera::resetView() {
  position = glm::vec3(rho * sin(M_PI * phi / 180.0) * cos(M_PI * theta / 180.0),
		       rho * cos(M_PI * phi / 180.0),
		       rho * sin(M_PI * phi / 180.0) * sin(M_PI * theta / 180.0));

  camera::resetView();
}

void trackBallCamera::pan(double deltaTheta, double deltaPhi) {
  theta += deltaTheta;
  phi += deltaPhi;

  if (phi <= 0.0) {
    phi = 0.01;
  }

  if (phi >= 180.0) {
    phi = 179.99;
  }

  resetView();
}

void trackBallCamera::zoom(double deltaRho) {
  rho += deltaRho;
  if (rho < 0.1) {
    rho = 0.1;
  }

  resetView();
}
