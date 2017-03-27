#version 330 core

#define MAX_LIGHTS 50

struct material {
     vec4 ambient;
     vec4 diffuse;
     vec4 specular;
	 float specularIntensity;
};

struct light {
     vec4 position;
     vec3 color;
};

in vec2 fragUV;
in vec4 fragNormal;
in vec4 fragView;

uniform material mat;
uniform int numLights;
uniform light lights[MAX_LIGHTS];
uniform sampler2D texSampler;

uniform mat4 viewMatrix;

out vec4 color;

void main(void) {
  vec3 tex_color = clamp(texture(texSampler, fragUV).rgb + mat.diffuse.rgb, vec3(0.0), vec3(1.0));
  vec3 light_color = vec3(0.0);
  
  for (int i = 0; i < numLights; i++) {
	  // Calculate vectors
      vec3 lightVector = normalize((lights[i].position - fragView).xyz);
      vec3 halfVector = normalize(normalize(viewMatrix * vec4(lightVector, 1.0)).xyz - normalize((viewMatrix * fragView).xyz));

	  // Calculate diffuse and specular
      vec3 diffuseResult = max(dot(fragNormal.xyz, lightVector), 0.0) * tex_color * lights[i].color;
      vec3 specularResult = pow(max(dot(fragNormal.xyz, halfVector), 0.0), mat.specularIntensity)
                        	* mat.specular.rgb * lights[i].color;

      light_color += (diffuseResult);
  }
  color = vec4(clamp(mat.ambient.rgb + light_color, vec3(0.0), vec3(1.0)), 1.0);
}
