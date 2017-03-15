
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#if defined(__APPLE__)
#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL\glew.h>		// must be downloaded
#include <GL\freeglut.h>	// must be downloaded unless you have an Apple
#endif

using namespace std;

int windowWidth = 512, windowHeight = 512;
bool moveKeys[] = { false, false, false, false };

// OpenGL major and minor versions
int majorVersion = 3, minorVersion = 0;

void getErrorInfo(unsigned int handle)
{
	int logLen;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 0)
	{
		char * log = new char[logLen];
		int written;
		glGetShaderInfoLog(handle, logLen, &written, log);
		printf("Shader log:\n%s", log);
		delete log;
	}
}

// check if shader could be compiled
void checkShader(unsigned int shader, char * message)
{
	int OK;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &OK);
	if (!OK)
	{
		printf("%s!\n", message);
		getErrorInfo(shader);
	}
}

// check if shader could be linked
void checkLinking(unsigned int program)
{
	int OK;
	glGetProgramiv(program, GL_LINK_STATUS, &OK);
	if (!OK)
	{
		printf("Failed to link shader program!\n");
		getErrorInfo(program);
	}
}

// vertex shader in GLSL
const char *vertexSource = R"(
	#version 130
	precision highp float;

	in vec2 vertexPosition;		// variable input from Attrib Array selected by glBindAttribLocation
	in vec3 vertexColor;	    // output attribute
	uniform mat4 MVP;
	out vec3 color;

	void main()
	{
		color = vertexColor;				 		// set vertex color
		gl_Position = vec4(vertexPosition.x, vertexPosition.y, 0, 1)*MVP;  	// copy position from input to output
	}
)";

// fragment shader in GLSL
const char *fragmentSource = R"(
	#version 130
	precision highp float;

	in vec3 color;			// variable input: interpolated from the vertex colors
	out vec4 fragmentColor;		// output that goes to the raster memory as told by glBindFragDataLocation

	void main()
	{
		fragmentColor = vec4(color, 1); // extend RGB to RGBA
	}
)";

// row-major matrix 4x4
struct mat4
{
	float m[4][4];
public:
	mat4() {}
	mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	mat4 operator*(const mat4& right)
	{
		mat4 result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.m[i][j] = 0;
				for (int k = 0; k < 4; k++) result.m[i][j] += m[i][k] * right.m[k][j];
			}
		}
		return result;
	}
	operator float*() { return &m[0][0]; }
};


// 3D point in homogeneous coordinates
struct vec4
{
	float v[4];

	vec4(float x = 0, float y = 0, float z = 0, float w = 1)
	{
		v[0] = x; v[1] = y; v[2] = z; v[3] = w;
	}

	vec4 operator*(const mat4& mat)
	{
		vec4 result;
		for (int j = 0; j < 4; j++)
		{
			result.v[j] = 0;
			for (int i = 0; i < 4; i++) result.v[j] += v[i] * mat.m[i][j];
		}
		return result;
	}

	vec4 operator+(const vec4& vec)
	{
		vec4 result(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2], v[3] + vec.v[3]);
		return result;
	}
};

// 2D point in Cartesian coordinates
struct vec2
{
	float x, y;

	vec2() {
		srand(time(NULL));
		double px = ((double)rand() / (RAND_MAX)) * 2 - 1;
		double py = ((double)rand() / (RAND_MAX)) * 2 - 1;
		x = px;
		y = py;
	}

	vec2(float x, float y) : x(x), y(y) {}

	vec2 operator+(const vec2& v)
	{
		return vec2(x + v.x, y + v.y);
	}

	vec2 operator+= (const vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2 operator*(const vec2& v) {
		return vec2(x*v.x, y*v.y);
	}

	vec2 operator*=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
};


// handle of the shader program
unsigned int shaderProgram;

class Object {
	protected:
		vec2 scaling; float orientation; vec2 position;
	public:
		Object() : scaling(1.0, 1.0), orientation(0.0), position() {}
		void Draw() {
			// define the MVPTransform here as mat4 according to scaling, orientation, and position
			mat4 MVPTransform(1,0,0,0,
							  0,1,0,0,
				              0,0,1,0,
				              0,0,0,1);

			// define scaling, rotation, and translation matrices separately and multiply them
			mat4 scale(scaling.x, 0, 0, 0,
					   0, scaling.y, 0, 0,
					   0, 0, 1, 0,
				       0, 0, 0, 1);

			mat4 translate(1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   position.x, position.y, 0, 1);

			float alpha = cos(orientation);
			float beta = sin(orientation);
			mat4 rotate(alpha,beta,0,0,
						-beta,alpha,0,0,
						0,0,1,0,
						0,0,0,1);

			mat4 fit (1, 0, 0, 0,
					  0, float(windowWidth) / windowHeight, 0, 0,
					  0, 0, 1, 0,
					  0, 0, 0, 1);

			MVPTransform = scale*rotate*translate*fit;

			// set GPU uniform matrix variable MVP with the content of CPU variable MVPTransform
			int location = glGetUniformLocation(shaderProgram, "MVP");
			// set uniform variable MVP to the MVPTransform
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, MVPTransform);
			else printf("uniform MVPTransform cannot be set\n");
			DrawModel();
		}

		Object* Scale(const vec2& s) {
			scaling = scaling*s;
			return this;
		}
		Object* Rotate(float angle) {
			orientation += angle;
			return this;
		}
		Object* Translate(const vec2& t) {
			position += t;
			return this;
		}

		virtual void DrawModel() = 0;

};

class Triangle:public Object
{
	unsigned int vao;	// vertex array object id

public:
	void Create()
	{
		glGenVertexArrays(1, &vao);	// create a vertex array object
		glBindVertexArray(vao);		// make it active

		unsigned int vbo[2];		// vertex buffer object
		glGenBuffers(2, &vbo[0]);		// generate a vertex buffer object

		// vertex coordinates: vbo -> Attrib Array 0 -> vertexPosition of the vertex shader
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // make it active, it is an array
		static float vertexCoords[] = { 0, 0, 1, 0, 0, 1 };	// vertex data on the CPU

		glBufferData(GL_ARRAY_BUFFER,	// copy to the GPU
			sizeof(vertexCoords),	// size of the vbo in bytes
			vertexCoords,		// address of the data array on the CPU
			GL_STATIC_DRAW);	// copy to that part of the memory which is not modified

		// map Attribute Array 0 to the currently bound vertex buffer (vbo)
		glEnableVertexAttribArray(0);

		// data organization of Attribute Array 0
		glVertexAttribPointer(0,	// Attribute Array 0
			2, GL_FLOAT,		// components/attribute, component type
			GL_FALSE,		// not in fixed point format, do not normalized
			0, NULL);		// stride and offset: it is tightly packed

		//  same thing for color
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		static float vertexColors[] = { 1,0,0, 0,1,0, 0,0,1 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	void DrawModel()
	{
		glBindVertexArray(vao);	// make the vao and its vbos active playing the role of the data source
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw a single triangle with vertices defined in vao
	}
};


class Quad :public Object
{
	unsigned int vao;	// vertex array object id

public:
	void Create()
	{
		glGenVertexArrays(1, &vao);	// create a vertex array object
		glBindVertexArray(vao);		// make it active

		unsigned int vbo[2];		
		glGenBuffers(1, &vbo[0]);		
		
		//  vertex cord setup
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		static float vertexCoords[] = { -0.25,0.25, 0.25,0.25, 0.25,-0.25, -0.25,-0.25 };
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,2, GL_FLOAT,GL_FALSE,0, NULL);	

		// color of vertex setup
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		static float vertexColors[] = {0,0,0, 1,0,0, 0,1,0, 0,0,1};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	void DrawModel()
	{
		glBindVertexArray(vao);	// make the vao and its vbos active playing the role of the data source
		glDrawArrays(GL_QUADS, 0, 4); // draw a single rectangle with vertices defined in vao
	}
};


// the virtual world: a single triangle
Triangle triangle;
Quad rectangle;

// initialization, create an OpenGL context
void onInitialization()
{
	glViewport(0, 0, windowWidth, windowHeight);

	// create objects by setting up their vertex data on the GPU
	rectangle.Create();
	rectangle.Translate(vec2(-0.25, 0.25));

	// create vertex shader from string
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (!vertexShader) { printf("Error in vertex shader creation\n"); exit(1); }

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	checkShader(vertexShader, "Vertex shader error");

	// create fragment shader from string
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragmentShader) { printf("Error in fragment shader creation\n"); exit(1); }

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	checkShader(fragmentShader, "Fragment shader error");

	// attach shaders to a single program
	shaderProgram = glCreateProgram();
	if (!shaderProgram) { printf("Error in shader program creation\n"); exit(1); }

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// connect Attrib Array to input variables of the vertex shader
	glBindAttribLocation(shaderProgram, 0, "vertexPosition"); // vertexPosition gets values from Attrib Array 0

	// connect the fragmentColor to the frame buffer memory
	glBindFragDataLocation(shaderProgram, 0, "fragmentColor"); // fragmentColor goes to the frame buffer memory

	// program packaging
	glLinkProgram(shaderProgram);
	checkLinking(shaderProgram);
	// make this program run
	glUseProgram(shaderProgram);
}

void onExit()
{
	glDeleteProgram(shaderProgram);
	printf("exit");
}

// window has become invalid: redraw
void onDisplay()
{
	glClearColor(0, 0, 0, 0); // background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

	rectangle.Draw();

	glutSwapBuffers(); // exchange the two buffers
}

void onResize(int w, int h) {
	windowHeight = h;
	windowWidth = w;
	glViewport(0, 0, w, h);
}

void onKeyboard(unsigned char c, int x, int y) {

	if (c == 'w'  || c == 'W') {
		moveKeys[0] = true;
	}
	else if (c == 's' || c == 'S') {
		moveKeys[1] = true;
	}
	else if (c == 'a' ||  c == 'A') {
		moveKeys[2] = true;
	}
	else if (c == 'd' || c == 'D') {
		moveKeys[3] = true;
	}
	glutPostRedisplay();
}

void onIdle() {

	// variable to remember last time idle was called
	static double lastTime = 0.0;
	// time elapsed since program started, in seconds
	double t = glutGet(GLUT_ELAPSED_TIME) * 0.001;
	double dt = t - lastTime;  // change in time
	/*/
	if (moveKeys[0]) {
		offSet[1] += 0.01;
		moveKeys[0] = false;
		}
	if (moveKeys[1]) {
		offSet[1] -= 0.01;
		moveKeys[1] = false;
	}
	if (moveKeys[2]) {
		offSet[0] -= 0.01;
		moveKeys[2] = false;
	}
	if (moveKeys[3]) {
		offSet[0] += 0.01;
		moveKeys[3] = false;
	}*/

	//make it go in a circle
	//offSet[0] = sin(t);
	//offSet[1] = cos(t);
	rectangle.Rotate(0.0001);
	rectangle.Translate(vec2(sin(dt)*0.0001, cos(dt)*0.0001));
	//show result
	glutPostRedisplay();

}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
#if !defined(__APPLE__)
	glutInitContextVersion(majorVersion, minorVersion);
#endif
	glutInitWindowSize(windowWidth, windowHeight); 	// application window is initially of resolution 512x512
	glutInitWindowPosition(50, 50);			// relative location of the application window
#if defined(__APPLE__)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);  // 8 bit R,G,B,A + double buffer + depth buffer
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutCreateWindow("Triangle Rendering");

#if !defined(__APPLE__)
	glewExperimental = true;
	glewInit();
#endif
	printf("GL Vendor    : %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer  : %s\n", glGetString(GL_RENDERER));
	printf("GL Version (string)  : %s\n", glGetString(GL_VERSION));
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	printf("GL Version (integer) : %d.%d\n", majorVersion, minorVersion);
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	onInitialization();

	// register event handlers
	glutDisplayFunc(onDisplay); 
	glutReshapeFunc(onResize);
	glutIdleFunc(onIdle);
	//glutKeyboardFunc(onKeyboard);

	glutMainLoop();
	onExit();
	return 1;
}
