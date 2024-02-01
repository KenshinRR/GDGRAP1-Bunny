#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <string>
#include <iostream>

#define PI 3.14159265

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


//modifiers for the model's position
float x_mod = 0;
float y_mod = 0;
float z_mod = -2.0f;

//scale modifiers
float x_scale = 1;
float y_scale = 1;
float z_scale = 1;

//rotation modifiers
float x_axis = 0;
float y_axis = 1;
float z_axis = 0;

float theta = 90;

//modifiers for color
float r_mod = 0;
float g_mod = 0;
float b_mod = 0;


void Key_Callback(GLFWwindow* window, //pointer to the window
    int key, //keycode of the press
    int scancode, //physical position of the press
    int action, //either press / release
    int mods //which modifier keys is held down
)
{
    
    if (action == GLFW_PRESS) { //remove GLFW_PRESS to not just tapping but holding
        switch (key)
        {
        //movement
        case GLFW_KEY_W:
            z_mod -= 0.5f;
            break;
        case GLFW_KEY_A:
            x_mod -= 0.1f;
            break;
        case GLFW_KEY_S:
            z_mod += 0.5f;
            break;
        case GLFW_KEY_D:
            x_mod += 0.1f;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            y_mod -= 0.1f;
            break;
        case GLFW_KEY_SPACE:
            y_mod += 0.1f;
            break;
        case GLFW_KEY_R:
            x_mod = 0.0f;
            y_mod = 0.0f;
            z_mod = 0.0f;
            break;

        //color
        case GLFW_KEY_1:
            r_mod += 0.1f;
            break;
        case GLFW_KEY_2:
            g_mod += 0.1f;
            break;
        case GLFW_KEY_3:
            b_mod += 0.1f;
            break;
        case GLFW_KEY_4:
            b_mod -= 0.1f;
            break;
        case GLFW_KEY_5:
            b_mod -= 0.1f;
            break;
        case GLFW_KEY_6:
            b_mod -= 0.1f;
            break;
        

        //rotation
        case GLFW_KEY_LEFT:
            theta -= 30;
            break;
        case GLFW_KEY_RIGHT:
            theta += 30;
            break;
        default:
            break;
        }
    }
    

}

void assignGLValueByAddress(GLchar* varName, float varMod, GLuint shaderProg) {
    //get the address of variable z in the shader
    unsigned int varLoc = glGetUniformLocation(shaderProg, varName);
    //assign x by using its address
    glUniform1f(varLoc, varMod);
}

int main(void)
{
    //initialize variables
    GLFWwindow* window;

    double conv = PI / 180;
    double radius = 0.8;

    float x, y, z;
    x = y = 0;
    z = -2.0f;

    float window_width = 600.f, window_height = 600.f;

    //setting the projection
    //orthographic projection
    //glm::mat4 projection = glm::ortho(
    //    -2.0f, //left most point
    //    2.0f, //right most point
    //    -2.0f, //bottom most point
    //    2.0f, //top most point
    //    -1.f, //Z near
    //    1.0f, //Z far
    //    );

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f), //FOV
        //zoom in -FOV
        //zoom out +FOV
        window_height / window_width, // Aspect ratio
        0.1f, //Z near > 0
        100.f //Z far
    );

    /* Initialize the identity matrix */
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Reblando, Kenshin R.", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(
        0, //min X
        0, //min Y
        window_width, //width
        window_height //height
    );

    //set the callback function to the window
    glfwSetKeyCallback(window, Key_Callback);

    //importing the files for shaders (.vert)
    //load the shader file into a string stream
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    //add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    //convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //importing the files for fragments (.frag)
    //load the shader file into a string stream
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    //add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();

    //convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //create a vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);
    //compile the vertex shader
    glCompileShader(vertexShader);

    //create a fragmant shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //assign the source to the fragment shader
    glShaderSource(fragmentShader, 1, &f, NULL);
    //compile the fragment shader
    glCompileShader(fragmentShader);

    //error testing 
    /*
    GLint isCompiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragmentShader); // Don't leak the shader.
        std::cout << errorLog[0];
    }
    */

    //create the shader program
    GLuint shaderProg = glCreateProgram();
    //attach the compild vertex shader
    glAttachShader(shaderProg, vertexShader);
    //attach the compiled fragment shader
    glAttachShader(shaderProg, fragmentShader);

    //Finalie the compilation process
    glLinkProgram(shaderProg);

    //defining the 3D object
    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    //get the EBO indices array
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    //defining the "3D" triangle
    GLfloat vertices[]{
        //x     y   z
        0.0f, 0.5f, 0.0f, //first point
        -0.5f, -0.5f, 0.0f, //second point
        0.5f, -0.5f, 0.0f //third point
    };

    GLuint indices[]{
        0,1,2 //triangle
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    //currently editing VAO = null
    glBindVertexArray(VAO); //after calling
    //currently editing VAO = VAO (or whatever is inside the parameter)

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, //0 position, 1 texture, 2 norms
        3, //means xyz
        GL_FLOAT, //what data type
        GL_FALSE,
        3 * (sizeof(GL_FLOAT)),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW);

    //VAO <- EBO

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //colour
        assignGLValueByAddress((GLchar*)("r"), r_mod, shaderProg);
        assignGLValueByAddress((GLchar*)("g"), g_mod, shaderProg);
        assignGLValueByAddress((GLchar*)("b"), b_mod, shaderProg);
        
        //start with the translation matrix
        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix4,
            glm::vec3(x, y, z_mod)
        );

        //multiply the resultin matrix with the scale
        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(x_scale, y_scale, z_scale)
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(x_axis, y_axis, z_axis))
        );

        //setting the projection
        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(
            projLoc, //address of the transform variable
            1, //how many matrices to assign
            GL_FALSE, //transpose?
            glm::value_ptr(projectionMatrix) //pointer to the matrix
        );

        //setting the transformation
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(
            transformLoc, //address of the transform variable
            1, //how many matrices to assign
            GL_FALSE, //transpose?
            glm::value_ptr(transformation_matrix) //pointer to the matrix
        );

        //tell open GL to use this shader for the VAOs below
        glUseProgram(shaderProg);

        //bind the VAO to prep for drawing
        glBindVertexArray(VAO);

        //drawing
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}