#version 330 core
out vec4 FragColor;
float near = 0.01;	//Valor del plano cercano
float far = 100.0;	//Valor del plano lejano
float LinearDepth(float depth){
	//Se retorna a coordenadas normalizadas de dispositivo
    float z = depth * 2.0 - 1.0;
	//Se regresa la profundidad linealizada
    return (2.0 * near * far)/( far + near - z * (far - near));
}

void main()
{            
	//Se calcula la profundidad linealizada
    float depthL = LinearDepth(gl_FragCoord.z) / far;
	//Se asigna el color anterior al color de salida
    FragColor = vec4(vec3(depthL), 1.0);
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);	
}
