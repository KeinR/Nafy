#include "Text.h"

#include <iostream>

#include "glfw.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image_write.h>

#include "env.h"
#include "defs.h"

nafy::Text::Text(Font *font, const uint32_t shader):
    uniformMatLoc(glGetUniformLocation(shader, SHADER_MODEL_NAME)),
    font(font),
    wrapWidth(0),
    xPos(0), yPos(0),
    fontSize(10),
    red(1), green(1), blue(1), alpha(1)
{

    float vertices[] = {
        // positions   // texture coords
        1.0,   1.0,  1.0f, 1.0f, // top right
        1.0,  -1.0,  1.0f, 0.0f, // bottom right
        -1.0, -1.0,  0.0f, 0.0f, // bottom left
        -1.0,  1.0,  0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VA);
    glGenBuffers(1, &VB);
    glGenBuffers(1, &EB);
    glGenTextures(1, &TX);

    glBindTexture(GL_TEXTURE_2D, TX);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(VA);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    constexpr int stride = 4 * sizeof(float);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, SHADER_SPRITE_SAMPLER_NAME), 0);
}

nafy::Text::~Text() {
    glDeleteVertexArrays(1, &VA);
    glDeleteBuffers(1, &VB);
    glDeleteBuffers(1, &EB);
    glDeleteTextures(1, &TX);
}

// stbtt_GetGlyphHMetrics(info, index, aw, lsb);
void nafy::Text::generate() {
    if (!text.length()) {
        return;
    }
    std::vector<int> index = font->indexString(text);
    fontMetrics metrics = font->getMetrics(fontSize);
    int pixelsCount;
    unsigned char *tmpBmp;
    if (wrapWidth) {
        std::vector<wrapLine> lines = font->wrapString(&metrics, index, wrapWidth);

        displayWidth = font->glyphStrPixelWidth(&metrics, index);
        displayHeight = fontSize * lines.size();
        pixelsCount = displayWidth * displayHeight;
        tmpBmp = new unsigned char[pixelsCount]{0};

        int yBmpOffset = 0;
        const int descent = fontSize * displayWidth;
        std::vector<int>::const_iterator git = index.cbegin();
        for (std::vector<wrapLine>::const_iterator it = lines.cbegin(), end = lines.cend(); it != end; ++it) {
            font->render(&metrics, git, index.cbegin()+it->end, tmpBmp + yBmpOffset, displayWidth);
            git = index.cbegin()+it->nextResume;
            yBmpOffset += descent;
        }
    } else {
        displayWidth = font->glyphStrPixelWidth(&metrics, index);
        displayHeight = fontSize;
        pixelsCount = displayWidth * displayHeight;
        tmpBmp = new unsigned char[pixelsCount]{0};
        font->render(&metrics, index.cbegin(), index.cend(), tmpBmp, displayWidth);
    }
    // Convert from grayscale to RGBA

    unsigned char *bmp = new unsigned char[pixelsCount * 4];
    int pos = 0;// @TEMP
    int lastY = -1;// @TEMP
    for (int i = 0; i < pixelsCount; i++) {
        // Need to flip because OpenGL is fucky that way
        const int y = displayHeight - 1 - i / displayWidth;
        const int ci = (i % displayWidth + y * displayWidth) * 4;
        if (ci >= pixelsCount * 4) { // @TEMP
            std::cout << "OH GOD OH FRICK " << y << std::endl;
        }
        if (lastY != y) {
            pos = 0;
        } else {
            pos++;
        }
        lastY = y;
        if (tmpBmp[i]) {
            bmp[ci] = tmpBmp[i] * red;
            bmp[ci+1] = tmpBmp[i] * green;
            bmp[ci+2] = tmpBmp[i] * blue;
            bmp[ci+3] = tmpBmp[i] * alpha;
        } else {
            bmp[ci] = 0;
            bmp[ci+1] = 0;
            bmp[ci+2] = 0;
            bmp[ci+3] = 0;
        }
    }

    // @TEMP
    stbi_write_bmp("test.bmp", displayWidth, displayHeight, 1, tmpBmp);
    // @TEMP
    stbi_write_png("finale.png", displayWidth, displayHeight, 4, bmp, displayWidth * 4);

    delete[] tmpBmp;

    glBindTexture(GL_TEXTURE_2D, TX);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, displayWidth, displayHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

    delete[] bmp;
}

void nafy::Text::setFont(Font *fnt) {
    font = fnt;
    imageValid = false;
}

void nafy::Text::setText(const std::string &str) {
    text = str;
    imageValid = false;
}

void nafy::Text::setFontSize(float size) {
    fontSize = size;
    imageValid = false;
}

void nafy::Text::setX(int x) {
    xPos = x;
}

void nafy::Text::setY(int y) {
    yPos = y;
}

void nafy::Text::setWrapWidth(int w) {
    wrapWidth = w;
    imageValid = false;
}

void nafy::Text::setHexColor(unsigned int hex) {
    setColorVal((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
}

void nafy::Text::setHexColorRGBA(unsigned int hex) {
    setColorVal((hex >> 24) & 0xFF, (hex >> 16) & 0xFF, (hex >> 8) & 0xFF);
    setAlpha(hex & 0xFF);
}

void nafy::Text::setColorVal(unsigned char r, unsigned char g, unsigned char b) {
    red = r / 255.0f;
    green = g / 255.0f;
    blue = b / 255.0f;
}

void nafy::Text::setColor(float r, float g, float b) {
    red = r;
    green = g;
    blue = b;
}

void nafy::Text::setAlphaVal(unsigned char a) {
    alpha = a / 255.0f;
}

void nafy::Text::setAlpha(float a) {
    alpha = a;
}

void nafy::Text::render() {
    if (!imageValid) {
        imageValid = true;
        std::cout << "Try" << std::endl;
        generate();
        std::cout << "ues" << std::endl;
    }

    // displayWidth *= 0.2;
    // displayHeight *= 0.2;

    glm::mat4 model(1.0f);
    const float x = normX(xPos + displayWidth / 2);
    const float y = normY(yPos + displayHeight / 2);
    // std::cout << "ix=" << x << ", iy=" << y << std::endl;
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3((float)displayWidth / getWinWidth(), (float)displayHeight / getWinHeight(), 0.0f));
    glUniformMatrix4fv(uniformMatLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VA);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
    glBindTexture(GL_TEXTURE_2D, TX);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
