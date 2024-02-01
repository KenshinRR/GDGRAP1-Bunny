#version 330

//gets the data at Attrib Index 0
//Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;

//declare a varialbe to hole the data that we're going to pass
//uniform float x;
//uniform float y;
//uniform float z;

uniform mat4 transform;
uniform mat4 projection;

void main(){
	//create a new vec for new position
	//					add x to aPos.x
	//vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z + z);

	//g_Position is predefined
	//this denotes the final position of the vertext/point
	//gl_Position = vec4(newPos, 1.0); //Turns the vec3 into a vec4

	//multiply the transformation matrix to the vec4 version of aPos to get the final position
	gl_Position = projection * transform * vec4(aPos, 1.0);
}