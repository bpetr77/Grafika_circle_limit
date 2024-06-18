//=============================================================================================
// Mintaprogram: Zöld háromszög. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!! 
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak 
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Bakó Péter
// Neptun : OTQVE5
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"


const char* vertexSource = R"(
	#version 330
    precision highp float;

	uniform mat4 MVP;			
	layout(location = 0) in vec2 vp;
	layout(location = 1) in vec2 uv;
	out vec2 texcoord;
	void main() {
		texcoord = uv;							
		gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP; 		
	}
)";


const char* fragmentSource = R"(
	#version 330
    precision highp float;

	uniform sampler2D textureUnit;
	in  vec2 texcoord;			
	out vec4 outColor;	

	void main() {
		outColor = texture(textureUnit, texcoord); 
	}
)";
GPUProgram gpuProgram;


class Camera2D {
	vec2 wCenter; 
	vec2 wSize;   
public:

	Camera2D() : wCenter(20, 30), wSize(150, 150) { }
	mat4 V() { return TranslateMatrix(-wCenter); }
	mat4 P() { return ScaleMatrix(vec2(2 / wSize.x, 2 / wSize.y)); }

	mat4 Vinv() { return TranslateMatrix(wCenter); }
	mat4 Pinv() { return ScaleMatrix(vec2(wSize.x / 2, wSize.y / 2)); }
};

Camera2D camera;       
bool IsLinear = true;
float s = 40.0f;

class Star {
	unsigned int vao = 0, vbo = 0;
	Texture starTexture; 
public:

	vec2 center = vec2(50, 30);
	float width = 80.0f;
	float height = 80.0f;
	std::vector<vec2> vertices;
	float angle = 0.0f;
	long startTime = -1; 
	unsigned int textureID;
	std::vector<vec2> texCoords;
	unsigned int vboTexCoords = 0;

	Star(int width, int height, std::vector<vec4> image) {
		create();


		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, image.data());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	void updateTexture(int width, int height, std::vector<vec4> image) {
		glDeleteTextures(1, &textureID);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, image.data());

		if (IsLinear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void create() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glGenBuffers(1, &vboTexCoords);
		glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);

		glEnableVertexAttribArray(1); 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL); 

		CalculateVertices();
	}

	void UpdateGPU() {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(vec2), texCoords.data(), GL_STATIC_DRAW);
	}
	void startAnimation() {
		startTime = glutGet(GLUT_ELAPSED_TIME);
	}

	void update() {
		if (startTime < 0) return; 

		long time = glutGet(GLUT_ELAPSED_TIME);
		float t = (time - startTime) / 1000.0f; 


		float radius = 30.0f;
		vec2 circleCenter = vec2(20.0f, 30.0f);
		center.x = (float)(circleCenter.x + radius * cosf(2.0f * M_PI * t / 10.0f));
		center.y = (float)(circleCenter.y + radius * sinf(2.0f * M_PI * t / 10.0f));

	
		angle = (float)fmod(2.0f * M_PI * t * 0.2f, 2.0f * M_PI);

		vertices = CalculateVertices();
	}
	

	std::vector<vec2> CalculateVertices() {
		vertices.clear();
		texCoords.clear(); 
		vertices.push_back(center);
		vertices.push_back(rotate(vec2(center.x - s, center.y), angle));
		vertices.push_back(rotate(vec2(center.x - (width / 2), center.y + (height / 2)), angle));
		vertices.push_back(rotate(vec2(center.x, center.y + s), angle));
		vertices.push_back(rotate(vec2(center.x + (width / 2), center.y + (height / 2)), angle));
		vertices.push_back(rotate(vec2(center.x + s, center.y), angle));
		vertices.push_back(rotate(vec2(center.x + (width / 2), center.y - (height / 2)), angle));
		vertices.push_back(rotate(vec2(center.x, center.y - s), angle));
		vertices.push_back(rotate(vec2(center.x - (width / 2), center.y - (height / 2)), angle));
		vertices.push_back(rotate(vec2(center.x - s, center.y), angle));

		texCoords.push_back(vec2(0.5, 0.5));
		texCoords.push_back(vec2(0.0, 0.5));
		texCoords.push_back(vec2(0.0, 1.0));
		texCoords.push_back(vec2(0.5, 1.0));
		texCoords.push_back(vec2(1.0, 1.0));
		texCoords.push_back(vec2(1.0, 0.5));
		texCoords.push_back(vec2(1.0, 0.0));
		texCoords.push_back(vec2(0.5, 0.0));
		texCoords.push_back(vec2(0.0, 0.0));
		texCoords.push_back(vec2(0.0, 0.5));
		return vertices;
	}

	vec2 rotate(vec2 v, float angle) {
		vec2 point = v - center; 
		vec2 rotated = vec2(point.x * cosf(angle) - point.y * sinf(angle), point.x * sinf(angle) + point.y * cosf(angle));
		return rotated + center; 
	}

	void Draw() {
		UpdateGPU();
		if (vertices.size() == 0) vertices = CalculateVertices();

		int sampler = 0;
		int location = glGetUniformLocation(gpuProgram.getId(),
			"textureUnit");
		glUniform1i(location, sampler);
		glActiveTexture(GL_TEXTURE0 + sampler); 
		glBindTexture(GL_TEXTURE_2D, textureID);

		mat4 MVPtransf = camera.V() * camera.P();
		gpuProgram.setUniform(MVPtransf, "MVP");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		gpuProgram.setUniform(0, "textureUnit");


		glBindVertexArray(vao); glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

	}

};

class Circle {
public:
	vec2 center; 
	float radius; 
	bool In(vec2 r) { return (dot(r - center, r - center) - radius * radius < 0); }
	Circle(vec2 _center, float _R) { center = _center; radius = _R;}
};
class PoincareDisk {
public:
	size_t pix_x = 300, pix_y = 300;
	std::vector<vec4> image;
	Circle poincareDisk = Circle(vec2(0, 0), 1.0f);

	std::vector<Circle> circels;


	void generatechessBoard() {
		circels.clear();
		for (float angle = 0.0f; angle < 360.0f; angle += 40.0f) {
			for (float i = 0.5f; i < 6.0f; i++) {
				float angle_radian = angle / 180.0f * M_PI;	
				vec3 v0(cosf(angle_radian), sinf(angle_radian), 0);
				vec3 HyperbolicPoint = (vec3(0,0,1) * coshf(i) + v0 * sinhf(i));
				vec3 EukleidesPoint(HyperbolicPoint.x / (HyperbolicPoint.z + 1), HyperbolicPoint.y / (HyperbolicPoint.z + 1), 0);

				float euklidesz_length = length(EukleidesPoint);
				float inverz_euklidesz_length = 1 / euklidesz_length;

				vec3 inversedEukleidesPoint = vec3(cosf(angle_radian) * inverz_euklidesz_length, sinf(angle_radian) * inverz_euklidesz_length, 0);
				float radius = length(EukleidesPoint - inversedEukleidesPoint) / 2.0f;
				Circle c = Circle(vec2((EukleidesPoint.x + inversedEukleidesPoint.x) / 2.0f, (EukleidesPoint.y + inversedEukleidesPoint.y) / 2.0f), radius);
				circels.push_back(c);
			}
		}
	}

	std::vector<vec4> RenderToTexture() {
		generatechessBoard();
		image.resize(pix_x * pix_y);
		for (size_t y = 0; y < pix_y; y++) {
			for (size_t x = 0; x < pix_x; x++) {
				vec2 texcoord = vec2(2.0f * (float)x / pix_x - 1.0f, 2.0f * (float)y / pix_y - 1.0f);
				float distanceFromCenter = length(texcoord - poincareDisk.center);
				int InCircle = 0;
				for (size_t i = 0; i < circels.size(); i++) {
					if (circels[i].In(texcoord)) {
						InCircle++;
					}
				}
				if (distanceFromCenter < poincareDisk.radius) {
					if (InCircle % 2 == 0) {
						image[y * pix_x + x] = vec4(1, 1, 0, 1); 
					}
					else {
						image[y * pix_x + x] = vec4(0, 0, 1, 1); 
					}
				}
				else{
					image[y * pix_x + x] = vec4(0, 0, 0, 1);
				}
			}
		}
		return image;
	}


};

Star* star;
PoincareDisk poincareDisk;

void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);

	star = new Star(poincareDisk.pix_x, poincareDisk.pix_y, poincareDisk.RenderToTexture());

	gpuProgram.create(vertexSource, fragmentSource, "outColor");
}


void onDisplay() {
	glClearColor(0, 0, 0, 0);    
	glClear(GL_COLOR_BUFFER_BIT); 

	
	star->Draw(); 
	glutSwapBuffers(); 
}

void onKeyboard(unsigned char key, int pX, int pY) {
	if (key == 'h') {
		s -= 10.0f;
		star->CalculateVertices();
		glutPostRedisplay(); 
	}else if(key == 'a') {
		star->startAnimation();
	}
	else if (key == 'r') {
		poincareDisk.pix_x -= 100;
		poincareDisk.pix_y -= 100;
		star->updateTexture(poincareDisk.pix_x, poincareDisk.pix_y, poincareDisk.RenderToTexture());
	}
	else if (key == 'R') {
		poincareDisk.pix_x += 100;
		poincareDisk.pix_y += 100;
		star->updateTexture(poincareDisk.pix_x, poincareDisk.pix_y, poincareDisk.RenderToTexture());
	}
	else if (key == 't') {
		IsLinear = false;
		star->updateTexture(poincareDisk.pix_x, poincareDisk.pix_y, poincareDisk.RenderToTexture());
	}
	else if (key == 'T') {
		IsLinear = true;
		star->updateTexture(poincareDisk.pix_x, poincareDisk.pix_y, poincareDisk.RenderToTexture());
	}else if(key == 'H'){
		s += 10.0f;
		star->CalculateVertices();
		glutPostRedisplay();
	}

}

void onKeyboardUp(unsigned char key, int pX, int pY) {
}

void onMouseMotion(int pX, int pY) {	
}

void onMouse(int button, int state, int pX, int pY) { 
}

void onIdle() {
	star->update();
	glutPostRedisplay();
}