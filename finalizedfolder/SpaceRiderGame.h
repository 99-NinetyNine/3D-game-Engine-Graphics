#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include "gravity.h"

class SpaceRiderGame : public Window, public InputListener
{
  public:
	SpaceRiderGame();
	~SpaceRiderGame();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;


	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
	void checkCollision();
	void strikewithplanets();
public:
	void render();
	void update();
	void updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials);


	void updateCamera();
	void updateThirdPersonCamera();
	void updateSkyBox();
	void updateLight();
	
	void updateSpaceship();
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials);
	void createAsteroid();
	Vector3D getPositionofRevolvings(Vector3D center, float longradii, float shortradii);

	Vector3D getPositionofRevolvings(Vector3D center, float longradii, float shortradii, float angle);
	void renderHelper(MaterialPtr mat, MeshPtr mesh, Vector3D pos, Vector3D rot, Vector3D scale);
	void normalizeCamera();

private:


	Vector3D Collidedpos;
	Vector3D Collidedscale;

	bool m_collided = false;
	SwapChainPtr m_swap_chain;
	

	TexturePtr m_spaceship_tex;
	MeshPtr m_spaceship_mesh;
	MaterialPtr m_spaceship_mat;
	float m_spaceship_mass = 25;


	TexturePtr m_asteroid_tex;
	MeshPtr m_asteroid_mesh;
	MaterialPtr m_asteroid_mat;
	


	TexturePtr m_sun_tex;
	MeshPtr m_sun_mesh;
	MaterialPtr m_sun_mat;
	float m_sun_mass = 2*pow(10,30);
	
	TexturePtr m_damaged_asteroid_tex;
	MeshPtr m_damaged_asteroid_mesh;
	MaterialPtr m_damaged_asteroid_mat;

	TexturePtr m_bullet_tex;
	MeshPtr m_bullet_mesh;
	MaterialPtr m_bullet_mat;


	TexturePtr m_finishing_sky_tex;
	MeshPtr m_finishing_sky_mesh;
	MaterialPtr m_finishing_sky_mat;
	float m_earth_mass = 5.9*pow(10,24);

	TexturePtr m_mars_tex;
	MeshPtr m_mars_mesh;
	MaterialPtr m_mars_mat;
	float m_mars_mass = 6.0*pow(10,23);
	float m_jupiter_mass = 2.0 * pow(10, 27);

	Vector3D m_mars_pos;
	Vector3D m_earth_pos;
	Vector3D m_jupiter_pos;


	TexturePtr m_jupiter_tex;
	MeshPtr m_jupiter_mesh;
	MaterialPtr m_jupiter_mat;

	MeshPtr m_gameOver_mesh;
	MaterialPtr m_gameOver_mat;

	TexturePtr m_sky_tex;
	MeshPtr m_sky_mesh;
	MaterialPtr m_sky_mat;

	MaterialPtr m_base_mat;

private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;


	float m_force;


	float m_current_cam_distance ;


    float m_cam_distance   = 14.0f;
	Vector3D m_cam_rot;
	Vector3D m_cam_pos;

	Vector3D m_current_cam_rot;
	//Vector3D m_current_cam_pos;

	float m_spaceship_speed ;
	float m_normal_speed = 1000.0f;
	float m_turbo_counter = 0;



	Vector3D m_current_spaceship_pos = Vector3D(0,0,-5000);



	Vector3D m_spaceship_pos;
	//Vector3D m_current_bullet_pos = m_spaceship_pos;

	Vector3D m_current_spaceship_rot;

	Vector3D m_spaceship_rot;

	float m_delta_mouse_x = 0.0f, m_delta_mouse_y = 0.0f;

	Vector3D m_asteroid_pos[200];
	Vector3D m_asteroid_rot[200];
	Vector3D m_asteroid_scale[200];
	Vector3D m_asteroid_actual_pos[200];
	float m_asteroid_mass[200];
	
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	bool m_turbo_mode  = false;


	bool cull = true;
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	float m_time = 0.0f;
	
	bool m_anim = false;
	bool m_intefered = false;
	bool m_success = false;

	
	Matrix4x4 m_light_rot_matrix;

	bool m_play_state = false;
	bool m_fullscreen_state = false;






	std::vector<MaterialPtr> m_list_materials;
	Vector4D m_light_position;
};

