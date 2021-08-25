#include "SpaceShooterGame.h"


#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include  "MathUtils.h"
#include "Material.h"
#include <time.h>
#include "gravity.h"
#include <iostream>

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;

	Vector4D m_light_position = Vector4D(0, 1, 0, 0);
	float m_light_radius = 5000.0f;
	float m_time = 0.0f;

};


SpaceShooterGame::SpaceShooterGame()
{

	
}

void SpaceShooterGame::render()

{

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0.0f, .0f, 0.0f, 0.9f);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);





	//Render SKY



	//RENDER SPACESHIP
	if (m_success )
	{
		
		m_sun_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\Sucess.png");

		MaterialPtr m_success_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
		m_success_mat->addTexture(m_sun_tex);
		m_success_mat->setCullMode(CULL_MODE_BACK);
	
		renderHelper(m_success_mat, m_sun_mesh, Vector3D(0, 0, 90000), Vector3D(0, 3.57*1.57/2, 0), Vector3D(4000, 4000, 4000));
	}
	
	else if (m_intefered || m_collided )
	{
		
	
	
		
		m_sun_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\GameOver.png");

		MaterialPtr m_gameover_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
		m_gameover_mat->addTexture(m_sun_tex);
		m_gameover_mat->setCullMode(CULL_MODE_BACK);
		
		renderHelper(m_gameover_mat, m_sun_mesh, Vector3D(0, 0, 90000), Vector3D(0, 1.57, 0), Vector3D(4000, 4000, 4000));
	}
	else
	{
		
		m_list_materials.clear();
	m_list_materials.push_back(m_sky_mat);
	drawMesh(m_sky_mesh, m_list_materials);

		renderHelper(m_spaceship_mat, m_spaceship_mesh, m_current_spaceship_pos, m_spaceship_rot , Vector3D(1, 1, 1));



		if (m_spaceship_pos.m_z > -20000)
		{
			//ASTEROIDS
			m_list_materials.clear();

			m_list_materials.push_back(m_asteroid_mat);




			for (unsigned int i = 0; i < 200; i++)
			{

				m_asteroid_actual_pos[i] = getPositionofRevolvings(Vector3D(0, 0, 80000), m_asteroid_pos[i].m_z + 20 * i + 00000, (m_asteroid_pos[i].m_z + 70000), (i % 250 + 0.5f) / (1000)) + Vector3D(0, m_spaceship_pos.m_y + pow(-1, i), i * 0.001);

				updateModel(m_asteroid_actual_pos[i], m_asteroid_rot[i], m_asteroid_scale[i], m_list_materials);


				drawMesh(m_asteroid_mesh, m_list_materials);

			}

			// DEFORMED ASTEROID


			if (m_collided)
			{

				renderHelper(m_damaged_asteroid_mat, m_asteroid_mesh, Collidedpos, Vector3D(0, 5 * m_time, 0), Collidedscale);

			}






		}

		//Render Earth
		m_earth_pos = getPositionofRevolvings(Vector3D(0, 0, 80000), 25000.0f, 24000.0f);
		renderHelper(m_finishing_sky_mat, m_finishing_sky_mesh, getPositionofRevolvings(Vector3D(0, 0, 80000), 25000.0f, 24000.0f), Vector3D(1, 0.25 * m_time, 1), Vector3D(3000, 3000, 3000));

		// RENDER MARS
		m_mars_pos = getPositionofRevolvings(Vector3D(0, 0, 80000), 40000.0f, 38000.0f);
		renderHelper(m_mars_mat, m_mars_mesh, m_mars_pos, Vector3D(1, 0.9 * m_time, 1), Vector3D(1000, 1000, 1000));
		//RENDER SUN
		renderHelper(m_sun_mat, m_sun_mesh, Vector3D(0, 0, 80000), Vector3D(0, m_time, 0), Vector3D(4000, 4000, 4000));
		//RENDER JUPITER
		m_jupiter_pos = getPositionofRevolvings(Vector3D(0, 0, 80000), 75000.0f, 73000.0f, 0.4);
		renderHelper(m_jupiter_mat, m_jupiter_mesh, m_jupiter_pos, Vector3D(0, 0.9 * m_time, 0), Vector3D(3000, 3000, 3000));
		for (int i = 0; i < 4; i++)
		{
			renderHelper(m_asteroid_mat, m_asteroid_mesh, getPositionofRevolvings(m_jupiter_pos, 4000.0 + 200 * i, 4000.0f + 200 * i, i), Vector3D(0, 0.9 * m_time, 0), Vector3D(300, 300, 300));
		}


	}
	


	m_swap_chain->present(true);
	strikewithplanets();
	if (m_intefered)
	{
		//m_current_spaceship_pos = Vector3D::lerp(m_spaceship_pos, getPositionofRevolvings(m_mars_pos, 3000, 2750), 3 * m_delta_time);

	}
	
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	

	m_time += m_delta_time;
}

void SpaceShooterGame::update()
{
	
	updateSpaceship();
	updateThirdPersonCamera();
	updateLight();
	updateSkyBox();
	checkCollision();


}

void SpaceShooterGame::updateModel(Vector3D position, Vector3D rotation, Vector3D scale,const std::vector<MaterialPtr>& list_materials)
{
	constant cc;

	Matrix4x4 temp;
	cc.m_world.setIdentity();

	temp.setIdentity();
	temp.setScale(scale);

	cc.m_world *= temp;



	temp.setIdentity();
	temp.setRotationX(rotation.m_x);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(rotation.m_y);

	cc.m_world *= temp;


	temp.setIdentity();
	temp.setRotationZ(rotation.m_z);

	cc.m_world *= temp;


	temp.setIdentity();
	temp.setTranslation(position);

	cc.m_world *= temp;


	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();


	cc.m_light_position = m_light_position;
	cc.m_light_radius =0.0f;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;


	for (size_t m = 0; m < list_materials.size(); m++)
	{
		list_materials[m]->setData(&cc, sizeof(constant));
		list_materials[m]->position = position;
	}

}

void SpaceShooterGame::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_cam_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_cam_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;



	temp.setIdentity();
	temp.setRotationX(m_cam_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_cam_rot.m_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.05f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.05f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void SpaceShooterGame::updateThirdPersonCamera()
{

	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_cam_rot.m_x +=m_delta_mouse_y *m_delta_time * 0.1f;
	m_cam_rot.m_y += m_delta_mouse_x *m_delta_time * 0.1f;

	if (m_cam_rot.m_x >= 1.57f)  m_cam_rot.m_x = 1.57f;
	else if (m_cam_rot.m_x <= -1.57f)  m_cam_rot.m_x = -1.57f;


	m_current_cam_rot = Vector3D::lerp(m_current_cam_rot, m_cam_rot, 3.0f * m_delta_time);


	temp.setIdentity();
	temp.setRotationX(m_current_cam_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_current_cam_rot.m_y);
	world_cam *= temp;

	

	m_cam_pos = m_current_spaceship_pos;


	if (m_forward)
	{

		if (m_turbo_mode)
		{

			if (m_forward > 0.0f) m_cam_distance = 30.0f;
			else m_cam_distance = 5.0f;


		}
		else
		{
if (m_forward > 0.0f)  m_cam_distance = 16.0f;
		
		else m_cam_distance = 9.0f;
		}
		
		



	}
	else
	{
		m_cam_distance = 14.0f;

	}

	 
	m_current_cam_distance = lerp(m_current_cam_distance, m_cam_distance, 2.0f * m_delta_time);



	Vector3D new_pos = m_cam_pos + world_cam.getZDirection() * (-m_current_cam_distance);
	new_pos = new_pos + world_cam.getYDirection() * (5.0f);

	

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 50000000.0f);


}

void SpaceShooterGame::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(450000.0f, 450000.0f, 450000.0f));

	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	m_sky_mat->setData(&cc, sizeof(constant));
	

}

void SpaceShooterGame::updateLight()
{
	//strikewithplanets();
	Matrix4x4 temp;
	temp.setIdentity();
	m_light_rot_matrix.setIdentity();


	temp.setIdentity();
	temp.setRotationX(-0.707f);
	m_light_rot_matrix *= temp;



	temp.setIdentity();
	temp.setRotationY(0.707f);
	m_light_rot_matrix *= temp;
}

void SpaceShooterGame::updateSpaceship()
{
	Matrix4x4 world_model, temp;
	world_model.setIdentity();
	

	m_spaceship_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_spaceship_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	if (m_spaceship_rot.m_x >= 1.57f)  m_cam_rot.m_x = 1.57f;
	else if (m_spaceship_rot.m_x <= -1.57f)  m_cam_rot.m_x = -1.57f;

       m_current_spaceship_rot = Vector3D::lerp(m_current_spaceship_rot, m_spaceship_rot, 5.0f * m_delta_time);

	temp.setIdentity();
	temp.setRotationX(m_current_spaceship_rot.m_x);
	world_model *= temp;

	temp.setIdentity();
	temp.setRotationY(m_current_spaceship_rot.m_y);
	world_model *= temp;

	
	m_spaceship_speed ;
	if (m_turbo_mode) {
		m_spaceship_speed =1500.0f;

	}
	else
	{
		m_spaceship_speed = m_normal_speed;
	}
	

	
	
	
	if (m_spaceship_pos.m_z > 50000)
	{
m_spaceship_pos = m_spaceship_pos + world_model.getZDirection() * (m_forward)*m_spaceship_speed * m_delta_time;
	m_current_spaceship_pos = Vector3D::lerp(m_current_spaceship_pos, m_spaceship_pos, 3.0f * m_delta_time);
	m_spaceship_pos = m_spaceship_pos + world_model.getXDirection() * (m_rightward) * 125.0f * m_delta_time;
	
	}
	else
	{
		if (m_spaceship_pos.m_x > 1000 || m_spaceship_pos.m_x < -1000)
		{
			m_spaceship_pos = m_spaceship_pos + world_model.getZDirection() * (m_forward)*m_spaceship_speed * m_delta_time;
			m_current_spaceship_pos = Vector3D::lerp(m_current_spaceship_pos, m_spaceship_pos, 3.0f * m_delta_time);
			m_spaceship_pos = m_spaceship_pos + world_model.getXDirection() * (m_rightward) * 0.0f * m_delta_time;
		}
		else
		{
			m_spaceship_pos = m_spaceship_pos + world_model.getZDirection() * (m_forward)*m_spaceship_speed * m_delta_time;
			m_current_spaceship_pos = Vector3D::lerp(m_current_spaceship_pos, m_spaceship_pos, 3.0f * m_delta_time);
			m_spaceship_pos = m_spaceship_pos + world_model.getXDirection() * (m_rightward) * 125.0f * m_delta_time;
		}
	}

	

	
	
	

	
	

	

}

void SpaceShooterGame::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{
	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());


	for (size_t m = 0; m < mesh->getNumMaterialSlots(); m++)
	{

		if (m >= list_materials.size()) break;

		MaterialSlot mat = mesh->getMaterialSlot(m);

		GraphicsEngine::get()->setMaterial(list_materials[m]);
		// FINALLY DRAW THE TRIANGLE
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
 
	}

}

SpaceShooterGame::~SpaceShooterGame()
{
}

void SpaceShooterGame::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this); 

	m_play_state = true;
	InputSystem::get()->showCursor(false);

    RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	
	srand((unsigned int) time(NULL));

	for (unsigned int i = 0; i < 200; i++)
	{
		float x = rand() % 2000 + (-1000);
		float y = rand() % 1800 + (-900);
		float z = rand() % 3600 + (-1800);
		if ((x <= 10&& x >= -10) && (y <= 10 && y >= -10) && (z <= 10 && z >= -10))
		{
			x = x + 100;
			y = y + 100;
			z = z + 100;
		}
		m_asteroid_pos[i] = Vector3D( x*1.25,y*1.25,z*1.25);

		m_asteroid_rot[i] = Vector3D((rand() % 628)/100.0f , (rand() % 628) / 100.0f, (rand() % 628) / 100.0f);
		float scale = (rand() % 20 + (6));

		m_asteroid_scale[i] = Vector3D(scale,scale,scale);
		m_asteroid_mass[i] = (rand() % 4 + 28) * pow(10, 21);


	}
	m_intefered = false;


	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\stars_map.jpg");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\sphere.obj");
	
	m_bullet_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\bullet.jpg");
	m_bullet_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\Spaceshipbullet.obj");

	m_spaceship_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\spaceship.jpg");
	m_spaceship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\spaceship.obj");

	m_asteroid_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\asteroid.jpg");
	m_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\asteroid.obj");


	m_sun_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\sunmap.jpg");
	m_sun_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\Sun.obj");


	m_finishing_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\earth_color.jpg");
	m_finishing_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\sphere.obj");

	m_mars_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\mars.jpg");
	m_mars_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\sphere.obj");

	m_asteroid_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\asteroid.jpg");
	m_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\asteroid.obj");


	m_jupiter_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\jupitermap.jpg");
	m_jupiter_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\sphere.obj");

	
	m_gameOver_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\Gameoverfont.obj");

	m_damaged_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\destroyedasteroid.obj");




	m_base_mat = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	m_base_mat->setCullMode(CULL_MODE_BACK);

	m_spaceship_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_spaceship_mat->addTexture(m_spaceship_tex);
	m_spaceship_mat->setCullMode(CULL_MODE_BACK);


	m_sun_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_sun_mat->addTexture(m_sun_tex);
	m_sun_mat->setCullMode(CULL_MODE_BACK);

	m_asteroid_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_asteroid_mat->addTexture(m_asteroid_tex);
	m_asteroid_mat->setCullMode(CULL_MODE_BACK);

	m_damaged_asteroid_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_damaged_asteroid_mat->addTexture(m_asteroid_tex);
	m_damaged_asteroid_mat->setCullMode(CULL_MODE_BACK);

	


	m_sky_mat = GraphicsEngine::get()->createMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	m_sky_mat->addTexture(m_sky_tex);
	m_sky_mat->setCullMode(CULL_MODE_FRONT);

	m_finishing_sky_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_finishing_sky_mat->addTexture(m_finishing_sky_tex);
	m_finishing_sky_mat->setCullMode(CULL_MODE_BACK);


	m_mars_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_mars_mat->addTexture(m_mars_tex);
	m_finishing_sky_mat->setCullMode(CULL_MODE_BACK);

	m_jupiter_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_jupiter_mat->addTexture(m_jupiter_tex);

	m_gameOver_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_gameOver_mat->addTexture(m_jupiter_tex);
	


	m_bullet_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_bullet_mat->addTexture(m_bullet_tex);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));
    m_list_materials.reserve(32);
	m_intefered = false;
}

void SpaceShooterGame::onUpdate()
{
	
	Window::onUpdate();
	InputSystem::get()->update();

	this->update();
	
	this->render();
	
	if (m_collided)
	{
		//GameOver();

		m_spaceship_speed = 0.0f;
		m_normal_speed = 0.0f;
		m_turbo_mode = false;
		m_current_spaceship_pos = m_spaceship_pos - Vector3D(0, 0, 50);
	};
	
}

void SpaceShooterGame::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}

void SpaceShooterGame::onFocus()
{
	InputSystem::get()->addListener(this);
}

void SpaceShooterGame::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void SpaceShooterGame::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right-rc.left, rc.bottom - rc.top);
	this->render();
}

void SpaceShooterGame::onKeyDown(int key)
{
	if (!m_play_state) return;

	if (key == 'W')
	{
		
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f*m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		
		m_rightward = 1.0f;
	}
	
	else if (key == VK_SHIFT)
	{
		if(!m_intefered && !m_collided)
			m_turbo_mode = true;
		

		
		


	}
	else if (key == VK_SPACE)
	{

	
		updateModel(m_current_spaceship_pos, m_current_spaceship_rot, Vector3D(1, 1, 1), m_list_materials);

		

	}


	

}

void SpaceShooterGame::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == VK_ESCAPE)
	{
		if (m_play_state) {
                      m_play_state =  false ;
	                	InputSystem::get()->showCursor(!m_play_state);
		 }
		
	}
	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = this->getSizeScreen();

		m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
	}

	else if (key == VK_SHIFT)
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_turbo_mode = false;
	}
	else if (key == 'I')
	{
		m_spaceship_pos = Vector3D(0,0,-5000);
	}

}

void SpaceShooterGame::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	RECT win_size = this->getClientWindowRect();


	int width = (win_size.right - win_size.left);
	int height = (win_size.bottom - win_size.top);  

	m_delta_mouse_x =(int) (mouse_pos.m_x - (int)(win_size.left + (width / 2.0f)));
	m_delta_mouse_y = (int)(mouse_pos.m_y - (int)(win_size.top + (height / 2.0f)));

	


	InputSystem::get()->setCursorPosition(Point(win_size.left +(int)(width / 2.0f), win_size.top + (int)(height / 2.0f)));

}

void SpaceShooterGame::onLeftMouseDown(const Point& mouse_pos)
{
	if (!m_play_state) {
		m_play_state = true;
		InputSystem::get()->showCursor(!m_play_state);
	}
	
}

void SpaceShooterGame::onLeftMouseUp(const Point& mouse_pos)
{
	
}

void SpaceShooterGame::onRightMouseDown(const Point& mouse_pos)
{
	
}

void SpaceShooterGame::onRightMouseUp(const Point& mouse_pos)
{
	
}

void SpaceShooterGame::checkCollision()
{
	if (m_spaceship_pos.m_z <= 15)
	{
		
		return ;
	}
	

	//FOR ASTEROIDS
	for (unsigned int i = 0; i < 200; i++)
	{
		Vector3D* dist =new Vector3D( m_spaceship_pos - m_asteroid_actual_pos[i]);
		float magnitude = pow((dist->m_x *dist-> m_x +dist-> m_y *dist-> m_y +dist-> m_z * dist->m_z), 0.5)*pow(10,3);
		
		Vector3D* scale = new Vector3D(m_asteroid_scale[i]);
		float scalemagn = pow((scale->m_x * scale->m_x + scale->m_y * scale->m_y + scale->m_z * scale->m_z), 0.5);
		gravity* gr = new gravity(m_asteroid_mass[i],magnitude);
		float force = gr->getfieldintensity() * m_spaceship_mass;
		
		if (force >= 10000)
		{	
		m_spaceship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\deformedspaceship.obj");

		Collidedpos = m_asteroid_actual_pos[i];
		Collidedscale = Vector3D(5, 7.5, 2.5);
		
		m_asteroid_scale[i] = Vector3D(0.2, 0.2, 0.2);

		m_collided = true;
		delete gr;
		delete scale;
		delete dist;
		
		break;
		}


	}

	
}

void SpaceShooterGame::strikewithplanets()
{
	Vector3D* dist_mars = new Vector3D(m_spaceship_pos - m_mars_pos);
	float magnitude_mars = pow((dist_mars->m_x * dist_mars->m_x + dist_mars->m_y * dist_mars->m_y + dist_mars->m_z * dist_mars->m_z), 0.5) * pow(10, 3);
	gravity* gr = new gravity(m_mars_mass, magnitude_mars);
	
	 m_force = gr->getfieldintensity() * m_spaceship_mass;
	 if (m_force >= 200)
	 {
		 m_intefered = true;
	 }
	
	 m_force = 0;
	 Vector3D* dist_earth = new Vector3D(m_spaceship_pos - m_earth_pos );
	 float magnitude_earth = pow((dist_earth->m_x * dist_earth->m_x + dist_earth->m_y * dist_earth->m_y + dist_earth->m_z * dist_earth->m_z), 0.5) * pow(10, 3);
	 gr = new gravity(m_earth_mass, magnitude_earth);
	 m_force = gr->getfieldintensity() * m_spaceship_mass;
	
	
	 if (m_force > 200)
	 {
		 m_turbo_mode = false;
		 m_play_state = false;
		 m_current_spaceship_pos = m_earth_pos+Vector3D(4000,4000,4000);
		 m_success = true;
	}

	 Vector3D* dist_jupiter = new Vector3D(m_spaceship_pos - m_jupiter_pos);
	 float magnitude_jupiter = pow((dist_jupiter->m_x * dist_jupiter->m_x + dist_jupiter->m_y * dist_jupiter->m_y + dist_jupiter->m_z * dist_jupiter->m_z), 0.5) * pow(10, 3);
 gr = new gravity(m_jupiter_mass, magnitude_jupiter);

	 m_force = gr->getfieldintensity() * m_spaceship_mass;
	
	 

	
		
		 m_intefered = m_force >= 100000;
	


}



Vector3D SpaceShooterGame::getPositionofRevolvings(Vector3D center, float longradii, float shortradii)
{

	float x = center.m_x + longradii * cos(.2 * m_time);
	float y = 0.0f;
	float z = center.m_z + shortradii * sin(.2 * m_time);

	return Vector3D(x, y, z);


}

Vector3D SpaceShooterGame::getPositionofRevolvings(Vector3D center, float longradii, float shortradii, float angle)
 {
	if (m_time < 0.01)
	{
		float x = center.m_x + longradii * cos(1.57 + angle + m_time / 1);
		float y = 0;
		float z = center.m_z + shortradii * sin(1.57 + angle + m_time / 1);
		return Vector3D(x, y, z);
	}
	else
	{
    float x = center.m_x + longradii * cos(1.57+angle* m_time/2);
	float y = 0;
	float z = (center.m_z + shortradii * sin(1.57+angle * m_time/(2)));
	return Vector3D(x, y, z);
	}
	

	}

void SpaceShooterGame::renderHelper(MaterialPtr mat,MeshPtr mesh, Vector3D pos, Vector3D rot, Vector3D scale)
{
	m_list_materials.clear();
	m_list_materials.push_back(mat);


	updateModel(pos, rot, scale, m_list_materials);

	drawMesh(mesh, m_list_materials);
}

void SpaceShooterGame::normalizeCamera()
{
	

	
}


