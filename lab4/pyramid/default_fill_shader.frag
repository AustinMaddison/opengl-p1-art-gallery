#version 330 core  
out vec4 FragColor;

in vec2 vTexCoord;
    
uniform float uTime;
uniform sampler2D texture0;


// https://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
// vec3 rgb_to_hsv(vec3 c)
// {
//     vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
//     vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
//     vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

//     float d = q.x - min(q.w, q.y);
//     float e = 1.0e-10;
//     return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
// }

// // https://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
// vec3 hsv_to_rgb(vec3 c)
// {
//     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
// }

void main()  
{  
    // vec3 color = rgb_to_hsv(vColor);
    // color += vec3(fract(uTime));
    // color = hsv_to_rgb(color); 

    //FragColor = vec4(vTexCoord, 0.0f, 0.4f);  
    FragColor = vec4(texture(texture0, vTexCoord).rgb, 0.8f);  
}