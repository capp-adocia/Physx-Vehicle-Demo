#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/trackBallCameraControl.h"
#include "application/camera/GameCameraControl.h"
#include "application/camera/vehicleCameraControl.h"

#include "glframework/geometry.h"
#include "glframework/material/phongMaterial.h"
#include "glframework/material/whiteMaterial.h"

#include "mesh/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glframework/scene.h"
#include "application/assimpLoader.h"
#include "glframework/material/depth.h"
#include "glframework/material/opacityMaskMaterial.h"
#include "glframework/material/screenMaterial.h"
#include "glframework/framebuffer/FrameBuffer.h"
#include "glframework/material/cubeMaterial.h"
#include "glframework/material/phongEnvMaterial.h"
#include "glframework/material/phongInstanceMaterial.h"
#include "glframework/material/GrassInstanceMaterial.h"
#include "mesh/InstanceMesh.h"
#include <vld.h>
#include "physx_tool.h"

#define CameraControlType VehicleCameraControl

std::unique_ptr<Renderer> renderer = nullptr;
std::shared_ptr<Scene> sceneInscreen = nullptr;
//std::shared_ptr<Scene> scene = nullptr;

std::shared_ptr<DirectionalLight> dirLight = nullptr;

std::shared_ptr<AmbientLight> ambLight = nullptr;

std::shared_ptr<Camera> camera = nullptr;
std::shared_ptr<CameraControlType> cameraControl = nullptr;
std::shared_ptr<GrassInstanceMaterial> grassMaterial = nullptr;

glm::vec3 clearColor{ 0.6f, 0.7f, 0.6f };

// 记录上一帧的时间，用于计算帧率
std::shared_ptr<FrameBuffer> framebuffer = nullptr;

float lastTime = 0.0f; // 上一帧的时间
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float fps = 0.0f; // 帧率

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
}

void OnKey(int key, int scancode, int action, int mods) {
	// 处理特殊键
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(glApp.getWindow(), true); // 按下 ESC 键，关闭窗口
		return;
	}
	if (action == GLFW_PRESS || action == GLFW_RELEASE || action == GLFW_REPEAT) {
		// 处理油门、后退、转向
		switch (key) {
		case GLFW_KEY_UP:
			handleThrottle(action);
			break;
		case GLFW_KEY_DOWN:
			handleBackward(action);
			break;
		case GLFW_KEY_LEFT:
			handleSteering(action, true); // true表示左转
			break;
		case GLFW_KEY_RIGHT:
			handleSteering(action, false); // false表示右转
			break;
		default:
			break;
		}
		// 调用相机控制
		cameraControl->onKey(key, action, mods);
	}
}

void OnMouse(int button, int action, int mods) {
	double x, y;
	glApp.getCursorPosition(&x, &y);
	cameraControl->onMouse(button, action, x, y);
}

void OnCursor(double xpos, double ypos) {
	cameraControl->onCursor(xpos, ypos);
}

void OnScroll(double offset) {
	cameraControl->onScroll(offset);
}
// 设置模型颜色混合
void setModelBlend(std::shared_ptr<Object> object, bool blend, float opacity)
{
	if (object->getType() == ObjectType::Mesh)
	{
		auto mesh = std::dynamic_pointer_cast<Mesh>(object);
        mesh->getMaterial()->setBlend(blend);
        mesh->getMaterial()->setOpacity(opacity);
		mesh->getMaterial()->setDepthWrite(false);
	}
	auto children = object->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		setModelBlend(children[i], blend, opacity);
	}
}
// 设置某个实例物体的位置
void setInstanceMatrix(std::shared_ptr<Object> object, int index, glm::mat4 matrix)
{
	if (object->getType() == ObjectType::InstanceMesh)
	{
		auto mesh = std::dynamic_pointer_cast<InstanceMesh>(object);
		(*mesh)[index] = matrix;
	}
	auto children = object->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		setInstanceMatrix(children[i], index, matrix);
	}
}

void updateInstanceMatrix(std::shared_ptr<Object> object)
{
	if (object->getType() == ObjectType::InstanceMesh)
	{
		auto mesh = std::dynamic_pointer_cast<InstanceMesh>(object);
		mesh->updateMatrices();
	}
	auto children = object->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		updateInstanceMatrix(children[i]);
	}
}
// 为指定模型添加材质
void setInstanceMaterial(std::shared_ptr<Object> obj, std::shared_ptr<Material> material)
{
	if (obj->getType() == ObjectType::InstanceMesh)
	{
		auto mesh = std::dynamic_pointer_cast<InstanceMesh>(obj);
		mesh->setMaterial(material);
	}
	auto children = obj->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		setInstanceMaterial(children[i], material);
	}
}

void prepare() {
	renderer = std::make_unique<Renderer>();
	sceneInscreen = std::make_shared<Scene>();
	scene = std::make_shared<Scene>();

	framebuffer = std::make_shared<FrameBuffer>(glApp.getWidth(), glApp.getHeight());

	auto geometry = Geometry::createPlane(3.0f, 3.0f);
	auto geometry2 = Geometry::createBox(1.0f);
	// 离屏渲染的box
	auto materialA = std::make_shared<CubeMaterial>(); // 天空盒材质
	materialA->setDiffuse(std::make_shared<Texture>("assets/textures/bk.jpg", 0));
	auto meshA = std::make_shared<Mesh>(geometry2, materialA);
	scene->addChild(meshA);

	//贴到屏幕上的矩形
	auto geo = Geometry::createScreenPlane();
	auto mat = std::make_shared<ScreenMaterial>();
	mat->setScreenTexture(framebuffer->getColorAttachment()); // !!!重要!!!!
	auto mesh = std::make_shared<Mesh>(geo, mat);
	sceneInscreen->addChild(mesh);

	auto sphereGeo = Geometry::createSphere(1.0f);
	auto sphereMat = std::make_shared<PhongInstanceMaterial>();
	sphereMat->setDiffuse(std::make_shared<Texture>("assets/textures/earth.png", 0));

	dirLight = std::make_shared<DirectionalLight>();
	dirLight->mDirection = glm::vec3(-1.0f);
	dirLight->mSpecularIntensity = 0.1f;

	ambLight = std::make_shared<AmbientLight>();
	ambLight->mColor = glm::vec3(0.4f);
}

void prepareFBO()
{
	// 创建一个帧缓冲对象
	
	// 生成颜色附件

	// 生成depth Stencil附件，加入fbo

	// 检查帧缓冲对象是否完整

}

void prepareCamera() {
	//float size = 10.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	camera = std::make_shared<PerspectiveCamera>(
		60.0f, 
		(float)glApp.getWidth() / (float)glApp.getHeight(),
		0.1f,
		1000.0f
	);

	cameraControl = std::make_shared<CameraControlType>();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivity(0.07f);
	//cameraControl->setSpeed(0.05f);
	camera->mPosition = glm::vec3(0.0f, 2.5f, 12.0f);
}

void initIMGUI() {
	ImGui::CreateContext(); // 创建imgui上下文
	ImGui::StyleColorsDark(); // 选择一种深色主题
	// 初始化ImGui与GLFW和OpenGL的接口
	ImGui_ImplGlfw_InitForOpenGL(glApp.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void UpdateFPS()
{
	// 获取当前时间
	float currentTime = glfwGetTime();
	// 计算时间差
	deltaTime = currentTime - lastTime;
	// 更新帧率
	if (deltaTime > 0.0f) {
		fps = 1.0f / deltaTime;
	}
	// 更新上一帧的时间
	lastTime = currentTime;
}

void renderIMGUI()
{
	// 创建一个新的IMGUI帧
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 创建UI并放置一些控件
	ImGui::Begin("OpenGL"); // 创建一个新的窗口
	ImGui::Text("FPS: %.2f", fps); // 显示帧率
	// 坐标
	{
		// 获取小车的位置
		glm::mat4 model = scene->getChildren()[2]->getModelMatrixV2();
		glm::vec3 targetPosition = glm::vec3(model[3]); // 获取模型矩阵的平移部分，即物体的位置
		ImGui::Text("Position: (%.2f, %.2f, %.2f)", targetPosition.x, targetPosition.y, targetPosition.z);
	}
	// 速度
	{
		// 获取小车的线性速度
		const PxVec3 linVel1 = gVehicle.mPhysXState.physxActor.rigidBody->getLinearVelocity();
		ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", linVel1.x, linVel1.y, linVel1.z);

		// 计算速度的大小（即速度的模长）
		float speedMagnitude = linVel1.magnitude();
		float speedKmh = speedMagnitude * 3.6f; // 转成km/h
		ImGui::Text("Speed: %.2f Km/h", speedKmh); // 显示速度的大小
	}
	ImGui::End();
	// 执行UI渲染
	ImGui::Render();
	// 获取当前窗口的宽高
	int display_w, display_h;
	glfwGetFramebufferSize(glApp.getWindow(), &display_w, &display_h);
	// 设置OpenGL视口大小
	glViewport(0, 0, display_w, display_h);

	// 渲染ImGui的绘制数据
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
	glApp.init(1280, 768, "LearnOpenGL");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// 设置回调函数
	glApp.setResizeCallBack(OnResize);
	glApp.setKeyCallBack(OnKey);
	glApp.setMouseCallBack(OnMouse);
	glApp.setCursorCallBack(OnCursor);
	glApp.setScrollCallBack(OnScroll);

	// 初始化OpenGL接口和清空颜色
	GL_CALL(glViewport(0, 0, 1600, 1200));
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	prepareCamera(); // 准备相机
	// 准备帧缓冲对象
	prepare(); // 准备其他资源
	initIMGUI(); // 初始化IMGUI
	initPhysics(); // 初始化物理引擎
	preparePhysics(); // 准备渲染图形

	// 期望的每帧时间：1/60秒 (60 FPS)
	const float targetFrameTime = 1.0f / 60.0f;
	while (glApp.update()) { // 主循环

		stepPhysics();
		//gScene->simulate(1.0f / 60.0f);
		//gScene->fetchResults(true);
		// render physx shapes
		const int MAX_NUM_ACTOR_SHAPES = 128;

		PxU32 nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		PxArray<PxRigidActor*> actors(nbActors);
		gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);

		PxShape* shapes[MAX_NUM_ACTOR_SHAPES] = { 0 };
		
		for (PxU32 i = 0; i < static_cast<PxU32>(actors.size()); i++)
		{
			const PxU32 nbShapes = actors[i]->getNbShapes();
			PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
			actors[i]->getShapes(shapes, nbShapes);
			
			for (PxU32 j = 0; j < nbShapes; j++) // nbShapes
			{
				// 获取形状的变换矩阵
				const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
				// 获取形状的几何信息
				const PxGeometry& shapeGeometry = shapes[j]->getGeometry();
				// 设置model矩阵
				
				switch (shapeGeometry.getType())
				{
				case PxGeometryType::eBOX:
				{
					scene->getChildren()[2]->setModelMatrix(glm::make_mat4(&shapePose.column0.x)); // 跳过天空盒和地面
					break;
				}
				case PxGeometryType::eCONVEXMESH:
				{
					scene->getChildren()[2 + j]->setModelMatrix(glm::make_mat4(&shapePose.column0.x));
					break;
				}
				}
			}
		}
		if (auto temp = std::dynamic_pointer_cast<VehicleCameraControl>(cameraControl))
		{
			// 开启相机跟随
			glm::mat4 model = scene->getChildren()[2]->getModelMatrixV2();
			glm::vec3 targetPosition = glm::vec3(model[3]); // 获取模型矩阵的平移部分，即物体的位置
			// 提取小车的前向方向（假设模型矩阵的Z轴为前进方向）
			glm::vec3 forward = glm::normalize(glm::vec3(model[2]));

			// 计算相机参数
			float distanceBehind = 5.0f; // 距离小车后方距离
			float height = 1.5f;         // 相机高度偏移
			const glm::vec3 cameraPos = targetPosition - forward * distanceBehind + glm::vec3(0, height, 0);
			const glm::vec3 cameraTarget = targetPosition + forward; // 看向小车前方

			temp->setCameraPosTarget(cameraPos, cameraTarget); // 设置相机位置和目标点
		}
		// 记录开始时间
		auto frameStartTime = std::chrono::high_resolution_clock::now();
		UpdateFPS();
		cameraControl->update(); // 更新相机控制

		renderer->setClearColor(clearColor); // 设置渲染器的清除颜色
		renderer->render(scene, camera, dirLight, ambLight, framebuffer->getFBO()); // 渲染到离屏缓冲区
		renderer->render(sceneInscreen, camera, dirLight, ambLight); // 渲染到屏幕
		//renderer->render(scene, camera, dirLight, ambLight);
		renderIMGUI(); // 渲染IMGUI界面

		 // 计算当前帧渲染的时间
		auto frameEndTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> frameDuration = frameEndTime - frameStartTime;
		float frameTime = frameDuration.count();

		// 如果渲染时间小于 16.67 毫秒，则睡眠直到满足 60 FPS
		if (frameTime < targetFrameTime) {
			std::this_thread::sleep_for(std::chrono::duration<float>(targetFrameTime - frameTime));
		}
	}
	releasePhysics();
	glApp.destroy();

	return 0;
}