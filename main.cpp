#define GLFW_DLL

// OpenGL
#include "GL/glew.h"	// glViewPort...
#include <GLFW/glfw3.h>	// glfw*

// C
#include <cstdlib>	// EXIT_SUCCESS, EXIT_FAILURE, std::exit
#include <cstdio>	// TODO: Is this needed?
#include <cmath>	// sin, cos, ...

// C++
#include <iostream>	// std::cout
#include <list>		// List<RenderObject>
#include <string>	// std::string
#include <iomanip>	// std::setprecision
#include <list>
#include <sstream>


// Custom
#include "math/mat4.h"
#include "math/vec2.h"
#include "renderer/RenderObject.h"
#include "generators/Simplex.h"
#include "generators/Perlin.h"

// Used to get the current directory, can use later for something?
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define abs(x) ((x)<0 ? -(x) : (x))
#endif

Vec3 p1, p2, p3;

// Position Data
Vec3 Camera(0.0f, 1.0f, 0.0f);

double horizontalAngle = 0.0;
double verticalAngle = 0.0;
double initialiFOV = 45.0;
float initial_speed = 0.00005f;
double mouseSpeed = 0.0005;

// The window and related data
static GLFWwindow *window;
static float height = 768, width = 1024;

const float znear	=  0.0001f;
const float zfar	= 100.0f;

Mat4 TranslateMatrix;
Mat4 ProjectionMatrix = Mat4::Perspective(90.0f, (float)width / (float)height, znear, zfar);


#define checkGL() {							\
	GLenum err = glGetError();				\
	if (err != GL_NO_ERROR) {				\
		std::cout << err << ": " <<			\
			gluErrorString(err) <<			\
			" at line " << __LINE__ <<		\
			" in file " << __FILE__ <<		\
			std::endl;						\
		std::exit(-1);						\
	}										\
}
GLenum MODES[2] = { GL_FILL, GL_LINE };
int mode = 0;

float sqr(float a) { return a * a; }

float interpolateFloat(float min, float max, float ratio) {
	return min + (max-min) * ratio;
}

//////////////////////////////////////////////
//			Simple Random Data				//
//////////////////////////////////////////////
float* generateGround(float min_x, float max_x, float min_z, float max_z, int div, int numMountains) {

	// Generate mountains
	std::list<Vec3> mountains;
	while (numMountains--) {
		
while (true) {
			bool add = true;

			float x = interpolateFloat(.01f + min_x, max_x - .01f, rand() / float(RAND_MAX)); // [0...1)
			float y = interpolateFloat(.1f, .01f, rand() / float(RAND_MAX)); // [0...1)
			float z = interpolateFloat(.01f + min_z, max_z - .01f, rand() / float(RAND_MAX)); // [0...1)

			Vec3 newMountain(x, y, z);

			// Only add if not to close to other mountains
			for (Vec3 m : mountains) {
				double dist = (m - newMountain).getMagnitude();
				if (dist < 0.01) {
					add = false;
					break;
				}
			}

			if (add) {
				mountains.insert(mountains.begin(), newMountain);
				break;
			}
		}
	}


	init_simplex(242342);

	float x_len = max_x - min_x;
	float z_len = max_z - min_z;
	float delta_x = x_len / (div-1);
	float delta_z = z_len / (div-1);
	float* data = (float*)calloc(3 * div * div, sizeof(float));
	
	for (int i = 0; i < div; i++) { // z
		float z = max_z - i * delta_z;
		for (int j = 0; j < div; j++) { // x
			float x = min_x + j * delta_x;
			int pos = 3 * i * div + 3 * j;
			data[pos] = x;


			int num_seen = 0;
			float height = 0;
			for (Vec3 m : mountains) {
				float dx = abs(x - m.getX());
				if (dx < 0.01) {
					float dz = abs(z - m.getZ());
					if (dz < .001) {
						++num_seen;
						float scale = dx + dz;
						float wave = 1.0f * float(abs(sin(x * z))) / (1.0f + scale);
						height += wave + m.getY()/(2.0f + sqrt(scale + 1.0f));
					}
				}
			}

			if (num_seen) {
				height = height / num_seen;
			}
			
			//data[pos + 1] = height + (float)simplex2d( x , z ,7,2.323f)/10.0f;
			double d[2] = { x, z };
			//data[pos + 1] = height + 50.0f * (float)perlin2d(d);
			data[pos + 1] = height + 0.005f * (rand() / float(RAND_MAX));
			//data[pos + 1] = z / 8.0f;
			//data[pos + 1] = height + 1.0f;
			data[pos + 2] = z;
		}
	}
	return data;
}

float getY(const Vec3 &A, const Vec3 &B, const Vec3 &C, float x, float z) {
	Vec3 Normal = (C - A).cross(B - A);
	Normal.normalize();
	if (Normal.getY() == 0.0) return A.getY();
	float d = -(A * Normal);
	return -(Normal.getX() * x + Normal.getZ() * z + d) / Normal.getY();
}

GLuint* generateIndices(int div) {
	GLuint* data = (GLuint*)calloc((div - 1) * (div - 1) * 6, sizeof(GLuint));
	int max_i, max_j;
	max_i = max_j = div - 1;

	int A = 0;
	int B = 1;
	int C = div;
	int D = div + 1;

	int i = 0;
	for (; i < max_i; i++){
		int j = 0;
		for (; j < max_j; j++) {
			int pos = 6 * (i * (div - 1) + j);

			// Create indices
			// Triangle One
			data[pos] = A;			// 0
			data[pos + 1] = B;		// 1
			data[pos + 2] = D;		// 3

			// Triangle Two
			data[pos + 3] = A;		// 0
			data[pos + 4] = D;		// 3
			data[pos + 5] = C;		// 2

			A++;
			B++;
			C++;
			D++;
		}
		A++;
		B++;
		C++;
		D++;
	}
	return data;
}

bool shift_down = false;
bool enable_flying = false;
bool handleKey(int key, int check_key) {
	if (key == check_key) return true;
	return glfwGetKey(window, check_key) != GLFW_RELEASE;
}

void update() {

	float speed = initial_speed;

	if (enable_flying) {
		speed *= 20.0f;
	}
	
	if (shift_down) {
		speed *= 10.0f;
	}

	double xpos,ypos;

	glfwGetCursorPos(window, &xpos, &ypos);
	
	double new_x = mouseSpeed * (width / 2 - xpos);
	double new_y = mouseSpeed * (height / 2 - ypos);


	if (abs(new_x) > 0.001) {
		horizontalAngle += new_x;
	}

	if (abs(new_y) > 0.001) {
		verticalAngle += new_y;
	}

	double sVA = sin(verticalAngle);
	double cVA = cos(verticalAngle);
	
	double sHA = sin(horizontalAngle);
	double cHA = cos(horizontalAngle);
	

	Vec3 direction(
		float(cVA * sHA),
		(float)sVA,
		(float)(cVA * cHA)
	);

	Vec3 right(
		float(sin(horizontalAngle - 1.570796325)),
		0.0f,
		float(cos(horizontalAngle - 1.570796325))
	);

	Vec3 up = right.cross(direction);

	if (glfwGetKey(window, GLFW_KEY_W) != GLFW_RELEASE) {
		Camera += direction * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) != GLFW_RELEASE) {
		Camera -= direction * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) != GLFW_RELEASE) {
		Camera += right * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) != GLFW_RELEASE) {
		Camera -= right * speed;
	}

	TranslateMatrix = Mat4::LookAt(Camera, Camera + direction, up);

	glfwSetCursorPos(window, width / 2, height / 2);
}

//////////////////////////////////////////////
//				Callbacks					//
//////////////////////////////////////////////
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	shift_down = mods == GLFW_MOD_SHIFT;

	if (action == GLFW_REPEAT || action == GLFW_PRESS){

		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key == GLFW_KEY_P) {
			mode = (mode + 1) % 2;
			glPolygonMode(GL_FRONT_AND_BACK, MODES[mode]);
		}

		if (key == GLFW_KEY_SPACE) {
			enable_flying = !enable_flying;
		}

		if (key == GLFW_KEY_LEFT) {
			horizontalAngle = -1.57079632679;
			verticalAngle = 0.0f;
		}

		if (key == GLFW_KEY_RIGHT) {
			horizontalAngle = 1.57079632679;
			verticalAngle = 0.0f;
		}

		if (key == GLFW_KEY_UP) {
			horizontalAngle = 0.0f;
			verticalAngle = 0.0f;
		}

		if (key == GLFW_KEY_DOWN) {
			horizontalAngle = 3.14159265359f;
			verticalAngle = 0.0f;
		}
	}
}

void resized_callback(GLFWwindow *window, int w, int h) {
	width = float(w);
	height = float(h);
	glViewport(0, 0, w, h);

	ProjectionMatrix = Mat4::Perspective(45.0f, width / height, znear, zfar);
	update();
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

//////////////////////////////////////////////
//				Helper Functions			//
//////////////////////////////////////////////
// Initializes all the subsystems, create the window.
void init() {

	// Set up the error callback for GLFW
	glfwSetErrorCallback(error_callback);

	// Initialize the GLFW system, if failure jump ship
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	int major = 3;
	int minor = 1;
	#ifdef __MAC 
	minor = 2;
	#endif

	// Set the hints on how to render to the 
	// screen, which OpenGL version we have, etc
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	if (major >= 3 && minor > 1) {
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// Create our window using the hints given above
	window = glfwCreateWindow((int)width, (int)height, "Terrain Generator", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}glfwMakeContextCurrent(window);

	// Set callbacks for GLFW, window, and keyboard.
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resized_callback);

	// Disable the mouse so that we can lookaround
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Setup GLEW, enable newer OpenGL functions
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		glfwTerminate();
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(EXIT_FAILURE);
	}glGetError();

	// The background color should be a 
	// nice light blue color
	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

}

// Just prints OpenGL information
void print() {
	char current[FILENAME_MAX];
	if (GetCurrentDir(current, sizeof(current))){
		printf("Current Working Directory : %s\n", current);
	}
	printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL Vendor: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL Vendor: %s\n", glGetString(GL_VERSION));
}

float area(const Vec3& A, const Vec3& B, const Vec3& C) {
	Vec3 AB = A - B;
	Vec3 AC = A - C;
	float theta = acos(AB * AC);
	return float(0.5f * AB.getMagnitude() * AC.getMagnitude() * sin(theta));
}

float interpolate(float min, float max, float alpha) {
	return min * (1.0f - alpha) + max * alpha;
}

#define clamp(v,a,b) (v<a)?a:(v>b)?b:v

Vec3 getAsVec3(const GLfloat *data, int i, int j, int div) {
	return Vec3(data + 3 * (j * div + i));
}

float getHeight(RenderObject &ground) {
	
	float result = 80.0;

	// Get ground data
	const GLfloat *data = ground.getRawData();

	// Get current x and y position in world space
	float x = Camera.getX();
	float z = Camera.getZ();
		
	// Guess the number of divisions
	int divisions = (int)sqrt(ground.getNumberVertices() / 3);
	
	// Guess the deltas
	float min_x = data[0];
	float max_z = data[2];

	float del = abs(2.0f * data[0]) / (divisions-1);

	// x and z position index
	float x_index = (x - min_x) / del;
	float z_index = (max_z - z) / del;

	int x_idx = int(x_index);
	int z_idx = int(z_index);

	// If we are on the ground
	if (x_idx >= 0 && x_idx <= divisions && z_idx >= 0 && z_idx <= divisions ) {
		
		// Get inner point in the square
		float low_x = x_index - x_idx;
		float low_z = z_index - z_idx;

		int upper = clamp(z_idx + 1,	0,	divisions - 1);
		int lower = clamp(z_idx,		0,	divisions - 1);

		int right = clamp(x_idx + 1,	0,	divisions - 1);
		int left = clamp(x_idx,			0,	divisions - 1);

		// If we are in the upper triangle...
		if ( low_x < low_z ) {			// Upper Triangle

			p1 = getAsVec3(data, left,	upper, divisions);		// Upper left
			p2 = getAsVec3(data, right, upper, divisions);		// Upper Right
			p3 = getAsVec3(data, left,	lower, divisions);		// Lower left

			result = getY(p1, p2, p3, x, z);
		} else if (low_x > low_z) {		// Lower Triangle

			p1 = getAsVec3(data, right, upper, divisions);		// Upper right
			p2 = getAsVec3(data, left,  lower, divisions);		// Lower left
			p3 = getAsVec3(data, right, lower, divisions);		// Lower right

			result = getY(p1, p2, p3, x, z);
		} else {						// On the line
			if (x_index == x_idx) {

				p1 = p2 = p3 = getAsVec3(data, left, right, divisions);
				result = p1.getY();
			} else{
				p1 = getAsVec3(data, right, upper, divisions);	// Upper right
				p2 = getAsVec3(data, left, lower, divisions);	// Lower left
				p3 = p2;

				Vec2 u(p1.getX(), p1.getZ());
				Vec2 b(p2.getX(), p2.getZ());

				float len = (u - Vec2(x, z)).getMagnitude();
				float total = (u - b).getMagnitude();
		
				result = interpolate(p1.getY(), p2.getY() , len / total);
			}
		}
	}
	// Not on the ground

	if (enable_flying) {
		return Camera.getY();
	}
	return result + 0.002f;
}

//////////////////////////////////////////////
//					Main					//
//////////////////////////////////////////////
int main(int argc, char** args)
{
	init(); print();
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Load the shader and compile it
	const std::string BaseShaderDir = std::string("../resources/shaders/");
	Shader shader(BaseShaderDir, std::string("shader"));

	if (shader.getError() == SHADER_ERROR::NO_SHADER_ERROR) {
		printf("No error loading shader\n");
	}
	else {
		printf("Error with shader\n");
		std::exit(-1);
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);


	const int divisions = 500;
	//const int divisions = 16;
	const int number_vertices = 3 * divisions * divisions;
	const int number_indicies = 6 * (divisions - 1) * (divisions - 1);
	const float size = 1.0f;
	GLfloat *ground_data = generateGround(-size, size, -size, size, divisions, 100);
	GLuint *indices = generateIndices(divisions);

	RenderObject ground(shader, ground_data, number_vertices, indices, number_indicies);

	std::list<RenderObject> objs;

	Camera = Vec3(Camera.getX(), getHeight(ground), Camera.getZ());

	glfwSetCursorPos(window, width / 2, height / 2);

	update();

	double duration = 0;
	double previous = glfwGetTime();

	// Main Loop.  Do the stuff!
	while (!glfwWindowShouldClose(window)) {
		// Clear everything on the screen
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		ground.render(ProjectionMatrix, TranslateMatrix, Camera,p1,p2,p3);

		// for objects to be rendered
		for (RenderObject obj : objs) {
			obj.render(ProjectionMatrix, TranslateMatrix, Camera,p1,p2,p3);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		// duration = duration + (current - previous)
		double cTime = glfwGetTime();
		duration += (cTime - previous);
		previous = cTime;

		if (duration > 1.0/60.0) {
			update();
			duration = 0.0f;
			Camera = Vec3(Camera.getX(),getHeight(ground), Camera.getZ());
		}

	}

	delete indices;
	delete ground_data;

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
