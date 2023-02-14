//
//  main.c
//  2D Lights - sandbox
//
//  Created by David Kviloria on 10/11/20.
//

#include <stdio.h>
#include <math.h> // cos, sin
#include "GLFW/glfw3.h"
#include "linmath/linmath.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define LIBBL_USE_APPLE_OPENGL
#define LIBBL_IMPLEMENTATION
#include "libbl.h"

f32_t global_vertices[32] = 
{
   // @Vertex Position      // @Color (RGB)       // @UV Coord
   0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 0.0f,      0.0f, 0.0f,
   0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,      0.0f, 1.0f,
  -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 1.0f,      1.0f, 1.0f,
  -0.5f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
};

u32_t global_indices[6] = 
{
  0, 1, 3, // @first triangle
  1, 3, 2, // @second triangle
};

void key_callback(GLFWwindow* window, u32_t key, u32_t scancode, u32_t action, u32_t mods);

int main(int argc, const char* argv[]) {

    GLFWwindow* window;
    int mvp_location, uTime_location, uCircle_location;

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1080, 720, "2D Lights Example with SD Shapes", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    bl_ent_t player = { .name = "Player",
                        .index = 0,
                        .position = { 1.2f, 1.2f } };

    bl_register_entity(&player, global_manager);
    
    vertex_buffer_t vbo;
    gen_vertex_buffer(&vbo);
    bind_vertex_buffer(&vbo);
    vertex_buffer_data(sizeof(global_vertices), global_vertices);
    
    indice_buffer_t ebo;
    gen_indice_buffer(&ebo);
    bind_indice_buffer(&ebo);

    vertex_array_t vao;
    gen_and_bind_vertex_array(&vao);
        
    bind_vertex_buffer(&vbo);
    vertex_buffer_data(sizeof(global_vertices), global_vertices);

    bind_indice_buffer(&ebo);
    indice_buffer_data(sizeof(global_indices), global_indices);
    
    shader_program_t program;
    create_program(
       &program,
       compile_shader(
        read_file_content("shaders/base_vertex_shader.vert"),
        GL_VERTEX_SHADER
       ),
       compile_shader(
        read_file_content("shaders/light_fragment_shader.frag"),
        GL_FRAGMENT_SHADER
       )
    );

    u32_t texture_id;
    load_and_create_texture("./assets/base.png", &texture_id, GL_TEXTURE0, GL_TEXTURE_2D);

    mvp_location = glGetUniformLocation(program.m_program_id, "MVP");
    uTime_location = glGetUniformLocation(program.m_program_id, "uTime");
    uCircle_location = glGetUniformLocation(program.m_program_id, "uCircle");

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(f32_t), (void*) 0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(f32_t), (void*)(3 * sizeof(f32_t)));
    glEnableVertexAttribArray(1);
    
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(f32_t), (void*)(6 * sizeof(f32_t)));
    glEnableVertexAttribArray(2);   

    f32_t ratio;
    u32_t width, height;
    
    mat4x4 model, projection, mvp;
    f64_t deltaTime, previousTime, elapsedTime;
    
    f32_t p_speed = 0.01f;
    bl_ent_t *ent_player = bl_get_entity("Player", global_manager);
    if (ent_player == NULL) {
        printf("Entity: Player not found");
            glfwTerminate();exit(EXIT_FAILURE);
    }

    u32_t tex0Uni = glGetUniformLocation(program.m_program_id, "tex0");
    attach_program(&program);
    
    while (!glfwWindowShouldClose(window)) {
    
        deltaTime = glfwGetTime();
        previousTime = deltaTime;
        elapsedTime = deltaTime - previousTime;
        
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (f32_t) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);
        
        mat4x4_identity(model);
        mat4x4_ortho(projection, -ratio, ratio, 1.f, -1.f, 1.f, -1.f);

        for (int x = 0; x < 50; x++) {
            for (int y = 0; y < 50; y++) {

                mat4x4_translate(model, 
                    ent_player->position[0] - x, 
                    ent_player->position[1] - y, 0.f); 

                if (glfwGetKey(window, GLFW_KEY_H) != GLFW_PRESS) {
                    mat4x4_scale_aniso(model, model, 0.98, 0.98, 0);
                }
                
                mat4x4_mul(mvp, projection, model);
                glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
                glUniform1f(uTime_location, deltaTime);
                
                if ((x + y) % 2 == 0) {
                    glUniform1f(uCircle_location, 1.0f);
                } else {
                    glUniform1f(uCircle_location, 2.0f);
                }
                
                attach_program(&program);
                glUniform1i(tex0Uni, 0);

                draw_triangles(sizeof(global_vertices) / sizeof(global_vertices[0]));
                glDeleteTextures(1, &texture_id);
                detach_program(&program);
            }
        }

        // Controlls
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            ent_player->position[0] = 
                ent_player->position[0] -= p_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            ent_player->position[0] =
                ent_player->position[0] += p_speed;
        }
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            ent_player->position[1] =
                 ent_player->position[1] += p_speed;
        }
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            ent_player->position[1] = 
                ent_player->position[1] -= p_speed;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);

    glfwTerminate();
    return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* window, u32_t key, u32_t scancode, u32_t action, u32_t mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
