#define SOKOL_IMPL
#define SOKOL_GLCORE33

#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"

#include "shaders/basic.glsl.h"

sg_buffer vbuf;
sg_shader shd;
sg_pipeline pip;
sg_bindings bind;
sg_pass_action pass_action;

void init(void) {
    sg_setup({
        .context = sapp_sgcontext()
    });
    pass_action = sg_pass_action {
        .colors = {
         {.action=SG_ACTION_CLEAR, .value={1.0f, 0.0f, 0.0f, 1.0f}}
        }
    };
    /* a vertex buffer */
    const float vertices[] = {
        // positions            // colors
        0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
    };

    vbuf = sg_make_buffer({
        .data = SG_RANGE(vertices)
    });

    /* a shader */

    shd = sg_make_shader(basic_shd_shader_desc(sg_query_backend()));

    /* a pipeline state object (default render states are fine for triangle) */
    pip = sg_make_pipeline({
        .shader = shd,
        .layout = {
            .attrs = {
                {.format = SG_VERTEXFORMAT_FLOAT3},
                {.format = SG_VERTEXFORMAT_FLOAT4},
            }
        }
    });

    /* resource bindings */
    bind = {
        .vertex_buffers = {vbuf}
    };
}

void frame(void) {
    float g = pass_action.colors[0].value.g + 0.01f;
    pass_action.colors[0].value.g = (g > 1.0f) ? 0.0f : g;
    sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(pip);
    sg_apply_bindings(&bind);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return sapp_desc {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = 400,
        .height = 300,
        .window_title = "Clear Sample",
    };
}