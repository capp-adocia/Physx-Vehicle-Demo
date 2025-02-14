#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 uvw;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	vec4 transformPosition = vec4(aPos, 1.0);
	transformPosition = modelMatrix * transformPosition;
	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	gl_Position = gl_Position.xyww; // 为了让天空盒立方体一直可以被深入写入，需要将z值设为1.0
//	(x,y,z,w)/w = Zndc
//  (Zndc + 1) / 2 = z
	uvw = aPos;
}