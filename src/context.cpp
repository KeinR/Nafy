#include "context.h"

#include <iostream>

#include "error.h"
#include "env.h"

#include "shaders.h"
#include "Font.h"
#include "Text.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stb/stb_image_write.h>

nafy::context::context(scene *root): root(root), current(root), run(false) {
}

void nafy::context::setRoot(scene *root) {
    this->root = root;
}

void nafy::context::setCurrent(scene *current) {
    this->current = current;
    this->current->reset();
}

void nafy::context::revert() {
    setCurrent(root);
}

void nafy::context::start() {
    revert();
    resume();
}

void nafy::context::resume() {
    if (current == nullptr) {
        throw error("`current` must NOT be null");
    }
    run = true;
    std::cout << "Enter?" << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Font font("Arial.ttf");
    Text text(&font, shaders::sprite);
    text.setText("Hello gamers");
    text.setFontSize(15);
    text.setHexColor(0);
    text.setX(0);
    text.setY(0);
    text.render();

    int error;
    FT_Library library;

    error = FT_Init_FreeType(&library);
    if (error) {
        std::cout << "FUCK-1" << std::endl;
    }

    FT_Face face;

    error = FT_New_Face(
        library,
        "resources/fonts/Arial.ttf",
        0,
        &face
    );
    if (error) {
        std::cout << "FUCK-2" << std::endl;
    }

    // error = FT_Set_Char_Size(
    //       face,    /* handle to face object           */
    //       0,       /* char_width in 1/64th of points  */
    //       16*64,   /* char_height in 1/64th of points */
    //       300,     /* horizontal device resolution    */
    //       300 );   /* vertical device resolution      */
    // if (error) {
    //     std::cout << "FUCK-3" << std::endl;
    // }

    error = FT_Set_Pixel_Sizes(
        face,   /* handle to face object */
        0,      /* pixel_width           */
        10 );   /* pixel_height          */
    if (error) {
        std::cout << "FUCK-3" << std::endl;
    }

    // int glyph_index = FT_Get_Char_Index(face, 'c');

    // error = FT_Load_Glyph(
    //     face,          /* handle to face object */
    //     glyph_index,   /* glyph index           */
    //     FT_LOAD_DEFAULT );  /* load flags, see below */
    // if (error) {
    //     std::cout << "FUCK-4" << std::endl;
    // }


    FT_GlyphSlot slot = face->glyph;  /* a small shortcut */
    int pen_x, pen_y, n;


    // ... initialize library ...
    // ... create face object ...
    // ... set character size ...

    pen_x = 300;
    pen_y = 200;

    std::string str("HEllo");

    for (n = 0; n < str.size(); n++) {
        FT_UInt  glyph_index;


        /* retrieve glyph index from character code */
        glyph_index = FT_Get_Char_Index( face, str[n] );

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        if ( error )
            continue;  /* ignore errors */

        /* convert to an anti-aliased bitmap */
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        if ( error )
            continue;

        /* now, draw to our target surface */
        // my_draw_bitmap( &slot->bitmap,
        //                 pen_x + slot->bitmap_left,
        //                 pen_y - slot->bitmap_top );
        if (str[n] == 'o') {
            std::cout << std::endl << (pen_x + slot->bitmap_left) << ", " << (pen_y - slot->bitmap_top) << std::endl;
        }

        /* increment pen position */
        pen_x += slot->advance.x >> 6;
        pen_y += slot->advance.y >> 6; /* not useful for now */
    }

    stbi_write_bmp("test22222.bmp", slot->bitmap.width, slot->bitmap.rows, 1, slot->bitmap.buffer);

    

    while (run && engineUp()) {
        std::cout << "_";
        current->run(this);

        // glClearColor(1, 1, 1, 1.0f);
        // glClearColor(0, 0, 0, 1.0f);
        glClearColor(0.69, 0.52, 0.23, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        text.render();

        glfwSwapBuffers(getWindow());

        glfwPollEvents();
        

    }
}

void nafy::context::stop() {
    run = false;
}

void nafy::context::stopIfCurrent(scene *obj) {
    if (obj == current) {
        stop();
    }
}
