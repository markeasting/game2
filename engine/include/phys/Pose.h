#pragma once

#include "common/glm.h"

class Pose {
public:

    vec3 p = vec3(0);
    quat q = quat(1.0f, 0, 0, 0);		

    Pose() = default; 
    Pose(vec3 p, quat q = quat(1.0f, 0, 0, 0)); 

    void copy(Pose pose);

    Pose clone();
    
    void translate(vec3& v) const;
    void invTranslate(vec3& v) const;

    void rotate(vec3& v) const;
    void invRotate(vec3& v) const;

    void transform(vec3& v) const;
    void invTransform(vec3& v) const;

    void transformPose(Pose& pose) const;  

};
