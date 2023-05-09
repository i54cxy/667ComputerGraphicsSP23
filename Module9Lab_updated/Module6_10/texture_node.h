#pragma once
#include <IL/il.h>

class TextureNode : public SceneNode {
public:
    TextureNode(const char* fname) {
        ILuint id;
        ilGenImages(1, &id);
        ilBindImage(id);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        ilEnable(IL_ORIGIN_SET);
        ilLoadImage(fname);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        int w = ilGetInteger(IL_IMAGE_WIDTH);
        int h = ilGetInteger(IL_IMAGE_HEIGHT);
        unsigned char* data = ilGetData();

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        auto error = ilGetError();

        if (error == IL_NO_ERROR)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    };

    void Draw(SceneState& scene_state) {
        scene_state.useTexture = 1;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Draw children of this node
        SceneNode::Draw(scene_state);
        scene_state.useTexture = 0;
    }

private:
    unsigned int texture;
};