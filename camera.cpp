Math::Vector observer_position(0.0,0.0,4.0);
Math::Vector grid_position(0.0,0.0,2.0);
Math::Vector up(0.0,1.0,0.0);

Math::Vector y = up.normalize();
Math::Vector z = (grid_position - observer_position).normalize();
Math::Vector x = y.cross_product(z);


void Camera::look_at(const Math::Vector& _observer_position,const Math::Vector& _grid_position,const Math::Vector& _up) {
	Math::Vector y = _up.normalize();
	Math::Vector z = (_grid_position - _observer_position).normalize();
	Math::Vector x = y.cross_product(z);

	grid->set_x(x);
	grid->set_y(y);
	grid->set_z(z);
	observer->set_position(_observer_position);
	observer->set_up(_up);
}

        return Vector(y*_vector.z - z*_vector.y, z*_vector.x - x*_vector.z,x*_vector.y - y*_vector.x);

// Tomas Möller and Ben Trumbore ray-triangle intersection
