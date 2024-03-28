#include "Bullet.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>



namespace game {
	Bullet::Bullet(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		scale_ = glm::vec2(1.0f, 1.0f);
		exist_timer = new Timer();
		lifetimer();
		toggleCollide();
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	Bullet::~Bullet(){
		delete exist_timer;
		
	}


	void Bullet::lifetimer() {
		if (exist_timer != nullptr) {
			exist_timer->Start(5.0);
		}
	}


	glm::vec3 Bullet::GetVelocity() {
		return velocity;
	}

	bool Bullet::lifeover() {
		if (exist_timer != nullptr) {
			if (exist_timer->Finished()) {
				this->SetTemporary(true);
				return true;
			}
		}
		return false; //Assume timer has not started if null
	}
	void Bullet::SetVelocity(const glm::vec3& ber) {
		velocity = ber * 10.0f;
	}


    void Bullet::Update(double delta_time) {

		position_ += velocity * static_cast<float>(delta_time);

        GameObject::Update(delta_time);

        
    }

	void Bullet::Render(glm::mat4 view_matrix, double current_time) {
		shader_->Enable();
		shader_->SetUniformMat4("view_matrix", view_matrix);

		glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0f));
		glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0f));
		glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
		glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

		shader_->SetUniformMat4("transformation_matrix", transformation_matrix);
		geometry_->SetGeometry(shader_->GetShaderProgram());
		glBindTexture(GL_TEXTURE_2D, texture_);
		glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
	}


}