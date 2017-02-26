#version 330 core

#define MAX_LIGHTS 50

struct material {
     vec4 ambient;
     vec4 diffuse;
     vec4 specular;
};

struct light {
     vec4 position;
     vec3 color;
};

in vec2 UV;
in vec4 fragNormal;
in vec4 vertPos;

uniform material mat;
uniform int numLights;
uniform light lights[MAX_LIGHTS];
uniform sampler2D texSampler;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;

void main(void) {
  vec3 tex_color = texture(texSampler, UV).rgb;
  vec3 light_color = vec3(0.0, 0.0, 0.0);
  
  for (int i = 0; i < numLights; i++) {
      vec4 result = vec4(0.0);

      // Calcs for diffuse
      vec4 lightVector = normalize(viewMatrix * lights[i].position - viewMatrix * modelMatrix * vertPos);
      float cosTheta = clamp(dot(viewMatrix * modelMatrix * normalize(fragNormal), lightVector), 0, 1);

      // Calcs for spec
      vec4 eyeVector = vec4(0.0) - viewMatrix * modelMatrix * vertPos;
      vec3 reflection = reflect(-lightVector.xyz, normalize(fragNormal.xyz));
      float cosAlpha = clamp(dot(eyeVector, vec4(reflection, 1.0)), 0.0, 1.0);

      float dist = distance(lights[i].position, modelMatrix * vertPos);

      vec4 diffuseResult = vec4(tex_color, 1.0) * vec4(lights[i].color, 1.0) * cosTheta / (dist * dist);
      vec4 ambientResult = mat.ambient * vec4(tex_color, 1.0);
      vec4 specularResult = mat.specular * vec4(lights[i].color, 1.0) * pow(cosAlpha, 5) / (dist * dist);

      result = diffuseResult + ambientResult + specularResult;
      light_color += result.rgb;
  }
  light_color = clamp(light_color, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));

  color = tex_color * light_color;
}
