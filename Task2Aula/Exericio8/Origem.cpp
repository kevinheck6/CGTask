/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 02/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <vector>


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;


bool rotateX=false, rotateY=false, rotateZ=false;

float translateX = 0.0f;
float translateY = 0.0f;
float translateZ = 0.0f;
float scale = 1.0f;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();


	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Pirâmide!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader = Shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();


	glUseProgram(shader.ID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);


	// Loop da aplicação - "game loop"
	// Loop da aplicação - "game loop"
	// Defina as transformações para cada cubo
	std::vector<glm::mat4> cubeTransforms = {
		glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
		glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
	};

	while (!glfwWindowShouldClose(window))
	{
		// Checa se houve eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Chamada de desenho - draw call
		// Polígono preenchido - GL_TRIANGLES
		glBindVertexArray(VAO);

		for (const auto& transform : cubeTransforms)
		{
			glm::mat4 model = glm::mat4(1.0f);

			if (rotateX)
			{
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else if (rotateY)
			{
				model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (rotateZ)
			{
				model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			}

			model = glm::translate(model, glm::vec3(translateX, translateY, translateZ));
			model = glm::scale(model, glm::vec3(scale));
			model = transform * model;

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}




	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		translateY += 0.1f;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		translateY -= 0.1f;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		translateX -= 0.1f;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		translateX += 0.1f;
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		translateZ -= 0.1f;
	}

	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		translateZ += 0.1f;
	}

	if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
	{
		scale -= 0.1f;
		if (scale < 0.1f)
			scale = 0.1f;
	}

	if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
	{
		scale += 0.1f;
	}
}



int setupGeometry()
{

	GLfloat vertices[] = {
		// Face frontal
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0,  // V0
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0,   // V1
		0.5, 0.5, -0.5, 1.0, 0.0, 0.0,    // V2

		0.5, 0.5, -0.5, 1.0, 0.0, 0.0,    // V2
		-0.5, 0.5, -0.5, 1.0, 0.0, 0.0,   // V3
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0,  // V0

		// Face traseira
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0,   // V4
		0.5, -0.5, 0.5, 0.0, 1.0, 0.0,    // V5
		0.5, 0.5, 0.5, 0.0, 1.0, 0.0,     // V6

		0.5, 0.5, 0.5, 0.0, 1.0, 0.0,     // V6
		-0.5, 0.5, 0.5, 0.0, 1.0, 0.0,    // V7
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0,   // V4

		// Face superior
		-0.5, 0.5, -0.5, 0.0, 0.0, 1.0,   // V8
		0.5, 0.5, -0.5, 0.0, 0.0, 1.0,    // V9
		0.5, 0.5, 0.5, 0.0, 0.0, 1.0,     // V10

		0.5, 0.5, 0.5, 0.0, 0.0, 1.0,     // V10
		-0.5, 0.5, 0.5, 0.0, 0.0, 1.0,    // V11
		-0.5, 0.5, -0.5, 0.0, 0.0, 1.0,   // V8

		// Face inferior
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0,  // V12
		0.5, -0.5, -0.5, 1.0, 1.0, 0.0,   // V13
		0.5, -0.5, 0.5, 1.0, 1.0, 0.0,    // V14

		0.5, -0.5, 0.5, 1.0, 1.0, 0.0,    // V14
		-0.5, -0.5, 0.5, 1.0, 1.0, 0.0,   // V15
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0,  // V12

		// Face lateral esquerda
		-0.5, -0.5, -0.5, 1.0, 0.0, 1.0,  // V16
		-0.5, 0.5, -0.5, 1.0, 0.0, 1.0,   // V17
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0,    // V18

		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0,    // V18
		-0.5, -0.5, 0.5, 1.0, 0.0, 1.0,   // V19
		-0.5, -0.5, -0.5, 1.0, 0.0, 1.0,  // V16

		// Face lateral direita
		0.5, -0.5, -0.5, 0.0, 1.0, 1.0,   // V20
		0.5, 0.5, -0.5, 0.0, 1.0, 1.0,    // V21
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0,     // V22

		0.5, 0.5, 0.5, 0.0, 1.0, 1.0,     // V22
		0.5, -0.5, 0.5, 0.0, 1.0, 1.0,    // V23
		0.5, -0.5, -0.5, 0.0, 1.0, 1.0,   // V20
	};

	GLuint VBO, VAO;

	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Envie os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);


	// Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);


	return VAO;
}

