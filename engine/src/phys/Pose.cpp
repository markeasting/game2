#include "phys/Pose.h"

Pose::Pose(vec3 p, quat q)
    :p(p), q(q) {}

void Pose::copy(Pose pose) {
    this->p = pose.p;
    this->q = pose.q;
}

Pose Pose::clone() {
    Pose newPose = Pose(this->p, this->q);
    return newPose;
}

void Pose::translate(vec3& v) const {
    v += this->p;
}

void Pose::invTranslate(vec3& v) const {
    v -= this->p;
}

void Pose::rotate(vec3& v) const {
    // v.applyQuaternion(this.q);
    v = this->q * v;
}

void Pose::invRotate(vec3& v) const {
    // let inv = this.q.clone();
    // inv.conjugate();
    // v.applyQuaternion(inv);
    
    // v = v * conjugate(this->q);
    v = conjugate(this->q) * v;
}

void Pose::transform(vec3& v) const {
    // v.applyQuaternion(this.q);
    // v.add(this.p);
    this->rotate(v);
    this->translate(v);
}

void Pose::invTransform(vec3& v) const {
    // v.sub(this.p);
    // this.invRotate(v);
    this->invTranslate(v);
    this->invRotate(v);
}

void Pose::transformPose(Pose& pose) const {
    // pose.q.multiplyQuaternions(this.q, pose.q);
    // this.rotate(pose.p);
    // pose.p.add(this.p);

    pose.q = this->q * pose.q;
    pose.p = this->q * pose.p;
    pose.p = pose.p + this->p;
}
