var vec = new Vec3f(1, 1, 1);
var vec2 = new Vec3f(1, 1, 1);
vec.addEq(vec2);
var vec3 = vec.add(vec2);
log(vec3.x, vec3.y, vec3.z);
log(typeof vec3);