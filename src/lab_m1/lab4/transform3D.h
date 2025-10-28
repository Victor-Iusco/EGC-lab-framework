#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translate matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {

        glm::mat4 m(1.0f); // Matricea identitate
        m[3][0] = translateX;
        m[3][1] = translateY;
        m[3][2] = translateZ;
        return m;

    }

    // Scale matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {

        glm::mat4 m(1.0f);
        m[0][0] = scaleX;
        m[1][1] = scaleY;
        m[2][2] = scaleZ;
        return m;

    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {

        float c = cos(radians);
        float s = sin(radians);

        glm::mat4 m(1.0f);
        m[0][0] = c;
        m[0][1] = s;
        m[1][0] = -s;
        m[1][1] = c;
        return m;

    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {

        float c = cos(radians);
        float s = sin(radians);

        glm::mat4 m(1.0f);
        m[0][0] = c;
        m[0][2] = -s; // Atenție la semne și poziții!
        m[2][0] = s;
        m[2][2] = c;
        return m;

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {

        float c = cos(radians);
        float s = sin(radians);

        glm::mat4 m(1.0f);
        m[1][1] = c;
        m[1][2] = s;
        m[2][1] = -s;
        m[2][2] = c;
        return m;

    }
}   // namespace transform3D