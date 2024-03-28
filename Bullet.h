#ifndef BULLET_H
#define BULLET_H

#include "game_object.h" 
#include "Timer.h"
#include <glm/glm.hpp>

namespace game {
	class Bullet : public GameObject {



	public:

		Bullet(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

		virtual ~Bullet();



		void lifetimer();
		bool lifeover();

		virtual void Update(double delta_time) override;
		virtual void Render(glm::mat4 view_matrix, double current_time) override;

		void SetVelocity(const glm::vec3& position);

		glm::vec3 GetVelocity();


	private:
		Timer* exist_timer;
		glm::vec3 velocity;












	};
}
#endif //BULLET_H