var vec = new Vec3f(1, 1, 1);
var vec2 = new Vec3f(1, 1, 1);
vec.addEq(vec2);
var vec3 = vec.add(vec2);
log(vec3.x, vec3.y, vec3.z);
var number = vec.x + vec2.x;
log(number);